#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSizePolicy>
#include <QMouseEvent>
#include <QCompleter>
#include <QFileSystemModel>

class ModelineLineEdit : public QLineEdit {

public:
    explicit ModelineLineEdit(QWidget *parent = nullptr)
    : QLineEdit(parent) {
        this->setReadOnly(true);
    }


protected:
    void mouseDoubleClickEvent(QMouseEvent *e) override {
        this->setReadOnly(false);
        this->selectAll();
        this->setFocus();
    }

};

class Modeline : public QWidget {
Q_OBJECT
public:
    Modeline(QWidget *parent = nullptr);
    ~Modeline();

    void setCurrentDir(const QString& path) noexcept;

signals:
    void directoryChangeRequested(const QString& path);

private:
    QHBoxLayout *m_layout = new QHBoxLayout();
    ModelineLineEdit *m_path_line = new ModelineLineEdit();
    QCompleter *m_completer = nullptr;
    QFileSystemModel *m_completer_model = new QFileSystemModel(this);
};