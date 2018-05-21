#include "scripts/AlCapData_BeamProfiles.C"

void R15b_MuonBeamSim_BeamProfileValidation_Horizontal() {
  gStyle->SetOptFit(1);

  gROOT->ProcessLine(".x scripts/AlCapData_BeamProfiles.C");

  //  std::string filename = "output/R15b_100k_Geom-S16_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-S17_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-P5_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-P5_Ekin-1-19MeV_EkinSpread-0-157MeV_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-P5_Ekin-1-19MeV_EkinSpread-0-157MeV_ThetaSpread-1-71887deg_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-S16_Ekin-1-19MeV_EkinSpread-0-157MeV_ThetaSpread-1deg_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-S16_Ekin-1-19MeV_EkinSpread-0-157MeV_PhiSpread-1deg_muon-beam-sim.root";
  //  std::string filename = "output/raw_g4sim.root";
  //  std::string filename = "output/R15b_100k_Geom-S16_Ekin-1-19MeV_EkinSpread-0-157MeV_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-S16_Ekin-1-19MeV_EkinSpread-0-157MeV_new-beam-pos_muon-beam-sim.root";
  std::string filename = "output/raw_g4sim.root";
  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");
  
  //  std::string cutcmd = "M_volName==\"Target\" && M_particleName==\"mu-\"";
  std::string cutcmd = "M_volName==\"SiL1\" && M_particleName==\"mu-\"";
  tree->Draw("M_volID>>hSim_GeomS16", cutcmd.c_str(), "HIST E SAME");

  std::string title = filename + ": " + cutcmd;
  hSim_GeomS16->SetTitle(title.c_str());
  hSim_GeomS16->SetXTitle("Vol Number");
  hSim_GeomS16->SetLineColor(kBlack);

  // Draw on a new canvas
  hNMuonsPerStrip_Run10473->SetMarkerStyle(8);
  hNMuonsPerStrip_Run10473->SetLineColor(kBlack);
  hNMuonsPerStrip_Run10473->Draw("P E");
  hNMuonsPerStrip_Run10473->GetFunction("gaus_data")->SetLineColor(kBlack);
  hNMuonsPerStrip_Run10473->GetFunction("gaus_data")->Draw("LSAME");
  hNMuonsPerStrip_Run10473->SetStats(true);
  hNMuonsPerStrip_Run10473->SetMaximum(35000);

  double data_content = hNMuonsPerStrip_Run10473->GetBinContent(8);
  double sim_content = hSim_GeomS16->GetBinContent(8);
  hSim_GeomS16->Scale(data_content/sim_content);
  hSim_GeomS16->SetLineColor(kRed);
  hSim_GeomS16->Draw("HIST E SAMES");

  hSim_GeomS16->Fit("gaus");
  hSim_GeomS16->GetFunction("gaus")->SetLineColor(kRed);
  hSim_GeomS16->GetFunction("gaus")->Draw("LSAME");
}
