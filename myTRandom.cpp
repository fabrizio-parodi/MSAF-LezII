#include "myTRandom.h"

void myTRandom::KleinNishina(double initialEnergy, double& scatteredEnergy, double& scatteringAngle) {

  double m_e   = 0.511; // MeV
    double eps0  = 1.0 / (1.0 + 2.0*initialEnergy/m_e);
    
    double eps, cosTheta, geps;
    
    do {
        // Campiona epsilon secondo il metodo di Kahn (1954)
        double eta1 = Rndm();
        double eta2 = Rndm();

	double alpha1 = -log(eps0);
	double alpha2 = (1-eps0*eps0)/2;
	double norm  = alpha1+alpha2;
	alpha1 = alpha1/norm;
	alpha2 = alpha2/norm;

        if (eta1 < alpha1) {
	  eps = exp(eta2*log(eps0));
        } else {
	  eps = sqrt(eps0*eps0+eta2*(1-eps0*eps0));
        }
        
        // Calcola cosTheta dalla relazione Compton
	cosTheta = 1.0 - m_e/initialEnergy*(1.0/eps - 1.0);
        
        // Assicura che cosTheta sia nel range fisico
        if (cosTheta < -1.0) cosTheta = -1.0;
        if (cosTheta > 1.0) cosTheta = 1.0;

	geps = 1 - eps*(1-cosTheta*cosTheta)/(1+eps*eps);
                
    } while (Rndm() > geps);
    
    scatteredEnergy = eps * initialEnergy;
    scatteringAngle = acos(cosTheta);
}
