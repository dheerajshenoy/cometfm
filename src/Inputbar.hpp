#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QEventLoop>

class Inputbar : public QWidget {

public:
    Inputbar(QWidget *parent = nullptr);
    ~Inputbar();

    QString getInput(const QString& prompt) noexcept ;

private:
    QHBoxLayout *m_layout = new QHBoxLayout();
    QLabel *m_prompt_label = new QLabel();
    QLineEdit *m_line_edit = new QLineEdit();
};