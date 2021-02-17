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
//void QColor::getHsv(int *h, int *s, int *v, int *a = nullptr) const
    //int QColor::value() const

    int width = pixmap->width();
    int height = pixmap->height();

    int hist[255];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++){
//                hist++
             qDebug() << i << j;

        }
    }
}
