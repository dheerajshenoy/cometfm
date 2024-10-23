#pragma once

#include <QBrush>
#include <QFileSystemModel>
#include <QFont>
#include <QList>
#include <QMap>
#include <QModelIndex>

enum class Role {
  Marked = Qt::UserRole + 1,
};

class FileSystemModel : public QFileSystemModel {
  Q_OBJECT

public:
  explicit FileSystemModel(QObject *parent = nullptr)
      : QFileSystemModel(parent) {}

    void removeMarkedFile(const QModelIndex& index) {
        m_markedFiles.contains(index) && m_markedFiles.remove(index);
    }

    bool hasMarks() {
        return m_markedFiles.size() > 0;
    }

  QList<QString> getMarkedFiles() {
    QList<QString> markedFiles(m_markedFiles.size());
    for (auto i = m_markedFiles.cbegin(); i != m_markedFiles.cend(); i++) {
      markedFiles.append(this->fileInfo(i.key()).fileName());
    }

    qDebug() << markedFiles;

    return markedFiles;
  }

  // // Override flags to make items editable
  // Qt::ItemFlags flags(const QModelIndex &index) const override {
  //   Qt::ItemFlags defaultFlags = QFileSystemModel::flags(index);
  //   return defaultFlags | Qt::ItemIsEditable; // Make items editable
  // }

  // Override setData to support the custom role
  bool setData(const QModelIndex &index, const QVariant &value,
               int role) override {
    if (role == static_cast<int>(Role::Marked)) {
      // Store the marked status
      m_markedFiles[index] = value.toBool();
      // Emit dataChanged signal to update the view
      emit dataChanged(index, index, {role});
      return true;
    }

    // Fallback to the base class's setData method for other roles
    return QFileSystemModel::setData(index, value, role);
  }

  // Override data to return the current value for the custom role
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override {
    if (role == static_cast<int>(Role::Marked)) {
      // Return the marked status if available
      return m_markedFiles.value(index,
                                 false); // Default to false if not marked
    }

    // if (role == Qt::ForegroundRole) {
    //   // Change the color based on the marked status
    //   bool isMarked = m_markedFiles.value(index, false);
    //   if (isMarked) {
    //     return QBrush(QColor("blue")); // Change to blue for marked files
    //   }
    // }

    if (role == Qt::FontRole) {
      // Change font to bold for marked files
      bool isMarked = m_markedFiles.value(index, false);
      QFont font = QFileSystemModel::data(index, Qt::FontRole).value<QFont>();
      if (isMarked) {
        font.setBold(true); // Set font to bold
        font.setItalic(true);
      }
      return font; // Return the modified font
    }

    // Default behavior
    return QFileSystemModel::data(index, role);
  }

private:
  // Map to store the marked status of files
  QMap<QModelIndex, bool> m_markedFiles;
};