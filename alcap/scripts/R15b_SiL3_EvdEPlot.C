
#include "scripts/R15b_EvdEPlot.C+"

#include "TLatex.h"
#include "TLegend.h"

void R15b_SiL3_EvdEPlot() {
  //  std::string filename = "output/raw_g4sim.root";
  std::string filename = "output/R15b_200k_Geom-P1_muon-beam-sim_SF1-12.root";
  std::string outfilename = "dump.root";
  
  //  std::string filename = "output/R15b_200k_Geom-S14_muon-beam-sim_SF1-03.root";
  //  std::string outfilename = "MC_raw_spectra09040.root";
  
  std::vector<Arm> arms;
  Arm left_arm;
  left_arm.detname = "SiL";
  left_arm.thin_energy_thresholds.push_back(100000); // turn off SiL1-1-S
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_1", "TMath::Gaus(x, 0, 30.9)", -250, 250));
  left_arm.thick_energy_resolution = new TF1("res_SiL3", "TMath::Gaus(x, 0, 30.9)", -250, 250);
  left_arm.thick_energy_threshold = 35;

  arms.push_back(left_arm); // don't need SiL arm for Si16b analysis
  R15b_EvdEPlot(filename, arms);


  TFile* outfile = new TFile(outfilename.c_str(), "RECREATE");
  for (std::vector<Arm>::iterator i_arm = arms.begin(); i_arm != arms.end(); ++i_arm) {
    i_arm->hEvdE->Write();
    i_arm->hEvdE->ProjectionX()->Write();
    i_arm->hThinEnergy_Veto->Write();
    i_arm->hThinEnergy_noVeto->Write();
    i_arm->hThickEnergy_noVeto->Write();
    i_arm->hThickEnergy_Veto->Write();
    i_arm->hBeamPosition->Write();
  }
  outfile->Write();
  outfile->Close();
}
