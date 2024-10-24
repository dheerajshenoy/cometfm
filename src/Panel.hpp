#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QObject>
#include <QKeyEvent>
#include <QShortcut>
#include <QHash>
#include <QKeySequence>
#include <QMessageBox>
#include <QInputDialog>
#include <QDesktopServices>
#include <QTimer>
#include "ListView.hpp"
#include "FileSystemModel.hpp"
#include <QContextMenuEvent>
#include <QMenu>
#include <QModelIndexList>

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
    void MarkOrUnmarkItems() noexcept;
    void MarkItems() noexcept;
    void UnmarkItems() noexcept;
    void GotoFirstItem() noexcept;
    void GotoLastItem() noexcept;
    bool RenameItems() noexcept;
    bool MoveItems() noexcept;
    bool DeleteItems() noexcept;
    bool TrashItems() noexcept;
    void ToggleHiddenFiles() noexcept;
    void Search(const QString& searchExpression) noexcept;
    void SearchNext() noexcept;
    void SearchPrev() noexcept;

  signals:
    void afterDirChange(QString path);
    void currentItemChanged(const QString& path);

protected:
    void contextMenuEvent(QContextMenuEvent *e) override;

private:
    QString getCurrentItem() noexcept;
    QString currentItem() noexcept;
    void initKeybinds() noexcept;
    void initContextMenu() noexcept;
    void handleItemDoubleClicked(const QModelIndex& index) noexcept;
    void initSignalsSlots() noexcept;
    bool isValidPath(QString path) noexcept;

    QVBoxLayout *m_layout = new QVBoxLayout();
    ListView *m_list_view = new ListView();
    FileSystemModel *m_model = new FileSystemModel();

    QString m_current_dir;

    bool m_hidden_files_shown = false;

    QAction* m_context_action_open = nullptr;
    QAction* m_context_action_open_with = nullptr;
    QAction* m_context_action_cut = nullptr;
    QAction* m_context_action_copy = nullptr;
    QAction* m_context_action_paste = nullptr;
    QAction* m_context_action_delete = nullptr;
    QAction* m_context_action_trash = nullptr;
    QAction* m_context_action_properties = nullptr;
    QAction* m_context_action_open_terminal = nullptr;

    QModelIndexList m_search_index_list;
    int m_search_index_list_index = -1;
};