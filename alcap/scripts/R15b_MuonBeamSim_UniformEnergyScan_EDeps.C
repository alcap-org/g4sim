void R15b_MuonBeamSim_UniformEnergyScan_EDeps() {

  std::string filename = "output/R15b_1M_Geom-P5_Ekin-1-5MeV_EkinSpread-1.2MeVUniform_new-beam-pos_muon-beam-sim.root";
  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  double min_energy = 0.8;
  double max_energy = 2.2;
  double bin_width = 0.01;
  int n_bins_energy = (max_energy - min_energy) / bin_width;

  double min_edep = 0.0;
  double max_edep = 2.1;
  double bin_width_edep = 0.01;
  int n_bins_edep = (max_edep - min_edep) / bin_width_edep;

  TH2F* hAll = new TH2F("hAll", "", n_bins_energy,min_energy,max_energy, n_bins_edep,min_edep,max_edep);
  TH2F* hStops = (TH2F*) hAll->Clone("hStops");
  hStops->SetMarkerColor(kRed);
  TH2F* hNotStops = (TH2F*) hAll->Clone("hNotStops");

  tree->Draw("M_edep*1e3:sqrt(i_px^2 + i_py^2 + i_pz^2 + 105.67^2) - 105.67>>hAll", "M_volName==\"Target\" && M_particleName==\"mu-\"", "goff");
  tree->Draw("M_edep*1e3:sqrt(i_px^2 + i_py^2 + i_pz^2 + 105.67^2) - 105.67>>hNotStops", "M_stopped==0 && M_volName==\"Target\" && M_particleName==\"mu-\"", "HIST E");
  tree->Draw("M_edep*1e3:sqrt(i_px^2 + i_py^2 + i_pz^2 + 105.67^2) - 105.67>>hStops", "M_stopped==1 && M_volName==\"Target\" && M_particleName==\"mu-\"", "HIST E SAMES");

  hAll->Draw("COLZ");
  hNotStops->Draw();
  hStops->Draw("SAMES");
  hAll->Draw();
}
