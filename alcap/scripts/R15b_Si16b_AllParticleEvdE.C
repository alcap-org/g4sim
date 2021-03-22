#include "scripts/R15b_Si16b_EvdEPlot.C"

#include "TLatex.h"
#include "TLegend.h"

void R15b_Si16b_AllParticleEvdE() {

  std::vector<Arm> protons = R15b_Si16b_EvdEPlot("output/R15b_10M_Geom-P5_protons_new.root", "proton");
  //  std::vector<Arm> notstopped_protons = R15b_Si16b_EvdEPlot("output/R15b_10M_Geom-P5_protons_new.root", "proton", 0);
  //  std::vector<Arm> deuterons = R15b_Si16b_EvdEPlot("output/R15b_10M_Geom-P5_deuterons_new.root", "deuteron");
  //  std::vector<Arm> tritons = R15b_Si16b_EvdEPlot("output/R15b_100k_Geom-S14_tritons_forCIPANP.root", "triton");
  //  std::vector<Arm> alphas = R15b_Si16b_EvdEPlot("output/R15b_100k_Geom-S14_alphas_forCIPANP.root", "alpha");
  //  std::vector<Arm> muons = R15b_Si16b_EvdEPlot("output/R15b_100k_Geom-S14_muons_forCIPANP.root", "mu-");
  
  protons.at(0).hEvdE->Draw("COLZ");
  protons.at(0).hEvdE->SetMarkerColor(kRed);
  //  notstopped_protons.at(0).hEvdE->Draw("COLZ SAME");
  //  notstopped_protons.at(0).hEvdE->SetMarkerColor(kBlue);
  //  deuterons.at(0).hEvdE->Draw("COLZ SAME");
  //  deuterons.at(0).hEvdE->SetMarkerColor(kCyan);
  /*  tritons.at(0).hEvdE->Draw("COLZ SAME");
  tritons.at(0).hEvdE->SetMarkerColor(kMagenta);
  alphas.at(0).hEvdE->Draw("COLZ SAME");
  alphas.at(0).hEvdE->SetMarkerColor(kSpring);
  muons.at(0).hEvdE->Draw("COLZ SAME");
  muons.at(0).hEvdE->SetMarkerColor(kBlack);
  */
  TLegend* legend = new TLegend(0.12, 0.85, 0.32, 0.55, "");
  legend->SetBorderSize(0);
  legend->SetTextSize(0.04);
  legend->SetFillColor(kWhite);
  legend->AddEntry(protons.at(0).hEvdE, "stopped protons", "p");
  legend->AddEntry(notstopped_protons.at(0).hEvdE, "not stopped protons", "p");
  legend->AddEntry(deuterons.at(0).hEvdE, "stopped deuterons", "p");
  /*  legend->AddEntry(tritons.at(0).hEvdE, "stopped tritons", "p");
  legend->AddEntry(alphas.at(0).hEvdE, "stopped alphas", "p");
  legend->AddEntry(muons.at(0).hEvdE, "stopped muons", "p");
  legend->Draw();
  */
  TLatex* latex = new TLatex();
  latex->DrawLatexNDC(0.55, 0.7, "AlCap Monte Carlo");
  latex->Draw();
}
