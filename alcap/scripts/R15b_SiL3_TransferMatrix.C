#include "R15b_TransferMatrix.C"

void R15b_SiL3_TransferMatrix() {

  //  std::string particle = "proton";
  //  std::string particle = "deuteron";
  
  //  std::string filename = "output/raw_g4sim.root";
  //  std::string filename = "output/R15b_1M_Geom-P1_deuterons-flat.root";
  
  //  std::string outfilename = "dump.root";
  //  std::string outfilename = "protonCorr_1M_Geom-P1_proton-flat.root";
  //  std::string outfilename = "protonCorr_1M_Geom-P1-3mm_proton-flat_Thresh0-1MeV.root";
  //  std::string outfilename = "protonCorr_1M_Geom-P1_proton-flat_Thresh0-5MeV.root";
  //  std::string outfilename = "protonCorr_1M_Geom-P1_proton-sw.root";
  //  std::string outfilename = particle + "Corr_1M_Geom-P1_" + particle + "-flat_Thresh0-1MeV.root";
  std::string outfilename = "combinedCorr_1M_Geom-P1_flat_Thresh0-1MeV.root";

  std::vector<Arm> arms;
  Arm left_arm;
  left_arm.detname = "SiL";
  left_arm.thin_exists = false;
  left_arm.thick_exists = true;
  left_arm.thick_energy_resolution = new TF1("res_SiL3", "TMath::Gaus(x, 0, 30.9)", -250, 250);
  left_arm.thick_threshold = 35;
  left_arm.thick_k_factor = 1.0;
  left_arm.thick_saturation_energy = -1; // no saturation
  left_arm.third_exists = false;
  left_arm.two_layer_response = NULL;
  left_arm.three_layer_response = NULL;
  left_arm.middle_layer_response = NULL;

  arms.push_back(left_arm);

  std::string tgtvolname = "SiL3";
  double max_axis = 50000;

  std::string particle = "deuteron";
  std::string filename = "output/R15b_1M_Geom-P1_" + particle + "s-flat.root";
  int n_deuteron_entries = 900000;
  R15b_TransferMatrix(filename, outfilename, particle, tgtvolname, max_axis, arms, n_deuteron_entries);

  particle = "proton";
  filename = "output/R15b_1M_Geom-P1_" + particle + "s-flat.root";
  int n_proton_entries = 100000;
  R15b_TransferMatrix(filename, outfilename, particle, tgtvolname, max_axis, arms, n_proton_entries);
}
