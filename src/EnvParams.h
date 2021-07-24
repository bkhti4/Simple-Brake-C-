#include<iostream>
#include <vector>

#ifndef ENVPARAMS_H
#define ENVPARAMS_H


class EnvParams{
private:
    std::vector<double> windSpeed = {0.0, 1.0, 2.5, 4.25, 6.25, 9.5, 12.5, 15.25, 18.5, 22, 25.5, 29.25, 35}; // (m/s) wind speeds from calm -> hurricane
    double ambientTemp = 27.0; // degrees

public:
    double roadSlope = 0.0;
    double rollingResistance = 0.002; // N

};


#endif // ENVPARAMS_H
