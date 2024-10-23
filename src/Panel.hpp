#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QObject>
#include <QKeyEvent>
#include <QShortcut>
#include <QHash>
#include <QKeySequence>
#include "ListView.hpp"
#include "FileSystemModel.hpp"

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
    void MarkOrUnmarkItem() noexcept;
    void GotoFirstItem() noexcept;
    void GotoLastItem() noexcept;
    void RenameItems() noexcept;
    void MoveItems() noexcept;

  signals:
    void beforeDirChange();
    void afterDirChange(QString path);
    void currentItemChanged(const QString& path);

private:
    QString currentItem() noexcept;
    void initKeybinds() noexcept;
    void handleItemDoubleClicked(const QModelIndex& index) noexcept;
    void initSignalsSlots() noexcept;
    bool isValidPath(QString path) noexcept;

    QVBoxLayout *m_layout = new QVBoxLayout();
    ListView *m_list_view = new ListView();
    FileSystemModel *m_model = new FileSystemModel();

    QString m_current_dir;
};