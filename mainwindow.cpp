#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    p_tiff = new myTIFF("MainWindow");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pickImageButton_clicked()
{
    QFileDialog dialog(this);
    QString fileName = dialog.getOpenFileName(this, "Select an image", "", "Image (*.jpeg *.png *.jpg *.bmp *.tiff *.tif)");
    QImage image(fileName);
    showPreview(&image);
    calculateHistogram(&image);
}

void MainWindow::showPreview(QImage *image)
{
    QSize picSize = ui->image->size();
    QPixmap originalPixmap = QPixmap::fromImage(*image);
    qDebug() << image->format();
    QPixmap scaledPixmap = originalPixmap.scaled(picSize, Qt::KeepAspectRatio);
    ui->image->setPixmap(scaledPixmap);
}

void MainWindow::calculateHistogram(QImage *image)
{
    qDebug() << "Start of calculation";

    int width = image->width();
    int height = image->height();

    long hist[256];
    prepareArray(hist, 256);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            QColor pixel = image->pixelColor(j, i);
            int brightness = pixel.lightness();
            hist[brightness]++;
        }
    }

    printArray(hist, 256);
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
        qDebug() << i << " : " << array[i];
    }
}

void MainWindow::on_pushButton_clicked()
{
    uint imageWidth;
    uint imageHeight;
    ushort * imageData, *tmp;
    QByteArray ba;
    QString fp = QFileDialog::getOpenFileName();
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
        QPixmap pixmap;
        pixmap.convertFromImage(image);
        ui->image->setPixmap(pixmap);

        delete [] imageData;
}
