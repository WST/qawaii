#include "NoteEditorWidget.h"
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>

NoteEditorWidget::NoteEditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_splitter(nullptr)
    , m_editor(nullptr)
    , m_preview(nullptr)
{
    setupUI();
    retranslateUI();
}

NoteEditorWidget::~NoteEditorWidget()
{
}

void NoteEditorWidget::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_splitter = new QSplitter(Qt::Vertical, this);
    layout->addWidget(m_splitter);

    m_editor = new QTextEdit(this);
    m_preview = new QLabel(this);
    m_preview->setWordWrap(true);
    m_preview->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_preview->setTextFormat(Qt::RichText);

    m_splitter->addWidget(m_editor);
    m_splitter->addWidget(m_preview);

    m_splitter->setStretchFactor(0, 1);
    m_splitter->setStretchFactor(1, 1);
    m_splitter->setSizes({400, 400});
}

void NoteEditorWidget::retranslateUI()
{
    // TODO: Добавить переводы для редактора
}

