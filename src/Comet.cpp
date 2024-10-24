#include "Comet.hpp"

Comet::Comet(QWidget *parent) : QMainWindow(parent) {
  initLayout();       // init layout
  initMenubar();      // init menubar
  initSignalsSlots(); // init signals and slots
  initKeybinds();
  setupCommandMap();
  setCurrentDir("~"); // set the current directory
}

void Comet::initSignalsSlots() noexcept {

  connect(m_file_panel, &Panel::currentItemChanged, m_preview_panel,
          &PreviewPanel::Preview);

  connect(m_file_panel, &Panel::currentItemChanged, m_statusbar,
          &Statusbar::SetFile);

  // connect(m_minibuffer, &Minibuffer::returnPressed, this, &Comet::ProcessMinibuffer);

  connect(m_file_panel, &Panel::afterDirChange, m_modeline, &Modeline::setCurrentDir);
  connect(m_modeline, &Modeline::directoryChangeRequested, m_file_panel, &Panel::setCurrentDir);

}

void Comet::ShowHelp() noexcept {

}

void Comet::setupCommandMap() noexcept {
    commandMap["rename"] = [this]() { RenameItems(); };
    commandMap["help"] = [this]() { ShowHelp(); };
    commandMap["delete"] = [this]() { DeleteItems(); };
    commandMap["mark"] = [this]() { m_file_panel->MarkItems(); };
    commandMap["toggle-mark"] = [this]() { m_file_panel->MarkOrUnmarkItems(); };
    commandMap["unmark"] = [this]() { m_file_panel->UnmarkItems(); };
    commandMap["new-file"] = [this]() { NewFile(); };
    commandMap["new-folder"] = [this]() { NewFolder(); };
    commandMap["trash"] = [this]() { TrashItems(); };
    commandMap["exit"] = [this]() { QApplication::quit(); };
    commandMap["toggle-hidden-files"] = [this]() { m_file_panel->ToggleHiddenFiles(); };
}

void Comet::ProcessMinibuffer(const QStringList& commandlist) noexcept {
    if (commandlist.isEmpty())
        return;

    QString command = commandlist[0];
    // QString arg = (commandlist.size() > 1) ? commandlist[1] : "";

    if (commandMap.contains(command)) {
        commandMap[command]();  // Call the associated function
    } else {
    }
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
    m_modeline = new Modeline();
    m_statusbar = new Statusbar();
    m_inputbar = new Inputbar();

    m_file_panel->setFocus();

    m_inputbar->hide();

    m_layout->setContentsMargins(0, 0, 0, 0);
    m_splitter->setContentsMargins(0, 0, 0, 0);
    m_widget->setContentsMargins(0, 0, 0, 0);
    m_widget->setLayout(m_layout);
    m_layout->addWidget(m_modeline);
    m_layout->addWidget(m_splitter);
    m_splitter->addWidget(m_file_panel);
    m_splitter->addWidget(m_preview_panel);
    m_layout->addWidget(m_inputbar);
    m_layout->addWidget(m_statusbar);
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
    QShortcut *kb_rename_items = new QShortcut(QKeySequence("Shift+r"), this);
    QShortcut *kb_delete_items = new QShortcut(QKeySequence("Shift+d"), this);
    QShortcut *kb_search = new QShortcut(QKeySequence("/"), this);
    QShortcut *kb_search_next = new QShortcut(QKeySequence("n"), this);
    QShortcut *kb_search_prev = new QShortcut(QKeySequence("Shift+n"), this);

    connect(kb_next_item, &QShortcut::activated, m_file_panel, &Panel::NextItem);
    connect(kb_prev_item, &QShortcut::activated, m_file_panel, &Panel::PrevItem);
    connect(kb_select_item, &QShortcut::activated, m_file_panel, &Panel::SelectItem);
    connect(kb_up_directory, &QShortcut::activated, m_file_panel, &Panel::UpDirectory);
    connect(kb_goto_last_item, &QShortcut::activated, m_file_panel, &Panel::GotoLastItem);
    connect(kb_goto_first_item, &QShortcut::activated, m_file_panel, &Panel::GotoFirstItem);
    connect(kb_mark_item, &QShortcut::activated, m_file_panel, &Panel::MarkOrUnmarkItems);
    connect(kb_command, &QShortcut::activated, this, &Comet::ExecuteExtendedCommand);
    connect(kb_rename_items, &QShortcut::activated, this, &Comet::RenameItems);
    connect(kb_delete_items, &QShortcut::activated, this, &Comet::DeleteItems);
    connect(kb_search, &QShortcut::activated, this, &Comet::Search);
    connect(kb_search_next, &QShortcut::activated, this, &Comet::SearchNext);
    connect(kb_search_prev, &QShortcut::activated, this, &Comet::SearchPrev);
}

void Comet::Search() noexcept {
    m_file_panel->Search(m_inputbar->getInput("Search"));
}

