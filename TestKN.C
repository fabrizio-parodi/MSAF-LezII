
double genKN(double E, TRandom3 *rnd){
  double cthe = rnd->Rndm()*2-1;
  return cthe;
}

double kn(double *x, double *par){
  double me   = 0.511;
  //Klein-Nishina
  double E    = par[0];
  double cthe = x[0];
  double eps  = me/(me+E*(1-cthe));
  return par[1]*(eps*eps)*(eps+1/eps-(1-cthe*cthe));
}

void TestKN(){
  TRandom3 rnd;
  TCanvas *c = new TCanvas();
  TH1D *h = new TH1D("h","",100,-1,1);

  double E=0.1;
  
  for (int i=0;i<1000000;i++){
    //  Prende E e genera distribuzione in cos(theta)
    h->Fill(genKN(E,&rnd));
  }
  h->Draw();
  TF1 *f = new TF1("Klein Nishina",kn,-1,1,2);
  f->SetParameter(0,E);
  f->SetParameter(1,1);
  double fint = f->Integral(-1,1);
  f->SetParameter(1,1/fint*h->GetEntries()*h->GetBinWidth(1));
  f->Draw();
  h->Draw("SAME");
}
