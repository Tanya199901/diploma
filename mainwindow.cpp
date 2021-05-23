#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorSpace>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    p_tiff = new myTIFF("MainWindow");
    this->setMouseTracking(true);
    this->ui->image->installEventFilter(this);
    qInfo() << "=== " << QDateTime::currentDateTime().toString(Qt::DateFormat::LocaleDate) << " ===";
    //ui->scrollArea->setWidget(ui->image);
    this->ui->image->adjustSize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showPreview(QImage *image)
{
    QSize picSize = ui->image->size();
    QPixmap originalPixmap = QPixmap::fromImage(*image);
    qDebug() << image->format();
    // QPixmap scaledPixmap = originalPixmap.scaled(picSize, Qt::KeepAspectRatio);
    //ui->image->setPixmap(scaledPixmap);
    ui->image->setPixmap(originalPixmap);
    ui->image->adjustSize();
}

void MainWindow::calculateHistogram(QImage *image, int x1, int y1, int x2, int y2)
{
    qInfo() << tr("Image [w: %1; h: %2]").arg(image->width()).arg(image->height());
    qDebug() << tr("Label w: %1; h: %2").arg(ui->image->width()).arg(ui->image->height());
    qInfo() << tr("Calculating histogram for: x1: %1; y1: %2; x2: %3; y2: %4")
                .arg(x1).arg(y1).arg(x2).arg(y2);

    long hist[256];
    prepareArray(hist, 256);
    for (int i = y1; i < y2; i++) {
        for (int j = x1; j < x2; j++) {
            QColor pixel = image->pixelColor(j, i);
            int brightness = pixel.lightness();
            hist[brightness]++;
        }
    }

    printArray(hist, 256);
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {

    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* me = static_cast<QMouseEvent*>(event);
        if (me) {
            qDebug() << tr("mouseEvent emitted: [x, y] = [%1, %2]") .arg(me->x()).arg(me->y());
            QPoint press(ui->image->pressLocation.x(), ui->image->pressLocation.y());
            QPoint release(ui->image->releaseLocation.x(), ui->image->releaseLocation.y());
            convertCoords(&press, &release);
            int min = INT32_MAX;
            int max = -1;
            for (int i = press.y(); i < release.y(); i++) {
                for (int j = press.x(); j < release.x(); j++) {
                    QColor pixel = handledImage->pixelColor(j, i);
                    int value = pixel.value();
                    if (value > max) {
                        max = value;
                    } else if (value < min) {
                        min = value;
                    }
                    //int brightness = pixel.lightness();
                }
            }
            qDebug() << "~~~ min: " << min << "; max: " << max;
            float coefA = 255.0F / (max - min);
            float coefB = -1.0F * ((255.0F * min) / (max - min));
            // qDebug() << "~~~ A: " << coefA << "; B: " << coefB;
            for (int i = press.y(); i < release.y(); i++) {
                for (int j = press.x(); j < release.x(); j++) {
                    QColor pixel = handledImage->pixelColor(j, i);
                    int newValue = pixel.value() * coefA + coefB;
                    // qDebug() << "~~~ oldV" << pixel.value() << "; newV: " << newValue;
                    pixel.setHsv(pixel.hsvHue(), pixel.hsvSaturation(), newValue);
                    handledImage->setPixelColor(j, i, pixel);
                    //int brightness = pixel.lightness();
                }
            }
            //showPreview(handledImage);
            ui->image->setPixmap(QPixmap::fromImage(*handledImage).scaled(ui->image->size(), Qt::KeepAspectRatio));
            calculateHistogram(handledImage, press.x(), press.y(), release.x(), release.y());
        }
        else
            qDebug() << tr("Стремное событие: %1");
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::prepareArray(long array[], int length)
{
    for(int i=0; i < length; i++)
    {
        array[i] = 0;
    }
}
void MainWindow::printArray(long array[], int length)
{
    for(int i=0; i < length; i++)
    {
        qInfo() << i << " : " << array[i];
    }
}

void MainWindow::on_pickFileButton_clicked()
{
    uint imageWidth;
    uint imageHeight;
    ushort * imageData, *tmp;
    QByteArray ba;
    QString fp = QFileDialog::getOpenFileName(this, "Select an image", "", "Image (*.tiff *.tif)");
    if (fp.isEmpty()) return;
    qInfo() << "Selected image: " << fp;
    const char *c_str;

    qDebug() << "MainWindow::1";
    ba = fp.toLocal8Bit();
    qDebug() << "MainWindow::2";
    c_str = ba.data();
    qDebug() << "MainWindow::3";
    tmp = p_tiff->openFile(c_str, &imageWidth, &imageHeight);
    qDebug() << "MainWindow::4 " << imageWidth << imageHeight;

    imageData = new ushort[imageWidth * imageHeight];
    memcpy(imageData, tmp, imageWidth * imageHeight * 2);
    qDebug() << "MainWindow::5";
    QImage image(imageWidth, imageHeight, QImage::Format_Indexed8);
    qDebug() << "MainWindow::6";
    quint8 * line = image.scanLine(0);
    int stride = image.bytesPerLine();
    qDebug() << "MainWindow::7";
    qint32 pixel = 0;
    for ( int y = 0; y < imageHeight; ++y, line += stride)
    {
        quint8 * pix = line;
        for ( int x = 0; x < imageWidth; ++x, pix += 1)
        {
            pixel  = imageData[(y*imageWidth)+x]/255;

            if (pixel > 255)
            {
                pixel = 255;
            }
            if (pixel < 0)
            {
                pixel = 0;
            }

            pix[0] = pixel;
        }
    }
    qDebug() << "MainWindow::8";
//    QPixmap pixmap;
//    pixmap.convertFromImage(image);
//    ui->image->setPixmap(pixmap);
    showPreview(&image);
//    image.convertToColorSpace(QColorSpace::SRgb);
    image.convertTo(QImage::Format_ARGB32_Premultiplied);
    qDebug() << "Image format" << image.format();
    sourceImage = image;
    handledImage = new QImage(sourceImage);
    calculateHistogram(
                &sourceImage,
                0,
                0,
                sourceImage.width(),
                sourceImage.height()
                );

    delete [] imageData;
}

void MainWindow::convertCoords(QPoint* press, QPoint* release)
{
    qDebug() << "convertCoords";
    float factor = 1;
    if (sourceImage.width() < sourceImage.height()) {
        // height
        qDebug() << "by height";
        factor = ((float) sourceImage.height()) / ui->image->pixmap()->height();
    } else {
        // width
        qDebug() << "by width";
        factor = ((float) sourceImage.width()) / ui->image->pixmap()->width();
    }
    qDebug() << "factor: " << factor;
    qDebug() << "pre: press: " << *press << "; release: " << *release;
    press->setX(press->x() * factor);
    press->setY(press->y() * factor);
    release->setX(release->x() * factor);
    release->setY(release->y() * factor);
    qDebug() << "post: press: " << *press << "; release: " << *release;
}

void MainWindow::on_clear_clicked()
{
    handledImage = new QImage(sourceImage);
    showPreview(&sourceImage);
}


void MainWindow::on_saveButton_clicked()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, "Save", "", "JPEG (*.jpeg);; TIFF(*.tif)");
    if (saveFileName.isEmpty()) return;
    handledImage->save(saveFileName);
}


