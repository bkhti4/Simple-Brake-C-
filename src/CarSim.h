#ifndef CARSIM_H
#define CARSIM_H


class CarSim
{
public:
    double initialVelocity = 16.66; // mps
    double velocity = 0.0; // mps
    double acceleration = 0.0; //mps^2
    double distance = 0.0; // m
    double mass = 16000; // kg
    double fR = 0.007;
    double lambda = 1.0;

    double brakePower = 0.0; // kW
    double prevBrakePower = 0.0; // kW
    double throtlePower = 0.0; // kW
    double maxBrakePower = 1500.0; // kW
    double maxThrotlePower = 1000.0; // kW
    double brakeTemperature = 237.0; // K
    double brakingDistance = 0.0; // m
};


#endif // CARSIM_H
