#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
QT_END_NAMESPACE

class NotesTreeWidget;
class NoteEditorWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void createFolder();
    void createNote();
    void syncNotes();
    void showSettings();

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupConnections();
    void retranslateUI();

    // UI компоненты
    QSplitter *m_splitter;
    NotesTreeWidget *m_notesTree;
    NoteEditorWidget *m_noteEditor;

    // Меню
    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QMenu *m_viewMenu;
    QMenu *m_helpMenu;

    // Действия меню
    QAction *m_createFolderAction;
    QAction *m_createNoteAction;
    QAction *m_deleteAction;
    QAction *m_syncAction;
    QAction *m_settingsAction;
    QAction *m_exitAction;
    QAction *m_aboutAction;
    QAction *m_aboutQtAction;

    // Тулбар
    QToolBar *m_mainToolBar;
};

#endif // MAINWINDOW_H

