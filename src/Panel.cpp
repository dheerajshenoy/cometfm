#include "Panel.hpp"
#include <qabstractitemview.h>
#include <qdesktopservices.h>
#include <qevent.h>
#include <qitemselectionmodel.h>
#include <qnamespace.h>

Panel::Panel(QWidget *parent) : QWidget(parent) {
    this->setLayout(m_layout);
    m_layout->addWidget(m_list_view);
    m_list_view->setModel(m_model);
    m_model->index(0, 0);
    m_list_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_list_view->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    this->show();

    initSignalsSlots();
    initKeybinds();
    initContextMenu();
}

void Panel::initContextMenu() noexcept {

    m_context_action_open = new QAction("Open");
    m_context_action_open_with = new QAction("Open With");
    m_context_action_open_terminal = new QAction("Open Terminal Here");
    m_context_action_cut = new QAction("Cut");
    m_context_action_copy = new QAction("Copy");
    m_context_action_paste = new QAction("Paste");
    m_context_action_delete = new QAction("Delete");
    m_context_action_trash = new QAction("Trash");
    m_context_action_properties = new QAction("Properties");
}

void Panel::initKeybinds() noexcept {
}

void Panel::initSignalsSlots() noexcept {
    connect(m_list_view, &QListView::doubleClicked, this, &Panel::handleItemDoubleClicked);

    connect(m_model, &QFileSystemModel::directoryLoaded, this, [&]() {
        m_list_view->setCurrentIndex(m_model->index(0, 0, m_list_view->rootIndex()));
        m_list_view->setCurrentIndex(m_model->index(0, 0, m_list_view->rootIndex()));
      m_list_view->selectionModel()->select(m_list_view->currentIndex(), QItemSelectionModel::SelectionFlag::Select);
    });

    connect(m_list_view->selectionModel(), &QItemSelectionModel::currentChanged,
            this,
            [this](const QModelIndex &current, const QModelIndex &previous) {
              emit currentItemChanged(
                  m_current_dir + QDir::separator() +
                  m_model->data(current, Qt::DisplayRole).toString());
            });
}

QString Panel::getCurrentItem() noexcept {
    return m_current_dir + QDir::separator() + m_model->data(m_list_view->currentIndex()).toString();
}

void Panel::handleItemDoubleClicked(const QModelIndex& index) noexcept {
    QString filepath = m_model->filePath(index);
    if (m_model->isDir(index)) {
        setCurrentDir(filepath);
    } else {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filepath));
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
        m_model->setRootPath(path);
        m_list_view->setRootIndex(m_model->index(path));
        m_current_dir = path;

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

    QModelIndex nextIndex = m_model->index(nextRow, 0, m_list_view->rootIndex());
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
    QString filepath = m_model->filePath(currentIndex);
    if (m_model->isDir(currentIndex)) {
        setCurrentDir(filepath);
    } else {
      // TODO: handle File
      QDesktopServices::openUrl(QUrl::fromLocalFile(filepath));
    }

    // TODO: Add hook
}

void Panel::UpDirectory() noexcept {
    QString old_dir = m_current_dir;
    QDir currentDir(old_dir);

    if (currentDir.cdUp()) {
        m_current_dir = currentDir.absolutePath();
        m_list_view->setRootIndex(m_model->index(m_current_dir));
        m_list_view->setCurrentIndex(m_model->index(old_dir));
        emit afterDirChange(m_current_dir);
    }
    // TODO: Add hook
}

void Panel::MarkOrUnmarkItems() noexcept {
    auto indexes = m_list_view->selectionModel()->selectedIndexes();

    for (auto index : indexes) {
      if (m_model->data(index, static_cast<int>(Role::Marked))
              .toBool()) {
        m_model->setData(index, false, static_cast<int>(Role::Marked));
        m_model->removeMarkedFile(index);
      } else
        m_model->setData(index, true, static_cast<int>(Role::Marked));
    }
}

