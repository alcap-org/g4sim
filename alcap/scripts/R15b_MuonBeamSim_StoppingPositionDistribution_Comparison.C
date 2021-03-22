void R15b_MuonBeamSim_StoppingPositionDistribution_Comparison() {

  const int n_settings = 4;
  double rotations[n_settings] = {45, 40, 35, 30};
  Int_t colours[n_settings] = {kBlue, kRed, kBlack, kMagenta};

  TLegend *leg = new TLegend(0.19,0.64,0.39,0.88,NULL,"brNDC");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.03);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  
  std::stringstream filename, histname, leglabel;
  for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
    double i_rotation = rotations[i_setting];
    Int_t i_colour = colours[i_setting];
    
    filename.str("");
    filename << "R15b_muon-stop-positions_SiL1-Ephi--" << i_rotation << "deg.root";
    TFile* file = new TFile(filename.str().c_str(), "READ");
    TH3F* h3DStopPos = (TH3F*) file->Get("hGlobalStoppingPositions");

    histname.str("");
    histname << "hZStopPos_" << i_rotation << "deg";
    
    TH1D* hZStopPos = h3DStopPos->ProjectionZ(histname.str().c_str());
    hZStopPos->SetLineWidth(2);
    hZStopPos->SetLineColor(i_colour);
    hZStopPos->SetStats(false);
    hZStopPos->SetMaximum(4000);
    hZStopPos->SetTitle("Muon Stopping Depth for Si16 Target");
    hZStopPos->Draw("HIST E SAME");

    leglabel.str("");
    leglabel << "SiL1 @ " << i_rotation << " degrees";
    leg->AddEntry(hZStopPos, leglabel.str().c_str(), "l");
  }
  leg->Draw();
}
