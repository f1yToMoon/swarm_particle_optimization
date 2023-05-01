#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <qlabel.h>
#include <QTimer>
#include <QGraphicsView>

#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg2);

    void on_spinBox_5_valueChanged(int arg4);

private:
    Ui::MainWindow *ui;    

    QPushButton pushButton;
    QSpinBox spinBox, spinBox_2, spinBox_5;
    QLabel label_5, label;

    int particles = 0, k = 0;
    float w = 0.729, c1 = 1.49445, c2 = 1.49445;
    int t_left = 0, t_right = 0;
    QList<QString> fun, x1, x2, id;
    vector<int> duration;
    vector<int> x_step = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<int> y_step = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

};

#endif // MAINWINDOW_H
