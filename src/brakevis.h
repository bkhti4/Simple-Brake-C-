#ifndef BRAKEVIS_H
#define BRAKEVIS_H

#include <QMainWindow>
#include <QLabel>
#include <QKeyEvent>
#include <string>
#include <opencv2/opencv.hpp>
#include <thread>

#include "CarSim.h"
#include "BrakeSim.h"

QT_BEGIN_NAMESPACE
namespace Ui { class BrakeVis; }
QT_END_NAMESPACE


class BrakeVis : public QMainWindow
{
    Q_OBJECT

public:
    BrakeVis(QWidget *parent = nullptr);
    ~BrakeVis();


    CarSim carSim;
    BrakeSim brakeSim;
    double colorMax = 0.0; // color value when brake Temperature is at Max
    double colorMin = 100.0; // color value when brake Temperature is at Min
    double colorChangeSpeed = -40.0; // if < 0 moves faster
    bool updatePredicts = false;
    double brakeTime = 0.0;
    double brakeDistance = 0.0;

private:
    int timerId;
    Ui::BrakeVis *ui;

    void keyPressEvent(QKeyEvent *event) override;
    void UpdateBrakeColor();

protected:
    void timerEvent(QTimerEvent *event) override;
};
#endif // BRAKEVIS_H