void MainWindow::on_normalSizeButton_clicked()
{
    ui->image->setPixmap(QPixmap::fromImage(*handledImage));
    ui->image->adjustSize();
}

void MainWindow::on_fitButton_clicked()
{
    ui->image->resize(ui->scrollArea->size());
    ui->image->setPixmap(QPixmap::fromImage(*handledImage).scaled(ui->scrollArea->size(), Qt::KeepAspectRatio));
}


void MainWindow::on_zoomInButton_clicked()
{
    scaleImage(1.25F);
}


void MainWindow::on_zoomOutButton_clicked()
{
    scaleImage(0.8F);
}

void MainWindow::scaleImage(float factor)
{
    qDebug() << tr("Scale factor: %1").arg(factor);
    qDebug() << "Size: pre : " << ui->image->size();
    ui->image->resize(factor * ui->image->size());
    qDebug() << "Size: post: " << ui->image->size();
    if (ui->image->size().width() < ui->scrollArea->size().width()
            || ui->image->size().height() < ui->scrollArea->size().height()) {
        ui->image->resize(ui->scrollArea->size());
    }
    ui->image->setPixmap(QPixmap::fromImage(*handledImage).scaled(ui->image->size(), Qt::KeepAspectRatio));

    adjustScrollBar(ui->scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(ui->scrollArea->verticalScrollBar(), factor);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, float factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                                 + ((factor - 1) * scrollBar->pageStep()/2)));
}
