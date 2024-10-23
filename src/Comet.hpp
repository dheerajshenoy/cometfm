#pragma once

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QSplitter>
#include <QShortcut>
#include <QKeySequence>
#include "Panel.hpp"
#include "PreviewPanel.hpp"
#include "Minibuffer.hpp"

class Comet : public QMainWindow {
    Q_OBJECT

public:
  Comet(QWidget *parent = nullptr);
    ~Comet();
    void setCurrentDir(QString path);
    void TogglePreviewPanel(bool state) noexcept;
    void ExecuteExtendedCommand() noexcept;
    void RenameItems() noexcept;
    void NewFile(const int& nfiles = -1) noexcept;
    void NewFolder(const int& nfolders = -1) noexcept;
    void DeleteItems() noexcept;

private:
    void initLayout() noexcept;
    void initMenubar() noexcept;
    void initStatusbar() noexcept;
    void initSignalsSlots() noexcept;
    bool isValidPath(QString path);
    void handleDirChange() noexcept;
    void initKeybinds() noexcept;
    bool renderDir();
    bool createEmptyFile(const QString& filename) noexcept;

    QWidget *m_widget = new QWidget();
    QVBoxLayout *m_layout = new QVBoxLayout();
    QSplitter *m_splitter = new QSplitter();
    QMenuBar *m_menubar = nullptr;
    QMenu *m_filemenu = nullptr;
    QMenu *m_viewmenu = nullptr;

    QMenu *m_filemenu__create_new_menu = nullptr;

    QAction *m_filemenu__new_window = nullptr;
    QAction *m_filemenu__new_tab = nullptr;
    QAction *m_filemenu__create_new_folder = nullptr;
    QAction *m_filemenu__create_new_file = nullptr;

    QAction *m_viewmenu__preview_panel = nullptr;

    Panel *m_file_panel = nullptr;
    PreviewPanel *m_preview_panel = nullptr;
    Minibuffer *m_minibuffer = nullptr;
};