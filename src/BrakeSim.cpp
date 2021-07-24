#include "BrakeSim.h"
#include "CarSim.h"

double BrakeSim::modUpdateBrakePower(double deltaTime)
{
    carParams.distance += (carParams.initialVelocity + (carParams.acceleration * deltaTime)) * deltaTime;
    double appliedPower = (carParams.fR + envParams.roadSlope + carParams.lambda * (carParams.acceleration / 9.81)) * (carParams.mass * 9.81) * (carParams.initialVelocity + (carParams.acceleration * totalTime));
    carParams.velocity = (carParams.initialVelocity + (carParams.acceleration * totalTime));
    prevPower = appliedPower;
    return appliedPower;
}


double BrakeSim::modUpdateTemp(double deltaTime, double appliedPower)
{
    dotChangeTemp = ((abs(appliedPower) * 1000.0) / (brakeParams.specificHeatImpulse * brakeParams.brakeMass))
                    - ((brakeParams.heatTransferCoefficient * brakeParams.heatEmittingSurface) / (brakeParams.specificHeatImpulse * brakeParams.brakeMass)) * changeTemp;                // (degreeOfAbsorption * radiationCoefficient * radiationEmittingSurface)
    changeTemp += (dotChangeTemp * deltaTime);
    //std::cout << changeTemp << std::endl;
    prevPower = appliedPower;
    totalTime += deltaTime;
    return changeTemp;
}


void BrakeSim::updateCorrosion(double Temp)
{
    totalDamage += (0.1 * (Temp - 300.0));
}

void BrakeSim::printAll()
{
    std::cout << "Total Time: " << totalTime << " sec " << std::endl;
    std::cout << "Current Velocity: " << carParams.velocity << " mps " << std::endl;
    std::cout << "Current Acceleration: " << carParams.acceleration << " mps^2 " << std::endl;
    std::cout << "Total distance: " << carParams.distance << " m " << std::endl;
    std::cout << "Brake Temperature: " << changeTemp << " K " << std::endl;
    std::cout << "Current Pressure on Brake Disc: " << prevPower << " kW " << std::endl;
    std::cout << "Total damage: " << totalDamage << std::endl;
    std::cout << "##################################" << std::endl;
}


double BrakeSim::brakeResponseFunc(double brakePowerDelta)
{
    return (brakePowerDelta / brakeParams.brakeResponseDelaySlope);
}


double BrakeSim::predictStopTime(double vi , double acc)
{
    return (- vi / acc);
}


double BrakeSim::predictStopDistance(double vi , double acc)
{
    return ( (- (vi * vi)) / (2 * acc) );
}
