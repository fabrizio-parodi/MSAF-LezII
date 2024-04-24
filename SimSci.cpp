#include <TH1D.h>
#include <TGraph.h>
#include <TVector3.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TArc.h>
#include <TApplication.h>
#include <TFile.h>
#include <TStopwatch.h>
#include <iostream>
#include "myTRandom.h"
#include "Geometry.h"

using namespace std;

namespace par{
  // Parametri dello scintillatore (NaI)
  double me      = 0.511;
  double rho     = 3.67; // g/cm^3
  double cm      = 0.01;
  // Volume scintillatore cilindro di raggio r con asse z, una base a z=0, l'altra a z=dz
  double r       = 0.02;
  double dz      = 0.025;
  double dist    = 0.3;
  double coslim  = dist/sqrt(dist*dist+r*r);
  // Fotone
  // Energia
  double en      = 0.6617;
  // Numero di eventi
  int    nev     = 100000;
  // Massimo numero di fotoni graficati
  int    ngr     = 100;
  // ROOT & utilities
  myTRandom   *rnd;
  Geometry    *geo;
};


TVector3 GetDirection(TVector3 d, double theta, double phi){

  TVector3 dp;
  if (d.Z()!=1){
    dp.SetX(d.X()*cos(theta)+sin(theta)/sqrt(1-pow(d.Z(),2))*(d.X()*d.Z()*cos(phi)-d.Y()*sin(phi)));
    dp.SetY(d.Y()*cos(theta)+sin(theta)/sqrt(1-pow(d.Z(),2))*(d.Y()*d.Z()*cos(phi)+d.X()*sin(phi)));
    dp.SetZ(d.Z()*cos(theta)-sqrt(1-pow(d.Z(),2))*sin(theta)*cos(phi));
  } else if (d.Z()==1) {
    dp.SetX(sin(theta)*cos(phi));
    dp.SetY(sin(theta)*sin(phi));
    dp.SetZ(cos(theta));
  } else if (d.Z()==-1) {
    dp.SetX(sin(theta)*cos(phi+TMath::Pi()));
    dp.SetY(sin(theta)*sin(phi+TMath::Pi()));
    dp.SetZ(-cos(theta));
  }
  return dp;

}


TGraph grPhoto("NaiPhoto.data");
TGraph grCompton("NaiCompton.data");

double CalcEnergyDeposition(TVector3 x0, TVector3 d, double E){
  // Return the energy deposition
  
  double Edep = 0;
  do {
    
    //TODO-I compute mu
    
    //TODO-II compute free path
    double s;
    
    // calcolo posizione 
    TVector3 x  = x0 + d*s;
    x0 = x;
    
    // check that the particle is still in the detector
    if (!par::geo->Contains(x))
      break;
    //Update graphics
    par::geo->Update(x);

    //TODO-III choose interaction
    bool lfoto = true; // True if Photoelectric
    if (lfoto){
      Edep = E;
      E = 0;
    } else {
      //TODO-IV
      //Edep,E, phi,theta
      double theta=0,phi=0;
      d = GetDirection(d,theta,phi);
    }
  } while (E!=0);

  return Edep;

}

void SimSci(string modus){
  
  // Three options
  // detsimple - particle direction along z-axis
  // det       - particle issued uniformly
  // bias      - source bias

  par::rnd = new myTRandom;
  par::rnd->SetSeed(time(NULL));
  
  par::geo = new Geometry("CYLINDER");
  par::geo->SetDim(par::r,par::dz);  
  par::geo->Draw();

  TH1D *hE = new TH1D("hE","",100,0,1.0);
  TStopwatch tstop;

  for (int iev=0;iev<par::nev;iev++){
    // Particle direction
    TVector3 x0,d;
    // Event weight
    double w = 1;
    if (modus=="detsimple"){
      // Punto d'ingresso (0,0,0) versore l'asse z
      x0 = TVector3(0,0,0);
      d  = TVector3(0,0,1);
    } else if (modus=="det"){
    } else if (modus=="bias"){
    }
    
    par::geo->Event(x0);
    double Edep  = CalcEnergyDeposition(x0,d,par::en);
    
    if (Edep!=0){
      // TODO-V
      // Energy resolution
      double Emeas = Edep;
      hE->Fill(Emeas,w);
    }
  }
  
  TCanvas *c = new TCanvas("h","Energy",620,10,320,320);
  c->Draw();
  
  hE->Draw("E");
  
  string file="output_"; file+=modus; file+=".root";
  TFile f(file.c_str(),"recreate");
  hE->Write();
  f.Close();

}

#ifndef __CINT__
int main(int argc,char* argv[]){
  if (argc!=2){
    cout << "Devi specificare un parametro" << endl;
    cout << " detsimple  - simulazione dettagliata semplificata (fotone lungo z) " << endl;
    cout << " det        - simulazione dettagliata con direzione corretta " << endl;
    cout << " bias       - simulazione con bias della sorgente " << endl;
    return 1;
  }
  TApplication app("app",0,NULL);

  SimSci(argv[1]);

  gPad->Update();
  app.Run(true);

  return 0;
}
#endif
