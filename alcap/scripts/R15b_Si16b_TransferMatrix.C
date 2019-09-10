#include "R15b_TransferMatrix.C"

void R15b_Si16b_TransferMatrix() {

  std::string particle = "proton";
  //  std::string particle = "deuteron";
  //  std::string particle = "triton";
  //  std::string particle = "alpha";
  
  //  std::string filename = "output/R15b_10M_Geom-P5_protons_new_wBugFix.root";
  //  std::string outfilename = "R15b_Si16b_response-matrix_500keV-bins_protons.root";
  std::string filename = "output/raw_g4sim.root";
  //  std::string filename = "output/R15b_10M_Geom-P5_protons-flat.root";
  
  //  std::string outfilename = "dump.root";
  //  std::string outfilename = "respMatrix_1M_Geom-P5_proton.root";
  //  std::string outfilename = "respMatrix_1M_Geom-P5_" + particle + ".root";
  //  std::string outfilename = "respMatrix_10M_Geom-P5_" + particle + ".root";
  //  std::string outfilename = "respMatrix_10M_Geom-P5-lowerE_" + particle + ".root";
  std::string outfilename = "respMatrix_10M_Geom-P5-lowestE_" + particle + ".root";
  //  std::string outfilename = "respMatrix_10M_Geom-P5-higherE_" + particle + ".root";
  //  std::string outfilename = "respMatrix_10M_Geom-P5-highestE_" + particle + ".root";
  //  std::string outfilename = "respMatrix_10M_Geom-P5-1mmCloserSiR_" + particle + ".root";
  //  std::string outfilename = "respMatrix_1M_Geom-P5_" + particle + "-sw.root";

  std::vector<Arm> arms;
  Arm right_arm;
  Arm left_arm;
  right_arm.detname = "SiR";
  right_arm.thin_exists = true;
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_1", "TMath::Gaus(x, 0, 42.7)", -250, 250));
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_2", "TMath::Gaus(x, 0, 37.7)", -250, 250));
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_3", "TMath::Gaus(x, 0, 40.7)", -250, 250));
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_4", "TMath::Gaus(x, 0, 43.0)", -250, 250));
  right_arm.thin_thresholds.push_back(90);
  right_arm.thin_thresholds.push_back(100);
  right_arm.thin_thresholds.push_back(80);
  right_arm.thin_thresholds.push_back(90);
  right_arm.thin_saturation_energy = -1; // no saturation
  
  right_arm.thin_k_factor = 1.0;//1.5;
  right_arm.resolution_hists.push_back(new TH1F("resHist_SiR1_1", "", 500,-250,250));
  right_arm.resolution_hists.push_back(new TH1F("resHist_SiR1_2", "", 500,-250,250));
  right_arm.resolution_hists.push_back(new TH1F("resHist_SiR1_3", "", 500,-250,250));
  right_arm.resolution_hists.push_back(new TH1F("resHist_SiR1_4", "", 500,-250,250));
  right_arm.thick_exists = true;
  right_arm.thick_energy_resolution = new TF1("res_SiR2", "TMath::Gaus(x, 0, 32.7)", -250, 250);
  right_arm.thick_threshold = 200;
  right_arm.thick_k_factor = 1.0;//0.8;//1.4;
  right_arm.thick_saturation_energy = -1; // no saturation
  right_arm.third_exists = true;
  right_arm.third_energy_resolution = new TF1("res_SiR3", "TMath::Gaus(x, 0, 63.8)", -250, 250);
  right_arm.third_threshold = 300;
  right_arm.third_saturation_energy = -1; // no saturation
  right_arm.third_k_factor = 1.0;//0.8;//0.8;
  right_arm.two_layer_response = NULL;
  right_arm.three_layer_response = NULL;
  right_arm.middle_layer_response = NULL;
  
  left_arm.detname = "SiL";
  left_arm.thin_exists = false;
  left_arm.thick_exists = true;
  left_arm.thick_energy_resolution = new TF1("res_SiL3", "TMath::Gaus(x, 0, 31.6)", -250, 250);
  left_arm.thick_threshold = 180;
  left_arm.thick_k_factor = 1.0;
  left_arm.thick_saturation_energy = -1; // no saturation
  left_arm.third_exists = false;
  left_arm.third_threshold = 0;
  left_arm.two_layer_response = NULL;
  left_arm.three_layer_response = NULL;
  left_arm.middle_layer_response = NULL;

  arms.push_back(right_arm);
  //  arms.push_back(left_arm);

  std::string tgtvolname = "SiL1";
  double max_axis = 25000;
  R15b_TransferMatrix(filename, outfilename, particle, tgtvolname, max_axis, arms);
}
