#include "TestSQLiteDriver.h"
#include "../src/core/storage/SQLiteDriver.h"
#include "../src/core/models/Folder.h"
#include "../src/core/models/Note.h"
#include <QTemporaryDir>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QTest>

TestSQLiteDriver::TestSQLiteDriver()
    : m_driver(nullptr)
    , m_tempDir(nullptr)
{
}

void TestSQLiteDriver::initTestCase()
{
    // Создаем временную директорию для тестовой БД
    m_tempDir = new QTemporaryDir();
    QVERIFY(m_tempDir->isValid());
    
    m_testDbPath = QDir(m_tempDir->path()).filePath("test_notes.db");
    
    // Создаем драйвер
    m_driver = new SQLiteDriver(this);
}

void TestSQLiteDriver::cleanupTestCase()
{
    delete m_driver;
    m_driver = nullptr;
    
    delete m_tempDir;
    m_tempDir = nullptr;
}

void TestSQLiteDriver::init()
{
    // Удаляем тестовую БД перед каждым тестом
    if (QFile::exists(m_testDbPath)) {
        QFile::remove(m_testDbPath);
    }
    
    // Инициализируем драйвер с тестовым путем
    QVariantMap config;
    config["database_path"] = m_testDbPath;
    QVERIFY(m_driver->initialize(config));
}

void TestSQLiteDriver::cleanup()
{
    // Отключаемся после каждого теста
    m_driver->disconnect();
}

void TestSQLiteDriver::testInitialize()
{
    QVariantMap config;
    config["database_path"] = m_testDbPath;
    
    QVERIFY(m_driver->initialize(config));
    QCOMPARE(m_driver->getConfigKeys(), QStringList() << "database_path");
}

void TestSQLiteDriver::testConnect()
{
    QVERIFY(m_driver->connect());
    QVERIFY(m_driver->isConnected());
    
    // Проверяем, что БД создалась
    QVERIFY(QFile::exists(m_testDbPath));
}

void TestSQLiteDriver::testDisconnect()
{
    QVERIFY(m_driver->connect());
    QVERIFY(m_driver->isConnected());
    
    m_driver->disconnect();
    QVERIFY(!m_driver->isConnected());
}

void TestSQLiteDriver::testIsConnected()
{
    QVERIFY(!m_driver->isConnected());
    
    QVERIFY(m_driver->connect());
    QVERIFY(m_driver->isConnected());
    
    m_driver->disconnect();
    QVERIFY(!m_driver->isConnected());
}

void TestSQLiteDriver::testCreateFolder()
{
    QVERIFY(m_driver->connect());
    
    Folder *folder = m_driver->createFolder("Test Folder");
    QVERIFY(folder != nullptr);
    QVERIFY(!folder->id().isEmpty());
    QCOMPARE(folder->name(), QString("Test Folder"));
    QCOMPARE(folder->iconName(), QString("folder"));
    QVERIFY(folder->createdAt().isValid());
    QVERIFY(folder->modifiedAt().isValid());
    
    delete folder;
}

void TestSQLiteDriver::testGetFolders()
{
    QVERIFY(m_driver->connect());
    
    // Создаем несколько папок
    Folder *folder1 = m_driver->createFolder("Folder 1");
    Folder *folder2 = m_driver->createFolder("Folder 2");
    QVERIFY(folder1 != nullptr);
    QVERIFY(folder2 != nullptr);
    
    // Получаем все папки
    QList<Folder*> folders = m_driver->getFolders();
    QCOMPARE(folders.size(), 2);
    
    // Проверяем, что папки найдены
    bool found1 = false, found2 = false;
    for (Folder *f : folders) {
        if (f->name() == "Folder 1") found1 = true;
        if (f->name() == "Folder 2") found2 = true;
        delete f;
    }
    QVERIFY(found1);
    QVERIFY(found2);
    
    delete folder1;
    delete folder2;
}

