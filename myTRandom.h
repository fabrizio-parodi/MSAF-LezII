#include <TRandom3.h>
class myTRandom: public TRandom3{
public:
  void KleinNishina(double E, double& Ep, double& theta);
};


