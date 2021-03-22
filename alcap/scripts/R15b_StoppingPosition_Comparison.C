void R15b_StoppingPosition_Comparison() {

  const int n_settings = 2;
  std::string filenames[n_settings] = {"data/R15b_output_Geom-P1_muon-stop-positions_new.root",
				       "data/dump.root"};
  Int_t colours[n_settings] = {kBlue, kRed};//, kBlack, kMagenta, kGreen, kGray};

  std::stringstream histname;
  for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
    TFile* file = new TFile(filenames[i_setting].c_str(), "READ");
    TH3F* hLocalStopPos = (TH3F*) file->Get("hLocalStoppingPositions");

    histname.str("");
    histname << "_pz" << i_setting;
    TH1D* hLocalZ = hLocalStopPos->ProjectionZ(histname.str().c_str());

    hLocalZ->SetLineColor(colours[i_setting]);
    hLocalZ->Scale(1.0 / hLocalZ->Integral());
    hLocalZ->SetYTitle("Unit Area");
    hLocalZ->SetMaximum(0.1);
    //    hLocalZ->GetXaxis()->SetRangeUser(0, 5000);
    
    hLocalZ->Draw("HIST SAMES");
  }
  
}
