void R15b_MuPlusEDep_Comparison() {

  const int n_settings = 2;
  std::string filenames[n_settings] = {"decayCorr_10M_Geom-P1_muplus.root",
				       /*				       "decayCorr_100k_Geom-P1_muplus_stopPosZ--1mm.root",
				       "decayCorr_100k_Geom-P1_muplus_stopPosZ-1mm.root",
  				       "decayCorr_100k_Geom-P1_muplus_stopPosZ-0-1mm.root",
				       "decayCorr_100k_Geom-P1_muplus_stopPosZ-0-5mm.root",*/
				       "dump.root"};
  Int_t colours[n_settings] = {kBlue, kRed};//, kBlack, kMagenta, kGreen, kGray};
  
  for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
    TFile* file = new TFile(filenames[i_setting].c_str(), "READ");
    TH1F* hEDep = (TH1F*) file->Get("hEDep_muplus");
    hEDep->SetLineColor(colours[i_setting]);

    hEDep->Scale(1.0 / hEDep->Integral());
    hEDep->SetYTitle("Unit Area");
    hEDep->GetXaxis()->SetRangeUser(0, 5000);
    hEDep->SetMaximum(0.11);
    
    hEDep->Draw("HIST SAME");
  }
  
}
