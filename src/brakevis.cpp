#include "brakevis.h"
#include "./ui_brakevis.h"

BrakeVis::BrakeVis(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BrakeVis)
{
    timerId = startTimer(100);
    ui->setupUi(this);
    QLabel *accStatus = ui->AccelerationLabel;
    accStatus->clear();
    accStatus->setText("Car Speed:  " + QString::fromStdString(std::to_string(carSim.velocity)) + " kph");

    QLabel *thrtStatus = ui->Throtlepowerlabel;
    thrtStatus->clear();
    thrtStatus->setText("Throtle Power:  " + QString::fromStdString(std::to_string(carSim.throtlePower)) + " kW");

    QLabel *brkStatus = ui->Brakepowerlabel;
    brkStatus->clear();
    brkStatus->setText("Brake Power:  " + QString::fromStdString(std::to_string(carSim.brakePower)) + " kW");

    QLabel *tmpStatus = ui->Temperaturelabel;
    tmpStatus->clear();
    tmpStatus->setText("Brake Temperature:  " + QString::fromStdString(std::to_string(carSim.brakeTemperature)) + " K");
    UpdateBrakeColor();

    QLabel *brkdstStatus = ui->Brakedistancelabel;
    brkdstStatus->clear();
    brkdstStatus->setText("Braking Distance:  " + QString::fromStdString(std::to_string(carSim.brakingDistance)) + " m");

    QLabel *brktimeStatus = ui->BrakeTimelabel;
    brktimeStatus->clear();
    brktimeStatus->setText("Brake Time Predict:  " + QString::fromStdString(std::to_string(0.0)) + " sec");

    QLabel *brkpredStatus = ui->BrakeStoplabel;
    brkpredStatus->clear();
    brkpredStatus->setText("Brake Distance Predict:  " + QString::fromStdString(std::to_string(0.0)) + " m");
}

BrakeVis::~BrakeVis()
{
    killTimer(timerId);
    delete ui;
}

void BrakeVis::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
     {
        carSim.throtlePower += 20.0;
        carSim.throtlePower = std::min(carSim.throtlePower, carSim.maxThrotlePower);
        QLabel *thrtStatus = ui->Throtlepowerlabel;
        thrtStatus->clear();
        thrtStatus->setText("Throtle Power:  " + QString::fromStdString(std::to_string(carSim.throtlePower)) + " kW");
    }

    if(event->key() == Qt::Key_S)
    {
        carSim.throtlePower -= 20.0;
        carSim.throtlePower = std::max(carSim.throtlePower, 0.0);
        QLabel *thrtStatus = ui->Throtlepowerlabel;
        thrtStatus->clear();
        thrtStatus->setText("Throtle Power:  " + QString::fromStdString(std::to_string(carSim.throtlePower)) + " kW");
    }


    if(event->key() == Qt::Key_Down)
     {
        carSim.brakePower -= 20.0;
        carSim.brakePower = std::max(carSim.brakePower, 0.0);
        QLabel *brkStatus = ui->Brakepowerlabel;
        brkStatus->clear();
        brkStatus->setText("Brake Power:  " + QString::fromStdString(std::to_string(carSim.brakePower)) + " kW");
    }

    if(event->key() == Qt::Key_Up)
    {
        carSim.brakePower += 20.0;
        carSim.brakePower = std::min(carSim.brakePower, carSim.maxBrakePower);
        QLabel *brkStatus = ui->Brakepowerlabel;
        brkStatus->clear();
        brkStatus->setText("Brake Power:  " + QString::fromStdString(std::to_string(carSim.brakePower)) + " kW");
    }
}

