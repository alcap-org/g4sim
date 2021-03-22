void R15b_DataMCDiscrepancy_Protons() {

  std::string data_filename = "plotsTi50.root";
  //  std::string data_filename = "plotsSi16b.root";
  //  std::string mc_filename = "R15b_Ti50_response-matrix_500keV-bins_protons-SWSpec.root";
  //  std::string mc_filename = "dump_protons.root";

  /*  
  const int n_mc_files = 3;
  std::string mc_particles[n_mc_files] = {"protons", "deuterons", "tritons"};
  Int_t stopped_colours[n_mc_files] = {kRed, kMagenta, kSpring};
  Int_t notstopped_colours[n_mc_files] = {kBlue, kCyan, kYellow};
  */

  
  const int n_mc_files = 1;
  std::string mc_particles[n_mc_files] = {"protons"};
  Int_t stopped_colours[n_mc_files] = {kRed};
  Int_t notstopped_colours[n_mc_files] = {kBlue};
  
  std::string arm = "SiL";
  //  //  std::string data_histname = "all_charged_" + arm + "/hSingleDetAxes_TwoLayer_12";
  //  std::string data_histname = "all_charged_" + arm + "_timecut/hSingleDetAxes_TwoLayer_12";
  std::string data_histname = "all_proton_" + arm + "/hSingleDetAxes_TwoLayer_12";
  //  //  std::string data_histname = "all_proton_" + arm + "_timecut/hSingleDetAxes_TwoLayer_12";
  //    std::string data_histname = "all_" + arm + "/hSingleDetAxes_TwoLayer_12";
  std::string mc_histname_stopped = arm + "_SingleDetAxes_TwoLayer_stopped";
  std::string mc_histname_notstopped = arm + "_SingleDetAxes_TwoLayer_notstopped";
  
  //  std::string data_histname = "all_" + arm + "/hSingleDetAxes_ThreeLayer_23";
  //  std::string mc_histname_stopped = arm + "_SingleDetAxes_ThreeLayer23_stopped";
  //  std::string mc_histname_notstopped = arm + "_SingleDetAxes_ThreeLayer23_notstopped";

  TFile* data_file = new TFile(data_filename.c_str(), "READ");
  TH2F* data_hist = (TH2F*) data_file->Get(data_histname.c_str());
  data_hist->SetTitle("E_{1} vs E_{2}");
  data_hist->Sumw2();
  data_hist->SetStats(false);
  data_hist->SetMarkerStyle(kFullDotSmall);

  double x_axis_min = 0; double x_axis_max = 20000;
  double y_axis_min = 0; double y_axis_max = 3000;
  //  double y_axis_min = 0; double y_axis_max = 20000;
  data_hist->GetXaxis()->SetRangeUser(x_axis_min, x_axis_max);
  data_hist->GetYaxis()->SetRangeUser(y_axis_min, y_axis_max);

  TLegend* leg = new TLegend(0.42, 0.85, 0.62, 0.55, "");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->SetFillColor(kWhite);
  leg->AddEntry(data_hist, "Data", "p");

  TCanvas* c1 = new TCanvas("c1", "c1");
  data_hist->Draw();

  TCanvas* c1_inverse = new TCanvas("c1_inverse", "c1_inverse");

  // Try getting some slices of data and MC and look at the differences
  TCanvas* c_E1Proj = new TCanvas("c_E1Proj", "c_E1Proj");
  c_E1Proj->Divide(4, 4);
  const int n_slices = 16;
  double min_energy_slices[n_slices] = {  0,  500, 1000, 1500, 2500, 3500, 4000, 5500, 7000, 8500, 10000, 11500, 13000, 14500, 15000, 16500};
  double max_energy_slices[n_slices] = {500, 1000, 1500, 2000, 3000, 4000, 5500, 7000, 8500, 10000, 11500, 13000,14500, 15000, 16500, 18000};
  double max_x_axis[n_slices];// = {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000};
  for (int i_slice = 0; i_slice < n_slices; ++i_slice) {
    max_x_axis[i_slice] = y_axis_max;
  }

  //  double min_energy_slices[n_slices] = {2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 11000, 12000, 13000, 14000, 15000, 16000, 17000};
  //  double max_energy_slices[n_slices] = {3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000,11000, 12000, 13000, 14000, 15000, 16000, 17000, 18000};
  TCanvas* c_E2Proj = new TCanvas("c_E2Proj", "c_E2Proj");
  c_E2Proj->Divide(4,3);
  const int n_E1_slices = 10;
  double min_E1_slices[n_E1_slices] = {  0, 50, 100, 150, 200, 250, 300, 350, 400, 450};
  double max_E1_slices[n_E1_slices] = {50, 100, 150, 200, 250, 300, 350, 400, 450, 500};
  //  double max_x_axis[n_E1_slices];// = {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000};

  TH1F* hChi2_thin;
  TH1F* hChi2_thick;
  TH1F* hMCE1Sums[n_slices] = {0};
  TH1F* hMCE2Sums[n_E1_slices] = {0};
  
  std::stringstream leglabel, histname, histtitle;
  for (int i_mc_file = 0; i_mc_file < n_mc_files; ++i_mc_file) {
    std::string i_particle = mc_particles[i_mc_file];
    std::string mc_filename = "dump_" + i_particle + ".root";
    TFile* mc_file = new TFile(mc_filename.c_str(), "READ");

    // Draw 2D plots first
    TH2F* mc_hist_stopped = (TH2F*) mc_file->Get(mc_histname_stopped.c_str());
    mc_hist_stopped->SetMarkerColor(stopped_colours[i_mc_file]);
    //    mc_hist_stopped->SetMarkerColor(kRed);
    mc_hist_stopped->SetTitle("E_{1} vs E_{2}");
    mc_hist_stopped->SetStats(false);
    mc_hist_stopped->SetMarkerStyle(kFullDotSmall);
    mc_hist_stopped->Sumw2();
    TH2F* mc_hist_notstopped = (TH2F*) mc_file->Get(mc_histname_notstopped.c_str());
    mc_hist_notstopped->SetMarkerColor(notstopped_colours[i_mc_file]);
    //    mc_hist_notstopped->SetMarkerColor(kBlue);
    mc_hist_notstopped->SetMarkerStyle(kFullDotSmall);
    mc_hist_notstopped->Sumw2();
    
    mc_hist_stopped->GetXaxis()->SetRangeUser(x_axis_min, x_axis_max);
    mc_hist_stopped->GetYaxis()->SetRangeUser(y_axis_min, y_axis_max);
    mc_hist_notstopped->GetXaxis()->SetRangeUser(x_axis_min, x_axis_max);
    mc_hist_notstopped->GetYaxis()->SetRangeUser(y_axis_min, y_axis_max);

    c1->cd();
    mc_hist_stopped->Draw("SAME");
    if (i_mc_file==0) {
      mc_hist_notstopped->Draw("SAME");
    }

    c1_inverse->cd();
    mc_hist_stopped->Draw("SAME");
    if (i_mc_file==0) {
      mc_hist_notstopped->Draw("SAME");
    }

    
    leglabel.str("");
    leglabel << "MC (stopped " << i_particle << ")";
    leg->AddEntry(mc_hist_stopped, leglabel.str().c_str(), "p");
    leglabel.str("");
    leglabel << "MC (not stopped " << i_particle << ")";
    leg->AddEntry(mc_hist_notstopped, leglabel.str().c_str(), "p");  


    // Now draw slices
    hChi2_thin = new TH1F("hChi2_thin", "", n_slices,0,n_slices);
    hChi2_thin->SetTitle(arm.c_str());
    hChi2_thin->SetYTitle("#chi^{2} / ndf");
    hChi2_thin->SetXTitle("Slice");
    std::stringstream binlabel;
    for (int i_slice = 0; i_slice < n_slices; ++i_slice) {
      c_E1Proj->cd(i_slice+1);
      c_E1Proj->GetPad(i_slice+1)->SetLogy();

      double low_energy_slice = min_energy_slices[i_slice];
      double high_energy_slice = max_energy_slices[i_slice];

      binlabel.str("");
      binlabel << low_energy_slice << " keV < E < " << high_energy_slice << " keV";
      hChi2_thin->GetXaxis()->SetBinLabel(i_slice+1, binlabel.str().c_str());

      TH1F* hMCE1Sum = hMCE1Sums[i_slice];
    
      int low_energy_slice_data_bin = data_hist->GetXaxis()->FindBin(low_energy_slice);
      int high_energy_slice_data_bin = data_hist->GetXaxis()->FindBin(high_energy_slice);  
      int low_energy_slice_mc_bin = mc_hist_stopped->GetXaxis()->FindBin(low_energy_slice);
      int high_energy_slice_mc_bin = mc_hist_stopped->GetXaxis()->FindBin(high_energy_slice);
      histname.str("");
      histname << "data_slice_" << low_energy_slice << "_" << i_mc_file;
      TH1D* hDataSlice = data_hist->ProjectionY(histname.str().c_str(), low_energy_slice_data_bin, high_energy_slice_data_bin);
      hDataSlice->SetLineColor(kBlack);
      histname.str("");
      histname << "mc_slice_stopped_" << low_energy_slice << "_" << i_mc_file;
      TH1D* hMCSliceStopped = mc_hist_stopped->ProjectionY(histname.str().c_str(), low_energy_slice_mc_bin, high_energy_slice_mc_bin);
      hMCSliceStopped->SetLineColor(stopped_colours[i_mc_file]);
      histname.str("");
      histname << "mc_slice_not_stopped_" << low_energy_slice << "_" << i_mc_file;
      TH1D* hMCSliceNotStopped = mc_hist_notstopped->ProjectionY(histname.str().c_str(), low_energy_slice_mc_bin, high_energy_slice_mc_bin);
      hMCSliceNotStopped->SetLineColor(notstopped_colours[i_mc_file]);
    
      int rebin_factor = 2;
      hDataSlice->Rebin(rebin_factor);
      hMCSliceStopped->Rebin(rebin_factor);
      hMCSliceNotStopped->Rebin(rebin_factor);

      // Want to avoid the noise peak in the data since we want the proton peak
      int data_high_bin = hDataSlice->GetMaximumBin();
      int mc_stopped_high_bin = hMCSliceStopped->GetMaximumBin();
      double mc_stopped_bin_center = hMCSliceStopped->GetXaxis()->GetBinCenter(mc_stopped_high_bin);
      int mc_notstopped_high_bin = hMCSliceNotStopped->GetMaximumBin();
      double mc_notstopped_bin_center = hMCSliceNotStopped->GetXaxis()->GetBinCenter(mc_notstopped_high_bin);

      //      hDataSlice->Scale(1.0 / hDataSlice->GetBinContent(data_high_bin));
      double mc_stopped_scale_factor = hMCSliceStopped->GetBinContent(mc_stopped_high_bin) / hDataSlice->GetBinContent(hDataSlice->FindBin(mc_stopped_bin_center));
      if (mc_stopped_scale_factor > 0) {
	hMCSliceStopped->Scale(1.0 / mc_stopped_scale_factor);
      }
      double mc_notstopped_scale_factor = hMCSliceNotStopped->GetBinContent(mc_notstopped_high_bin) / hDataSlice->GetBinContent(hDataSlice->FindBin(mc_notstopped_bin_center));
      if (mc_notstopped_scale_factor > 0) {
	hMCSliceNotStopped->Scale(1.0 / mc_notstopped_scale_factor);
      }


      //      std::cout << "AE: " << hDataSlice->GetBinContent(hDataSlice->FindBin(mc_stopped_bin_center)) << " " << hDataSlice->GetBinContent(hDataSlice->FindBin(mc_notstopped_bin_center)) << std::endl;

      histname.str("");
      histname << "mc_slice_" << low_energy_slice << "_" << i_mc_file;
      TH1D* hMCSlice = (TH1D*) hMCSliceStopped->Clone(histname.str().c_str());
      hMCSlice->Add(hMCSliceNotStopped);
      hMCSlice->SetLineColor(kGreen);

      //      if (hMCSlice->GetEntries()==0 || hDataSlice->GetEntries()==0) {
      //	continue;
      //      }	

      
      histtitle.str("");
      histtitle << low_energy_slice << " keV < E_{2} < " << high_energy_slice << " keV";// << std::endl;
      hDataSlice->SetTitle(histtitle.str().c_str());
      hDataSlice->GetXaxis()->SetRangeUser(0,max_x_axis[i_slice]);
      hDataSlice->Draw("HIST E SAMES");
      hMCSliceStopped->Draw("HIST E SAMES");
      if (i_mc_file==0) {
	hMCSliceNotStopped->Draw("HIST E SAMES");
      }
      //      hMCSlice->Draw("HIST E SAMES");
      
      //      std::cout << hMCE1Sum << std::endl;
      if (!hMCE1Sum) {
	histname.str("");
	histname << "mc_slice_sum_" << low_energy_slice;
	hMCE1Sum = (TH1F*) hMCSliceStopped->Clone(histname.str().c_str());
	if (i_mc_file==0) {
	  hMCE1Sum->Add(hMCSliceNotStopped);
	}
      }
      else {
	hMCE1Sum->Add(hMCSliceStopped);
	if (i_mc_file==0) {
	  hMCE1Sum->Add(hMCSliceNotStopped);
	}
      }
      hMCE1Sum->SetLineColor(kCyan);
      //      hMCE1Sum->Draw("HIST E SAMES");
      hMCE1Sums[i_slice] = hMCE1Sum;
      /*      if (hMCSliceStopped->Integral()>0) {
	std::cout << i_particle << " stopped " << histtitle.str() << " KS = " << hDataSlice->KolmogorovTest(hMCSliceStopped) << std::endl;
      }
      if (hMCSliceNotStopped->Integral()>0) {
	std::cout << i_particle << " notstopped " << histtitle.str() << " KS = " << hDataSlice->KolmogorovTest(hMCSliceNotStopped) << std::endl;
      }
      */
      //      std::cout << hMCE1Sum->Integral() << std::endl;
      //      std::cout << histtitle.str() << " KS = " << hDataSlice->KolmogorovTest(hMCE1Sum) << std::endl;
      double chi2_ndf = hDataSlice->Chi2Test(hMCE1Sum, "UW CHI2/NDF");
      std::cout << histtitle.str() << " Chi^2/ndf = " << chi2_ndf << std::endl;
      if (chi2_ndf > 0) {
	hChi2_thin->SetBinContent(i_slice+1, chi2_ndf);
      }
      
      //    hMCSliceStopped->Draw("HIST E SAMES");
      //    hMCSliceNotStopped->Draw("HIST E SAMES");
    }

    c_E2Proj->cd();
    //    c_E2Proj->Divide(4,4);
    //    const int n_E1_slices = 16;
    //    double min_E1_slices[n_E1_slices] = {  0,  100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500};
    //    double max_E1_slices[n_E1_slices] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600};
    for (int i_E1_slice = 0; i_E1_slice < n_E1_slices; ++i_E1_slice) {
      max_x_axis[i_E1_slice] = x_axis_max;
    }


    hChi2_thick = new TH1F("hChi2_thick", "", n_E1_slices,0,n_E1_slices);
    hChi2_thick->SetTitle(arm.c_str());
    hChi2_thick->SetYTitle("#chi^{2} / ndf");
    hChi2_thick->SetXTitle("Slice");
    for (int i_E1_slice = 0; i_E1_slice < n_E1_slices; ++i_E1_slice) { 
      c_E2Proj->cd(i_E1_slice+1);
      double low_energy_slice = min_E1_slices[i_E1_slice];
      double high_energy_slice = max_E1_slices[i_E1_slice];

      
      binlabel.str("");
      binlabel << low_energy_slice << " keV < E < " << high_energy_slice << " keV";
      hChi2_thick->GetXaxis()->SetBinLabel(i_E1_slice+1, binlabel.str().c_str());

      TH1F* hMCE2Sum = hMCE2Sums[i_E1_slice];
      
      int low_energy_slice_data_bin = data_hist->GetYaxis()->FindBin(low_energy_slice);
      int high_energy_slice_data_bin = data_hist->GetYaxis()->FindBin(high_energy_slice);  
      int low_energy_slice_mc_bin = mc_hist_stopped->GetYaxis()->FindBin(low_energy_slice);
      int high_energy_slice_mc_bin = mc_hist_stopped->GetYaxis()->FindBin(high_energy_slice);
      histname.str("");
      histname << "data_E1_slice_" << low_energy_slice << "_" << i_mc_file;
      TH1D* hDataSlice = data_hist->ProjectionX(histname.str().c_str(), low_energy_slice_data_bin, high_energy_slice_data_bin);
      hDataSlice->SetLineColor(kBlack);
      histname.str("");
      histname << "mc_E1_slice_stopped_" << low_energy_slice << "_" << i_mc_file;
      TH1D* hMCSliceStopped = mc_hist_stopped->ProjectionX(histname.str().c_str(), low_energy_slice_mc_bin, high_energy_slice_mc_bin);
      hMCSliceStopped->SetLineColor(stopped_colours[i_mc_file]);
      histname.str("");
      histname << "mc_E1_slice_not_stopped_" << low_energy_slice << "_" << i_mc_file;
      TH1D* hMCSliceNotStopped = mc_hist_notstopped->ProjectionX(histname.str().c_str(), low_energy_slice_mc_bin, high_energy_slice_mc_bin);
      hMCSliceNotStopped->SetLineColor(notstopped_colours[i_mc_file]);
    
      int rebin_factor = 50;
      hDataSlice->Rebin(rebin_factor);
      hMCSliceStopped->Rebin(rebin_factor);
      hMCSliceNotStopped->Rebin(rebin_factor);

      // Want to avoid the noise peak in the data since we want the proton peak
      int data_high_bin = hDataSlice->GetMaximumBin();
      int mc_stopped_high_bin = hMCSliceStopped->GetMaximumBin();
      double mc_stopped_bin_center = hMCSliceStopped->GetXaxis()->GetBinCenter(mc_stopped_high_bin);
      int mc_notstopped_high_bin = hMCSliceNotStopped->GetMaximumBin();
      double mc_notstopped_bin_center = hMCSliceNotStopped->GetXaxis()->GetBinCenter(mc_notstopped_high_bin);

      double mc_stopped_scale_factor = hMCSliceStopped->GetBinContent(mc_stopped_high_bin) / hDataSlice->GetBinContent(hDataSlice->FindBin(mc_stopped_bin_center));
      if (mc_stopped_scale_factor > 0) {
	hMCSliceStopped->Scale(1.0 / mc_stopped_scale_factor);
      }
      double mc_notstopped_scale_factor = hMCSliceNotStopped->GetBinContent(mc_notstopped_high_bin) / hDataSlice->GetBinContent(hDataSlice->FindBin(mc_notstopped_bin_center));
      if (mc_notstopped_scale_factor > 0) {
	hMCSliceNotStopped->Scale(1.0 / mc_notstopped_scale_factor);
      }

      histname.str("");
      histname << "mc_E1_slice_" << low_energy_slice << "_" << i_mc_file;
      TH1D* hMCSlice = (TH1D*) hMCSliceStopped->Clone(histname.str().c_str());
      hMCSlice->Add(hMCSliceNotStopped);
      hMCSlice->SetLineColor(kGreen);

      
      histtitle.str("");
      histtitle << low_energy_slice << " keV < E_{1} < " << high_energy_slice << " keV";// << std::endl;
      hDataSlice->SetTitle(histtitle.str().c_str());
      hDataSlice->GetXaxis()->SetRangeUser(0,max_x_axis[i_E1_slice]);
      hDataSlice->Draw("HIST E SAMES");
      hMCSliceStopped->Draw("HIST E SAMES");
      if (i_mc_file==0) {
	hMCSliceNotStopped->Draw("HIST E SAMES");
      }

      if (!hMCE2Sum) {
	histname.str("");
	histname << "mc_E1_slice_sum_" << low_energy_slice;
	hMCE2Sum = (TH1F*) hMCSliceStopped->Clone(histname.str().c_str());
	if (i_mc_file==0) {
	  hMCE2Sum->Add(hMCSliceNotStopped);
	}
      }
      else {
	hMCE2Sum->Add(hMCSliceStopped);
	if (i_mc_file==0) {
	  hMCE2Sum->Add(hMCSliceNotStopped);
	}
      }
      hMCE2Sum->SetLineColor(kCyan);
      hMCE2Sums[i_E1_slice] = hMCE2Sum;
      
      hMCE2Sum->SetXTitle("E_{2} Energy [keV]");
      hMCSlice->SetXTitle("E_{2} Energy [keV]");
      //      hMCE2Sum->Draw("HIST E SAMES");
      //      hDataSlice->Draw("HIST E SAMES");
      //      hMCSliceStopped->Draw("HIST E SAME");
      //      if (i_mc_file==0) {
      //	hMCSliceNotStopped->Draw("HIST E SAME");
      //      }
      //    hMCE2Sum->Draw("HIST E SAMES");

      double chi2_ndf = hDataSlice->Chi2Test(hMCE2Sum, "UW CHI2/NDF");
      std::cout << histtitle.str() << " Chi^2/ndf = " << chi2_ndf << std::endl;
      if (chi2_ndf > 0) {
	hChi2_thick->SetBinContent(i_E1_slice+1, chi2_ndf);
      }
    }
  }
  
  c1->cd();
  leg->Draw();

  c1_inverse->cd();
  data_hist->Draw("SAME");
  leg->Draw();

  TCanvas* c_chi2_thin = new TCanvas("c_chi2_thin", "c_chi2_thin");
  hChi2_thin->SetMinimum(0);
  hChi2_thin->SetMaximum(50);
  //  hChi2_thin->SetMaximum(20);
  hChi2_thin->Draw();

  TCanvas* c_chi2_thick = new TCanvas("c_chi2_thick", "c_chi2_thick");
  hChi2_thick->SetMinimum(0);
  //  hChi2_thick->SetMaximum(50);
  hChi2_thick->SetMaximum(20);
  hChi2_thick->Draw();


  /*
  double min_energy_slice = 0;
  double max_energy_slice = 14000;
  double energy_slice_width = 200;
  int n_slices_all = (max_energy_slice - min_energy_slice) / energy_slice_width;
  TH1F* hThinRatios = new TH1F("hThinRatios", "Ratio per Thick Slice", n_slices_all, min_energy_slice, max_energy_slice);
  TCanvas* c3 = new TCanvas("c3", "c3");

  for (double low_energy_slice = min_energy_slice; low_energy_slice < max_energy_slice; low_energy_slice += energy_slice_width) {

    double high_energy_slice = low_energy_slice + energy_slice_width;
    
    int low_energy_slice_data_bin = data_hist->GetXaxis()->FindBin(low_energy_slice);
    int high_energy_slice_data_bin = data_hist->GetXaxis()->FindBin(high_energy_slice);  
    int low_energy_slice_mc_bin = mc_hist_stopped->GetXaxis()->FindBin(low_energy_slice);
    int high_energy_slice_mc_bin = mc_hist_stopped->GetXaxis()->FindBin(high_energy_slice);

    histname.str("");
    histname << "data_slice_" << low_energy_slice;
    TH1D* hDataSlice = data_hist->ProjectionY(histname.str().c_str(), low_energy_slice_data_bin, high_energy_slice_data_bin);
    hDataSlice->SetLineColor(kBlack);
    histname.str("");
    histname << "mc_slice_stopped_" << low_energy_slice;
    TH1D* hMCSliceStopped = mc_hist_stopped->ProjectionY(histname.str().c_str(), low_energy_slice_mc_bin, high_energy_slice_mc_bin);
    hMCSliceStopped->SetLineColor(kRed);
    histname.str("");
    histname << "mc_slice_not_stopped_" << low_energy_slice;
    TH1D* hMCSliceNotStopped = mc_hist_notstopped->ProjectionY(histname.str().c_str(), low_energy_slice_mc_bin, high_energy_slice_mc_bin);
    hMCSliceNotStopped->SetLineColor(kBlue);

    histname.str("");
    histname << "mc_slice_" << low_energy_slice;
    TH1D* hMCSlice = (TH1D*) hMCSliceStopped->Clone(histname.str().c_str());
    hMCSlice->Add(hMCSliceNotStopped);
    hMCSlice->SetLineColor(kMagenta);

    if (hMCSlice->GetEntries()==0 || hDataSlice->GetEntries()==0) {
      continue;
    }	
    
    int rebin_factor = 1;
    hDataSlice->Rebin(rebin_factor);
    hMCSlice->Rebin(rebin_factor);
    hMCSliceStopped->Rebin(rebin_factor);
    hMCSliceNotStopped->Rebin(rebin_factor);

    // Want to avoid the noise peak in the data since we want the proton peak
    int data_high_bin = hDataSlice->GetMaximumBin();
    int mc_high_bin = hMCSlice->GetMaximumBin();

    double mc_scale_factor = hMCSlice->GetBinContent(mc_high_bin);
    hDataSlice->Scale(1.0 / hDataSlice->GetBinContent(data_high_bin));
    hMCSlice->Scale(1.0 / mc_scale_factor);
    hMCSliceStopped->Scale(1.0 / mc_scale_factor);
    hMCSliceNotStopped->Scale(1.0 / mc_scale_factor);

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
  }

  hThinRatios->Draw();
  */
  
}
