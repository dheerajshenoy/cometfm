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


class Comet : public QMainWindow {
  Q_OBJECT

public:
  Comet(QWidget *parent = nullptr);
  ~Comet();
  void setCurrentDir(QString path);

private:
  void initLayout() noexcept;
  void initMenubar() noexcept;
  void initStatusbar() noexcept;
  void initSignalsSlots() noexcept;
  bool isValidPath(QString path);
  void handleDirChange() noexcept;
  void initKeybinds() noexcept;
  bool renderDir();

  QWidget *m_widget = new QWidget();
  QVBoxLayout *m_layout = new QVBoxLayout();
  QSplitter *m_splitter = new QSplitter();
  QMenuBar *m_menubar = nullptr;
  QMenu *m_filemenu = nullptr;
  QMenu *m_filemenu__create_new_menu = nullptr;

  QAction *m_filemenu__new_window = nullptr;
  QAction *m_filemenu__new_tab = nullptr;
  QAction *m_filemenu__create_new_folder = nullptr;
  QAction *m_filemenu__create_new_file = nullptr;

  Panel *m_file_panel = nullptr;
  PreviewPanel *m_preview_panel = nullptr;
};