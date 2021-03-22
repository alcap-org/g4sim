void R15b_DataMCDiscrepancy_Beam() {

  //  std::string data_filename = "plotsSiL3.root";
  std::string data_filename = "plots08986.root";
  std::string mc_filename = "dump.root";

  std::string data_histname = "Target/hEnergyTime";
  std::string mc_histname = "hEDep_beam";

  std::cout << data_filename << ":" << data_histname << std::endl;
  std::cout << mc_filename << ":" << mc_histname << std::endl;
  
  TFile* data_file = new TFile(data_filename.c_str(), "READ");
  TFile* mc_file = new TFile(mc_filename.c_str(), "READ");

  TH2F* data_hist_2D = (TH2F*) data_file->Get(data_histname.c_str());
  data_hist_2D->Sumw2();

  double min_time = -200;
  double max_time = 200;
  int min_time_bin = data_hist_2D->GetXaxis()->FindBin(min_time);
  int max_time_bin = data_hist_2D->GetXaxis()->FindBin(max_time)-1;
  TH1F* data_hist = (TH1F*) data_hist_2D->ProjectionY("data_hist", min_time_bin, max_time_bin);
  data_hist->SetLineColor(kBlack);
  data_hist->SetTitle("Data Vs MC (Beam)");
  data_hist->SetYTitle("Normalised");
  data_hist->SetStats(false);
  data_hist->Sumw2();
  TH1F* mc_hist = (TH1F*) mc_file->Get(mc_histname.c_str());
  mc_hist->SetLineColor(kRed);
  mc_hist->Sumw2();

  TLegend* leg = new TLegend(0.62, 0.85, 0.82, 0.55, "");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->SetFillColor(kWhite);
  leg->AddEntry(data_hist, "Data", "l");
  leg->AddEntry(mc_hist, "MC", "l");


  double x_axis_min = 0; double x_axis_max = 10000;
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
