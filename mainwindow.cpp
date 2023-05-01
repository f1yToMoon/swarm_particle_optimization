#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

float f(float x1, float x2) {
    float res = 4 * pow(x1 - 5, 2) + pow(x2 - 6, 2);
    return res;
}

void printGraph() {
    std::vector<std::vector<double>> x, y, z;
    for (double i = -100; i <= 100;  i += 0.25) {
        std::vector<double> x_row, y_row, z_row;
        for (double j = -100; j <= 100; j += 0.25) {
            x_row.push_back(i);
            y_row.push_back(j);
            z_row.push_back(f(i, j));
        }
        x.push_back(x_row);
        y.push_back(y_row);
        z.push_back(z_row);
    }

    plt::plot_surface(x, y, z);
    plt::save("/home/nick/2.png");
    plt::cla();
    plt::clf();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    printGraph();

    srand (static_cast <unsigned> (time(0)));

    ui->setupUi(this);

    ui->spinBox->setValue(0);
    ui->spinBox->setMinimum(-10);

    ui->spinBox_2->setValue(10);
    ui->spinBox_2->setMaximum(20);

    ui->spinBox_5->setMinimum(100);
    ui->spinBox_5->setMaximum(1000);
    ui->spinBox_5->setValue(100);
    ui->spinBox_5->setSingleStep(100);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    duration.clear();
    k = 0;
    vector<float> x, y;
    vector<pair<vector<float>, vector<float>>> vizualize;

    pair<float, float> globalBestPos;
    vector<pair<pair<pair<float, float>, pair<float, float>>, pair<float, pair<float, float>>>> init;

    //initialization
    for (int i = 0; i < particles; ++i) {
        init.push_back(make_pair(make_pair(make_pair(0, 0), make_pair(0, 0)), make_pair(0, make_pair(0, 0))));
        init[i].first.first.first = t_left + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (t_right - t_left)));
        init[i].first.first.second = t_left + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (t_right - t_left)));
        init[i].first.second.first = t_left + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (t_right - t_left)));
        init[i].first.second.second = t_left + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (t_right - t_left)));
        init[i].second.first = f(init[i].first.first.first, init[i].first.first.second);
        init[i].second.second.first = init[i].first.first.first;
        init[i].second.second.second = init[i].first.first.second;
    }

    sort(init.begin(), init.end(), [](auto& left, auto& right) {
        return left.second.first < right.second.first;
    });

    globalBestPos = make_pair(init[0].second.second.first, init[0].second.second.second);

    while (f(globalBestPos.first, globalBestPos.second) != 0) {

        k++; x.clear(); y.clear();

        for (int j = 0; j < particles; ++j) {
            float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1)));
            float r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1)));
            init[j].first.second.first = w * init[j].first.second.first + c1 * r1 * (init[j].second.second.first - init[j].first.first.first) + c2 * r * (globalBestPos.first - init[j].first.first.first);
            r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1)));
            r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1)));
            init[j].first.second.second = w * init[j].first.second.second + c1 * r1 * (init[j].second.second.second - init[j].first.first.second) + c2 * r * (globalBestPos.second - init[j].first.first.second);
        }

        for (int j = 0; j < particles; ++j) {
            init[j].first.first.first += init[j].first.second.first;
            init[j].first.first.second += init[j].first.second.second;
        }

        for (int j = 0; j < particles; ++j) {
            if (f(init[j].first.first.first, init[j].first.first.second) < f(init[j].second.second.first, init[j].second.second.second)) {
                init[j].second.second.first = init[j].first.first.first;
                init[j].second.second.second = init[j].first.first.second;
            }
            init[j].second.first = f(init[j].second.second.first, init[j].second.second.second);
        }

        sort(init.begin(), init.end(), [](auto& left, auto& right) {
            return left.second.first < right.second.first;
        });


        globalBestPos = make_pair(init[0].second.second.first, init[0].second.second.second);


        for (int j = 0; j < particles; ++j) {
            x.push_back(init[j].first.first.first);
            y.push_back(init[j].first.first.second);
        }
        vizualize.push_back(make_pair(x, y));

    }

    int timeCount = vizualize.size();
    for (int i = 0 ; i < timeCount; ++i) {
        duration.push_back(i*1000);
    }

    for (int i = 0; i < timeCount; ++i) {
        QTimer * timer = new QTimer(this);
        timer->setSingleShot(true);
        connect(timer, &QTimer::timeout, [=](){
            plt::scatter(vizualize[i].first, vizualize[i].second);
            string s = "/home/nick/1.png";
            plt::xlabel("x1");
            plt::ylabel("x2");
            plt::xticks(x_step);
            plt::yticks(y_step);
            plt::title(to_string(i + 1));
            plt::save(s);
            ui->label->setPixmap(QPixmap::fromImage(QImage(QString::fromStdString(s))));
            plt::cla();
        timer->deleteLater();
        });

        timer->start(duration[i]);

    }

    ui->label_5->setText(QString(QString::fromStdString("Количество итераций: " + to_string(k))));


}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    t_left = arg1;
}

void MainWindow::on_spinBox_2_valueChanged(int arg2)
{
    t_right = arg2;
}

void MainWindow::on_spinBox_5_valueChanged(int arg4)
{
    particles = arg4;
}


