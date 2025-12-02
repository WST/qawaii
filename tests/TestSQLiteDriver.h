#ifndef TESTSQLITEDRIVER_H
#define TESTSQLITEDRIVER_H

#include <QtTest/QtTest>
#include <QObject>
#include <QTemporaryDir>

class SQLiteDriver;

class TestSQLiteDriver : public QObject
{
    Q_OBJECT

public:
    TestSQLiteDriver();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Тесты подключения
    void testInitialize();
    void testConnect();
    void testDisconnect();
    void testIsConnected();

    // Тесты работы с папками
    void testCreateFolder();
    void testGetFolders();
    void testUpdateFolder();
    void testDeleteFolder();

    // Тесты работы с заметками
    void testCreateNote();
    void testGetNotes();
    void testGetNote();
    void testUpdateNote();
    void testDeleteNote();

    // Тесты транзакций
    void testTransactions();

    // Интеграционные тесты
    void testFolderWithNotes();
    void testCascadeDelete();

private:
    SQLiteDriver *m_driver;
    QTemporaryDir *m_tempDir;
    QString m_testDbPath;
};

#endif // TESTSQLITEDRIVER_H

