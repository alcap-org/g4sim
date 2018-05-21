#include "scripts/AlCapData_BeamProfile_45deg.C"

void R15b_MuonBeamSim_BeamProfileValidation_45deg() {
  gStyle->SetOptFit(1);

  gROOT->ProcessLine(".x scripts/AlCapData_BeamProfile_45deg.C");

  //  std::string filename = "output/R15b_100k_Geom-S16_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-S17_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-P5_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-P5_Ekin-1-19MeV_EkinSpread-0-157MeV_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-P5_Ekin-1-19MeV_EkinSpread-0-157MeV_ThetaSpread-1-71887deg_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-S16_Ekin-1-19MeV_EkinSpread-0-157MeV_ThetaSpread-1deg_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-S16_Ekin-1-19MeV_EkinSpread-0-157MeV_PhiSpread-1deg_muon-beam-sim.root";
  std::string filename = "output/raw_g4sim.root";
  //  std::string filename = "output/R15b_100k_Geom-S16_Ekin-1-19MeV_EkinSpread-0-157MeV_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-P5_Ekin-1-19MeV_EkinSpread-0-157MeV_new-beam-pos_muon-beam-sim.root";
  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");
  
  std::string cutcmd = "M_volName==\"Target\" && M_particleName==\"mu-\"";
  tree->Draw("M_volID>>hSim_GeomS14", cutcmd.c_str(), "HIST E SAME");

  std::string title = filename + ": " + cutcmd;
  hSim_GeomS14->SetTitle(title.c_str());
  hSim_GeomS14->SetXTitle("Vol Number");
  hSim_GeomS14->SetLineColor(kBlack);

  // Draw on a new canvas
  hNMuonsPerStrip_Run10404->SetMarkerStyle(8);
  hNMuonsPerStrip_Run10404->SetLineColor(kBlack);
  hNMuonsPerStrip_Run10404->Draw("P E");
  hNMuonsPerStrip_Run10404->GetFunction("gaus_data")->SetLineColor(kBlack);
  hNMuonsPerStrip_Run10404->GetFunction("gaus_data")->Draw("LSAME");
  hNMuonsPerStrip_Run10404->SetStats(true);
  hNMuonsPerStrip_Run10404->SetMaximum(80000);

  double data_content = hNMuonsPerStrip_Run10404->GetBinContent(8);
  double sim_content = hSim_GeomS14->GetBinContent(8);
  hSim_GeomS14->Scale(data_content/sim_content);
  hSim_GeomS14->SetLineColor(kRed);
  hSim_GeomS14->Draw("HIST E SAMES");

  hSim_GeomS14->Fit("gaus");
  hSim_GeomS14->GetFunction("gaus")->SetLineColor(kRed);
  hSim_GeomS14->GetFunction("gaus")->Draw("LSAME");

  std::cout << "Data:" << std::endl;
  for (int i_bin = 1; i_bin < hNMuonsPerStrip_Run10404->GetNbinsX(); ++i_bin) {

    double this_bin_content = hNMuonsPerStrip_Run10404->GetBinContent(i_bin);
    double next_bin_content = hNMuonsPerStrip_Run10404->GetBinContent(i_bin+1);

    double ratio = next_bin_content / this_bin_content;
    std::cout << "SiL1-" << i_bin << "-S / SiL1-" << i_bin+1 << "-S = " << ratio << std::endl;
  }

  std::cout << "Sim:" << std::endl;
  for (int i_bin = 1; i_bin < hSim_GeomS14->GetNbinsX(); ++i_bin) {

    double this_bin_content = hSim_GeomS14->GetBinContent(i_bin);
    double next_bin_content = hSim_GeomS14->GetBinContent(i_bin+1);

    double ratio = next_bin_content / this_bin_content;
    std::cout << "SiL1-" << i_bin << "-S / SiL1-" << i_bin+1 << "-S = " << ratio << std::endl;
  }
}
