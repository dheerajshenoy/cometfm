#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QTimer>
#include <QSizePolicy>
#include <QFileInfo>
#include <QFile>
#include <QLocale>
#include <QVBoxLayout>

class Statusbar : public QWidget {

public:
    Statusbar(QWidget *parent = nullptr);
    ~Statusbar();

    void Message(const QString& message, int ms = 2) noexcept;
    void SetFile(const QString& filename) noexcept;

private:
    QHBoxLayout *m_layout = new QHBoxLayout();
    QVBoxLayout *m_vert_layout = new QVBoxLayout();
    QLabel *m_message_label = new QLabel();
    QLabel *m_file_name_label = new QLabel();
    QLabel *m_file_size_label = new QLabel();
    QLabel *m_file_perm_label = new QLabel();
    QLabel *m_file_modified_label = new QLabel();

    QLocale m_locale;
};