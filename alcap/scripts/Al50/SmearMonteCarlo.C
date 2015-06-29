void SmearMonteCarlo() {
  TCanvas* c1 = new TCanvas("c1", "c1");

  const int n_arms = 2;
  TFile* proton_band_file = new TFile("proton_band.root", "READ"); // MC

  std::string armnames[n_arms] = {"SiL", "SiR"};
  TH2F* extracted_bands[n_arms];
  TH2F* smeared_bands[n_arms];
  double det_smears[n_arms] = {57, 51}; // keV

  for (int i_arm = 0; i_arm < n_arms; ++i_arm) {
    std::string histname = "hEvdEBand_" + armnames[i_arm] + ";2";
    extracted_bands[i_arm] = (TH2F*) proton_band_file->Get(histname.c_str());

    smeared_bands[i_arm] = (TH2F*) extracted_bands[i_arm]->Clone();
    smeared_bands[i_arm]->Reset();
    
    TF1* smear_function = new TF1("smear", "TMath::Gaus(x, 0, [0])", -200, 200);
    smear_function->SetParameter(0, det_smears[i_arm]);
    smear_function->Draw();

    // Loop through the bins
    int n_bins_x = extracted_bands[i_arm]->GetNbinsX();
    int n_bins_y = extracted_bands[i_arm]->GetNbinsY();
    for (int i_bin = 1; i_bin <= n_bins_x; ++i_bin) {
      double total_energy = extracted_bands[i_arm]->GetXaxis()->GetBinCenter(i_bin);
      for (int j_bin = 1; j_bin <= n_bins_y; ++j_bin) {
	
	// Loop through the number of entries and create the smeared band
	int n_bin_entries = extracted_bands[i_arm]->GetBinContent(i_bin, j_bin);
	double bin_energy = extracted_bands[i_arm]->GetYaxis()->GetBinCenter(j_bin);
	for (int i_entry = 0; i_entry < n_bin_entries; ++i_entry) {
	  double smear = smear_function->GetRandom();
	  double new_energy = bin_energy + smear;
	  std::cout << bin_energy << " " << smear << " = " << new_energy << std::endl;
	  smeared_bands[i_arm]->Fill(total_energy, new_energy);
	} // end loop through entries
      } // end y-axis loop
    } // end x-axis loop
    
    // Now remove anything with less than 10 entries
    for (int i_bin = 1; i_bin <= n_bins_x; ++i_bin) {
      for (int j_bin = 1; j_bin <= n_bins_y; ++j_bin) {
	
	int n_bin_entries = smeared_bands[i_arm]->GetBinContent(i_bin, j_bin);
	if (n_bin_entries < 10) {
	  smeared_bands[i_arm]->SetBinContent(i_bin, j_bin, 0);
	}
      }
    }

    c1->SetLogz();
    smeared_bands[i_arm]->Draw("COLZ");
    TLatex text;
    text.SetTextAlign(12);
    text.DrawLatex(15000, 2500, "#bf{#it{Monte Carlo}}");

    std::string pdfname = armnames[i_arm] + "-smeared-mc.pdf";
    c1->Print(pdfname.c_str());

  } // end arm loop
}
