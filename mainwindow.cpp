#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pickImageButton_clicked()
{
    QFileDialog dialog(this);
    QString fileName = dialog.getOpenFileName(this, "Select an image", "", "Image (*.jpeg *.png *.jpg *.bmp)");
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
