void R15b_MuonBeamSim_StoppingPositionDistribution_DifferentMuonBeams_Si16b() {

  // const int n_settings = 5;
  // std::string setting_names[n_settings] = {"-lowestE", "-lowerE", "", "-higherE", "-highestE"};
  // Int_t colours[n_settings] = {kRed, kMagenta, kBlack, kCyan, kBlue};
  // std::string leglabels[n_settings] = {"Ekin = 1.19 MeV", "Ekin = 1.34 MeV", "EKin = 1.37 MeV", "EKin = 1.40 MeV", "EKin = 1.55 MeV"};

  // const int n_settings = 5;
  // std::string setting_names[n_settings] = {"-midLowE", "-lowerE", "", "-higherE", "-midHighE"};
  // Int_t colours[n_settings] = {kRed, kMagenta, kBlack, kCyan, kBlue};
  // std::string leglabels[n_settings] = {"Ekin = 1.27 MeV", "Ekin = 1.34 MeV", "EKin = 1.37 MeV", "EKin = 1.40 MeV", "EKin = 1.47 MeV"};

  const int n_settings = 7;
  std::string setting_names[n_settings] = {"-lowestE", "-midLowE", "-lowerE", "", "-higherE", "-midHighE", "-highestE"};
  Int_t colours[n_settings] = {kRed, kBlue, kBlack, kMagenta, kGreen, kGray, kYellow};
  std::string leglabels[n_settings] = {"lowestE", "midLowE", "lowerE", "central", "higherE", "midHighE", "highestE"};
  double mc_energies[n_settings] = {1.19, 1.27, 1.34, 1.37, 1.40, 1.47, 1.55};
  double peak_positions[n_settings] = {0};

  // const int n_settings = 3;
  // std::string setting_names[n_settings] = {"-lowerE", "", "-higherE"};
  // Int_t colours[n_settings] = {kRed, kBlack, kBlue};
  // std::string leglabels[n_settings] = {"Ekin = 1.34 MeV", "EKin = 1.37 MeV", "EKin = 1.40 MeV"};

  TLegend *leg = new TLegend(0.17,0.64,0.37,0.88,NULL,"brNDC");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.03);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  
  std::stringstream filename, histname, leglabel;
  for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
    std::string i_setting_name = setting_names[i_setting];
    Int_t i_colour = colours[i_setting];
    
    filename.str("");
    //    filename << "data/R15b_Geom-P5" << i_setting_name << "_muon-stop-positions.root";
    filename << "data/R15b_Geom-P5" << i_setting_name << "_muon-stop-positions_new.root";
    TFile* file = new TFile(filename.str().c_str(), "READ");
    TH3F* h3DStopPos = (TH3F*) file->Get("hLocalStoppingPositions");

    histname.str("");
    histname << "hZStopPos_" << i_setting;

    TH1D* hZStopPos = h3DStopPos->ProjectionZ(histname.str().c_str());
    hZStopPos->SetLineWidth(2);
    hZStopPos->SetLineColor(i_colour);
    //    hZStopPos->SetStats(false);
    //    hZStopPos->SetMaximum(4000);
    hZStopPos->SetTitle("Muon Stopping Depth");
    hZStopPos->Rebin(2);
    //    hZStopPos->Draw("HIST E SAMES");
    hZStopPos->Draw("HIST E PLC SAMES");

    leglabel.str("");
    leglabel << leglabels[i_setting];
    leg->AddEntry(hZStopPos, leglabel.str().c_str(), "l");

    double max_bin_pos = hZStopPos->GetXaxis()->GetBinCenter(hZStopPos->GetMaximumBin());
    std::cout << "Peak Pos (max_bin_pos) = " << max_bin_pos << std::endl;
    //    peak_positions[i_setting] = max_bin_pos;

    double max_fit = max_bin_pos+3;
    if (i_setting >= 5) {
      max_fit = max_bin_pos+1;
    }
    hZStopPos->Fit("gaus", "", "", max_bin_pos-7, max_fit);
    double fit_pos = hZStopPos->GetFunction("gaus")->GetParameter(1);
    hZStopPos->GetFunction("gaus")->Draw("LSAME");
    std::cout << "Peak Pos (fit_pos) = " << fit_pos << std::endl;
    peak_positions[i_setting] = fit_pos;
  }

  TLatex* latex = new TLatex();
  latex->DrawLatexNDC(0.15, 0.55, "AlCap #it{#bf{Simulation} }");
  
  leg->Draw();

  TCanvas* c = new TCanvas();
  TGraph* gr = new TGraph(n_settings, mc_energies, peak_positions);
  gr->Draw("A*L");
  gr->GetXaxis()->SetTitle("MC Beam Energy [MeV]");
  gr->GetYaxis()->SetTitle("Stopping Depth [#mu m]");

  TLatex* latex2 = new TLatex();
  latex2->SetTextSize(0.03);
  std::stringstream text;
  for (int i_point = 0; i_point < gr->GetN(); ++i_point) {
    text.str("");
    text << std::fixed << std::setprecision(2) << " E_{MC} = " << mc_energies[i_point] << " MeV, stop = " << std::setprecision(1) << peak_positions[i_point] << " #mu m";
    if (i_point == gr->GetN()-1) {
      latex2->SetTextAlign(31);
    }
    latex2->DrawLatex(mc_energies[i_point], peak_positions[i_point], text.str().c_str());
  }
}
