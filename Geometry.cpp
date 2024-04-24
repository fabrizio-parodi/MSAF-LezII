#include <iostream>
#include "Geometry.h"
#include <TArc.h>

void Geometry::Draw(){
  if (m_type=="CYLINDER"){
    
    double r  = m_dim[0];
    double dz = m_dim[1];
    
    double framew = r*1.5;

    // Frame
    m_c = new TCanvas("c","Detector",10,10,600,320);
    m_c->Draw();
    m_c->Divide(2,1);

    // X-Y View
    m_c->cd(1);
    gPad->Range(-framew,-framew,framew,framew);
    TArc *arc = new TArc;
    arc->SetFillColor(11);
    arc->DrawArc(0.,0.,r);

    // Z-Y View
    m_c->cd(2);
    gPad->Range(-framew/2,-framew,framew+framew/2,framew);
    TGraph *grp = new TGraph;
    grp->SetPoint(0,0 ,-r);
    grp->SetPoint(1,dz,-r);
    grp->SetPoint(2,dz, r);
    grp->SetPoint(3,0 , r);
    grp->SetFillColor(11);
    grp->Draw("F");

  }
}

void Geometry::Event(TVector3 x0){
  m_evt++;
  Update(x0);
}

void Geometry::SetDim(double a, double b, double c){
  if (m_type=="CYLINDER"){
    m_dim[0] = a; // radius
    m_dim[1] = b; // height
  }
}

bool Geometry::Contains(TVector3 r){
  bool test = false;
  if (m_type=="CYLINDER"){
    test = (sqrt(r.X()*r.X()+r.Y()*r.Y()) < m_dim[0] && r.Z()>0 && r.Z() < m_dim[1]);
  }
  return test;
}

void Geometry::Update(TVector3 x0){

  if (m_evt<m_ngr){

    m_grxy[m_evt].SetPoint(m_grxy[m_evt].GetN(),x0.X(),x0.Y());
    m_grzy[m_evt].SetPoint(m_grzy[m_evt].GetN(),x0.Z(),x0.Y());
    
    if (m_grzy[m_evt].GetN()>0){
      
      m_grxy[m_evt].SetMarkerColor(m_evt+1);
      m_grzy[m_evt].SetMarkerColor(m_evt+1);
      
      m_grxy[m_evt].SetMarkerStyle(20);
      m_grzy[m_evt].SetMarkerStyle(20);
      
      m_grxy[m_evt].SetMarkerSize(0.4);
      m_grzy[m_evt].SetMarkerSize(0.4);

      m_c->cd(1);
      m_grxy[m_evt].Draw("PL");
      m_c->cd(2);
      m_grzy[m_evt].Draw("PL");
      
    }
  }

}
