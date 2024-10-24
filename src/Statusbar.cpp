#include "Statusbar.hpp"

Statusbar::Statusbar(QWidget *parent)
: QWidget(parent) {
    this->setLayout(m_vert_layout);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    m_vert_layout->addLayout(m_layout);
    m_layout->addWidget(m_file_name_label);
    m_layout->addStretch();
    m_layout->addWidget(m_file_size_label);
    m_layout->addWidget(m_file_modified_label);
    m_vert_layout->addWidget(m_message_label);
}

void Statusbar::Message(const QString& message, int ms) noexcept {
    m_message_label->setText(message);
    m_message_label->show();
    QTimer::singleShot(ms * 1000, [&]() {
        m_message_label->hide();
    });
}

void Statusbar::SetFile(const QString& file) noexcept {
    QFileInfo fileInfo(file);
    m_file_name_label->setText(fileInfo.fileName());
    // m_file_perm_label->setText(fileInfo.permissions());
    m_file_size_label->setText(m_locale.formattedDataSize(fileInfo.size()));
    m_file_modified_label->setText(fileInfo.lastModified().toString());
}

Statusbar::~Statusbar() {}