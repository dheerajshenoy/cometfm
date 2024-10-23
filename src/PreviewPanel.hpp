#pragma once

#include <QWidget>
#include <QVBoxLayout>
// #include <ImageMagick-7/Magick++.h>

class PreviewPanel : public QWidget {

public:
    PreviewPanel(QWidget *parent = nullptr);
    ~PreviewPanel();

    void setCurrentFile(const QString& path);

private:
    QVBoxLayout *m_layout = new QVBoxLayout();
};
