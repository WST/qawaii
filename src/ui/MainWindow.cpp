#include "MainWindow.h"
#include "widgets/NotesTreeWidget.h"
#include "widgets/NoteEditorWidget.h"
#include <QMessageBox>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_splitter(nullptr)
    , m_notesTree(nullptr)
    , m_noteEditor(nullptr)
{
    setupUI();
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    setupConnections();
    retranslateUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setWindowTitle(tr("Qawaii - Менеджер заметок"));
    resize(1200, 800);

    // Создаем центральный виджет с разделителем
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    m_splitter = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(m_splitter);

    // Создаем виджеты для сайдбара и редактора
    m_notesTree = new NotesTreeWidget(this);
    m_noteEditor = new NoteEditorWidget(this);

    m_splitter->addWidget(m_notesTree);
    m_splitter->addWidget(m_noteEditor);

    // Устанавливаем пропорции (сайдбар - 25%, редактор - 75%)
    m_splitter->setStretchFactor(0, 1);
    m_splitter->setStretchFactor(1, 3);
    m_splitter->setSizes({300, 900});
}

void MainWindow::setupMenuBar()
{
    // Меню "Файл"
    m_fileMenu = menuBar()->addMenu(tr("&Файл"));
    m_createFolderAction = m_fileMenu->addAction(tr("Создать &папку"));
    m_createNoteAction = m_fileMenu->addAction(tr("Создать &заметку"));
    m_fileMenu->addSeparator();
    m_syncAction = m_fileMenu->addAction(tr("&Синхронизировать"));
    m_fileMenu->addSeparator();
    m_settingsAction = m_fileMenu->addAction(tr("&Настройки..."));
    m_fileMenu->addSeparator();
    m_exitAction = m_fileMenu->addAction(tr("&Выход"));

    // Меню "Правка"
    m_editMenu = menuBar()->addMenu(tr("&Правка"));
    m_deleteAction = m_editMenu->addAction(tr("&Удалить"));

    // Меню "Вид"
    m_viewMenu = menuBar()->addMenu(tr("&Вид"));

    // Меню "Справка"
    m_helpMenu = menuBar()->addMenu(tr("&Справка"));
    m_aboutAction = m_helpMenu->addAction(tr("&О программе"));
    m_aboutQtAction = m_helpMenu->addAction(tr("О &Qt"));
}

void MainWindow::setupToolBar()
{
    m_mainToolBar = addToolBar(tr("Главная панель инструментов"));
    m_mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_mainToolBar->addAction(m_createFolderAction);
    m_mainToolBar->addAction(m_createNoteAction);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(m_syncAction);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(m_settingsAction);
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage(tr("Готово"));
}

void MainWindow::setupConnections()
{
    // Подключение действий
    connect(m_createFolderAction, &QAction::triggered, this, &MainWindow::createFolder);
    connect(m_createNoteAction, &QAction::triggered, this, &MainWindow::createNote);
    connect(m_syncAction, &QAction::triggered, this, &MainWindow::syncNotes);
    connect(m_settingsAction, &QAction::triggered, this, &MainWindow::showSettings);
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);
    connect(m_aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(m_aboutAction, &QAction::triggered, this, []() {
        QMessageBox::about(nullptr, 
            QApplication::translate("MainWindow", "О программе"),
            QApplication::translate("MainWindow", 
                "<h2>Qawaii</h2>"
                "<p>Версия 1.0.0</p>"
                "<p>Менеджер заметок на Qt</p>"
            )
        );
    });
}

void MainWindow::retranslateUI()
{
    setWindowTitle(tr("Qawaii - Менеджер заметок"));
    m_fileMenu->setTitle(tr("&Файл"));
    m_editMenu->setTitle(tr("&Правка"));
    m_viewMenu->setTitle(tr("&Вид"));
    m_helpMenu->setTitle(tr("&Справка"));
    m_mainToolBar->setWindowTitle(tr("Главная панель инструментов"));
    statusBar()->showMessage(tr("Готово"));
}

void MainWindow::createFolder()
{
    // TODO: Реализовать создание папки
    statusBar()->showMessage(tr("Создание папки..."), 2000);
}

void MainWindow::createNote()
{
    // TODO: Реализовать создание заметки
    statusBar()->showMessage(tr("Создание заметки..."), 2000);
}

void MainWindow::syncNotes()
{
    // TODO: Реализовать синхронизацию
    statusBar()->showMessage(tr("Синхронизация..."), 2000);
}

void MainWindow::showSettings()
{
    // TODO: Реализовать диалог настроек
    statusBar()->showMessage(tr("Открытие настроек..."), 2000);
}

