#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QWidget>

class Comet : public QMainWindow {
  Q_OBJECT

public:
  Comet(QWidget *parent = nullptr);
  ~Comet();
  void setCurrentDir(QString path);

signals:
  void dirChanged(QString path);

private:
  void initLayout() noexcept;
  void initMenubar() noexcept;
  void initStatusbar() noexcept;
  void initSignalsSlots() noexcept;
  bool isValidPath(QString path);

  QDir m_current_path;

  QWidget *m_widget = new QWidget();
  QVBoxLayout *m_layout = new QVBoxLayout();
  QMenuBar *m_menubar = nullptr;
  QMenu *m_filemenu = nullptr;
  QMenu *m_filemenu__create_new_menu = nullptr;

  QAction *m_filemenu__new_window = nullptr;
  QAction *m_filemenu__new_tab = nullptr;
  QAction *m_filemenu__create_new_folder = nullptr;
  QAction *m_filemenu__create_new_file = nullptr;
};