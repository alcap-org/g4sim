void R15b_DataMCDiscrepancy_Michels() {

  std::string data_filename = "plotsTi50.root";
  //  std::string data_filename = "unfoldTi50.root";
  //  std::string data_filename = "plotsSiL3.root";
  std::string mc_filename = "dump.root";

  std::string arm = "SiL";
  std::string data_histname = arm + "3_Michels/hEnergyTime";
  //  std::string data_histname = arm + "3_FlatBkg_TimeSlice200_400/hCorrectedSpectrum";
  double min_time = 200;
  double max_time = 400;
  //  std::string data_histname = "Target/hEnergyTime";
  //  double min_time = 2000;
  //  double max_time = 4000;
  //  std::string data_histname = arm + "3/hEnergyTime";
  //  std::string data_histname = "proton_" + arm + "/hSingleDetAxes_TwoLayer_12";
  std::string mc_histname = "hEDep_muplus";
  int mc_rebin_factor = 2;
  
  std::cout << data_filename << ":" << data_histname << std::endl;
  std::cout << mc_filename << ":" << mc_histname << std::endl;
  
  TFile* data_file = new TFile(data_filename.c_str(), "READ");
  TFile* mc_file = new TFile(mc_filename.c_str(), "READ");

  TH2F* data_hist_2D = (TH2F*) data_file->Get(data_histname.c_str());
  data_hist_2D->Sumw2();

  int min_time_bin = data_hist_2D->GetXaxis()->FindBin(min_time);
  int max_time_bin = data_hist_2D->GetXaxis()->FindBin(max_time)-1;
  TH1F* data_hist = (TH1F*) data_hist_2D->ProjectionY("data_hist", min_time_bin, max_time_bin);
  data_hist->Rebin(2);
  
  //  TH1F* data_hist = (TH1F*) data_file->Get(data_histname.c_str());
  data_hist->SetLineColor(kBlack);
  data_hist->SetTitle("Data Vs MC (Michels)");
  data_hist->SetYTitle("Normalised");
  data_hist->SetStats(false);
  data_hist->Sumw2();
  TH1F* mc_hist = (TH1F*) mc_file->Get(mc_histname.c_str());
  mc_hist->SetLineColor(kRed);
  mc_hist->Rebin(mc_rebin_factor);
  mc_hist->Sumw2();

  TLegend* leg = new TLegend(0.42, 0.85, 0.62, 0.55, "");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->SetFillColor(kWhite);
  leg->AddEntry(data_hist, "Data", "l");
  leg->AddEntry(mc_hist, "MC", "l");


  double x_axis_min = 0; double x_axis_max = 5000;
  //  double y_axis_min = 0; double y_axis_max = 3000;
  data_hist->GetXaxis()->SetRangeUser(x_axis_min, x_axis_max);
  //  data_hist->GetYaxis()->SetRangeUser(y_axis_min, y_axis_max);
  mc_hist->GetXaxis()->SetRangeUser(x_axis_min, x_axis_max);

  int data_high_bin = data_hist->GetMaximumBin();
  int mc_high_bin = mc_hist->GetMaximumBin();

  data_hist->Scale(1.0 / data_hist->GetBinContent(data_high_bin));
  mc_hist->Scale(1.0 / mc_hist->GetBinContent(mc_high_bin));
  
  TCanvas* c1 = new TCanvas("c1", "c1");
  data_hist->Draw("HIST");
  mc_hist->Draw("HIST SAME");
  leg->Draw();
  //  data_hist_2D->Draw("COLZ");
}
