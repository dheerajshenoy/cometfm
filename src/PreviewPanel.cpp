#include "PreviewPanel.hpp"

PreviewPanel::PreviewPanel(QWidget *parent) : QWidget(parent) {
    this->setLayout(m_layout);
    m_layout->addWidget(m_img_preview_widget);
    m_layout->addWidget(m_text_preview_widget);
    this->setContentsMargins(0, 0, 0, 0);
    m_img_preview_widget->setAlignment(Qt::AlignmentFlag::AlignTop);
    m_img_preview_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_text_preview_widget->setAcceptRichText(true);
    m_text_preview_widget->setReadOnly(true);
}

QString PreviewPanel::readFirstFewLines(const QString &filePath, int lineCount) noexcept {
    QFile file(filePath);
    QString result;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for reading:" << file.errorString();
        return QString(); // Return an empty string on error
    }

    QTextStream in(&file);
    for (int i = 0; i < lineCount; ++i) {
        if (in.atEnd()) {
            break; // Stop if we reach the end of the file
        }
        QString line = in.readLine();
        result.append(line + "\n"); // Append line and newline to result
    }

    result.append("...");
    file.close();
    return result; // Return the accumulated lines as a QString
}

void PreviewPanel::loadImage(const QString &filePath) {
    Magick::InitializeMagick(nullptr);
    Magick::Image image;
    image.read(filePath.toStdString());

    if (!image.isValid())
        return;

    int width = image.columns(), height = image.rows();

    // Ensure the image is in RGB format for compatibility with QImage
    Magick::Blob blob;
    image.write(&blob, "RGBA");  // Convert to RGBA

    // Create a QImage using the raw data from the Magick::Image
    const uchar *data = reinterpret_cast<const uchar *>(blob.data());
    QImage qimage(data, width, height, QImage::Format_RGBA8888);

    m_img_preview_widget->setPixmap(QPixmap::fromImage(qimage.copy()).scaled(this->size().width(), 320, Qt::KeepAspectRatio, Qt::FastTransformation));
}

  void PreviewPanel::Preview(const QString &filepath) noexcept {
    QString mimetype = getMimeType(filepath);

    if (mimetype.startsWith("image/")) {
        m_text_preview_widget->setHidden(true);
        m_img_preview_widget->setHidden(false);
        loadImage(filepath);
    }

    else if (mimetype.startsWith("text/")) {
        m_img_preview_widget->setHidden(true);
        m_text_preview_widget->setHidden(false);
        m_text_preview_widget->setText(readFirstFewLines(filepath));
    }
    else {
        m_img_preview_widget->setHidden(true);
        m_text_preview_widget->setHidden(true);
        m_img_preview_widget->clear();
        m_text_preview_widget->clear();
    }
}

PreviewPanel::~PreviewPanel() {}
