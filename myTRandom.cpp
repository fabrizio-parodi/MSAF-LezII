#include "myTRandom.h"

void myTRandom::KleinNishina(double initialEnergy, double& scatteredEnergy, double& scatteringAngle) {

    double eps0 = 1/(1+2*electronMass/initialEnergy);

    double a1_unnorm = -log(eps0);
    double a2_unnorm = (1-eps0 * eps0)/2;
    double a1 = a1_unnorm / (a1_unnorm + a2_unnorm);

    double eta1, eta2, eps, geps, cosTheta;
    do {
      
      eta1 = Rndm();
      eta2 = Rndm();
      
      if (Rndm() < a1) {
	eps = exp(eta1 * log(eps0));
      } else {
	eps = sqrt(eta1 * (1 - eps0*eps0) + eps0*eps0);
      }
      
      cosTheta = 1 - electronMass/initialEnergy*(1/eps-1);
      geps = ( 1 - eps * (1 - cosTheta*cosTheta) / (1 + eps * eps));
	
    } while (eta2 > geps);
    
    scatteredEnergy = eps * initialEnergy;
    scatteringAngle = acos(cosTheta);
}
