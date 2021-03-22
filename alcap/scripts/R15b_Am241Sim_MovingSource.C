void R15b_Am241Sim_MovingSource() {

  const int n_settings = 4;
  double source_positions[n_settings] = {0, 50, 100, 110};
  TH2F* hists[n_settings] = {0};
  Int_t colours[n_settings] = {kBlack, kRed, kBlue, kMagenta};

  TFile* output_file = new TFile("output_Am241.root", "RECREATE");

  std::stringstream mc_filename, mc_histname;
  for (int i_setting = 0; i_setting < n_settings; ++i_setting) {

    // Now get the Monte Carlo
    mc_filename.str("");
    mc_filename << "output/R15b_10M_Geom-C1-SiL1_Am241";
    if (i_setting > 0) {
      mc_filename << "_" << source_positions[i_setting] << "mmCloser";
    }
    mc_filename << ".root";
    std::cout << mc_filename.str() << std::endl;
    TFile* mc_file = new TFile(mc_filename.str().c_str(), "READ");
    if (mc_file->IsZombie()) {
      std::cout << "Problem opening MC file: " << mc_filename.str() << std::endl;
      return;
    }

    std::string mc_treename = "tree";
    TTree* mc_tree = (TTree*) mc_file->Get(mc_treename.c_str());
    if (!mc_tree) {
      std::cout << "Problem getting MC tree: " << mc_treename << std::endl;
      return;
    }

    // Create the MC 2D histogram
    double min_energy = 1500;
    double max_energy = 5600;
    double energy_width = 10;
    int n_energy_bins = (max_energy - min_energy) / energy_width;

    double front_z_position = source_positions[i_setting] + 1.5;
    double min_z = -1;
    double max_z = 11; // 11um
    double z_width = 0.1; // 0.1um
    const int n_z_bins = (max_z - min_z) / z_width;

    mc_histname.str("");
    mc_histname << "mc_2D_hist_" << source_positions[i_setting];
    TH2F* mc_2D_hist = new TH2F(mc_histname.str().c_str(), "", n_z_bins,min_z,max_z, n_energy_bins,min_energy,max_energy);
    mc_2D_hist->SetXTitle("Starting Depth [#mum]");
    mc_2D_hist->SetYTitle("Am241 Energy [keV]");
    mc_2D_hist->SetLineColor(colours[i_setting]);
    mc_2D_hist->SetMarkerColor(colours[i_setting]);

    std::stringstream drawcmd;
    drawcmd.str("");
    drawcmd << "M_edep*1e6:-((-1*i_z-" << front_z_position << ")*1e3)>>" << mc_histname.str();
    std::cout << drawcmd.str() << std::endl;
    mc_tree->Draw(drawcmd.str().c_str(), "M_ovolName==\"AmSource\" && M_volName==\"SiL1\" && M_particleName==\"alpha\"", "goff");
    std::cout << mc_2D_hist->GetEntries() << std::endl;

    mc_2D_hist->SetDirectory(0);
    mc_file->Close();
    hists[i_setting] = (TH2F*) mc_2D_hist->Clone();
  }

  TLegend *leg = new TLegend(0.09,0.64,0.29,0.88,NULL,"brNDC");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.03);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);

  TCanvas* c_first_slice = new TCanvas("c_first_slice", "c_first_slice");
  std::stringstream leglabel;
  for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
    std::string projname = hists[i_setting]->GetName();
    projname += "_px_first_slice";
    TH1F* h = (TH1F*) hists[i_setting]->ProjectionY(projname.c_str(), 11, 11);
    h->SetStats(false);
    h->SetTitle("First Slice (Normalised to number of entries)");
    h->Scale(1.0/h->GetEntries());
    h->Draw("SAME");

    leglabel.str("");
    leglabel << source_positions[i_setting] << " mm closer to detector";
    leg->AddEntry(h, leglabel.str().c_str(), "l");
  }
  leg->Draw();

  TCanvas* c_last_slice = new TCanvas("c_last_slice", "c_last_slice");
  for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
    std::string projname = hists[i_setting]->GetName();
    projname += "_px_last_slice";
    TH1F* h = (TH1F*) hists[i_setting]->ProjectionY(projname.c_str(), 110, 110);
    h->SetStats(false);
    h->SetTitle("Last Slice (Normalised to number of entries)");
    h->Scale(1.0/h->GetEntries());
    h->Draw("SAME");
  }
  leg->Draw();

  output_file->Write();
}
