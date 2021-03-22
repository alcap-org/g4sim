#include "R15b_TransferMatrix.C"

void R15b_Ti50_TransferMatrix() {

  //  std::string filename = "output/R15b_1M_Geom-P4_protons-SWSpec_wP5MuStopPositions_w1475umSiL3.root";
  //  std::string outfilename = "R15b_Ti50_response-matrix_500keV-bins_protons-SWSpec_w1475umSiL3.root";
  //  std::string filename = "output/raw_g4sim.root";
  //  std::string filename = "output/R15b_1M_Geom-P4_protons-SWSpec_wP5MuStopPositions.root";
  //  std::string filename = "output/R15b_1M_Geom-P4_protons-SWSpec_wP5MuStopPositions_w1475umSiL3.root";
  //  std::string filename = "output/R15b_1M_Geom-P4_protons_wP5MuStopPositions_w1475umSiL3.root";

  //  std::string filename = "output/R15b_10M_Geom-P4_protons-SWSpec_wP5MuStopPositions_w1475umSiL3.root";
  //  std::string filename = "output/R15b_10M_Geom-P4_protons-SWSpec_wP5MuStopPositions.root";
  //  std::string filename = "output/R15b_10M_Geom-P4_protons-SWSpec_wP5MuStopPositions_w1200umSiL3.root";
  //  std::string filename = "output/R15b_10M_Geom-P4_protons-SWSpec2_wP5MuStopPositions_w1200umSiL3.root";
  //  std::string filename = "output/R15b_10M_Geom-P4_protons-SWSpec_wP5MuStopPositions_w1475umSiL3_incAlDLs.root";
  std::string filename = "output/raw_g4sim.root";
  std::string outfilename = "dump_muon-beam-sim.root";
  std::string particle = "muon";
  //  std::string outfilename = "dump_deuterons.root";
  //  std::string particle = "deuteron";
  //  std::string outfilename = "dump_tritons.root";
  //  std::string particle = "triton";

  std::vector<Arm> arms;
  Arm right_arm;
  Arm left_arm;
  right_arm.detname = "SiR";
  right_arm.thin_exists = true;
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_1", "TMath::Gaus(x, 0, 42.7)", -250, 250));
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_2", "TMath::Gaus(x, 0, 37.7)", -250, 250));
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_3", "TMath::Gaus(x, 0, 40.7)", -250, 250));
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_4", "TMath::Gaus(x, 0, 43.0)", -250, 250));
  //  right_arm.thin_threshold = 100;
  right_arm.thin_thresholds.push_back(100); // set all SiR1 thresholds to be the same
  right_arm.thin_thresholds.push_back(100);
  right_arm.thin_thresholds.push_back(100);
  right_arm.thin_thresholds.push_back(100);
  right_arm.thin_saturation_energy = 7050;
  right_arm.thin_k_factor = 1.0;//1.5;
  right_arm.resolution_hists.push_back(new TH1F("resHist_SiR1_1", "", 500,-250,250));
  right_arm.resolution_hists.push_back(new TH1F("resHist_SiR1_2", "", 500,-250,250));
  right_arm.resolution_hists.push_back(new TH1F("resHist_SiR1_3", "", 500,-250,250));
  right_arm.resolution_hists.push_back(new TH1F("resHist_SiR1_4", "", 500,-250,250));
  right_arm.thick_exists = true;
  right_arm.thick_energy_resolution = new TF1("res_SiR2", "TMath::Gaus(x, 0, 32.7)", -250, 250);
  right_arm.thick_threshold = 200;
  right_arm.thick_saturation_energy = 17020;
  right_arm.thick_k_factor = 1.0;//0.8;//1.4;
  right_arm.third_exists = true;
  right_arm.third_energy_resolution = new TF1("res_SiR3", "TMath::Gaus(x, 0, 63.8)", -250, 250);
  right_arm.third_threshold = 300;
  right_arm.third_saturation_energy = -1; // no saturation
  right_arm.third_k_factor = 1.0;//0.8;//0.8;
  
  left_arm.detname = "SiL";
  left_arm.thin_exists = true;
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
  //  left_arm.thin_threshold = 140; // minimum threshold
  //  left_arm.thin_threshold = 234; // mean of all channels
  left_arm.thin_thresholds.push_back(100000); // turn off SiL1-1-S
  left_arm.thin_thresholds.push_back(296);
  left_arm.thin_thresholds.push_back(270);
  left_arm.thin_thresholds.push_back(219);
  left_arm.thin_thresholds.push_back(211);
  left_arm.thin_thresholds.push_back(199);
  left_arm.thin_thresholds.push_back(217);
  left_arm.thin_thresholds.push_back(292);
  left_arm.thin_thresholds.push_back(309);
  left_arm.thin_thresholds.push_back(229);
  left_arm.thin_thresholds.push_back(214);
  left_arm.thin_thresholds.push_back(210);
  left_arm.thin_thresholds.push_back(229);
  left_arm.thin_thresholds.push_back(233);
  left_arm.thin_thresholds.push_back(142);
  left_arm.thin_thresholds.push_back(100000); // turn off SiL1-16-S
  left_arm.thin_saturation_energy = -1;
  left_arm.thin_k_factor = 1.0;
  left_arm.thick_exists = true;
  left_arm.thick_energy_resolution = new TF1("res_SiL3", "TMath::Gaus(x, 0, 31.6)", -250, 250);
  left_arm.thick_threshold = 180;
  left_arm.thick_saturation_energy = 17430;
  left_arm.thick_k_factor = 0.85;//1.0;//0.875;//1.0;
  left_arm.third_exists = false;

  arms.push_back(right_arm);
  //  arms.push_back(left_arm);

  R15b_TransferMatrix(filename, outfilename, particle, arms);
}
