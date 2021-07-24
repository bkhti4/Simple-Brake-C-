#include <iostream>
#include <vector>
#include <math.h>

#ifndef BRAKEPARAMS_H
#define BRAKEPARAMS_H

class BrakeParams
{
public:

    std::string brakeDiscBaseMaterial = std::string("Carbon ceramic");
    std::string brakePadBaseMaterial = std::string("Carbon ceramic");
    double brakeMass = 189.4; // kg
    double specificHeatImpulse = 510.0; // J/kg*K
    double heatTransferCoefficient = 59.7; // W/m^2 * K
    double heatEmittingSurface = 1.82; // m^2
    double radiationEmittingSurface = 1.82; // m^2
    double radiationCoefficient = 5.77 * pow(10.0, -8.0); // W/m^2 * K^4
    double degreeOfAbsorption = 0.6;
    double timeToRespond = 0.00001; // sec
    double coolingRate = 0.1; // degrees/sec
    double maxMaterialTemperature = 1400; // K
    double brakeResponseDelaySlope = 10.0;
};

#endif // BRAKEPARAMS_H