void Panel::MarkItems() noexcept {
    auto indexes = m_list_view->selectionModel()->selectedIndexes();

    for (auto index : indexes) {
        m_model->setData(index, true, static_cast<int>(Role::Marked));
    }
}

void Panel::UnmarkItems() noexcept {
    auto indexes = m_list_view->selectionModel()->selectedIndexes();

    for (auto index : indexes) {
        m_model->setData(index, false, static_cast<int>(Role::Marked));
        m_model->removeMarkedFile(index);
    }
}

void Panel::GotoFirstItem() noexcept {
    m_list_view->setCurrentIndex(m_model->index(0, 0, m_list_view->rootIndex()));
}

void Panel::GotoLastItem() noexcept {
    m_list_view->setCurrentIndex(m_model->index(m_model->rowCount(m_list_view->rootIndex()) - 1, 0, m_list_view->rootIndex()));
}

bool Panel::MoveItems() noexcept {
    // TODO: Move items
    return true;
}

bool Panel::TrashItems() noexcept {
    // TODO: trash items
    return true;
}

bool Panel::RenameItems() noexcept {
  if (m_model->hasMarks()) {
    // TODO: handle multiple rename
    return true;
  } else {
    // current selection single rename
    QModelIndex currentIndex = m_list_view->currentIndex();
    QString newFileName = QInputDialog::getText(this, "Rename", "Enter a new name for the file");

    // If user cancels or the new file name is empty
    if (newFileName.isEmpty() || newFileName.isNull())
        return false;

    return QFile::rename(getCurrentItem(), m_current_dir + QDir::separator() + newFileName);
  }

  return false;
}

bool Panel::DeleteItems() noexcept {
    if (m_model->hasMarks()) {
        return true;
    } else {
        if (m_model->isDir(m_list_view->currentIndex())) {
            QDir dir(getCurrentItem());
            return dir.removeRecursively();
        }
        else
          return QFile::remove(getCurrentItem());
    }
    return false;
}

void Panel::ToggleHiddenFiles() noexcept {
  m_hidden_files_shown = !m_hidden_files_shown;
  if (m_hidden_files_shown)
    m_model->setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
  else
    m_model->setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot |
                       QDir::Hidden);
}

void Panel::Search(const QString &searchExpression) noexcept {
    m_search_index_list = m_model->match(m_model->index(0, 0, m_list_view->rootIndex()), Qt::DisplayRole, searchExpression, -1, Qt::MatchRegularExpression
                                         );
    if (m_search_index_list.isEmpty())
        return;
    m_list_view->setCurrentIndex(m_search_index_list.at(0));
    m_search_index_list_index = 0;
}

void Panel::SearchNext() noexcept {
    m_search_index_list_index++;
    if (m_search_index_list_index >= m_search_index_list.size() - 1)
        m_search_index_list_index = 0;
    if (m_search_index_list_index < m_search_index_list.size())
        m_list_view->setCurrentIndex(m_search_index_list.at(m_search_index_list_index));
}

void Panel::SearchPrev() noexcept {
    m_search_index_list_index--;
    if (m_search_index_list_index < 0)
        m_search_index_list_index = m_search_index_list.size() - 1;
    m_list_view->setCurrentIndex(m_search_index_list.at(m_search_index_list_index));
}

void Panel::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    if (m_model->hasMarks()) {
      menu.addAction(m_context_action_open);
      menu.addAction(m_context_action_open_terminal);
      menu.addAction(m_context_action_open_with);
      menu.addAction(m_context_action_cut);
      menu.addAction(m_context_action_copy);
      menu.addAction(m_context_action_paste);
      menu.addAction(m_context_action_delete);
      menu.addAction(m_context_action_trash);
      menu.addAction(m_context_action_properties);
    } else {
      menu.addAction(m_context_action_open_terminal);
      menu.addAction(m_context_action_paste);
      menu.addAction(m_context_action_properties);
    }
      menu.exec(event->globalPos());
}
