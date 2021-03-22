//#include "scripts/R15b_Si16b_EvdEPlot.C"

#include "TLatex.h"
#include "TLegend.h"

void R15b_Si16b_ParticleIDDemo() {

  const int n_particles = 5;
  std::string particles[n_particles] = {"proton", "deuteron", "triton", "alpha", "muon"};
  Int_t stop_colours[n_particles] = {kRed, kCyan, kMagenta, kSpring, kBlack};
  Int_t notstop_colours[n_particles] = {kBlue, kBlack, kYellow, kYellow, kBlack};
  Int_t marker_styles[n_particles] = {kDot, kOpenCircle, kFullTriangleUp, kFullStar, kDot};
  
  TCanvas* c_TwoLayer = new TCanvas();
  TCanvas* c_ThreeLayer = new TCanvas();

  int rebin_2L_x = 5;
  int rebin_2L_y = 5;
  int rebin_3L_x = 10;
  int rebin_3L_y = 10;

  TLegend* legend_2L = new TLegend(0.12, 0.85, 0.32, 0.55, "");
  legend_2L->SetBorderSize(0);
  legend_2L->SetTextSize(0.04);
  legend_2L->SetFillColor(kWhite);

  TPad* inset = new TPad("inset", "inset", 0.05, 0.5, 0.35, 0.8);
  
  TLegend* legend_3L = new TLegend(0.12, 0.85, 0.32, 0.55, "");
  legend_3L->SetBorderSize(0);
  legend_3L->SetTextSize(0.04);
  legend_3L->SetFillColor(kWhite);

  TLatex* latex = new TLatex();
  
  for (int i_particle = 0; i_particle < n_particles; ++i_particle) {
    std::string particle = particles[i_particle];
    Int_t stop_colour = stop_colours[i_particle];
    Int_t notstop_colour = notstop_colours[i_particle];
    Int_t marker_style = marker_styles[i_particle];
    
    std::string filename = "respMatrix_";
    if (particle == "muon") {
      filename += "1M";
    } else {
      filename += "10M";
    }
    filename += "_Geom-P5_" + particle;
    if (particle == "proton") {
      filename += "_two-layer-only";
    }
    else {
      filename += "_1000keVBins";
    }
    filename += ".root";

    TFile* file = new TFile(filename.c_str(), "READ");
    std::string histname = "SiR_hEvdE_TwoLayer_stopped";
    TH2F* hTwoLayerStopped = (TH2F*) file->Get(histname.c_str());
    hTwoLayerStopped->SetTitle("");
    hTwoLayerStopped->SetStats(false);

    //    hTwoLayerStopped->Rebin2D(rebin_2L_x, rebin_2L_y);
    hTwoLayerStopped->GetXaxis()->SetRangeUser(0,20000);
    hTwoLayerStopped->GetYaxis()->SetRangeUser(0,10000);

    // Change labels for x-axis
    int n_labels = 11;
    std::stringstream new_label;
    for (int i_label = 1; i_label <= n_labels; ++i_label) {
      new_label.str("");
      new_label << (i_label-1)*2; // convert to MeV labels
      hTwoLayerStopped->GetXaxis()->ChangeLabel(i_label, -1, -1, -1, -1, -1, new_label.str().c_str());
    }

    // Change labels for y-axis
    n_labels = 11;
    for (int i_label = 1; i_label <= n_labels; ++i_label) {
      new_label.str("");
      new_label << (i_label-1); // convert to MeV labels
      hTwoLayerStopped->GetYaxis()->ChangeLabel(i_label, -1, -1, -1, -1, -1, new_label.str().c_str());
    }

    hTwoLayerStopped->SetLineColor(stop_colour);
    hTwoLayerStopped->SetLineWidth(2);
    //    hTwoLayerStopped->SetMarkerStyle(marker_style);
    hTwoLayerStopped->SetMarkerColor(stop_colour);
    hTwoLayerStopped->GetXaxis()->SetTitleSize(0.05);
    hTwoLayerStopped->GetXaxis()->SetLabelSize(0.04);
    hTwoLayerStopped->GetXaxis()->SetTitleOffset(0.9);
    hTwoLayerStopped->GetYaxis()->SetTitleSize(0.05);
    hTwoLayerStopped->GetYaxis()->SetLabelSize(0.04);
    hTwoLayerStopped->GetYaxis()->SetTitleOffset(0.8);
    hTwoLayerStopped->SetXTitle("E_{1} + E_{2} [MeV]");
    hTwoLayerStopped->SetYTitle("E_{1} [MeV]");
    hTwoLayerStopped->SetDirectory(0);
    
    histname = "SiR_hEvdE_TwoLayer_notstopped";
    TH2F* hTwoLayerNotStopped = (TH2F*) file->Get(histname.c_str());
    //    hTwoLayerNotStopped->Rebin2D(rebin_2L_x, rebin_2L_y);
    hTwoLayerNotStopped->SetLineColor(notstop_colour);
    hTwoLayerNotStopped->SetLineWidth(2);
    hTwoLayerNotStopped->SetMarkerColor(notstop_colour);
    hTwoLayerNotStopped->SetDirectory(0);

    if (particle == "proton") {
      hTwoLayerStopped->Add(hTwoLayerNotStopped);
    }
    hTwoLayerStopped->SetMaximum(500);
    //    hTwoLayerNotStopped->SetMaximum(300);
    
    c_TwoLayer->cd();
    hTwoLayerStopped->Draw("COLZ SAME");
    //    hTwoLayerNotStopped->Draw("COLZ SAME");

    inset->cd();
    std::string name = hTwoLayerStopped->GetName();
    name += "_inset";
    TH2F* hInset = (TH2F*) hTwoLayerStopped->Clone(name.c_str());
    hInset->GetXaxis()->SetRangeUser(7000, 10000);
    hInset->GetYaxis()->SetRangeUser(0, 2000);
    hInset->Draw("COLZ SAME");

    c_TwoLayer->cd();
    latex->SetTextSize(0.05);
    latex->DrawLatex(1400, 2100, "p");
    TLine* p_line = new TLine(1800, 2000, 2500, 1500);
    p_line->SetLineColor(kRed);
    p_line->SetLineWidth(3);
    p_line->Draw("LSAME");
    latex->DrawLatex(1900, 2600, "d");
    TLine* d_line = new TLine(2350, 2550, 3000, 2100);
    d_line->SetLineColor(kRed);
    d_line->SetLineWidth(3);
    d_line->Draw("LSAME");
    latex->DrawLatex(2400, 3100, "t");
    TLine* t_line = new TLine(2750, 3000, 3500, 2500);
    t_line->SetLineColor(kRed);
    t_line->SetLineWidth(3);
    t_line->Draw("LSAME");
    latex->DrawLatex(7400, 8100, "#alpha");
    TLine* a_line = new TLine(7900, 8100, 8300, 7500);
    a_line->SetLineColor(kRed);
    a_line->SetLineWidth(3);
    a_line->Draw("LSAME");
    //    latex->DrawLatex(500, 100, "punch-throughs");

    TLine* line = new TLine(8000, 250, 12600, 1250);
    line->SetLineColor(kRed);
    line->SetLineWidth(3);
    line->Draw("LSAME");
    latex->SetTextSize(0.04);
    latex->DrawLatex(12750, 1250, "punch-through protons");

    latex->DrawLatex(7000, 1800, "scattered muons");
    TLine* mu_line = new TLine(6800, 1900, 1600, 1000);
    mu_line->SetLineColor(kRed);
    mu_line->SetLineWidth(3);
    mu_line->Draw("LSAME");

    std::string leglabel = "stopped " + particle + "s";
    legend_2L->AddEntry(hTwoLayerStopped, leglabel.c_str(), "p");
    if (i_particle == 0) {
      leglabel = "not stopped " + particle + "s";
      legend_2L->AddEntry(hTwoLayerNotStopped, leglabel.c_str(), "p");
    }

    
    if (particle == "proton") {
      file->Close();
      file = new TFile("respMatrix_10M_Geom-P5_proton_1000keVBins.root", "READ");
    }
    histname = "SiR_EvdE_ThreeLayer_stopped";
    TH2F* hThreeLayerStopped = (TH2F*) file->Get(histname.c_str());
    hThreeLayerStopped->SetTitle("");
    hThreeLayerStopped->SetStats(false);
    hThreeLayerStopped->Rebin2D(rebin_3L_x, rebin_3L_y);
    hThreeLayerStopped->SetLineColor(stop_colour);
    hThreeLayerStopped->SetLineWidth(2);
    hThreeLayerStopped->SetMarkerColor(stop_colour);
    hThreeLayerStopped->GetXaxis()->SetTitleSize(0.05);
    hThreeLayerStopped->GetXaxis()->SetLabelSize(0.04);
    hThreeLayerStopped->GetXaxis()->SetTitleOffset(1.0);
    hThreeLayerStopped->GetYaxis()->SetTitleSize(0.05);
    hThreeLayerStopped->GetYaxis()->SetLabelSize(0.04);
    hThreeLayerStopped->GetYaxis()->SetTitleOffset(1.0);

    
    histname = "SiR_EvdE_ThreeLayer_notstopped";
    TH2F* hThreeLayerNotStopped = (TH2F*) file->Get(histname.c_str());
    hThreeLayerNotStopped->Rebin2D(rebin_3L_x, rebin_3L_y);
    hThreeLayerNotStopped->SetLineColor(notstop_colour);
    hThreeLayerNotStopped->SetLineWidth(2);
    hThreeLayerNotStopped->SetMarkerColor(notstop_colour);

    c_ThreeLayer->cd();
    hThreeLayerStopped->Draw("BOX SAME");
    if (particle == "proton") {
      hThreeLayerNotStopped->Draw("BOX SAME");
    }

    if (i_particle <=1) {
      leglabel = "stopped " + particle + "s";
      legend_3L->AddEntry(hThreeLayerStopped, leglabel.c_str(), "l");
      if (i_particle == 0) {
	leglabel = "not stopped " + particle + "s";
	legend_3L->AddEntry(hThreeLayerNotStopped, leglabel.c_str(), "l");
      }
    }
  }

  c_TwoLayer->cd();
  latex->SetTextColor(kBlack);
  latex->SetTextSize(0.05);
  latex->DrawLatexNDC(0.50, 0.75, "AlCap #bf{#it{Simulation}}");
  //  inset->cd();
  //  inset->Draw();
  //  legend_2L->Draw();

  c_ThreeLayer->cd();
  latex->DrawLatexNDC(0.45, 0.75, "AlCap #bf{#it{Simulation}}");
  legend_3L->Draw();

  // std::vector<Arm> protons = R15b_Si16b_EvdEPlot("output/R15b_100k_Geom-S14_protons_forCIPANP.root", "proton");
  // //  std::vector<Arm> protons = R15b_Si16b_EvdEPlot("output/raw_g4sim.root", "proton");
  // std::vector<Arm> notstopped_protons = R15b_Si16b_EvdEPlot("output/R15b_100k_Geom-S14_protons_forCIPANP.root", "proton", 0);
  // //  std::vector<Arm> notstopped_protons = R15b_Si16b_EvdEPlot("output/raw_g4sim.root", "proton", 0);
  // std::vector<Arm> deuterons = R15b_Si16b_EvdEPlot("output/R15b_100k_Geom-S14_deuterons_forCIPANP.root", "deuteron");
  // std::vector<Arm> tritons = R15b_Si16b_EvdEPlot("output/R15b_100k_Geom-S14_tritons_forCIPANP.root", "triton");
  // std::vector<Arm> alphas = R15b_Si16b_EvdEPlot("output/R15b_100k_Geom-S14_alphas_forCIPANP.root", "alpha");
  // std::vector<Arm> tgt_muons = R15b_Si16b_EvdEPlot("output/R15b_100k_Geom-S14_muons_forCIPANP.root", "mu-");
  // std::vector<Arm> direct_muons = R15b_Si16b_EvdEPlot("output/R15b_1M_Geom-P5_muon-beam-sim_chamberAndCollsVacuum.root", "mu-");
  // //  std::vector<Arm> direct_muons = R15b_Si16b_EvdEPlot("output/R15b_1M_Geom-P5_muon-beam-sim.root", "mu-");
  // std::vector<Arm> protons_SiR1 = R15b_Si16b_EvdEPlot("output/R15b_100k_Geom-P5_proton-SiR1-UIV.root", "proton");
  // //  std::vector<Arm> protons_SiR1 = R15b_Si16b_EvdEPlot("output/raw_g4sim.root", "proton");
  
  // protons.at(0).hEvdE->Draw("BOX");
  // protons.at(0).hEvdE->SetLineWidth(2);
  // protons.at(0).hEvdE->SetLineColor(kRed);
  // /*  notstopped_protons.at(0).hEvdE->Draw("BOX SAME");
  // notstopped_protons.at(0).hEvdE->SetLineWidth(2);
  // notstopped_protons.at(0).hEvdE->SetLineColor(kBlue);
  // deuterons.at(0).hEvdE->Draw("BOX SAME");
  // deuterons.at(0).hEvdE->SetLineWidth(2);
  // deuterons.at(0).hEvdE->SetLineColor(kCyan);
  // tritons.at(0).hEvdE->Draw("BOX SAME");
  // tritons.at(0).hEvdE->SetLineWidth(2);
  // tritons.at(0).hEvdE->SetLineColor(kMagenta);
  // alphas.at(0).hEvdE->Draw("BOX SAME");
  // alphas.at(0).hEvdE->SetLineWidth(2);
  // alphas.at(0).hEvdE->SetLineColor(kSpring);
  // tgt_muons.at(0).hEvdE->Draw("BOX SAME");
  // tgt_muons.at(0).hEvdE->SetLineWidth(2);
  // tgt_muons.at(0).hEvdE->SetLineColor(kBlack);
  // direct_muons.at(0).hEvdE->Draw("BOX SAME");
  // direct_muons.at(0).hEvdE->SetLineWidth(2);
  // direct_muons.at(0).hEvdE->SetLineColor(kGreen);
  // */
  // protons_SiR1.at(0).hEvdE->Draw("COLZ");
  // protons.at(0).hEvdE->Draw("BOX SAME");
  // protons_SiR1.at(0).hEvdE->SetLineWidth(2);
  // protons_SiR1.at(0).hEvdE->SetLineColor(kBlack);

  // TLegend* legend = new TLegend(0.12, 0.85, 0.32, 0.55, "");
  // legend->SetBorderSize(0);
  // legend->SetTextSize(0.04);
  // legend->SetFillColor(kWhite);
  // //  legend->AddEntry(protons.at(0).hEvdE, "stopped protons", "l");
  // legend->AddEntry(protons.at(0).hEvdE, "stopped protons (from target)", "l");
  // //  legend->AddEntry(notstopped_protons.at(0).hEvdE, "not stopped protons", "l");
  // //  legend->AddEntry(deuterons.at(0).hEvdE, "stopped deuterons", "l");
  // //  legend->AddEntry(tritons.at(0).hEvdE, "stopped tritons", "l");
  // //  legend->AddEntry(alphas.at(0).hEvdE, "stopped alphas", "l");
  // //  legend->AddEntry(tgt_muons.at(0).hEvdE, "stopped muons", "l");
  // //  legend->AddEntry(tgt_muons.at(0).hEvdE, "stopped muons (from target)", "l");
  // //  legend->AddEntry(direct_muons.at(0).hEvdE, "stopped muons (direct)", "l");
  // legend->AddEntry(protons_SiR1.at(0).hEvdE, "stopped protons (from SiR1)", "l");
  // legend->Draw();
}
