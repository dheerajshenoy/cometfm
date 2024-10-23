#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QFileSystemModel>
#include <QObject>
#include <QKeyEvent>
#include "ListView.hpp"

class Panel : public QWidget {
Q_OBJECT
public:
    Panel(QWidget *parent = nullptr);
    ~Panel();

    void setCurrentDir(QString path) noexcept;
    QString getCurrentDir() noexcept;

    void UpDirectory() noexcept;
    void SelectItem() noexcept;
    void NextItem() noexcept;
    void PrevItem() noexcept;

  signals:
    void dirChanged(QString path);

protected:
    void keyPressEvent(QKeyEvent *e) override;

private:

    void handleItemDoubleClicked(const QModelIndex& index) noexcept;
    void initSignalsSlots() noexcept;
    bool isValidPath(QString path) noexcept;

    QVBoxLayout *m_layout = new QVBoxLayout();
    ListView *m_list_view = new ListView();
    QFileSystemModel *m_model = new QFileSystemModel();

    QString m_current_dir;
};