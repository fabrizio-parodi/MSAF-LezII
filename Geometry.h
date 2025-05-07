#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <string>
#include <vector>
#include <TCanvas.h>
#include <TGraph.h>
#include <TVector3.h>

class Geometry {
public:
    Geometry(const std::string& type, int ngr = 100);
    
    void SetDimensions(double dim1, double dim2, double dim3 = 0);
    bool Contains(const TVector3& point);
    void Draw();
    void StartEvent(const TVector3& startPoint);
    void UpdatePosition(const TVector3& point);
    
private:
    std::string m_type;
    int m_eventCounter;
    int m_maxGraphPoints;
    TCanvas* m_canvas;
    double m_dimensions[3];
    std::vector<TGraph> m_xyGraphs;
    std::vector<TGraph> m_zyGraphs;
    
    void InitializeGraphs();
    void ClearGraphs();
};

#endif // GEOMETRY_H
