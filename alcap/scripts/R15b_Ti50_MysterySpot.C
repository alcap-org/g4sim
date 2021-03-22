#include "scripts/R15b_EvdEPlot.C+"

#include "TLatex.h"
#include "TLegend.h"

void R15b_Ti50_MysterySpot() {

  //  std::string filename = "output/raw_g4sim.root";
  //  std::string filename = "output/R15b_50M_Geom-P4_muon-beam-sim_muOnlyInArms.root";
  //  std::string filename = "output/R15b_1M_Geom-P4_muplus_new.root";
  std::string filename = "output/R15b_500k_Geom-P4_muon-beam-sim_45deg-SiR.root";
  
  std::vector<Arm> arms;
  Arm right_arm;
  Arm left_arm;
  right_arm.detname = "SiR";

  right_arm.thin_energy_thresholds.push_back(90);
  right_arm.thin_energy_thresholds.push_back(100);
  right_arm.thin_energy_thresholds.push_back(80);
  right_arm.thin_energy_thresholds.push_back(90);
  right_arm.thick_energy_threshold = 200;

  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_1", "TMath::Gaus(x, 0, 42.7)", -1000, 1000));
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_2", "TMath::Gaus(x, 0, 37.7)", -1000, 1000));
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_3", "TMath::Gaus(x, 0, 40.7)", -1000, 1000));
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_4", "TMath::Gaus(x, 0, 43.0)", -1000, 1000));
  right_arm.thick_energy_resolution = new TF1("res_SiR2", "TMath::Gaus(x, 0, 32.7)", -1000, 1000);
  
  left_arm.detname = "SiL";
  left_arm.thin_energy_thresholds.push_back(100000); // turn off SiL1-1-S
  left_arm.thin_energy_thresholds.push_back(296);
  left_arm.thin_energy_thresholds.push_back(270);
  left_arm.thin_energy_thresholds.push_back(219);
  left_arm.thin_energy_thresholds.push_back(211);
  left_arm.thin_energy_thresholds.push_back(199);
  left_arm.thin_energy_thresholds.push_back(217);
  left_arm.thin_energy_thresholds.push_back(292);
  left_arm.thin_energy_thresholds.push_back(309);
  left_arm.thin_energy_thresholds.push_back(229);
  left_arm.thin_energy_thresholds.push_back(214);
  left_arm.thin_energy_thresholds.push_back(210);
  left_arm.thin_energy_thresholds.push_back(229);
  left_arm.thin_energy_thresholds.push_back(233);
  left_arm.thin_energy_thresholds.push_back(142);
  left_arm.thin_energy_thresholds.push_back(100000); // turn off SiL1-16-S
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_1", "TMath::Gaus(x, 0, 30.9)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_2", "TMath::Gaus(x, 0, 30.5)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_3", "TMath::Gaus(x, 0, 31.1)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_4", "TMath::Gaus(x, 0, 26.1)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_5", "TMath::Gaus(x, 0, 29.2)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_6", "TMath::Gaus(x, 0, 28.4)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_7", "TMath::Gaus(x, 0, 29.7)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_8", "TMath::Gaus(x, 0, 30.7)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_9", "TMath::Gaus(x, 0, 29.1)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_10", "TMath::Gaus(x, 0, 28.1)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_11", "TMath::Gaus(x, 0, 31.6)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_12", "TMath::Gaus(x, 0, 27.6)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_13", "TMath::Gaus(x, 0, 29.4)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_14", "TMath::Gaus(x, 0, 29.7)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_15", "TMath::Gaus(x, 0, 31.6)", -250, 250));
  left_arm.thin_energy_resolutions.push_back(new TF1("res_SiL1_16", "TMath::Gaus(x, 0, 37.1)", -250, 250));
  left_arm.thick_energy_resolution = new TF1("res_SiL3", "TMath::Gaus(x, 0, 31.6)", -250, 250);
  left_arm.thick_energy_threshold = 180;
  
  arms.push_back(right_arm);
  arms.push_back(left_arm); // don't need SiL arm for Si16b analysis

  R15b_EvdEPlot(filename, arms);
  //  std::vector<Arm> no_tgt_hit_muons = R15b_EvdEPlot(filename, "mu-", 1, false);

  TLegend* legend = new TLegend(0.12, 0.85, 0.32, 0.55, "");
  legend->SetBorderSize(0);
  legend->SetTextSize(0.04);
  legend->SetFillColor(kWhite);
  //  legend->AddEntry(tgt_hit_muons.at(1).hEvdE, "scattered muons (#mu^{-} thick stop)", "p");
  
  TCanvas* c_SiL = new TCanvas("c_SiL", "c_SiL");
  arms.at(1).hEvdE->GetXaxis()->SetRangeUser(0,10000);
  arms.at(1).hEvdE->GetYaxis()->SetRangeUser(0,2000);
  arms.at(1).hEvdE->Draw("COLZ");
  arms.at(1).hEvdE->SetLineWidth(2);
  arms.at(1).hEvdE->SetLineColor(kBlack);
  arms.at(1).hEvdE->SetMarkerColor(kBlack);
  arms.at(1).hEvdE->SetStats(true);
  std::cout << "N Entries (SiL) = " << arms.at(1).hEvdE->GetEntries() << std::endl;
  //  legend->Draw();

  TLatex* latex = new TLatex();
  latex->DrawLatexNDC(0.55, 0.7, "AlCap Monte Carlo");

  TCanvas* c_SiR = new TCanvas("c_SiR", "c_SiR");
  TH2F* hSiR = arms.at(0).hEvdE;
  //  TH1D* hSiR = arms.at(0).hEvdE->ProjectionY("no_tgt");
  //  TH1D* hSiR_tgt = tgt_hit_muons.at(0).hEvdE->ProjectionY("tgt");
  
  hSiR->GetXaxis()->SetRangeUser(0,10000);
  hSiR->GetYaxis()->SetRangeUser(0,2000);
  hSiR->SetLineWidth(2);
  hSiR->SetLineColor(kBlack);
  hSiR->SetMarkerColor(kBlack);
  hSiR->SetStats(true);
  std::cout << "N Entries (SiR) = " << hSiR->GetEntries() << std::endl;

  hSiR->Draw("COLZ");
  //  legend->Draw();
  
  latex->DrawLatexNDC(0.55, 0.7, "AlCap Monte Carlo");
}
