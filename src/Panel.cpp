#include "Panel.hpp"
#include <qfilesystemmodel.h>

Panel::Panel(QWidget *parent) : QWidget(parent) {
    this->setLayout(m_layout);
    m_layout->addWidget(m_list_view);
    m_list_view->setModel(m_model);
    m_model->index(0, 0);
    m_list_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->show();

    initSignalsSlots();
}

void Panel::initSignalsSlots() noexcept {
    connect(m_list_view, &QListView::doubleClicked, this, &Panel::handleItemDoubleClicked);
    connect(m_list_view, &QListView::clicked, this, [&](const QModelIndex &index) {
        m_list_view->setCurrentIndex(index);
    });

    connect(m_model, &QFileSystemModel::directoryLoaded, this, [&](const QString& path) {
        m_list_view->setRootIndex(m_model->index(path));
        m_list_view->setCurrentIndex(m_model->index(0, 0, m_list_view->rootIndex()));
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
        m_model->setRootPath(path);
        m_current_dir = path;
        emit dirChanged(m_current_dir);
    }
}

QString Panel::getCurrentDir() noexcept {
    return m_current_dir;
}

Panel::~Panel() {}

// Interactive Functions

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
        m_current_dir = currentDir.absolutePath();
        m_list_view->setRootIndex(m_model->index(m_current_dir));
        emit dirChanged(m_current_dir);
    }
    // TODO: Add hook
}

void Panel::keyPressEvent(QKeyEvent *e) {
    switch(e->key()) {
        case Qt::Key_H:
            UpDirectory();
            break;

        case Qt::Key_J:
            NextItem();
            break;

        case Qt::Key_K:
            PrevItem();
            break;

        case Qt::Key_L:
            SelectItem();
            break;
    }
}
