#include "BrakeParams.h"
#include "CarSim.h"
#include "EnvParams.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <unistd.h>

#ifndef BRAKESIM_H
#define BRAKESIM_H


class BrakeSim
{
public:
    double currentTemp = 27.0; // degrees
    double changeTemp = 0.0; // degrees
    double dotChangeTemp = 0.0;

    double prevPower = 0.0; // kW
    double brakePower = 0.0; // kW

    double totalTime = 0.0;
    float discDiameter = 0.1; //meters
    double totalDamage = 0.0;
    double brakingPower = 0.0; // kW

    BrakeParams brakeParams;
    CarSim carParams;
    EnvParams envParams;

    double modUpdateTemp(double deltaTime, double appliedPower);
    double modUpdateBrakePower(double deltaTime);
    bool modCheckBrakeDuration();

    void updateCorrosion(double Temp);

    void printAll();
    double brakeResponseFunc(double brakePowerDelta);
    double predictStopTime(double vi, double acc);
    double predictStopDistance(double vi, double acc);
};

#endif // BRAKESIM_H
