void R15b_Am241Sim_BetterFit() {
  gStyle->SetOptFit(1);

  
  /*  std::string data_filename = "output/out10483_si16-am241-calib.root";
  std::string mc_filename = "output/R15b_10M_Geom-C1_Am241_Preliminary.root";
  std::string channel = "SiL1-8-S";
  double front_z_position = -48.5;
  std::string M_volName = "Target";
  std::string flip = "1*";
  */
  /*
  std::string data_filename = "output/out09020_sil3-am241-calib.root";
  std::string mc_filename = "output/R15b_10M_Geom-C1-SiL3_Am241.root";
  std::string channel = "SiL3-S";
  double front_z_position = 1.5;//-1.501,-1.489;
  std::string M_volName = "SiL3";
  std::string flip = "-1*";
  */
  
  /*  std::string data_filename = "output/out10483_si16-am241-calib.root";
  std::string mc_filename = "output/R15b_10M_Geom-C1-SiL1_Am241.root";
  std::string channel = "SiL1-8-S";
  double front_z_position = 1.5;//-1.501,-1.489;
  std::string M_volName = "SiL1";
  std::string flip = "-1*";
  */
  /*  std::string data_filename = "output/out10483_si16-am241-calib.root";
  std::string mc_filename = "output/R15b_10M_Geom-C1-SiL1_Am241_50mmCloser.root";
  std::string channel = "SiL1-8-S";
  double front_z_position = 51.5;
  std::string M_volName = "SiL1";
  std::string flip = "-1*";
*/  
  std::string data_filename = "output/out10483_si16-am241-calib.root";
  std::string mc_filename = "output/R15b_10M_Geom-C1-SiL1_Am241_100mmCloser.root";
  std::string channel = "SiL1-8-S";
  double front_z_position = 101.5;
  std::string M_volName = "SiL1";
  std::string flip = "-1*";

  // First get the data
  TFile* data_file = new TFile(data_filename.c_str(), "READ");
  if (data_file->IsZombie()) {
    std::cout << "Problem opening data file: " << data_filename << std::endl;
    return;
  }

  std::string data_histname = "PlotTAP_Amplitude_wTimeCuts/h" + channel + "#MaxBinAPGenerator#{no_time_shift=true}_Amplitude_wTimeCuts";
  TH1F* data_1D_hist = (TH1F*) data_file->Get(data_histname.c_str());
  if (!data_1D_hist) {
    std::cout << "Problem getting data histogram: " << data_histname << std::endl;
    return;
  }

  int data_rebin_factor = 8;
  data_1D_hist->Rebin(data_rebin_factor);
  data_1D_hist->GetXaxis()->SetRangeUser(600,3000);
  TCanvas* c_data_fit = new TCanvas("c_data_fit", "c_data_fit");

  double max_bin = data_1D_hist->GetMaximumBin();
  TF1* expo_data = new TF1("expo_data", "TMath::Exp([0] + [1]*x)", data_1D_hist->GetXaxis()->GetBinCenter(max_bin)-300, data_1D_hist->GetXaxis()->GetBinCenter(max_bin));
  expo_data->SetParameter(0, -50);
  expo_data->SetParameter(1, 0.01);
  TFitResultPtr expo_fit_data = data_1D_hist->Fit(expo_data, "RSQ+");
  double data_expo_slope = expo_fit_data->Parameter(1);
  double data_expo_slope_error = expo_fit_data->ParError(1);
  std::cout << "Data: Exponential Slope = " << data_expo_slope << " +/- " << data_expo_slope_error << std::endl;

  // Now get the Monte Carlo
  TFile* mc_file = new TFile(mc_filename.c_str(), "READ");
  if (mc_file->IsZombie()) {
    std::cout << "Problem opening MC file: " << mc_filename << std::endl;
    return;
  }

  std::string mc_treename = "tree";
  TTree* mc_tree = (TTree*) mc_file->Get(mc_treename.c_str());
  if (!mc_tree) {
    std::cout << "Problem getting MC tree: " << mc_treename << std::endl;
    return;
  }

  // Create the MC 2D histogram
  double min_energy = 0;
  double max_energy = 5600;
  double energy_width = 100;
  int n_energy_bins = (max_energy - min_energy) / energy_width;

  double min_z = -1;
  double max_z = 11; // 11um
  double z_width = 0.1; // 0.1um
  const int n_z_bins = (max_z - min_z) / z_width;

  TH2F* mc_2D_hist = new TH2F("mc_2D_hist", "", n_z_bins,min_z,max_z, n_energy_bins,min_energy,max_energy);
  mc_2D_hist->SetXTitle("Starting Depth [#mum]");
  mc_2D_hist->SetYTitle("Am241 Energy [keV]");

  std::stringstream drawcmd;
  drawcmd.str("");
  drawcmd << "M_edep*1e6:-((" << flip << "i_z-" << front_z_position << ")*1e3)>>mc_2D_hist";
  std::stringstream cutcmd;
  cutcmd.str("");
  cutcmd << "M_ovolName==\"AmSource\" && M_volName==\"" << M_volName << "\" && M_particleName==\"alpha\"";
  std::cout << "Draw Cmd = " << drawcmd.str() << " Cut Cmd = " << cutcmd.str() << std::endl;
  mc_tree->Draw(drawcmd.str().c_str(), cutcmd.str().c_str(), "goff");

  const int n_slice_bins = n_z_bins;
  double slices[n_slice_bins] = {0};
  double slice_widths[n_slice_bins] = {0};
  double mc_expo_slopes[n_slice_bins] = {0};
  double mc_expo_slope_errors[n_slice_bins] = {0};

  // Now take projections onto the Y-axis for the Monte Carlo and do the fit
  std::stringstream histname;
  TH1F* best_hist = 0;
  int best_slice = 0;
  double min_diff = 999999;
  TCanvas* c1 = new TCanvas("c1", "c1");
  for (int i_slice = 60; i_slice < 61;/*n_slice_bins;*/ ++i_slice) {
  
    histname.str("");
    histname << "mc_1D_hist_slice" << i_slice;
    TH1D* mc_1D_hist = mc_2D_hist->ProjectionY(histname.str().c_str(), i_slice,i_slice+1, "e");  
    mc_1D_hist->SetLineColor(kRed);

    slices[i_slice-1] = mc_2D_hist->GetXaxis()->GetBinCenter(i_slice);
    slice_widths[i_slice-1] = mc_2D_hist->GetXaxis()->GetBinWidth(i_slice)/2;

    if (mc_1D_hist->GetEntries() == 0) {
      continue;
    }

    /*    TF1* expo_mc = new TF1("expo_mc", "TMath::Exp([0] + [1]*x)", mc_1D_hist->GetXaxis()->GetBinCenter(mc_1D_hist->GetMaximumBin())-300, mc_1D_hist->GetXaxis()->GetBinCenter(mc_1D_hist->GetMaximumBin()));
    expo_mc->SetParameter(0, -50);
    expo_mc->SetParameter(1, 0.01);
    TFitResultPtr expo_fit_mc = mc_1D_hist->Fit(expo_mc, "RSQ+");

    if ( (Int_t) expo_fit_mc == 0) {
      mc_expo_slopes[i_slice-1] = expo_fit_mc->Parameter(1);
      mc_expo_slope_errors[i_slice-1] = expo_fit_mc->ParError(1);
      double diff = std::fabs(expo_fit_mc->Parameter(1) - data_expo_slope);

      std::cout << "i_slice #" << i_slice << " MC: Exponential Slope = " << expo_fit_mc->Parameter(1) << " +/- " << expo_fit_mc->ParError(1) << " (Diff to Data = " << diff << ")" << std::endl;

      if (diff < min_diff) {
	min_diff = diff;
	best_hist = (TH1F*) mc_1D_hist->Clone();
	best_slice = i_slice;
      }
    }
    */

    // Create the workspace and functions for fitting and create the summed pdf as we go
    RooWorkspace* ws = new RooWorkspace("ws", kTRUE);
    std::stringstream factory_cmd, sum_factory_cmd;

    double adc_low = mc_1D_hist->GetXaxis()->GetXmin();
    double adc_high = mc_1D_hist->GetXaxis()->GetXmax();
    double peak_estimate = mc_1D_hist->GetXaxis()->GetBinCenter(mc_1D_hist->GetMaximumBin());
    // First, the Landau convoluted with a Gaussian
    factory_cmd << "Landau::landau_pdf(adc[" << adc_low << ", " << adc_high << "], landau_mean[" << peak_estimate - 400 << ", " << peak_estimate + 400 << "], landau_sigma[0.1, 500])";
    ws->factory(factory_cmd.str().c_str()); factory_cmd.str("");
    //    factory_cmd << "Gaussian::gauss_pdf(adc[" << adc_low << ", " << adc_high << "], gauss_mean[0], gauss_sigma[1, 200])";
    //    ws->factory(factory_cmd.str().c_str()); factory_cmd.str("");
    //    factory_cmd << "FCONV::landau_x_gauss(adc, landau_pdf, gauss_pdf)";
    //    ws->factory(factory_cmd.str().c_str()); factory_cmd.str("");

    //    factory_cmd << "CBShape::crystal(adc[" << adc_low << ", " << adc_high << "], cb_mean[" << peak_estimate-100 << ", " << peak_estimate+100 << "], cb_sigma[0.1, 100], cb_alpha[" << peak_estimate-500 << ", " << peak_estimate << "], cb_order[0,10])";
    //    ws->factory(factory_cmd.str().c_str()); factory_cmd.str("");

    //    factory_cmd << "Gaussian::gauss_pdf(adc[" << adc_low << ", " << adc_high << "], gauss_mean[" << peak_estimate-100 << ", " << peak_estimate+100 << "], gauss_sigma[1, 50])";
    //    ws->factory(factory_cmd.str().c_str()); factory_cmd.str("");
    //    factory_cmd << "EXPR::exp_pdf('exp(adc_shift + exp_slope*adc)', adc, exp_slope[0, 0.01], adc_shift[" << adc_low << ", " << adc_high << "])";
    //    ws->factory(factory_cmd.str().c_str()); factory_cmd.str("");
    //    factory_cmd << "FCONV::exp_x_gauss(adc, exp_pdf, gauss_pdf)";
    //    ws->factory(factory_cmd.str().c_str()); factory_cmd.str("");      

    //    factory_cmd << "Landau::landau_pdf(adc[" << adc_low << ", " << adc_high << "], landau_mean[" << peak_estimate - 400 << ", " << peak_estimate + 400 << "], landau_sigma[0.1, 500])";
    //    ws->factory(factory_cmd.str().c_str()); factory_cmd.str("");
    //    factory_cmd << "Exponential::exp_pdf(adc, exp_slope[0, 0.01])";
    //    ws->factory(factory_cmd.str().c_str()); factory_cmd.str("");
    //    factory_cmd << "FCONV::exp_x_landau(adc, exp_pdf, landau_pdf)";
    //    ws->factory(factory_cmd.str().c_str()); factory_cmd.str("");      

            
    // Import the histogram into a RooFit data hist
    RooDataHist data("data", "Slice", (ws->argSet("adc")), mc_1D_hist);
    (ws->pdf("landau_pdf"))->fitTo(data);
      
    // Draw the fit
    RooPlot* Eframe = (ws->var("adc"))->frame();
    data.plotOn(Eframe, RooFit::MarkerSize(0.05));
    (ws->pdf("landau_pdf"))->plotOn(Eframe, RooFit::LineWidth(1));
    //    (ws->pdf("landau_x_gauss"))->plotOn(Eframe, RooFit::LineWidth(1), RooFit::Components(ws->argSet("landau_pdf")), RooFit::LineColor(kRed));
    //    (ws->pdf("landau_x_gauss"))->plotOn(Eframe, RooFit::LineWidth(1), RooFit::Components(ws->argSet("gauss_pdf")), RooFit::LineColor(kBlue), RooFit::LineStyle(kDashed));
    Eframe->Draw();
      
    histtitle.str("");
    //    histtitle << "SF = " << (*i_scale_factor)->scale_factor << ", Run " << (*i_scale_factor)->run_number << " (SiL1-" << i_si_channel+1 << "-S)";
    Eframe->SetTitle(histtitle.str().c_str());
    Eframe->SetXTitle("Pulse Amplitude [ADC]");
    ws->Print();

    /*    // For Drawing MC and data together
    // Get the difference in the peak positions so we can draw them on top of each other
    TF1* gauss_mc = new TF1("gauss_mc", "[0]*TMath::Gaus(x, [1], [2])", mc_1D_hist->GetXaxis()->GetBinCenter(mc_1D_hist->GetMaximumBin())-15, mc_1D_hist->GetXaxis()->GetBinCenter(mc_1D_hist->GetMaximumBin()+15));
    gauss_mc->SetParameter(0, 100);
    gauss_mc->SetParameter(1, mc_1D_hist->GetXaxis()->GetBinCenter(mc_1D_hist->GetMaximumBin()));
    gauss_mc->SetParameter(2, 1);
    TFitResultPtr fit_mc = mc_1D_hist->Fit(gauss_mc, "RSQN0");
    double mc_peak_pos = fit_mc->Parameter(1);

    TF1* gauss_data = new TF1("gauss_data", "[0]*TMath::Gaus(x, [1], [2])", data_1D_hist->GetXaxis()->GetBinCenter(data_1D_hist->GetMaximumBin())-15, data_1D_hist->GetXaxis()->GetBinCenter(data_1D_hist->GetMaximumBin()+15));
    gauss_data->SetParameter(0, 100);
    gauss_data->SetParameter(1, data_1D_hist->GetXaxis()->GetBinCenter(data_1D_hist->GetMaximumBin()));
    gauss_data->SetParameter(2, 1);
    TFitResultPtr fit_data = data_1D_hist->Fit(gauss_data, "RSQN0");
    double data_peak_pos = fit_data->Parameter(1);

    double peak_difference = mc_peak_pos - data_peak_pos;

    //    std::cout << "AE: MC Peak Pos = " << mc_peak_pos << ", Data Peak Pos = " << data_peak_pos << ", Difference = " << peak_difference << std::endl;
    
    // now shift the data
    double cut_off = 0;
    histname.str("");
    histname << "hNewAmp_forSlice" << i_slice;
    TH1F* hNewAmp = new TH1F(histname.str().c_str(), "", mc_1D_hist->GetNbinsX(), mc_1D_hist->GetXaxis()->GetXmin(), mc_1D_hist->GetXaxis()->GetXmax());
    for (int i_bin = 1; i_bin < data_1D_hist->GetNbinsX(); ++i_bin) {
      double old_bin_center = data_1D_hist->GetBinCenter(i_bin);
      double new_bin_center = old_bin_center+peak_difference;
      int new_bin = hNewAmp->FindBin(new_bin_center);

      //      std::cout << "Old Bin #" << i_bin << " (center = " << old_bin_center << ") --> New Bin #" << new_bin << " (center = " << new_bin_center << ")" << std::endl;
      if (hNewAmp->GetBinCenter(new_bin) < cut_off ) {
	continue; // get rid of the noise peak
      }
      hNewAmp->SetBinContent(new_bin, data_1D_hist->GetBinContent(i_bin));
      hNewAmp->SetBinError(new_bin, data_1D_hist->GetBinError(i_bin));
    }

    for (int i_bin = 1; i_bin < mc_1D_hist->GetNbinsX(); ++i_bin) {
      if (mc_1D_hist->GetBinCenter(i_bin) < cut_off ) {
	mc_1D_hist->SetBinContent(i_bin, 0);
	mc_1D_hist->SetBinError(i_bin, 0);
      }
    }

    if (data_1D_hist->GetMaximum() > mc_1D_hist->GetMaximum()) {
    mc_1D_hist->SetMaximum(data_1D_hist->GetMaximum()*1.1);
    }
    mc_1D_hist->Draw("HIST E");
    hNewAmp->Draw("HIST E SAMES");
    /**/
  }

  TCanvas* c_gr = new TCanvas("c_gr", "c_gr");
  std::string gr_title = "Exponential Fits to Low-Side Am241 Tail for Data (" + channel + ") and MC (as a function of starting depth";
  TGraphErrors* gr_expo_slopes = new TGraphErrors(n_slice_bins, slices, mc_expo_slopes, slice_widths, mc_expo_slope_errors);
  gr_expo_slopes->Draw("APE");
  gr_expo_slopes->SetTitle(gr_title.c_str());
  gr_expo_slopes->GetXaxis()->SetTitle("Starting Depth [#mum]");
  gr_expo_slopes->GetYaxis()->SetTitle("Slope of Exponential Fit");
  gr_expo_slopes->SetMarkerColor(kRed);
  gr_expo_slopes->SetLineColor(kRed);

  TBox* box = new TBox(min_z, data_expo_slope-data_expo_slope_error, max_z, data_expo_slope+data_expo_slope_error);
  box->SetFillStyle(3004);
  box->SetFillColor(kBlack);
  box->SetLineColor(kBlack);
  box->Draw("LSAME");

  TLine* line = new TLine(min_z, data_expo_slope, max_z, data_expo_slope);
  line->SetLineWidth(1);
  line->Draw("LSAME");

  TCanvas* c_best_mc = new TCanvas("c_best_mc", "c_best_mc");
  best_hist->Draw("HIST E");
  best_hist->GetFunction("expo_mc")->Draw("LSAME");
  std::cout << "Best Slice = " << best_slice << " (Diff = " << min_diff << ")" << std::endl;
}
