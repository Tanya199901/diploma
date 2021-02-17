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
    QPixmap pixmap(fileName);
    showPreview(&pixmap);
    calculateHistogram(&pixmap);
}

void MainWindow::showPreview(QPixmap *originalPixmap)
{
    QSize picSize = ui->image->size();
    QPixmap scaledPixmap = originalPixmap->scaled(picSize, Qt::KeepAspectRatio);
    ui->image->setPixmap(scaledPixmap);
}

void MainWindow::calculateHistogram(QPixmap *pixmap)
{
    qDebug() << "Start of calculation";

    QImage image = pixmap->toImage();

    int width = image.width();
    int height = image.height();

    int hist[255];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            QColor pixel = image.pixelColor(j, i);
            int brightness = pixel.value();
            int count = hist[brightness];
            int countWithThisPixel = count + 1;
            hist[brightness] = countWithThisPixel;
        }
    }

    printArray(hist, 256);
}

void MainWindow::printArray(int array[], int length)
{
    for(int i=0; i < length; i++)
    {
        qDebug() << i << " : " << array[i];
    }
}
