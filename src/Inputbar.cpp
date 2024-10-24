#include "Inputbar.hpp"

Inputbar::Inputbar(QWidget *parent) : QWidget(parent) {
    this->setLayout(m_layout);
    m_layout->addWidget(m_prompt_label);
    m_layout->addWidget(m_line_edit);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
}

QString Inputbar::getInput(const QString& prompt) noexcept {
    this->show();
    m_prompt_label->setText(prompt);
    m_line_edit->setFocus();
    m_line_edit->clear();

    QString userInput;

    // Create an event loop to wait for the user to press Enter
    QEventLoop loop;

    // When the user presses Enter (returnPressed), capture the text and exit the loop
    connect(m_line_edit, &QLineEdit::returnPressed, this, [&]() {
        userInput = m_line_edit->text();  // Capture input
        loop.quit();  // Exit the loop
    });

    // Start the event loop
    loop.exec();

    this->hide();
    return userInput;  // Return the captured input
}

Inputbar::~Inputbar() {}
