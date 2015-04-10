void FindMCToDataScaleFactor() {

  std::string data_filename = "~/data/out/v94/total.root";
  TFile* data_file = new TFile(data_filename.c_str(), "READ");
  std::string data_plotname = "TME_Al50_EvdE/all_particles/SiL_EvdE";
  TH2F* hEvdE_data = (TH2F*) data_file->Get(data_plotname.c_str());
  hEvdE_data->SetDirectory(0);
  data_file->Close();

  std::string MC_filename = "plots_2015-03-04.root";
  TFile* MC_file = new TFile(MC_filename.c_str(), "READ");
  std::string MC_plotname = "hAll_EvdE_SiL";
  TH2F* hEvdE_MC = (TH2F*) MC_file->Get(MC_plotname.c_str());
  hEvdE_MC->SetDirectory(0);
  MC_file->Close();

  double energy_slice = 2500;
  int energy_slice_bin = hEvdE_data->GetXaxis()->FindBin(energy_slice);
  TH1D* hDataProjection = hEvdE_data->ProjectionY("_py", energy_slice_bin, energy_slice_bin);  
  energy_slice_bin = hEvdE_MC->GetXaxis()->FindBin(energy_slice);
  TH1D* hMCProjection = hEvdE_MC->ProjectionY("_py", energy_slice_bin, energy_slice_bin);

  hDataProjection->SetStats(false);
  hDataProjection->SetTitle("");
  hDataProjection->SetXTitle("E_{1} [keV]");
  hDataProjection->SetYTitle("Counts");
  hMCProjection->SetStats(false);
  hMCProjection->SetTitle("");
  hMCProjection->SetXTitle("E_{1} [keV]");
  hMCProjection->SetYTitle("Counts");

  TFitResultPtr data_fit_result = hDataProjection->Fit("gaus", "S");
  TFitResultPtr MC_fit_result = hMCProjection->Fit("gaus", "S");
  hDataProjection->GetFunction("gaus")->SetLineColor(kBlack);
  hDataProjection->GetFunction("gaus")->SetLineStyle(2);
  hMCProjection->GetFunction("gaus")->SetLineColor(kRed);
  hMCProjection->GetFunction("gaus")->SetLineStyle(2);

  hDataProjection->SetLineColor(kBlack);
  hDataProjection->SetLineWidth(2);
  hDataProjection->GetXaxis()->SetRangeUser(0, energy_slice);
  hDataProjection->GetYaxis()->SetRangeUser(0, 80);
  hDataProjection->Draw();
  hMCProjection->SetLineColor(kRed);
  hMCProjection->SetLineWidth(2);
  hMCProjection->GetXaxis()->SetRangeUser(0, energy_slice);
  hMCProjection->GetYaxis()->SetRangeUser(0, 80);
  hMCProjection->Draw("SAME");

  TLegend *leg = new TLegend(0.61,0.68,0.81,0.78);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->SetFillColor(kWhite);
  leg->AddEntry(hDataProjection, "Data", "l");
  leg->AddEntry(hMCProjection, "MC", "l");
  leg->Draw();

  //EvdE_data->Draw("COLZ");
  double data_mean = data_fit_result->Parameter(1);
  double data_mean_error = data_fit_result->ParError(1);
  double MC_mean = MC_fit_result->Parameter(1);
  double MC_mean_error = MC_fit_result->ParError(1);
  double scale_factor = data_mean / MC_mean;
  std::cout << "Data = " << data_mean << " +- " << data_mean_error << std::endl;
  std::cout << "MC = " << MC_mean << " +- " << MC_mean_error << std::endl;
  std::cout << "data / MC = " <<  data_mean << " / " << MC_mean << " = " << scale_factor << std::endl;

  // Go through the MC projection and scale each bin by the scale factor
  int n_bins = hMCProjection->GetNbinsX();
  double min_x = hMCProjection->GetXaxis()->GetXmin();
  double max_x = hMCProjection->GetXaxis()->GetXmax();
  TH1F* hMCProjection_scaled = new TH1F("hMCProjection_scaled", "", n_bins,min_x,max_x);

  for (int i_bin = 1; i_bin <= n_bins; ++i_bin) {
    double old_energy = hMCProjection->GetBinLowEdge(i_bin);
    double new_energy = old_energy * scale_factor;

    double old_bin_content = hMCProjection->GetBinContent(i_bin);
    hMCProjection_scaled->Fill(new_energy, old_bin_content);
  }

  //  hMCProjection_scaled->Draw();
  //  hDataProjection->Draw("SAME");
}
