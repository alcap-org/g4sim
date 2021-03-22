void R15b_Si16b_InitialProtonDistributions(std::string filename) {

  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  double bin_width = 0.1;

  double min_global_x = -25;
  double max_global_x = 25;
  double min_global_y = -25;
  double max_global_y = 25;
  double min_global_z = -25;
  double max_global_z = 25;
  int n_bins_global_x = (max_global_x - min_global_x) / bin_width;
  int n_bins_global_y = (max_global_y - min_global_y) / bin_width;
  int n_bins_global_z = (max_global_z - min_global_z) / bin_width;
  TCanvas* c_yx = new TCanvas("yx", "yx");
  TH2D* hYX = new TH2D("hYX", "Initial Position of Protons (global coordinates)", n_bins_global_x,min_global_x,max_global_x, n_bins_global_y,min_global_y,max_global_y);
  hYX->SetXTitle("Global X [mm]");
  hYX->SetYTitle("Global Y [mm]");
  tree->Draw("i_y:i_x>>hYX", "", "goff");
  hYX->SetStats(false);
  hYX->Draw("COLZ");

  TCanvas* c_yx = new TCanvas("yx_local", "yx_local");
  TH2D* hYX_local = new TH2D("hYX_local", "Initial Position of Protons (local coordinates)", n_bins_global_x,min_global_x,max_global_x, n_bins_global_y,min_global_y,max_global_y);
  hYX_local->SetXTitle("Local X [mm]");
  hYX_local->SetYTitle("Local Y [mm]");
  tree->Draw("i_y:(sqrt(2)/2)*i_x+(-sqrt(2)/2)*i_z>>hYX_local", "", "goff");
  hYX_local->SetStats(false);
  hYX_local->Draw("COLZ");


  min_global_x = 0;
  max_global_x = 10;
  n_bins_global_x = (max_global_x - min_global_x) / bin_width;
  min_global_z = -15;
  max_global_z = 0;
  n_bins_global_z = (max_global_z - min_global_z) / bin_width;
  TCanvas* c_xz = new TCanvas("xz", "xz");
  TH2D* hXZ = new TH2D("hXZ", "Initial Position of Protons (global coordinates)", n_bins_global_z,min_global_z,max_global_z, n_bins_global_x,min_global_x,max_global_x);
  hXZ->SetXTitle("Local Z [mm]");
  hXZ->SetYTitle("Local X [mm]");
  tree->Draw("i_x:i_z>>hXZ", "", "goff");
  hXZ->SetStats(false);
  hXZ->Draw("COLZ");

  min_global_x = 0;
  max_global_x = 20;
  n_bins_global_x = (max_global_x - min_global_x) / bin_width;
  TCanvas* c_xz = new TCanvas("xz_local", "xz_local");
  TH2D* hXZ_local = new TH2D("hXZ_local", "Initial Position of Protons (local coordinates)", n_bins_global_z,min_global_z,max_global_z, n_bins_global_x,min_global_x,max_global_x);
  hXZ_local->SetXTitle("Local Z [mm]");
  hXZ_local->SetYTitle("Local X [mm]");
  tree->Draw("(sqrt(2)/2)*i_x+(-sqrt(2)/2)*i_z:(sqrt(2)/2)*i_x-(-sqrt(2)/2)*i_z>>hXZ_local", "", "goff");
  hXZ_local->SetStats(false);
  hXZ_local->Draw("COLZ");


  min_global_z = -2.6;
  max_global_z = -2.4;
  bin_width = 1e-3;
  n_bins_global_z = (max_global_z - min_global_z) / bin_width;
  TCanvas* c_z = new TCanvas("z_local", "z_local");
  TH1D* hZ_local = new TH1D("hZ_local", "Initial Position of Protons (local coordinates)", n_bins_global_z,min_global_z,max_global_z);
  hZ_local->SetXTitle("Local Z [mm]");
  tree->Draw("(sqrt(2)/2)*i_x-(-sqrt(2)/2)*i_z>>hZ_local", "", "goff");
  hZ_local->SetStats(false);
  hZ_local->Draw("COLZ");



  //  tree->Draw("i_y:(sqrt(2)/2)*i_x+(-sqrt(2)/2)*i_z", "", "COLZ");
}
