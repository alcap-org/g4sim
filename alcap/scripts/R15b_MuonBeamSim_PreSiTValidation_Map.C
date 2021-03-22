
void R15b_MuonBeamSim_PreSiTValidation_Map() {

  TNtuple* ntuple = new TNtuple("ntuple", "", "energy:sigma:landau_mean:landau_sigma:gaus_sigma");
  ntuple->Fill(1.49, 0.31,    375.0,       32.8,      58.7);
	      

  double energy_width = 0.01; double min_energy = 1.49 - energy_width/2; double max_energy = 1.51 + energy_width/2; int n_energy_bins = (max_energy - min_energy)/energy_width + 1;
  double sigma_width = 0.01; double min_sigma = 0.28 - sigma_width/2; double max_sigma = 0.31 + sigma_width/2; int n_sigma_bins = (max_sigma - min_sigma)/sigma_width + 1;
  double min_landau_mean = 369; double max_landau_mean = 377; double landau_mean_width = 0.1; int n_landau_mean_bins = (max_landau_mean - min_landau_mean)/landau_mean_width;
  double min_landau_sigma = 21; double max_landau_sigma = 31; double landau_sigma_width = 0.1; int n_landau_sigma_bins = (max_landau_sigma - min_landau_sigma)/landau_sigma_width;
  double min_gaus_sigma = 53; double max_gaus_sigma = 59; double gaus_sigma_width = 0.1; int n_gaus_sigma_bins = (max_gaus_sigma - min_gaus_sigma)/gaus_sigma_width;
  
  double target_landau_mean = 369.2;
  double target_landau_sigma = 21.9;
  double target_gaus_sigma = 53.7;

  TH2F* hLandauMeanVsGausSigma_WeightSigma = new TH2F("hLandauMeanVsGausSigma_WeightSigma", "", n_landau_mean_bins,min_landau_mean,max_landau_mean, n_gaus_sigma_bins,min_gaus_sigma,max_gaus_sigma);
  hLandauMeanVsGausSigma_WeightSigma->SetXTitle("Landau Mean [keV]");
  hLandauMeanVsGausSigma_WeightSigma->SetYTitle("Gauss Sigma [keV]");
  hLandauMeanVsGausSigma_WeightSigma->SetZTitle("Energy Sigma [MeV]");
  TH2F* hLandauMeanVsGausSigma_WeightEnergy = (TH2F*) hLandauMeanVsGausSigma_WeightSigma->Clone("hLandauMeanVsGausSigma_WeightEnergy");
  hLandauMeanVsGausSigma_WeightEnergy->SetZTitle("Energy Mean [MeV]");
  TH2F* hLandauMeanVsGausSigma_Target = (TH2F*) hLandauMeanVsGausSigma_WeightSigma->Clone("hLandauMeanVsGausSigma_Target");
  hLandauMeanVsGausSigma_Target->Fill(target_landau_mean, target_gaus_sigma);
  hLandauMeanVsGausSigma_Target->SetMarkerStyle(kOpenTriangleUp);

  TCanvas* c1 = new TCanvas("c1", "c1");
  ntuple->Draw("gaus_sigma:landau_mean>>hLandauMeanVsGausSigma_WeightSigma", "sigma", "COLZ");
  hLandauMeanVsGausSigma_WeightSigma->SetMinimum(min_sigma);

  hLandauMeanVsGausSigma_Target->Draw("P SAME");

  TCanvas* c2 = new TCanvas("c2", "c2");
  ntuple->Draw("gaus_sigma:landau_mean>>hLandauMeanVsGausSigma_WeightEnergy", "energy", "COLZ");
  hLandauMeanVsGausSigma_WeightEnergy->SetMinimum(min_energy);
  hLandauMeanVsGausSigma_Target->Draw("P SAME");

  TH2F* hLandauMeanVsLandauSigma_WeightSigma = new TH2F("hLandauMeanVsLandauSigma_WeightSigma", "", n_landau_mean_bins,min_landau_mean,max_landau_mean, n_landau_sigma_bins,min_landau_sigma,max_landau_sigma);
  hLandauMeanVsLandauSigma_WeightSigma->SetXTitle("Landau Mean [keV]");
  hLandauMeanVsLandauSigma_WeightSigma->SetYTitle("Landau Sigma [keV]");
  hLandauMeanVsLandauSigma_WeightSigma->SetZTitle("Energy Sigma [MeV]");
  TH2F* hLandauMeanVsLandauSigma_WeightEnergy = (TH2F*) hLandauMeanVsLandauSigma_WeightSigma->Clone("hLandauMeanVsLandauSigma_WeightEnergy");
  hLandauMeanVsLandauSigma_WeightEnergy->SetZTitle("Energy Mean [MeV]");
  TH2F* hLandauMeanVsLandauSigma_Target = (TH2F*) hLandauMeanVsLandauSigma_WeightSigma->Clone("hLandauMeanVsLandauSigma_Target");
  hLandauMeanVsLandauSigma_Target->Fill(target_landau_mean, target_landau_sigma);
  hLandauMeanVsLandauSigma_Target->SetMarkerStyle(kOpenTriangleUp);

  TCanvas* c3 = new TCanvas("c3", "c3");
  ntuple->Draw("landau_sigma:landau_mean>>hLandauMeanVsLandauSigma_WeightSigma", "sigma", "COLZ");
  hLandauMeanVsLandauSigma_WeightSigma->SetMinimum(min_sigma);

  hLandauMeanVsLandauSigma_Target->Draw("P SAME");

  TCanvas* c4 = new TCanvas("c4", "c4");
  ntuple->Draw("landau_sigma:landau_mean>>hLandauMeanVsLandauSigma_WeightEnergy", "energy", "COLZ");
  hLandauMeanVsLandauSigma_WeightEnergy->SetMinimum(min_energy);
  hLandauMeanVsLandauSigma_Target->Draw("P SAME");

  ntuple->Print();
    ntuple->Scan();

  TCanvas* c5 = new TCanvas("c5", "c5");
  TH2F* hEnergyVsSigma_LandauMean = new TH2F("hEnergyVsSigma_LandauMean", "", n_energy_bins,min_energy,max_energy, n_sigma_bins,min_sigma,max_sigma);
  hEnergyVsSigma_LandauMean->SetXTitle("Energy Mean [MeV]");
  hEnergyVsSigma_LandauMean->SetYTitle("Energy Sigma [MeV]");
  hEnergyVsSigma_LandauMean->SetZTitle("Landau Mean [keV]");
  ntuple->Draw("sigma:energy>>hEnergyVsSigma_LandauMean", "landau_mean", "TEXT COLZ");
  hEnergyVsSigma_LandauMean->SetMinimum(min_landau_mean);

  TCanvas* c6 = new TCanvas("c6", "c6");
  TH2F* hEnergyVsSigma_LandauSigma = new TH2F("hEnergyVsSigma_LandauSigma", "", n_energy_bins,min_energy,max_energy, n_sigma_bins,min_sigma,max_sigma);
  hEnergyVsSigma_LandauSigma->SetXTitle("Energy Mean [MeV]");
  hEnergyVsSigma_LandauSigma->SetYTitle("Energy Sigma [MeV]");
  hEnergyVsSigma_LandauSigma->SetZTitle("Landau Sigma [keV]");
  ntuple->Draw("sigma:energy>>hEnergyVsSigma_LandauSigma", "landau_sigma", "TEXT COLZ");
  hEnergyVsSigma_LandauSigma->SetMinimum(min_landau_sigma);

  TCanvas* c7 = new TCanvas("c7", "c7");
  TH2F* hEnergyVsSigma_GausSigma = new TH2F("hEnergyVsSigma_GausSigma", "", n_energy_bins,min_energy,max_energy, n_sigma_bins,min_sigma,max_sigma);
  hEnergyVsSigma_GausSigma->SetXTitle("Energy Mean [MeV]");
  hEnergyVsSigma_GausSigma->SetYTitle("Energy Sigma [MeV]");
  hEnergyVsSigma_GausSigma->SetZTitle("Gaus Sigma [keV]");
  ntuple->Draw("sigma:energy>>hEnergyVsSigma_GausSigma", "gaus_sigma", "TEXT COLZ");
  hEnergyVsSigma_GausSigma->SetMinimum(min_gaus_sigma);
}
