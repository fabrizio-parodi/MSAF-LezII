#include "myTRandom.h"

void myTRandom::KleinNishina(double E, double& Ep, double& theta){
  
  double me   = 0.510998950;
  
  double eps0       = 1/(1+2*E/me);
  double a1_unnorm  = log(1/eps0);
  double a2_unnorm  = (1/2.)*(1-eps0*eps0);
  double a1         = a1_unnorm/(a1_unnorm+a2_unnorm);

  double eps,eta2,geps, cthe;
  do {
    double eta1 = Rndm();
    if (Rndm() < a1){
      eps = exp(eta1*log(eps0));
    } else {
      eps = sqrt(eta1*(1-eps0*eps0)+eps0*eps0);
    }
    cthe  = 1-me/E*(1/eps-1);
    double sthe2 = 1-cthe*cthe;
    eta2  = Rndm();
    geps  = (1-eps*sthe2/(1+eps*eps));
  } while (eta2>geps);

  Ep = eps*E;
  theta = acos(cthe);
  
  return;
}