void TestSQLiteDriver::testUpdateFolder()
{
    QVERIFY(m_driver->connect());
    
    Folder *folder = m_driver->createFolder("Original Name");
    QVERIFY(folder != nullptr);
    
    QString originalId = folder->id();
    QDateTime originalModified = folder->modifiedAt();
    
    // Обновляем папку
    QTest::qWait(100); // Задержка для гарантированного изменения времени
    folder->setName("Updated Name");
    folder->setIconName("custom_icon");
    QVERIFY(m_driver->updateFolder(folder));
    
    // Проверяем, что папка обновлена
    QList<Folder*> folders = m_driver->getFolders();
    Folder *updatedFolder = nullptr;
    for (Folder *f : folders) {
        if (f->id() == originalId) {
            updatedFolder = f;
            break;
        }
    }
    
    QVERIFY(updatedFolder != nullptr);
    QCOMPARE(updatedFolder->name(), QString("Updated Name"));
    QCOMPARE(updatedFolder->iconName(), QString("custom_icon"));
    // Проверяем, что modified_at установлен и валиден
    QVERIFY(updatedFolder->modifiedAt().isValid());
    // Проверяем, что время обновления не раньше создания
    QVERIFY(updatedFolder->modifiedAt() >= updatedFolder->createdAt());
    
    // Очистка
    for (Folder *f : folders) delete f;
    delete folder;
}

void TestSQLiteDriver::testDeleteFolder()
{
    QVERIFY(m_driver->connect());
    
    Folder *folder = m_driver->createFolder("To Delete");
    QVERIFY(folder != nullptr);
    QString folderId = folder->id();
    
    // Удаляем папку
    QVERIFY(m_driver->deleteFolder(folderId));
    
    // Проверяем, что папка удалена
    QList<Folder*> folders = m_driver->getFolders();
    QCOMPARE(folders.size(), 0);
    
    for (Folder *f : folders) delete f;
    delete folder;
}

void TestSQLiteDriver::testCreateNote()
{
    QVERIFY(m_driver->connect());
    
    Folder *folder = m_driver->createFolder("Test Folder");
    QVERIFY(folder != nullptr);
    
    Note *note = m_driver->createNote(folder->id(), "Test Note");
    QVERIFY(note != nullptr);
    QVERIFY(!note->id().isEmpty());
    QCOMPARE(note->title(), QString("Test Note"));
    QCOMPARE(note->folderId(), folder->id());
    QCOMPARE(note->iconName(), QString("note"));
    QCOMPARE(note->version(), QString("1"));
    QVERIFY(note->createdAt().isValid());
    QVERIFY(note->modifiedAt().isValid());
    
    delete note;
    delete folder;
}

void TestSQLiteDriver::testGetNotes()
{
    QVERIFY(m_driver->connect());
    
    Folder *folder = m_driver->createFolder("Test Folder");
    QVERIFY(folder != nullptr);
    
    Note *note1 = m_driver->createNote(folder->id(), "Note 1");
    Note *note2 = m_driver->createNote(folder->id(), "Note 2");
    QVERIFY(note1 != nullptr);
    QVERIFY(note2 != nullptr);
    
    // Получаем заметки
    QList<Note*> notes = m_driver->getNotes(folder->id());
    QCOMPARE(notes.size(), 2);
    
    // Проверяем, что заметки найдены
    bool found1 = false, found2 = false;
    for (Note *n : notes) {
        if (n->title() == "Note 1") found1 = true;
        if (n->title() == "Note 2") found2 = true;
        delete n;
    }
    QVERIFY(found1);
    QVERIFY(found2);
    
    delete note1;
    delete note2;
    delete folder;
}

void TestSQLiteDriver::testGetNote()
{
    QVERIFY(m_driver->connect());
    
    Folder *folder = m_driver->createFolder("Test Folder");
    QVERIFY(folder != nullptr);
    
    Note *note = m_driver->createNote(folder->id(), "Test Note");
    QVERIFY(note != nullptr);
    QString noteId = note->id();
    
    // Получаем заметку по ID
    Note *retrievedNote = m_driver->getNote(noteId);
    QVERIFY(retrievedNote != nullptr);
    QCOMPARE(retrievedNote->id(), noteId);
    QCOMPARE(retrievedNote->title(), QString("Test Note"));
    
    delete retrievedNote;
    delete note;
    delete folder;
}

void TestSQLiteDriver::testUpdateNote()
{
    QVERIFY(m_driver->connect());
    
    Folder *folder = m_driver->createFolder("Test Folder");
    QVERIFY(folder != nullptr);
    
    Note *note = m_driver->createNote(folder->id(), "Original Title");
    QVERIFY(note != nullptr);
    
    QString originalId = note->id();
    QString originalVersion = note->version();
    QDateTime originalModified = note->modifiedAt();
    
    // Обновляем заметку
    QTest::qWait(100); // Задержка для гарантированного изменения времени
    note->setTitle("Updated Title");
    note->setContent("Test content");
    note->setIconName("custom_icon");
    QVERIFY(m_driver->updateNote(note));
    
    // Проверяем, что версия увеличилась
    QVERIFY(note->version().toInt() > originalVersion.toInt());
    
    // Получаем обновленную заметку
    Note *updatedNote = m_driver->getNote(originalId);
    QVERIFY(updatedNote != nullptr);
    QCOMPARE(updatedNote->title(), QString("Updated Title"));
    QCOMPARE(updatedNote->content(), QString("Test content"));
    QCOMPARE(updatedNote->iconName(), QString("custom_icon"));
    // Проверяем, что modified_at установлен и валиден
    QVERIFY(updatedNote->modifiedAt().isValid());
    // Проверяем, что время обновления не раньше создания
    QVERIFY(updatedNote->modifiedAt() >= updatedNote->createdAt());
    
    delete updatedNote;
    delete note;
    delete folder;
}

