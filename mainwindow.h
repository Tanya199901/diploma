#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QColor>
#include <QScrollBar>

#include "service_functions.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    myTIFF *p_tiff;

private slots:
    void on_clear_clicked();

    void on_pickFileButton_clicked();

    void on_saveButton_clicked();

    void on_normalSizeButton_clicked();

    void on_fitButton_clicked();

    void on_zoomInButton_clicked();

    void on_zoomOutButton_clicked();

    void on_contrastButton_clicked();

private:
    Ui::MainWindow *ui;
    QImage sourceImage;
    QImage * handledImage;
    void showPreview(QImage *image);
    void calculateHistogram(QImage *image, int x1, int y1, int x2, int y2);
    void printArray(long array[], int length);
    void prepareArray(long array[], int length);
    bool eventFilter(QObject* obj, QEvent* event);
    void convertCoords(QPoint* press, QPoint* release);
    void scaleImage(float factor);
    void adjustScrollBar(QScrollBar *scrollBar, float factor);
    void contrastArea(QPoint a, QPoint b);
};
#endif // MAINWINDOW_H