void Comet::SearchNext() noexcept {
    m_file_panel->SearchNext();
}

void Comet::SearchPrev() noexcept {
    m_file_panel->SearchPrev();
}

void Comet::RenameItems() noexcept {
    if (m_file_panel->RenameItems())
        m_statusbar->Message("Rename Successful!");
    else
        m_statusbar->Message("Error while renaming!", 5);
}

void Comet::DeleteItems() noexcept {
    if (m_file_panel->DeleteItems())
        m_statusbar->Message("Deletion Successful!");
    else
        m_statusbar->Message("Error while deleting!", 5);
}

void Comet::MoveItems() noexcept {
    if (m_file_panel->MoveItems())
        m_statusbar->Message("Deletion Successful!");
    else
        m_statusbar->Message("Error while moving!", 5);
}

void Comet::TrashItems() noexcept {
    if (m_file_panel->TrashItems())
        m_statusbar->Message("Deletion Successful!");
    else
        m_statusbar->Message("Error while moving!", 5);
}

void Comet::ExecuteExtendedCommand() noexcept {
    // m_minibuffer->ExecuteCommand();
}

void Comet::initMenubar() noexcept {
  m_menubar = new QMenuBar();
  this->setMenuBar(m_menubar);

  m_filemenu = new QMenu("File");

  m_filemenu__new_window = new QAction("New Window");
  m_filemenu__new_tab = new QAction("New Tab");
  m_filemenu__create_new_menu = new QMenu("Create New");

  m_filemenu__create_new_folder = new QAction("New Folder");
  m_filemenu__create_new_file = new QAction("New File");


  m_filemenu->addAction(m_filemenu__new_window);
  m_filemenu->addAction(m_filemenu__new_tab);
  m_filemenu->addMenu(m_filemenu__create_new_menu);

  m_filemenu__create_new_menu->addAction(m_filemenu__create_new_folder);
  m_filemenu__create_new_menu->addAction(m_filemenu__create_new_file);

  m_viewmenu = new QMenu("View");

  m_viewmenu__preview_panel = new QAction("Preview Panel");
  m_viewmenu__preview_panel->setCheckable(true);
  m_viewmenu__preview_panel->setChecked(true);

  m_viewmenu->addAction(m_viewmenu__preview_panel);

  m_tools_menu = new QMenu("Tools");

  m_tools_menu__search = new QAction("Search");
  m_tools_menu__command_in_folder = new QAction("Command in Folder");

  m_tools_menu->addAction(m_tools_menu__search);
  m_tools_menu->addAction(m_tools_menu__command_in_folder);

  m_menubar->addMenu(m_filemenu);
  m_menubar->addMenu(m_viewmenu);
  m_menubar->addMenu(m_tools_menu);

  connect(m_viewmenu__preview_panel, &QAction::triggered, this,
          &Comet::TogglePreviewPanel);

  connect(m_filemenu__create_new_file, &QAction::triggered, this,
          [&]() { this->NewFile(); });

  connect(m_filemenu__create_new_folder, &QAction::triggered, this,
          [&]() { this->NewFolder(); });

  connect(m_tools_menu__search, &QAction::triggered, this, [&]() {
      QString searchText = QInputDialog::getText(this, "Search", "Enter a file/directory name");
      if (!(searchText.isEmpty() && searchText.isNull()))
          m_file_panel->Search(searchText);
  });
}

bool Comet::createEmptyFile(const QString &filePath) noexcept {
  QFile file(filePath);

  if (file.open(QIODevice::WriteOnly)) {
    file.close();
    return true;
  }
  return false;
}

// Create an empty file
void Comet::NewFile(const int& nfiles) noexcept {
    // Interactive file creation
    if (nfiles == -1) {
        QString filename = QInputDialog::getText(this, "Create New File", "Enter the name for the file");

        if (!(filename.isEmpty() && filename.isNull())) {
            if (createEmptyFile(m_file_panel->getCurrentDir() + QDir::separator() + filename))
                m_statusbar->Message("File created successfully!");
            else
                m_statusbar->Message("Error creating file!");
        }
    }
}

// Create an empty folder
void Comet::NewFolder(const int& nfolders) noexcept {
    // Interactive folder creation
    if (nfolders == -1) {
        QString dirname = QInputDialog::getText(this, "Create Folder(s)", "Enter a folder name (slash separations are considered as sub-directories)");
        if (!(dirname.isEmpty() && dirname.isNull())) {
            QDir dir;
            if (dir.mkpath(m_file_panel->getCurrentDir() + QDir::separator() + dirname))
                m_statusbar->Message("Folders created successfully!");
            else
                m_statusbar->Message("Error creating folders!");
        }
    }
}

void Comet::setCurrentDir(QString path) { m_file_panel->setCurrentDir(path); }

Comet::~Comet() {}
