# Simple-Brake-C-


A simple brake simulator based on C++ and Qt. The resource material used is [Dynamik der Kraftzeug by Manfred](https://link.springer.com/book/10.1007/978-3-658-05068-9). 

## Usage
```
mkdir build
cd build
cmake ..
make
./SimpleBrakeSim
```

The brake parameters and car parameters can be changed in [Brake Parameters](src/BrakeParams.h) and [Car Parameters](src/CarParams.h). 


## To-Do
- [] Integrate simple degradation.
- [] Model simple dyamic environment temperature.
