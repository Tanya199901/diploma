#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QColor>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pickImageButton_clicked();

private:
    Ui::MainWindow *ui;
    void showPreview(QPixmap *originalPixmap);
    void calculateHistogram(QPixmap *originalPixmap);
    void printArray(int array[], int length);
};
#endif // MAINWINDOW_H
