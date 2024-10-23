#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <ImageMagick-7/Magick++.h>
#include <QImage>
#include <QLabel>
#include <QMimeDatabase>
#include <QFile>
#include <QTextEdit>

class PreviewPanel : public QWidget {

public:
    PreviewPanel(QWidget *parent = nullptr);
    ~PreviewPanel();

    void Preview(const QString& filepath) noexcept;
    void loadImage(const QString& filepath);

private:
    QString readFirstFewLines(const QString &filePath, int lineCount = 5) noexcept;
    QString getMimeType( const QString& filePath ) {
        return QMimeDatabase().mimeTypeForFile( filePath ).name();
    }

    QVBoxLayout *m_layout = new QVBoxLayout();
    QLabel *m_img_preview_widget = new QLabel();
    QTextEdit *m_text_preview_widget = new QTextEdit();
};
