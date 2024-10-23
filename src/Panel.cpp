#include "Panel.hpp"

Panel::Panel(QWidget *parent) : QWidget(parent) {
    this->setLayout(m_layout);
    m_layout->addWidget(m_list_view);
    m_list_view->setModel(m_model);
    m_model->index(0, 0);
    m_list_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->show();

    initSignalsSlots();
    initKeybinds();
}

void Panel::initKeybinds() noexcept {

    QShortcut *kb_up_directory = new QShortcut(QKeySequence("h"), this);
    QShortcut *kb_next_item = new QShortcut(QKeySequence("j"), this);
    QShortcut *kb_prev_item = new QShortcut(QKeySequence("k"), this);
    QShortcut *kb_select_item = new QShortcut(QKeySequence("l"), this);
    QShortcut *kb_goto_first_item = new QShortcut(QKeySequence("g,g"), this);
    QShortcut *kb_goto_last_item = new QShortcut(QKeySequence("Shift+g"), this);
    QShortcut *kb_mark_item = new QShortcut(QKeySequence("Space"), this);

    connect(kb_next_item, &QShortcut::activated, this, &Panel::NextItem);
    connect(kb_prev_item, &QShortcut::activated, this, &Panel::PrevItem);
    connect(kb_select_item, &QShortcut::activated, this, &Panel::SelectItem);
    connect(kb_up_directory, &QShortcut::activated, this, &Panel::UpDirectory);
    connect(kb_goto_last_item, &QShortcut::activated, this, &Panel::GotoLastItem);
    connect(kb_goto_first_item, &QShortcut::activated, this, &Panel::GotoFirstItem);
    connect(kb_mark_item, &QShortcut::activated, this, &Panel::MarkOrUnmarkItem);
}

void Panel::initSignalsSlots() noexcept {
    connect(m_list_view, &QListView::doubleClicked, this, &Panel::handleItemDoubleClicked);
    connect(m_list_view, &QListView::clicked, this, [&](const QModelIndex &index) {
        m_list_view->setCurrentIndex(index);
    });

    connect(m_model, &QFileSystemModel::directoryLoaded, this, [&]() {
        m_list_view->setCurrentIndex(m_model->index(0, 0, m_list_view->rootIndex()));
    });

    connect(this, &Panel::beforeDirChange, this, [&]() {
    });

}

void Panel::handleItemDoubleClicked(const QModelIndex& index) noexcept {
    if (m_model->isDir(index)) {
        setCurrentDir(m_model->filePath(index));
    }
}


bool Panel::isValidPath(QString path) noexcept {
    QFileInfo file(path);
    return file.exists() && file.isReadable();
}

void Panel::setCurrentDir(QString path) noexcept {
    if (path.isEmpty())
        return;

    if (path.contains("~")) {
        path = path.replace("~", QDir::homePath());
    }

    if (isValidPath(path)) {
        emit beforeDirChange();
        m_model->setRootPath(path);
        m_current_dir = path;

        m_list_view->setRootIndex(m_model->index(path));
        emit afterDirChange(m_current_dir);
    }
}

QString Panel::getCurrentDir() noexcept {
    return m_current_dir;
}

Panel::~Panel() {}

///////////////////////////
// Interactive Functions //
///////////////////////////

void Panel::NextItem() noexcept {
    QModelIndex currentIndex = m_list_view->currentIndex();
    int nextRow = currentIndex.row() + 1;

    // Loop to start
    if (nextRow >= m_model->rowCount(m_list_view->rootIndex()))
        nextRow = 0;

    QModelIndex nextIndex = m_model->index(nextRow, currentIndex.column(), m_list_view->rootIndex());
    if (nextIndex.isValid()) {
        m_list_view->setCurrentIndex(nextIndex);
        m_list_view->scrollTo(nextIndex);
    }
    // TODO: Add hook
}

void Panel::PrevItem() noexcept {
    QModelIndex currentIndex = m_list_view->currentIndex();
    int prevRow = currentIndex.row() - 1;

    // Loop
    if (prevRow < 0)
        prevRow = m_model->rowCount(m_list_view->rootIndex()) - 1;

    QModelIndex prevIndex = m_model->index(prevRow, 0, m_list_view->rootIndex());

    if (prevIndex.isValid()) {
        m_list_view->setCurrentIndex(prevIndex);
        m_list_view->scrollTo(prevIndex);
    }
    //TODO: Add hook
}

void Panel::SelectItem() noexcept {
    QModelIndex currentIndex = m_list_view->currentIndex();
    if (m_model->isDir(currentIndex)) {
        setCurrentDir(m_model->filePath(currentIndex));
    }
    else {
        // TODO: handle File
    }

    // TODO: Add hook
}

void Panel::UpDirectory() noexcept {
    QDir currentDir(m_current_dir);

    if (currentDir.cdUp()) {
        emit beforeDirChange();
        m_current_dir = currentDir.absolutePath();
        m_list_view->setRootIndex(m_model->index(m_current_dir));
        emit afterDirChange(m_current_dir);
    }
    // TODO: Add hook
}

void Panel::MarkOrUnmarkItem() noexcept {
    QModelIndex currentIndex = m_list_view->currentIndex();
    currentIndex = m_model->index(currentIndex.row(), currentIndex.column(), m_list_view->rootIndex());
    if (m_model->data(currentIndex, static_cast<int>(Role::Marked)).toBool())
        m_model->setData(currentIndex, false, static_cast<int>(Role::Marked));
    else
        m_model->setData(currentIndex, true, static_cast<int>(Role::Marked));
}

void Panel::GotoFirstItem() noexcept {
    m_list_view->setCurrentIndex(m_model->index(0, 0, m_list_view->rootIndex()));
}

void Panel::GotoLastItem() noexcept {
    m_list_view->setCurrentIndex(m_model->index(m_model->rowCount(m_list_view->rootIndex()) - 1, 0, m_list_view->rootIndex()));
}