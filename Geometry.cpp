#include "Geometry.h"
#include <TArc.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <iostream>
#include <cmath>

Geometry::Geometry(const std::string& type, int ngr) 
    : m_type(type), m_eventCounter(-1), m_maxGraphPoints(ngr), m_canvas(nullptr) {
    m_xyGraphs.resize(m_maxGraphPoints);
    m_zyGraphs.resize(m_maxGraphPoints);
}

Geometry::~Geometry() {
    if (m_canvas) {
        delete m_canvas;
    }
}

void Geometry::SetDimensions(double dim1, double dim2, double dim3) {
    if (m_type == "CYLINDER") {
        m_dimensions[0] = dim1; // radius
        m_dimensions[1] = dim2; // height
        // dim3 unused for cylinder
    }
}

void Geometry::Draw() {
    if (m_type != "CYLINDER") {
        std::cerr << "Error: Unsupported geometry type: " << m_type << std::endl;
        return;
    }

    const double radius = m_dimensions[0];
    const double height = m_dimensions[1];
    const double frameWidth = radius * 1.5;

    // Create and configure canvas
    m_canvas = new TCanvas("geometryCanvas", "Detector Geometry", 10, 10, 600, 320);
    m_canvas->Divide(2, 1);

    // Draw XY view (top view)
    m_canvas->cd(1);
    gPad->Range(-frameWidth, -frameWidth, frameWidth, frameWidth);
    TArc* circle = new TArc(0., 0., radius);
    circle->SetFillColor(11); // Light blue fill
    circle->Draw("F");

    // Draw ZY view (side view)
    m_canvas->cd(2);
    gPad->Range(-frameWidth/2, -frameWidth, height + frameWidth/2, frameWidth);
    
    TGraph* sideView = new TGraph(4);
    sideView->SetPoint(0, 0,    -radius); // Bottom left
    sideView->SetPoint(1, height, -radius); // Bottom right
    sideView->SetPoint(2, height, radius);  // Top right
    sideView->SetPoint(3, 0,     radius);  // Top left
    sideView->SetFillColor(11); // Light blue fill
    sideView->Draw("F");
}

void Geometry::StartEvent(const TVector3& startPoint) {
    m_eventCounter++;
    UpdatePosition(startPoint);
}

bool Geometry::Contains(const TVector3& point){
    if (m_type == "CYLINDER") {
        const double radialDistance = std::hypot(point.X(), point.Y());
        return (radialDistance < m_dimensions[0]) && 
               (point.Z() > 0) && 
               (point.Z() < m_dimensions[1]);
    }
    return false;
}

void Geometry::UpdatePosition(const TVector3& position) {
    if (m_eventCounter >= m_maxGraphPoints) {
        return;
    }

    // Update XY view graph
    m_xyGraphs[m_eventCounter].SetPoint(
        m_xyGraphs[m_eventCounter].GetN(), 
        position.X(), 
        position.Y()
    );

    // Update ZY view graph
    m_zyGraphs[m_eventCounter].SetPoint(
        m_zyGraphs[m_eventCounter].GetN(), 
        position.Z(), 
        position.Y()
    );

    // Only draw if we have points
    if (m_zyGraphs[m_eventCounter].GetN() > 0) {
        // Configure graph appearance
        const int color = m_eventCounter + 1;
        const double markerSize = 0.4;
        
        for (auto& graph : {&m_xyGraphs[m_eventCounter], &m_zyGraphs[m_eventCounter]}) {
            graph->SetMarkerColor(color);
            graph->SetMarkerStyle(20); // Small circle
            graph->SetMarkerSize(markerSize);
        }

        // Draw the graphs
        m_canvas->cd(1);
        m_xyGraphs[m_eventCounter].Draw("PL");
        
        m_canvas->cd(2);
        m_zyGraphs[m_eventCounter].Draw("PL");
    }
}