void BrakeVis::timerEvent(QTimerEvent *event)
{
    double brakePowerStep = brakeSim.brakeResponseFunc(abs(carSim.brakePower - carSim.prevBrakePower));
    carSim.acceleration = (((carSim.throtlePower * 1000.0 * 0.1) - ((carSim.prevBrakePower + brakePowerStep) * 1000.0 * 0.1) - brakeSim.envParams.rollingResistance) / carSim.mass);
    carSim.velocity += carSim.acceleration * 0.1;
    if (carSim.velocity < 0.0)
    {
        carSim.brakePower = 0.0;
        carSim.velocity = 0.0;
    }

    carSim.distance += carSim.velocity * 0.1;

    if ( (carSim.brakePower > 0.0) || (carSim.brakePower > carSim.throtlePower) )
    {
        carSim.brakingDistance +=  carSim.velocity * 0.1;
    }
    else if (carSim.throtlePower > carSim.brakePower)
    {
        carSim.brakingDistance = 0.0;
    }

    if ((carSim.acceleration < 0.0))
    {
        updatePredicts = true;
        brakeTime = brakeSim.predictStopTime(carSim.velocity, carSim.acceleration);
        brakeDistance = brakeSim.predictStopDistance(carSim.velocity, carSim.acceleration);
    }
    else if (carSim.acceleration > 0.0)
    {
        brakeTime = 0.0;
        brakeDistance = 0.0;
        updatePredicts = false;
    }

    QLabel *lStatus = ui->AccelerationLabel;
    lStatus->clear();
    lStatus->setText("Car Speed:  " + QString::fromStdString(std::to_string(carSim.velocity)) + " kph");

    QLabel *brktimeStatus = ui->BrakeTimelabel;
    brktimeStatus->clear();
    brktimeStatus->setText("Brake Time Predict:  " + QString::fromStdString(std::to_string(brakeTime)) + " sec");

    QLabel *brkpredStatus = ui->BrakeStoplabel;
    brkpredStatus->clear();
    brkpredStatus->setText("Brake Distance Predict:  " + QString::fromStdString(std::to_string(brakeDistance)) + " m");

    QLabel *brkdstStatus = ui->Brakedistancelabel;
    brkdstStatus->clear();
    double brkDistForDisplay = (carSim.brakingDistance > 500.0) ? (carSim.brakingDistance / 1000.0) : carSim.brakingDistance;
    std::string brkUnitForDisplay = (carSim.brakingDistance > 500.0) ? " km" : " m";
    brkdstStatus->setText("Braking Distance:  " + QString::fromStdString(std::to_string(brkDistForDisplay)) + QString::fromStdString(brkUnitForDisplay));

    carSim.brakeTemperature = 237.1 + brakeSim.modUpdateTemp(0.1, carSim.brakePower);
    QLabel *tmpStatus = ui->Temperaturelabel;
    tmpStatus->clear();
    tmpStatus->setText("Brake Temperature:  " + QString::fromStdString(std::to_string(carSim.brakeTemperature)) + " K");
    UpdateBrakeColor();
    carSim.prevBrakePower = carSim.brakePower;
}

