void PlotAllParticles(std::string filename) {

  TFile* file = new TFile(filename.c_str(), "READ");

  const int n_arms = 2;
  std::string arm_names[n_arms] = {"SiL", "SiR"};

  for (int i_arm = 0; i_arm < n_arms; ++i_arm) {
    std::string canvasname = "c_" + arm_names[i_arm];
    TCanvas* c = new TCanvas(canvasname.c_str(), canvasname.c_str());

    std::string histname = "hEvdE_" + arm_names[i_arm] + "_proton_stopped";
    TH2F* hStoppedProtons = (TH2F*) file->Get(histname.c_str());
    histname = "hEvdE_" + arm_names[i_arm] + "_proton_not_stopped";
    TH2F* hNotStoppedProtons = (TH2F*) file->Get(histname.c_str());
    histname = "hEvdE_" + arm_names[i_arm] + "_deuteron";
    TH2F* hDeuterons = (TH2F*) file->Get(histname.c_str());
    histname = "hEvdE_" + arm_names[i_arm] + "_triton";
    TH2F* hTritons = (TH2F*) file->Get(histname.c_str());
    histname = "hEvdE_" + arm_names[i_arm] + "_alpha";
    TH2F* hAlphas = (TH2F*) file->Get(histname.c_str());

    hStoppedProtons->SetTitle("");

    hStoppedProtons->Draw();
    hNotStoppedProtons->Draw("SAME");
    hDeuterons->Draw("SAME");
    hTritons->Draw("SAME");
    hAlphas->Draw("SAME");

    TLegend* leg = new TLegend(0.55, 0.8, 0.75, 0.6, "");
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04);
    leg->SetFillColor(kWhite);
    leg->AddEntry(hStoppedProtons, "proton stopped", "p");
    leg->AddEntry(hNotStoppedProtons, "proton not stopped", "p");
    leg->AddEntry(hDeuterons, "deuteron", "p");
    leg->AddEntry(hTritons, "triton", "p");
    leg->AddEntry(hAlphas, "alpha", "p");
    leg->Draw();

    TLatex text;
    text.SetTextAlign(12);
    text.DrawLatex(3000, 6000, "#bf{#it{Monte Carlo}}");
  }
}
