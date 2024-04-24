#ifndef _GEOMETRY
#define _GEOMETRY

#include <string>
#include <TCanvas.h>
#include <TGraph.h>
#include <TVector3.h>

class Geometry{
public:
  Geometry(std::string type, int ngr=100){
    m_evt  = -1;
    m_ngr  =  ngr;
    m_type =  type;
    m_grxy.resize(ngr);
    m_grzy.resize(ngr);
  }
  void SetDim(double, double, double=0);
  bool Contains(TVector3);
  void Draw();
  void Event(TVector3);
  void Update(TVector3);
private:
  std::string m_type;
  int         m_evt,m_ngr;
  TCanvas    *m_c;
  double      m_dim[3];
  std::vector<TGraph> m_grxy;
  std::vector<TGraph> m_grzy;
};

#endif
