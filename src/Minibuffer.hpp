#pragma once

#include <QLineEdit>
#include <QWidget>
#include <QHBoxLayout>
#include <QTimer>
#include <QLabel>
#include <QKeyEvent>
#include <QShortcut>
#include <QKeySequence>

class LineEdit : public QLineEdit {
Q_OBJECT
public:
    LineEdit(QWidget *parent = nullptr) : QLineEdit(parent) {
}

    ~LineEdit() {}


signals:
    void hideRequested();

protected:
    void keyPressEvent(QKeyEvent *e) override {

        if (e->key() == Qt::Key_Escape) {
            this->hide();
            this->clear();
            emit hideRequested();
            return;
        }

        QLineEdit::keyPressEvent(e);
    }

};

class Minibuffer : public QWidget {
Q_OBJECT
public:
    Minibuffer(QWidget *parent = nullptr);
    ~Minibuffer();

    QString getInput(const QString& prompt) noexcept;
    void message(const QString& message, const int& s = 2) noexcept;
    void ExecuteCommand() noexcept;

signals:
    void returnPressed(const QStringList& commandlist);

private:

    void ReturnPressed() noexcept;
    void keybinds() noexcept;

    QHBoxLayout *m_layout = new QHBoxLayout();
    QLabel *m_prompt_label = new QLabel();
    LineEdit *m_minibuffer = new LineEdit();
    QStringList m_minibuffer_history;

    int m_minibuffer_history_current_index = -1;
};