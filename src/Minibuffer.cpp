#include "Minibuffer.hpp"

Minibuffer::Minibuffer(QWidget *parent)
        : QWidget(parent)
{
    m_layout->addWidget(m_prompt_label);
    m_layout->addWidget(m_minibuffer);
    this->setLayout(m_layout);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    connect(m_minibuffer, &LineEdit::hideRequested, this, [&]() {
        setHidden(true);
        clearFocus();
    });

    m_minibuffer->setHidden(true);
    m_prompt_label->setHidden(true);
}

QString Minibuffer::getInput(const QString& prompt) noexcept {
    m_prompt_label->setVisible(true);
    m_minibuffer->setVisible(true);
}

void Minibuffer::message(const QString& message, const int& s) noexcept {

    m_prompt_label->setVisible(true);
    m_minibuffer->setVisible(false);
    m_prompt_label->setText(message);

    QTimer::singleShot(s * 1000, [&]() {
        m_prompt_label->clear();
    });
}

void Minibuffer::ExecuteCommand() noexcept {
    this->setVisible(true);
    m_prompt_label->setVisible(true);
    m_minibuffer->setVisible(true);
    m_prompt_label->setText("Command");
    m_minibuffer->setFocus();
}

Minibuffer::~Minibuffer() {}
