#ifndef MY_TRANDOM_H
#define MY_TRANDOM_H

#include <TRandom3.h>

class myTRandom : public TRandom3 {
public:
    void KleinNishina(double initialEnergy, double& scatteredEnergy, double& scatteringAngle);
    
private:
    static constexpr double electronMass = 0.510998950; // MeV/c²
};

#endif // MY_TRANDOM_H


