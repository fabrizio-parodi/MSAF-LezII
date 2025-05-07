#include <iostream>

#include "Geometry.h"
#include "myTRandom.h"

#include <TH1D.h>
#include <TFile.h>
#include <TApplication.h>
#include <TStopwatch.h>

namespace {

  // Detector parameters
  struct DetectorParams {
    double density        = 3.67;     // g/cm³ (NaI)
    double radius         = 0.02;     // m
    double length         = 0.025;    // m
  };

  struct SourceParams {
    double sourceDistance = 0.3;      // m
    double energy         = 0.6617;   // MeV (Cs-137)
  };
  
  struct RunParams {
    int numEvents         = 100000;
    int maxGraphPoints    = 100;
  };
    
  DetectorParams det;
  SourceParams   src;
  RunParams      run;
  myTRandom* randomGenerator;
  Geometry* detectorGeometry;

}

TVector3 CalculateNewDirection(const TVector3& currentDirection, double theta, double phi) {
    TVector3 newDirection;
    
    if (currentDirection.Z() == 1.0) {
      newDirection.SetXYZ(sin(theta) * cos(phi),sin(theta) * sin(phi),cos(theta));
    } else if (currentDirection.Z() == -1.0) {
      newDirection.SetXYZ(sin(theta) * cos(phi + TMath::Pi()),sin(theta) * sin(phi + TMath::Pi()),-cos(theta));
    } else {
        const double sqrtTerm = sqrt(1.0 - currentDirection.Z() * currentDirection.Z());
        newDirection.SetXYZ(
            currentDirection.X() * cos(theta) + (sin(theta) / sqrtTerm) * (currentDirection.X() * currentDirection.Z() * cos(phi) - currentDirection.Y() * sin(phi)),
            currentDirection.Y() * cos(theta) + (sin(theta) / sqrtTerm) * (currentDirection.Y() * currentDirection.Z() * cos(phi) + currentDirection.X() * sin(phi)),
            currentDirection.Z() * cos(theta) - sqrtTerm * sin(theta) * cos(phi)
        );
    }
    
    return newDirection;
}

double CalculateEnergyDeposition(const TVector3& startPoint, TVector3 direction, double energy) {
    TVector3 currentPosition = startPoint;
    double energyDeposited = 0.0;
    
    while (energy > 0.0) {
      // Calculate interaction point
      double interactionLength = 0; /* TODO: Calculate mean free path */
      currentPosition += direction * interactionLength;
      
      if (!detectorGeometry->Contains(currentPosition)) {
	break;
      }
      
      detectorGeometry->UpdatePosition(currentPosition);
      
      bool isPhotoelectric = false; /* TODO: Determine interaction type */
      if (isPhotoelectric) {
	energyDeposited += energy;
	energy = 0.0;
      } else {
	// Compton scattering
	double scatteredEnergy, scatteringTheta;
	randomGenerator->KleinNishina(energy, scatteredEnergy, scatteringTheta);
        
	double scatteringPhi = 2.0 * TMath::Pi() * randomGenerator->Rndm();
	direction = CalculateNewDirection(direction, scatteringTheta, scatteringPhi);
        
	energyDeposited += (energy - scatteredEnergy);
	energy = scatteredEnergy;
      }
    }
    
    return energyDeposited;
}

void RunSimulation(const std::string& mode) {
    randomGenerator = new myTRandom();
    randomGenerator->SetSeed(time(nullptr));
    
    detectorGeometry = new Geometry("CYLINDER");
    detectorGeometry->SetDimensions(det.radius, det.length);
    detectorGeometry->Draw();
    
    TH1D* energyHistogram = new TH1D("hE", "Energy Deposition", 100, 0, 1.0);
    TStopwatch timer;
    
    for (int event = 0; event < run.numEvents; ++event) {
        TVector3 startPoint, direction;
        double weight = 1.0;
        
        if (mode == "detsimple") {
	  startPoint = TVector3(0, 0, 0);
	  direction = TVector3(0, 0, 1);
        } else if (mode == "det") {
	  // TODO: Implement general case
        }  else if (mode == "bias") {
            // TODO: Implement biased case
        }
        
        detectorGeometry->StartEvent(startPoint);
        const double depositedEnergy = CalculateEnergyDeposition(startPoint, direction, src.energy);
        
        if (depositedEnergy > 0.0) {
	  const double measuredEnergy = depositedEnergy; /* TODO: Apply energy resolution */
	  energyHistogram->Fill(measuredEnergy, weight);
        }
    }
    
    // Save results
    TFile outputFile(Form("output_%s.root", mode.c_str()), "RECREATE");
    energyHistogram->Write();
    outputFile.Close();
    
    // Cleanup
    delete randomGenerator;
    delete detectorGeometry;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <mode>\n"
                  << "Available modes:\n"
                  << "  detsimple - Simplified simulation (photon along z-axis)\n"
                  << "  det       - Detailed simulation with correct direction\n"
                  << "  bias      - Simulation with source bias\n";
        return 1;
    }
    
    TApplication app("app", &argc, argv);
    RunSimulation(argv[1]);
    
    gPad->Update();
    app.Run();
    
    return 0;
}
