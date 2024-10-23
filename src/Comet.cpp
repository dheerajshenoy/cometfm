#include "Comet.hpp"
#include <qkeysequence.h>

Comet::Comet(QWidget *parent) : QMainWindow(parent) {
  initLayout();       // init layout
  initMenubar();      // init menubar
  initStatusbar();    // init statusbar
  initSignalsSlots(); // init signals and slots
  initKeybinds();
  setCurrentDir("~"); // set the current directory
}

void Comet::initSignalsSlots() noexcept {
  // connect(m_file_panel, &Panel::dirChanged, this, [&](QString path) {});

  connect(m_file_panel, &Panel::currentItemChanged, m_preview_panel,
          &PreviewPanel::Preview);
}

void Comet::TogglePreviewPanel(bool state) noexcept {
  if (state) {
    m_preview_panel->show();
    connect(m_file_panel, &Panel::currentItemChanged, m_preview_panel,
            &PreviewPanel::Preview);
  } else {
    m_preview_panel->hide();
    disconnect(m_file_panel, &Panel::currentItemChanged, m_preview_panel,
               &PreviewPanel::Preview);
  }
}

void Comet::initLayout() noexcept {
  m_file_panel = new Panel();
  m_preview_panel = new PreviewPanel();
  m_minibuffer = new Minibuffer();

  m_layout->setContentsMargins(0, 0, 0, 0);
  m_splitter->setContentsMargins(0, 0, 0, 0);
  m_widget->setLayout(m_layout);
  m_layout->addWidget(m_splitter);
  m_splitter->addWidget(m_file_panel);
  m_splitter->addWidget(m_preview_panel);
  m_layout->addWidget(m_minibuffer);
  this->setCentralWidget(m_widget);
  this->show();
}

void Comet::initKeybinds() noexcept {

    QShortcut *kb_up_directory = new QShortcut(QKeySequence("h"), this);
    QShortcut *kb_next_item = new QShortcut(QKeySequence("j"), this);
    QShortcut *kb_prev_item = new QShortcut(QKeySequence("k"), this);
    QShortcut *kb_select_item = new QShortcut(QKeySequence("l"), this);
    QShortcut *kb_goto_first_item = new QShortcut(QKeySequence("g,g"), this);
    QShortcut *kb_goto_last_item = new QShortcut(QKeySequence("Shift+g"), this);
    QShortcut *kb_mark_item = new QShortcut(QKeySequence("Space"), this);
    QShortcut *kb_command = new QShortcut(QKeySequence(":"), this);

    connect(kb_next_item, &QShortcut::activated, m_file_panel, &Panel::NextItem);
    connect(kb_prev_item, &QShortcut::activated, m_file_panel, &Panel::PrevItem);
    connect(kb_select_item, &QShortcut::activated, m_file_panel, &Panel::SelectItem);
    connect(kb_up_directory, &QShortcut::activated, m_file_panel, &Panel::UpDirectory);
    connect(kb_goto_last_item, &QShortcut::activated, m_file_panel, &Panel::GotoLastItem);
    connect(kb_goto_first_item, &QShortcut::activated, m_file_panel, &Panel::GotoFirstItem);
    connect(kb_mark_item, &QShortcut::activated, m_file_panel, &Panel::MarkOrUnmarkItem);

    connect(kb_command, &QShortcut::activated, this, &Comet::ExecuteExtendedCommand);
}

void Comet::ExecuteExtendedCommand() noexcept {
    m_minibuffer->ExecuteCommand();
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

  m_viewmenu = new QMenu("View");

  m_viewmenu__preview_panel = new QAction("Preview Panel");
  m_viewmenu__preview_panel->setCheckable(true);

  m_viewmenu->addAction(m_viewmenu__preview_panel);

  m_menubar->addMenu(m_filemenu);
  m_menubar->addMenu(m_viewmenu);

  connect(m_viewmenu__preview_panel, &QAction::triggered, this, &Comet::TogglePreviewPanel);
}

void Comet::initStatusbar() noexcept {}

void Comet::setCurrentDir(QString path) { m_file_panel->setCurrentDir(path); }

Comet::~Comet() {}
