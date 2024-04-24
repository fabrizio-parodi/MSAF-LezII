#include "myTRandom.cpp"

double kn(double *x, double *par){
  double me   = 0.511;
  //Klein-Nishina
  double E    = par[0];
  double cthe = x[0];
  double eps  = me/(me+E*(1-cthe));
  return par[1]*(eps*eps)*(eps+1/eps-(1-cthe*cthe));
}

void TestKN(){
  myTRandom rnd;
  TCanvas *c = new TCanvas();
  TH1D    *h = new TH1D("h","",100,-1,1);

  double E=0.1;
  double Ep, theta;  
  for (int i=0;i<10000000;i++){
    //  Prende E e genera distribuzione in cos(theta)
    rnd.KleinNishina(E,Ep,theta);
    h->Fill(cos(theta));
  }
  h->Draw();
  TF1 *f = new TF1("Klein Nishina",kn,-1,1,2);
  f->SetParameter(0,E);
  f->SetParameter(1,1);
  double fint = f->Integral(-1,1);
  f->SetParameter(1,1/fint*h->GetEntries()*h->GetBinWidth(1));
  h->Draw();
  f->Draw("SAME");
}
