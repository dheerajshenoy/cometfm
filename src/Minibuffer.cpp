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

    connect(m_minibuffer, &LineEdit::returnPressed, this, &Minibuffer::ReturnPressed);

    m_minibuffer->setHidden(true);
    m_prompt_label->setHidden(true);

    keybinds();
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
    m_minibuffer->clear();
}

void Minibuffer::ReturnPressed() noexcept {
    QString minibufferText = m_minibuffer->text();
    m_minibuffer_history.push_back(minibufferText);
    const QStringList commandlist = minibufferText.split(" ");
    emit returnPressed(commandlist);
    emit m_minibuffer->hideRequested();
    m_minibuffer_history_current_index = -1;
}

void Minibuffer::keybinds() noexcept {
  QShortcut *kb_history_prev_item = new QShortcut(QKeySequence("Alt+p"), this);
  QShortcut *kb_history_next_item = new QShortcut(QKeySequence("Alt+n"), this);

  connect(kb_history_prev_item, &QShortcut::activated, this, [&]() {
      if (m_minibuffer_history.size() == 0) return; // Do nothing if history is empty

      if (m_minibuffer_history_current_index <= 0)
          m_minibuffer_history_current_index = m_minibuffer_history.size() - 1;
      else
          m_minibuffer_history_current_index--;
      m_minibuffer->setText(m_minibuffer_history.at(m_minibuffer_history_current_index));
  });

  connect(kb_history_next_item, &QShortcut::activated, this, [&]() {
      if (m_minibuffer_history.size() == 0) return; // Do nothing if history is empty

      if (m_minibuffer_history_current_index >= m_minibuffer_history.size() - 1)
          m_minibuffer_history_current_index = 0;
      else
          m_minibuffer_history_current_index++;
      m_minibuffer->setText(m_minibuffer_history.at(m_minibuffer_history_current_index));
  });
}

Minibuffer::~Minibuffer() {
    // TODO: write the minibuffer history file
}
