void RescaleStoppingDepthPlot() {

  TCanvas* c1 = new TCanvas("c1", "c1");

  TFile* file = new TFile("../../data/input_gen_hists_Al50.root");
  TH3F* hist = (TH3F*) file->Get("hLocalStoppingPositions");
  TH1D* hStopDepth = hist->ProjectionZ();
  
  int n_bins = hStopDepth->GetNbinsX();
  TH1D* hStopDepth_Scale = new TH1D("hStopDepth_scaled", "", n_bins,-5,55);

  hStopDepth_Scale->SetTitle("");
  hStopDepth_Scale->SetXTitle("Local Z [#mum]");
  hStopDepth_Scale->SetYTitle("Number of Muons");
  hStopDepth_Scale->GetYaxis()->SetTitleOffset(1.1);
  hStopDepth_Scale->SetStats(false);
  hStopDepth_Scale->SetLineWidth(2);

  // Shift the x-axis scale so that we start at 0 and muons are entering from the left
  for (int i_bin = 1; i_bin <= n_bins; ++i_bin) {
    double old_coord = hStopDepth->GetBinCenter(i_bin);
    double new_coord = -1*(old_coord - 25);
    
    double bin_content = hStopDepth->GetBinContent(i_bin);
    hStopDepth_Scale->Fill(new_coord, bin_content);
  }
  hStopDepth_Scale->Draw();

  c1->Print("~/plots/ThesisPlots/muon-stopping-depth.pdf");
}