void BrakeVis::UpdateBrakeColor()
{
    cv::Mat cir = cv::imread("../fig/SimpBrake.png");
    cv::resize(cir, cir, cv::Size(190, 190));
    cv::Mat src_gray = cir.clone();
    cv::cvtColor(cir, src_gray, cv::COLOR_BGR2GRAY);
    cv::Mat cannyOutput;
    cv::blur(cir, cir, cv::Size(3,3));
    cv::Canny(cir, cannyOutput, 0.1, 0.5);
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours( cannyOutput, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
    cv::Mat drawingOutput = cv::Mat::zeros(cannyOutput.size(), CV_8UC4);

    cv::Mat cir_hsv = cir.clone();
    cv::cvtColor(cir_hsv, cir_hsv, cv::COLOR_BGR2HSV);
    cir_hsv *= 0.0;

    // H range 0 -> 171 (red to teal)
    // Temperature range maxBrakeTemperature -> 0 (hot to cold)
    double tempColorSlope = (colorMin - colorChangeSpeed) / (0.0 - brakeSim.brakeParams.maxMaterialTemperature);
    double b = colorChangeSpeed - (tempColorSlope * brakeSim.brakeParams.maxMaterialTemperature);
    cv::Scalar color_hsv_brake = cv::Scalar(std::max(std::min(tempColorSlope * (carSim.brakeTemperature + 237.0) + b, colorMin), colorMax), 255.0, 255.0);

    cv::drawContours(cir_hsv, contours, 2, color_hsv_brake, -1);

    cv::cvtColor(drawingOutput, drawingOutput, cv::COLOR_BGRA2RGBA);
    cv::cvtColor(cir_hsv, cir_hsv, cv::COLOR_HSV2RGB);
    cv::cvtColor(cir_hsv, cir_hsv, cv::COLOR_RGB2RGBA);

    uint8_t* pixelPtr = (uint8_t*)cir_hsv.data;
    uint8_t* destPtr = (uint8_t*)drawingOutput.data;
    int cn = cir_hsv.channels();

    for (size_t i = 0; i < cir_hsv.rows; i++)
    {
        for (size_t j = 0; j < cir_hsv.cols; j++) {
            if ((pixelPtr[i*cir_hsv.cols*cn + j*cn + 0]) == 0.0 && (pixelPtr[i*cir_hsv.cols*cn + j*cn + 1]) == 0.0 && (pixelPtr[i*cir_hsv.cols*cn + j*cn + 2]) == 0.0)
            {
                continue;
            }
            else
            {
                destPtr[i*cir_hsv.cols*cn + j*cn + 0] = pixelPtr[i*cir_hsv.cols*cn + j*cn + 0]; // B
                destPtr[i*cir_hsv.cols*cn + j*cn + 1] = pixelPtr[i*cir_hsv.cols*cn + j*cn + 1]; // G
                destPtr[i*cir_hsv.cols*cn + j*cn + 2] = pixelPtr[i*cir_hsv.cols*cn + j*cn + 2]; // R
                destPtr[i*cir_hsv.cols*cn + j*cn + 3] = 255.0;
            }
        }
    }

    cv::drawContours(drawingOutput, contours, 2, cv::Scalar(0.0, 0.0, 0.0, 255.0), 5);
    cv::drawContours(drawingOutput, contours, 19, cv::Scalar(0.0, 0.0, 0.0, 255.0), -1);
    cv::drawContours(drawingOutput, contours, 19, cv::Scalar(0.0, 0.0, 0.0, 255.0), 15);

    cv::drawContours(drawingOutput, contours, 10, cv::Scalar(0.0, 0.0, 0.0, 0.0), -1);
    cv::drawContours(drawingOutput, contours, 10, cv::Scalar(0.0, 0.0, 0.0, 255.0), 2);
    cv::drawContours(drawingOutput, contours, 15, cv::Scalar(0.0, 0.0, 0.0, 0.0), -1);
    cv::drawContours(drawingOutput, contours, 15, cv::Scalar(0.0, 0.0, 0.0, 255.0), 2);

    cv::drawContours(drawingOutput, contours, 5, cv::Scalar(0.0, 0.0, 0.0, 255.0), 2);
    cv::drawContours(drawingOutput, contours, 7, cv::Scalar(0.0, 0.0, 0.0, 0.0), -1);
    cv::drawContours(drawingOutput, contours, 7, cv::Scalar(0.0, 0.0, 0.0, 255.0), 2);
     cv::drawContours(drawingOutput, contours, 12, cv::Scalar(0.0, 0.0, 0.0, 0.0), -1);
    cv::drawContours(drawingOutput, contours, 12, cv::Scalar(0.0, 0.0, 0.0, 255.0), 2);
    cv::drawContours(drawingOutput, contours, 9, cv::Scalar(0.0, 0.0, 0.0, 0.0), -1);
    cv::drawContours(drawingOutput, contours, 9, cv::Scalar(0.0, 0.0, 0.0, 255.0), 2);
    cv::drawContours(drawingOutput, contours, 16, cv::Scalar(0.0, 0.0, 0.0, 0.0), -1);
    cv::drawContours(drawingOutput, contours, 16, cv::Scalar(0.0, 0.0, 0.0, 255.0), 2);
    ui->DiscLabel->clear();
    QPixmap pixMap = QPixmap::fromImage(QImage((unsigned char*) drawingOutput.data, drawingOutput.cols, drawingOutput.rows, QImage::Format_RGBA8888));
    ui->DiscLabel->setPixmap(pixMap);
}

