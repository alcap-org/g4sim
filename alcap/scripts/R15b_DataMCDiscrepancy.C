void R15b_DataMCDiscrepancy() {

  std::string data_filename = "plotsTi50.root";
  //  std::string mc_filename = "R15b_Ti50_response-matrix_500keV-bins_protons-SWSpec.root";
  std::string mc_filename = "dump.root";

  std::string arm = "SiL";
  //  std::string data_histname = "all_" + arm + "/hSingleDetAxes_TwoLayer_12";
  std::string data_histname = "proton_" + arm + "/hSingleDetAxes_TwoLayer_12";
  std::string mc_histname_stopped = arm + "_SingleDetAxes_TwoLayer_stopped";
  std::string mc_histname_notstopped = arm + "_SingleDetAxes_TwoLayer_notstopped";  
  
  TFile* data_file = new TFile(data_filename.c_str(), "READ");
  TFile* mc_file = new TFile(mc_filename.c_str(), "READ");

  TH2F* data_hist = (TH2F*) data_file->Get(data_histname.c_str());
  data_hist->Sumw2();
  TH2F* mc_hist_stopped = (TH2F*) mc_file->Get(mc_histname_stopped.c_str());
  mc_hist_stopped->SetMarkerColor(kRed);
  mc_hist_stopped->Sumw2();
  TH2F* mc_hist_notstopped = (TH2F*) mc_file->Get(mc_histname_notstopped.c_str());
  mc_hist_notstopped->SetMarkerColor(kBlue);
  mc_hist_notstopped->Sumw2();

  double x_axis_min = 0; double x_axis_max = 20000;
  double y_axis_min = 0; double y_axis_max = 3000;
  data_hist->GetXaxis()->SetRangeUser(x_axis_min, x_axis_max);
  data_hist->GetYaxis()->SetRangeUser(y_axis_min, y_axis_max);
  mc_hist_stopped->GetXaxis()->SetRangeUser(x_axis_min, x_axis_max);
  mc_hist_stopped->GetYaxis()->SetRangeUser(y_axis_min, y_axis_max);
  mc_hist_notstopped->GetXaxis()->SetRangeUser(x_axis_min, x_axis_max);
  mc_hist_notstopped->GetYaxis()->SetRangeUser(y_axis_min, y_axis_max);

  
  TCanvas* c1 = new TCanvas("c1", "c1");
  data_hist->Draw();
  mc_hist_stopped->Draw("SAME");
  mc_hist_notstopped->Draw("SAME");

  TCanvas* c1_inverse = new TCanvas("c1_inverse", "c1_inverse");
  mc_hist_stopped->Draw();
  mc_hist_notstopped->Draw("SAME");
  data_hist->Draw("SAME");

  // Try getting some slices of data and MC and look at the differences
  TCanvas* cfit = new TCanvas("cfit", "cfit");
  double min_energy_slice = 200;
  double max_energy_slice = 20000;
  double energy_slice_step = 18000;//200;
  int n_slices = (max_energy_slice - min_energy_slice) / energy_slice_step;
  TH1F* hThinRatios = new TH1F("hThinRatios", "Ratio per Thick Slice", n_slices, min_energy_slice, max_energy_slice);
  for (double low_energy_slice = min_energy_slice; low_energy_slice < max_energy_slice; low_energy_slice += energy_slice_step) {
    double high_energy_slice = low_energy_slice + energy_slice_step;

    /*
    int low_energy_slice_data_bin = data_hist->GetXaxis()->FindBin(low_energy_slice);
    int high_energy_slice_data_bin = data_hist->GetXaxis()->FindBin(high_energy_slice);  
    int low_energy_slice_mc_bin = mc_hist_stopped->GetXaxis()->FindBin(low_energy_slice);
    int high_energy_slice_mc_bin = mc_hist_stopped->GetXaxis()->FindBin(high_energy_slice);      
    TH1D* hDataSlice = data_hist->ProjectionY("data_slice", low_energy_slice_data_bin, high_energy_slice_data_bin);    
    TH1D* hMCSlice = mc_hist_stopped->ProjectionY("mc_slice", low_energy_slice_mc_bin, high_energy_slice_mc_bin);
    */
    int low_energy_slice_data_bin = data_hist->GetYaxis()->FindBin(low_energy_slice);
    int high_energy_slice_data_bin = data_hist->GetYaxis()->FindBin(high_energy_slice);  
    int low_energy_slice_mc_bin = mc_hist_stopped->GetYaxis()->FindBin(low_energy_slice);
    int high_energy_slice_mc_bin = mc_hist_stopped->GetYaxis()->FindBin(high_energy_slice);      
    TH1D* hDataSlice = data_hist->ProjectionX("data_slice");//, low_energy_slice_data_bin, high_energy_slice_data_bin);    
    TH1D* hMCSlice = mc_hist_stopped->ProjectionX("mc_slice");//, low_energy_slice_mc_bin, high_energy_slice_mc_bin);
    
    hMCSlice->SetLineColor(kRed);

    int rebin_factor = 10;
    hDataSlice->Rebin(rebin_factor);
    hMCSlice->Rebin(rebin_factor);

    // Want to avoid the noise peak in the data since we want the proton peak
    int data_high_bin = hDataSlice->GetMaximumBin();
    int mc_high_bin = hMCSlice->GetMaximumBin();

    hDataSlice->Scale(1.0 / hDataSlice->GetBinContent(data_high_bin));
    hMCSlice->Scale(1.0 / hMCSlice->GetBinContent(mc_high_bin));

    double data_high_pos = hDataSlice->GetBinCenter(data_high_bin);
    double mc_high_pos = hMCSlice->GetBinCenter(mc_high_bin);

    TF1* peakfit = new TF1("peakfit", "[0]*TMath::Gaus(x, [1], [2])", data_high_pos-50, data_high_pos+50);
    peakfit->SetParameters(1, data_high_pos, 10);
    hDataSlice->Fit(peakfit, "QR");
    data_high_pos = peakfit->GetParameter(1);

    peakfit = new TF1("peakfit", "[0]*TMath::Gaus(x, [1], [2])", mc_high_pos-50, mc_high_pos+50);
    peakfit->SetParameters(1, mc_high_pos, 10);
    hMCSlice->Fit(peakfit, "QR");
    mc_high_pos = peakfit->GetParameter(1);

    double ratio = data_high_pos / mc_high_pos;
    std::cout << low_energy_slice << " -- " << high_energy_slice << "keV :Ratio = " << data_high_pos << " / " << mc_high_pos << " = " << ratio << std::endl;
    hThinRatios->Fill(low_energy_slice, ratio);

    hDataSlice->Draw("HIST E");
    hMCSlice->Draw("HIST E SAMES");
  }
  TCanvas* c2 = new TCanvas("c2", "c2");
  hMCSlice->SetXTitle(" Energy [keV]");
  hMCSlice->Draw("HIST E");
  hDataSlice->Draw("HIST E SAMES");
  hThinRatios->Draw();

}
