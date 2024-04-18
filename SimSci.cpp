#include <TH1D.h>
#include <TGraph.h>
#include <TVector3.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TRandom3.h>
#include <TArc.h>
#include <TApplication.h>
#include <TFile.h>
#include <TStopwatch.h>
#include <iostream>

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
  // ROOT
  TCanvas     *c;
  TRandom3  *rnd;
};

// Sezioni d'urto

// Disegno della geometria
void DrawGeom(double r, double dz){
  double framew = r*1.5;
  // Disegno il volume dello scintillatore
  par::c = new TCanvas("c","",10,10,900,320);
  par::c->Divide(3,1);
  //Vista X-Y
  par::c->cd(1);
  gPad->Range(-framew,-framew,framew,framew);
  TArc *arc = new TArc;
  arc->SetFillColor(11);
  arc->DrawArc(0.,0.,r);
  // Vista Z-Y
  par::c->cd(2);
  gPad->Range(-framew/2,-framew,framew+framew/2,framew);
  TGraph *grp = new TGraph;
  grp->SetPoint(0,0 ,-r);
  grp->SetPoint(1,dz,-r);
  grp->SetPoint(2,dz,r);
  grp->SetPoint(3,0 ,r);
  grp->SetFillColor(11);
  grp->Draw("F");
}

double CalcEnergyDeposition(TVector3 x0, TVector3 d, double E, TGraph*& grxy,TGraph*& grzy,bool fillgraph){
  // Ritorna l'energia deposita per singolo fotone con energia E,
  // punto di partenza x0 e direzione d
  
  double Edep = 0;
  do {
    
    //TODO calcolo mu
    
    //TODO calcolo cammino libero
    double s=0;
    
    // calcolo posizione 
    TVector3 x  = x0 + d*s;
    x0 = x;
    
    // verifico che non sia uscito dal rivelatore
    if (sqrt(x.X()*x.X()+x.Y()*x.Y())>par::r || x.Z()>par::dz || x.Z()<0)
      break;
    
    // grafica
    if (fillgraph){
      grxy->SetPoint(grzy->GetN(),x.X(),x.Y());
      grzy->SetPoint(grzy->GetN(),x.Z(),x.Y());
    }
    
    bool lfoto = true; // Vera se fotoelettrico (TODO)
    if (lfoto){
      //Fotoelettrico (TODO)
      // -> calcolo di Edep, E
      Edep = E;
      E = 0;
    } else {
      //Compton (TODO)
      // -> calcolo Edep,E, phi,theta
      double theta=0,phi=0;
      TVector3 dp;
      if (d.Z()!=1){
	dp.SetX(d.X()*cos(theta)+sin(theta)/sqrt(1-pow(d.Z(),2))*(d.X()*d.Z()*cos(phi)-d.Y()*sin(phi)));
	dp.SetY(d.Y()*cos(theta)+sin(theta)/sqrt(1-pow(d.Z(),2))*(d.Y()*d.Z()*cos(phi)+d.X()*sin(phi)));
	dp.SetZ(d.Z()*cos(theta)-sqrt(1-pow(d.Z(),2))*sin(theta)*cos(phi));
      } else {
	dp.SetX(sin(theta)*cos(phi));
	dp.SetY(sin(theta)*sin(phi));
	dp.SetZ(cos(theta));
      }
      d = dp;
    }
  } while (E!=0);

  return Edep;

}


void SimSci(string modus){
  // tre modalita'
  // detsimple - dettagliata semplificata
  // det       - dettagliata
  // bias      - bias della sorgente

  par::rnd = new TRandom3;
  par::rnd->SetSeed(time(NULL));
  

  DrawGeom(par::r,par::dz);  

  TH1D *hE = new TH1D("hE","",100,0,1.0);
  TStopwatch tstop;

  int naccept=0;
  for (int iev=0;iev<par::nev;iev++){

    double Edep = 0;

    TVector3 x0,u,d;

    double w = 1;
    if (modus=="detsimple"){
      // Punto d'ingresso (0,0,0) versore l'asse z
      x0 = TVector3(0,0,0);
      d  = TVector3(0,0,1);
    } else if (modus=="det"){
    } else if (modus=="bias"){
    }

    if (modus=="detsimple"){
      naccept++;
    } else if (modus=="det"){
    } else if (modus=="bias"){
    }
    
    TGraph *grxy,*grzy;
    bool fillgraph=false;
    if (naccept<=par::ngr){
      grxy = new TGraph();
      grxy->SetMarkerColor(naccept+1);
      grxy->SetMarkerStyle(20);
      grxy->SetMarkerSize(0.40);
      grzy = new TGraph(*grxy);
      grxy->SetPoint(grzy->GetN(),x0.X(),x0.Y());
      grzy->SetPoint(grzy->GetN(),x0.Z(),x0.Y());
      fillgraph = true;
      }
    
    Edep  = CalcEnergyDeposition(x0,d,par::en,grxy,grzy,fillgraph);
    if (Edep!=0){
      // Applico un risoluzione si 0.02 in quadratura con 0.04/sqrt(Edep)
	double Emeas = Edep;
        hE->Fill(Emeas,w);
    }
    
    //grafica
    if (naccept<=par::ngr){
      par::c->cd(1);
      grxy->Draw("PL");
      par::c->cd(2);
      grzy->Draw("PL");
    }

  }
  
  par::c->cd(3);
  hE->Draw("E");

  string file="output_";
  file+=modus;
  file+=".root";
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
