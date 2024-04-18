#include <cmath>
#include <TRandom3.h>

double GenKN(double E, TRandom3 *rnd){
  double me   = 0.511;
  double eps0 = 1/(1+2*E/me);
  double an1   = log(1/eps0);
  double an2   = (1/2.)*(1-eps0*eps0);
  double a1 = an1/(an1+an2);
  double a2 = an2/(an1+an2);

  double eps,eta2,geps;
  do {
    double eta1 = rnd->Rndm();
    if (rnd->Rndm() < a1){
      eps = exp(eta1*log(eps0));
    } else {
      eps = sqrt(eta1*(1-eps0*eps0)+eps0*eps0);
    }
    double cthe  = 1-me/E*(1/eps-1);
    double sthe2 = 1-cthe*cthe;
    eta2  = rnd->Rndm();
    geps  = (1-eps*sthe2/(1+eps*eps));
  } while (eta2>geps);
  double cthe = 1-me/E*(1/eps-1);
  return cthe;
}
