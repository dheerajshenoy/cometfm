#include "Modeline.hpp"

Modeline::Modeline(QWidget *parent)
: QWidget(parent) {
    this->setLayout(m_layout);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    m_path_line->setReadOnly(true);
    m_layout->addWidget(m_path_line);

    // Set up file system model for directory completion
    m_completer_model->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);  // Show only directories

    m_completer = new QCompleter(m_completer_model, this);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);  // Optional, make case insensit

    m_path_line->setCompleter(m_completer);

    connect(m_path_line, &QLineEdit::returnPressed, this, [&]() {
        m_path_line->setReadOnly(true);
        emit directoryChangeRequested(m_path_line->text());

    });
}

void Modeline::setCurrentDir(const QString& path) noexcept {
    m_path_line->setText(path);
    m_completer_model->setRootPath(path);
}

Modeline::~Modeline() {}