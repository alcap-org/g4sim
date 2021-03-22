void R15b_MuonBeamSim_StoppingPositionDistribution_DifferentMuonBeams_SiL3() {

  const int n_settings = 3;
  std::string setting_names[n_settings] = {"-lowerE", "", "-higherE"};
  Int_t colours[n_settings] = {kRed, kBlack, kBlue};
  std::string leglabels[n_settings] = {"Ekin = 3.75 MeV", "EKin = 3.80 MeV", "EKin = 3.85 MeV"};

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
    filename << "data/R15b_Geom-P1" << i_setting_name << "_muon-stop-positions.root";
    TFile* file = new TFile(filename.str().c_str(), "READ");
    TH3F* h3DStopPos = (TH3F*) file->Get("hLocalStoppingPositions");

    histname.str("");
    histname << "hZStopPos_" << i_setting;

    TH1D* hZStopPos = h3DStopPos->ProjectionZ(histname.str().c_str());
    hZStopPos->SetLineWidth(2);
    hZStopPos->SetLineColor(i_colour);
    hZStopPos->SetStats(false);
    hZStopPos->GetXaxis()->SetRangeUser(-600,600);
    //    hZStopPos->SetMaximum(4000);
    hZStopPos->SetTitle("Muon Stopping Depth");
    hZStopPos->Draw("HIST E SAME");

    leglabel.str("");
    leglabel << leglabels[i_setting];
    leg->AddEntry(hZStopPos, leglabel.str().c_str(), "l");
  }
  leg->Draw();
}