void TestSQLiteDriver::testDeleteNote()
{
    QVERIFY(m_driver->connect());
    
    Folder *folder = m_driver->createFolder("Test Folder");
    QVERIFY(folder != nullptr);
    
    Note *note = m_driver->createNote(folder->id(), "To Delete");
    QVERIFY(note != nullptr);
    QString noteId = note->id();
    
    // Удаляем заметку
    QVERIFY(m_driver->deleteNote(noteId));
    
    // Проверяем, что заметка удалена
    QList<Note*> notes = m_driver->getNotes(folder->id());
    QCOMPARE(notes.size(), 0);
    
    Note *deletedNote = m_driver->getNote(noteId);
    QVERIFY(deletedNote == nullptr);
    
    for (Note *n : notes) delete n;
    delete note;
    delete folder;
}

void TestSQLiteDriver::testTransactions()
{
    QVERIFY(m_driver->connect());
    
    // Начинаем транзакцию
    QVERIFY(m_driver->beginTransaction());
    
    Folder *folder = m_driver->createFolder("Transaction Test");
    QVERIFY(folder != nullptr);
    
    // Откатываем транзакцию
    QVERIFY(m_driver->rollbackTransaction());
    
    // Проверяем, что папка не сохранилась
    QList<Folder*> folders = m_driver->getFolders();
    QCOMPARE(folders.size(), 0);
    
    // Теперь коммитим
    QVERIFY(m_driver->beginTransaction());
    Folder *folder2 = m_driver->createFolder("Committed Folder");
    QVERIFY(folder2 != nullptr);
    QVERIFY(m_driver->commitTransaction());
    
    // Проверяем, что папка сохранилась
    folders = m_driver->getFolders();
    QCOMPARE(folders.size(), 1);
    
    for (Folder *f : folders) delete f;
    delete folder;
    delete folder2;
}

void TestSQLiteDriver::testFolderWithNotes()
{
    QVERIFY(m_driver->connect());
    
    Folder *folder = m_driver->createFolder("Folder with Notes");
    QVERIFY(folder != nullptr);
    
    // Создаем несколько заметок
    Note *note1 = m_driver->createNote(folder->id(), "Note 1");
    Note *note2 = m_driver->createNote(folder->id(), "Note 2");
    Note *note3 = m_driver->createNote(folder->id(), "Note 3");
    
    QVERIFY(note1 != nullptr);
    QVERIFY(note2 != nullptr);
    QVERIFY(note3 != nullptr);
    
    // Проверяем, что все заметки в папке
    QList<Note*> notes = m_driver->getNotes(folder->id());
    QCOMPARE(notes.size(), 3);
    
    for (Note *n : notes) delete n;
    delete note1;
    delete note2;
    delete note3;
    delete folder;
}

void TestSQLiteDriver::testCascadeDelete()
{
    QVERIFY(m_driver->connect());
    
    Folder *folder = m_driver->createFolder("Folder to Delete");
    QVERIFY(folder != nullptr);
    
    // Создаем заметки в папке
    Note *note1 = m_driver->createNote(folder->id(), "Note 1");
    Note *note2 = m_driver->createNote(folder->id(), "Note 2");
    QVERIFY(note1 != nullptr);
    QVERIFY(note2 != nullptr);
    
    QString note1Id = note1->id();
    QString note2Id = note2->id();
    
    // Удаляем папку
    QVERIFY(m_driver->deleteFolder(folder->id()));
    
    // Проверяем, что заметки тоже удалены (каскадное удаление)
    Note *deletedNote1 = m_driver->getNote(note1Id);
    Note *deletedNote2 = m_driver->getNote(note2Id);
    QVERIFY(deletedNote1 == nullptr);
    QVERIFY(deletedNote2 == nullptr);
    
    delete note1;
    delete note2;
    delete folder;
}

// Регистрация теста
QTEST_MAIN(TestSQLiteDriver)
#include "TestSQLiteDriver.moc"

