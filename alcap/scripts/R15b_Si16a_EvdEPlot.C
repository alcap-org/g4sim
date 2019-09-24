
#include "scripts/R15b_EvdEPlot.C+"

#include "TLatex.h"
#include "TLegend.h"

void R15b_Si16a_EvdEPlot() {
  std::string filename = "output/raw_g4sim.root";
  
  //  std::string filename = "output/R15b_200k_Geom-S14_muon-beam-sim_SF1-02.root";
  //  std::string outfilename = "MC_raw_spectra09740.root";

  //  std::string filename = "output/R15b_200k_Geom-S14_muon-beam-sim_SF1-03.root";
  //  std::string outfilename = "MC_raw_spectra09741.root";

  //  std::string outfilename = "MC_raw_spectra09742.root";
  
  //  std::string filename = "output/R15b_200k_Geom-S14_muon-beam-sim_SF1-05.root";
  std::string outfilename = "MC_raw_spectra09743.root";
  std::vector<Arm> arms;
  Arm right_arm;
  Arm left_arm;
  right_arm.detname = "SiR";

  right_arm.thin_energy_thresholds.push_back(90);
  right_arm.thin_energy_thresholds.push_back(100);
  right_arm.thin_energy_thresholds.push_back(80);
  right_arm.thin_energy_thresholds.push_back(90);
  right_arm.thick_energy_threshold = 200;

  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_1", "TMath::Gaus(x, 0, 42.7)", -1000, 1000));
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_2", "TMath::Gaus(x, 0, 37.7)", -1000, 1000));
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_3", "TMath::Gaus(x, 0, 40.7)", -1000, 1000));
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_4", "TMath::Gaus(x, 0, 43.0)", -1000, 1000));
  right_arm.thick_energy_resolution = new TF1("res_SiR2", "TMath::Gaus(x, 0, 32.7)", -1000, 1000);
  
  left_arm.detname = "SiL";
  left_arm.thin_energy_thresholds.push_back(100000); // turn off SiL1-1-S
  left_arm.thin_energy_thresholds.push_back(296);
  left_arm.thin_energy_thresholds.push_back(270);
  left_arm.thin_energy_thresholds.push_back(219);
  left_arm.thin_energy_thresholds.push_back(211);
  left_arm.thin_energy_thresholds.push_back(199);
  left_arm.thin_energy_thresholds.push_back(217);
  left_arm.thin_energy_thresholds.push_back(292);
  left_arm.thin_energy_thresholds.push_back(309);
  left_arm.thin_energy_thresholds.push_back(229);
  left_arm.thin_energy_thresholds.push_back(214);
  left_arm.thin_energy_thresholds.push_back(210);
  left_arm.thin_energy_thresholds.push_back(229);
  left_arm.thin_energy_thresholds.push_back(233);
  left_arm.thin_energy_thresholds.push_back(142);
  left_arm.thin_energy_thresholds.push_back(100000); // turn off SiL1-16-S
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_1", "TMath::Gaus(x, 0, 30.9)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_2", "TMath::Gaus(x, 0, 30.5)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_3", "TMath::Gaus(x, 0, 31.1)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_4", "TMath::Gaus(x, 0, 26.1)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_5", "TMath::Gaus(x, 0, 29.2)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_6", "TMath::Gaus(x, 0, 28.4)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_7", "TMath::Gaus(x, 0, 29.7)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_8", "TMath::Gaus(x, 0, 30.7)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_9", "TMath::Gaus(x, 0, 29.1)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_10", "TMath::Gaus(x, 0, 28.1)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_11", "TMath::Gaus(x, 0, 31.6)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_12", "TMath::Gaus(x, 0, 27.6)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_13", "TMath::Gaus(x, 0, 29.4)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_14", "TMath::Gaus(x, 0, 29.7)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_15", "TMath::Gaus(x, 0, 31.6)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_16", "TMath::Gaus(x, 0, 37.1)", -250, 250));
  left_arm.thick_energy_resolution = new TF1("res_SiL3", "TMath::Gaus(x, 0, 31.6)", -250, 250);
  //  left_arm.thick_energy_resolution = new TF1("res_SiL3", "TMath::Gaus(x, 0, 20)", -250, 250);
  left_arm.thick_energy_threshold = 180;

  Arm mu_counter;
  mu_counter.detname = "SiT";
  mu_counter.thin_energy_thresholds.push_back(90);
  mu_counter.thin_energy_thresholds.push_back(100);
  mu_counter.thin_energy_thresholds.push_back(80);
  mu_counter.thin_energy_thresholds.push_back(90);
  mu_counter.thick_energy_threshold = 200;

  mu_counter.thin_energy_resolutions.push_back(new TF1("res_SiT_1", "TMath::Gaus(x, 0, 42.7)", -1000, 1000));
  mu_counter.thin_energy_resolutions.push_back(new TF1("res_SiT_2", "TMath::Gaus(x, 0, 37.7)", -1000, 1000));
  mu_counter.thin_energy_resolutions.push_back(new TF1("res_SiT_3", "TMath::Gaus(x, 0, 40.7)", -1000, 1000));
  mu_counter.thin_energy_resolutions.push_back(new TF1("res_SiT_4", "TMath::Gaus(x, 0, 43.0)", -1000, 1000));
  mu_counter.thick_energy_resolution = new TF1("res_SiTUnused", "TMath::Gaus(x, 0, 32.7)", -1000, 1000);
  
  arms.push_back(right_arm);
  arms.push_back(left_arm); // don't need SiL arm for Si16b analysis
  arms.push_back(mu_counter);
  R15b_EvdEPlot(filename, arms);

  TCanvas* c1 = new TCanvas("c1", "c1");
  arms.at(0).hEvdE->Draw("COLZ");

  TCanvas* c2 = new TCanvas("c2", "c2");
  arms.at(1).hEvdE->Draw("COLZ");

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
  double entering_muons = arms.at(2).hThinEnergy_Veto->GetEntries();
  double thin_stopped_muons = arms.at(1).hThinEnergy_Veto->GetEntries();
  double thick_stopped_muons = arms.at(1).hEvdE->ProjectionX()->GetEntries();
  double stopped_muons = thin_stopped_muons + thick_stopped_muons;
  double stopping_fraction = stopped_muons / entering_muons;
  std::cout << "Mu Counter Entries = " << entering_muons << std::endl;
  std::cout << "Thin Stop Entries = " << thin_stopped_muons << std::endl;
  std::cout << "Thick Stop Entries = " << thick_stopped_muons << std::endl;
  std::cout << "Total Stop Entries = " << stopped_muons << std::endl;
  std::cout << "Stopping Fraction = " << stopping_fraction << std::endl;
  outfile->Write();
  outfile->Close();
}
