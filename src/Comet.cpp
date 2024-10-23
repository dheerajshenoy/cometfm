#include "Comet.hpp"
#include <qkeysequence.h>

Comet::Comet(QWidget *parent) : QMainWindow(parent) {
  initLayout();       // init layout
  initMenubar();      // init menubar
  initStatusbar();    // init statusbar
  initSignalsSlots(); // init signals and slots
  setCurrentDir("~"); // set the current directory
}

void Comet::initSignalsSlots() noexcept {
    // connect(m_file_panel, &Panel::dirChanged, this, [&](QString path) {
    // });
}

void Comet::initLayout() noexcept {
  m_file_panel = new Panel();
  m_preview_panel = new PreviewPanel();
  m_widget->setLayout(m_layout);
  m_layout->addWidget(m_splitter);
  m_splitter->addWidget(m_file_panel);
  m_splitter->addWidget(m_preview_panel);
  this->setCentralWidget(m_widget);
  this->show();
}

void Comet::initKeybinds() noexcept {
}

void Comet::initMenubar() noexcept {
  m_menubar = new QMenuBar();
  m_filemenu = new QMenu("File");
  m_filemenu__new_window = new QAction("New Window");
  m_filemenu__new_tab = new QAction("New Tab");
  m_filemenu__create_new_menu = new QMenu("Create New");

  m_filemenu__create_new_folder = new QAction("New Folder");
  m_filemenu__create_new_file = new QAction("New File");

  this->setMenuBar(m_menubar);

  m_filemenu->addAction(m_filemenu__new_window);
  m_filemenu->addAction(m_filemenu__new_tab);
  m_filemenu->addMenu(m_filemenu__create_new_menu);

  m_filemenu__create_new_menu->addAction(m_filemenu__create_new_folder);
  m_filemenu__create_new_menu->addAction(m_filemenu__create_new_file);

  m_menubar->addMenu(m_filemenu);
}

void Comet::initStatusbar() noexcept {}

void Comet::setCurrentDir(QString path) {
    m_file_panel->setCurrentDir(path);
}

Comet::~Comet() {}
