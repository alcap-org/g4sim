void R15b_Si16b_UnfoldProtonSpectrum() {

  TCanvas* c1 = new TCanvas("c1", "c1");
  //  c1->Divide(2);

  TFile* response_file = new TFile("R15b_Si16b_full-dir-response-matrix.root", "READ");
  //  TFile* response_file = new TFile("R15b_Si16b_restricted-dir-response-matrix.root", "READ");
  //  TFile* response_file = new TFile("R15b_Si16b_full-dir-response-matrix_shallower.root", "READ");
  //  TFile* response_file = new TFile("R15b_Si16b_full-dir-response-matrix_shallower-still.root", "READ");

  //  TFile* data_file = new TFile("Andy_Si16b_EvdEPlot.root", "READ");
  //  TFile* data_file = new TFile("Andy_Si16b_EvdEPlot_NoDoubleCounts.root", "READ");
  //  TFile* data_file = new TFile("Andy_Si16b_EvdEPlot_TimeCutGT200ns.root", "READ");
  TFile* data_file = new TFile("Andy_Si16b_EvdEPlot_NoDoubleCounts_TimeCutGT200ns.root", "READ");
    
  //  TFile* output = new TFile("R15b_Si16b_UnfoldedProtonSpectrum.root", "RECREATE");
  
  const int n_arms = 1;
  std::string arm_names[n_arms] = {"SiR"};
  int rebin_factor = 5;

  for (int i_arm = 0; i_arm < n_arms; ++i_arm) {
    c1->cd(i_arm+1);
    //    c1->GetPad(i_arm+1)->SetLogy();

    std::string arm_name = arm_names[i_arm];
    
    std::string response_name = arm_name + "_response";
    RooUnfoldResponse* response = (RooUnfoldResponse*) response_file->Get(response_name.c_str()); // what I'm trying to reconstruct

    //    TH1D* true_unfolded_spectrum = response->Htruth();
    //    true_unfolded_spectrum->SetLineWidth(2);
    //    true_unfolded_spectrum->SetLineColor(kBlack);
    //    true_unfolded_spectrum->Draw("HIST E");
    //    true_unfolded_spectrum->GetYaxis()->SetRangeUser(0.1, 1e6);

    TH2F* hEvdE_proton = (TH2F*) data_file->Get("hEvdE_Si16b_Proton_Veto");
    TH1D* folded_spectrum = hEvdE_proton->ProjectionX();
    folded_spectrum->Rebin(10);

    TH2D* response_matrix = response->Hresponse();
    //    response_matrix->Draw("COLZ");
    //    TBayesUnfold* svd_unfold = new TBayesUnfold(folded_spectrum, response_measured, response_true, response_matrix);

    //    TH1D* hist = svd_unfold->Unfold(10);
    //    TH1D* dhist = svd_unfold->GetD();
    //    hist->Draw("SAME");
    
    RooUnfoldBayes unfold (response, folded_spectrum);
    //    response->Hresponse()->Draw("COLZ");
    TH1D* unfolded_spectrum = (TH1D*) unfold.Hreco();
    unfolded_spectrum->Draw("HIST E SAMES");
    unfolded_spectrum->SetLineColor(kBlue);
    unfolded_spectrum->SetLineWidth(2);
    folded_spectrum->SetLineColor(kRed);
    folded_spectrum->SetLineWidth(2);
    folded_spectrum->Draw("HIST E SAMES");
    unfold.PrintTable(cout);

    double integral_min = 2000;
    double integral_max = 15000;
    int integral_bin_min = unfolded_spectrum->FindBin(integral_min);
    int integral_bin_max = unfolded_spectrum->FindBin(integral_max);
    double integral, error;
    integral = unfolded_spectrum->IntegralAndError(integral_bin_min, integral_bin_max, error);
    std::cout << "Integral (" << integral_min << " -- " << integral_max << " keV): " << integral << " +- " << error << std::endl;

    integral_min = 3000;
    integral_max = 10000;
    integral_bin_min = unfolded_spectrum->FindBin(integral_min);
    integral_bin_max = unfolded_spectrum->FindBin(integral_max);
    integral = unfolded_spectrum->IntegralAndError(integral_bin_min, integral_bin_max, error);
    std::cout << "Integral (" << integral_min << " -- " << integral_max << " keV): " << integral << " +- " << error << std::endl;

    integral_min = 4000;
    integral_max = 8000;
    integral_bin_min = unfolded_spectrum->FindBin(integral_min);
    integral_bin_max = unfolded_spectrum->FindBin(integral_max);
    integral = unfolded_spectrum->IntegralAndError(integral_bin_min, integral_bin_max, error);
    std::cout << "Integral (" << integral_min << " -- " << integral_max << " keV): " << integral << " +- " << error << std::endl;

    TH1F* hDiffs = unfolded_spectrum->Clone("hDiffs");
    hDiffs->Reset();
    hDiffs->SetXTitle("Energy [keV]");
    hDiffs->SetYTitle("(True - Unfold) / True");

    //    output->cd();
    //    unfolded_spectrum->Write();


    /*    for (int i_bin = 0; i_bin <= unfolded_spectrum->GetNbinsX(); ++i_bin) {
      double unfolded_val = unfolded_spectrum->GetBinContent(i_bin);
      double true_unfolded_val = true_unfolded_spectrum->GetBinContent(i_bin);

      if (true_unfolded_val > 0) { // check that we initially generated this energy (otherwise get divide by 0)
	hDiffs->SetBinContent(i_bin, (true_unfolded_val - unfolded_val)/true_unfolded_val);
      }
    }
    */
    //    hDiffs->Draw("HIST");
  }
  //  output->Write();
  //  output->Close();
}
