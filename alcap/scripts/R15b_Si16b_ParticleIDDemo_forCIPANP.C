#include "scripts/R15b_Si16b_EvdEPlot.C"

#include "TLatex.h"
#include "TLegend.h"

void R15b_Si16b_ParticleIDDemo_forCIPANP() {

  std::vector<Arm> protons = R15b_Si16b_EvdEPlot("output/R15b_100k_Geom-S14_protons_forCIPANP.root", "proton");
  //  std::vector<Arm> protons = R15b_Si16b_EvdEPlot("output/raw_g4sim.root", "proton");
  std::vector<Arm> notstopped_protons = R15b_Si16b_EvdEPlot("output/R15b_100k_Geom-S14_protons_forCIPANP.root", "proton", 0);
  //  std::vector<Arm> notstopped_protons = R15b_Si16b_EvdEPlot("output/raw_g4sim.root", "proton", 0);
  std::vector<Arm> deuterons = R15b_Si16b_EvdEPlot("output/R15b_100k_Geom-S14_deuterons_forCIPANP.root", "deuteron");
  std::vector<Arm> tritons = R15b_Si16b_EvdEPlot("output/R15b_100k_Geom-S14_tritons_forCIPANP.root", "triton");
  std::vector<Arm> alphas = R15b_Si16b_EvdEPlot("output/R15b_100k_Geom-S14_alphas_forCIPANP.root", "alpha");
  std::vector<Arm> muons = R15b_Si16b_EvdEPlot("output/R15b_100k_Geom-S14_muons_forCIPANP.root", "mu-");
  
  protons.at(0).hEvdE->Draw("BOX");
  protons.at(0).hEvdE->SetLineWidth(2);
  protons.at(0).hEvdE->SetLineColor(kRed);
  notstopped_protons.at(0).hEvdE->Draw("BOX SAME");
  notstopped_protons.at(0).hEvdE->SetLineWidth(2);
  notstopped_protons.at(0).hEvdE->SetLineColor(kBlue);
  deuterons.at(0).hEvdE->Draw("BOX SAME");
  deuterons.at(0).hEvdE->SetLineWidth(2);
  deuterons.at(0).hEvdE->SetLineColor(kCyan);
  tritons.at(0).hEvdE->Draw("BOX SAME");
  tritons.at(0).hEvdE->SetLineWidth(2);
  tritons.at(0).hEvdE->SetLineColor(kMagenta);
  alphas.at(0).hEvdE->Draw("BOX SAME");
  alphas.at(0).hEvdE->SetLineWidth(2);
  alphas.at(0).hEvdE->SetLineColor(kSpring);
  muons.at(0).hEvdE->Draw("BOX SAME");
  muons.at(0).hEvdE->SetLineWidth(2);
  muons.at(0).hEvdE->SetLineColor(kBlack);

  TLegend* legend = new TLegend(0.12, 0.85, 0.32, 0.55, "");
  legend->SetBorderSize(0);
  legend->SetTextSize(0.04);
  legend->SetFillColor(kWhite);
  legend->AddEntry(protons.at(0).hEvdE, "stopped protons", "l");
  legend->AddEntry(notstopped_protons.at(0).hEvdE, "not stopped protons", "l");
  legend->AddEntry(deuterons.at(0).hEvdE, "stopped deuterons", "l");
  legend->AddEntry(tritons.at(0).hEvdE, "stopped tritons", "l");
  legend->AddEntry(alphas.at(0).hEvdE, "stopped alphas", "l");
  legend->AddEntry(muons.at(0).hEvdE, "stopped muons", "l");
  legend->Draw();

  TLatex* latex = new TLatex();
  latex->DrawLatexNDC(0.55, 0.7, "AlCap Monte Carlo");
  latex->Draw();
}
