#include "R15b_MuPlusEDep.C"

void R15b_SiL3_MuPlusEDep() {

  //  std::string filename = "output/R15b_10M_Geom-P1_muplus.root";
  std::string filename = "output/R15b_1M_Geom-P1_muplus.root";
  //  std::string filename = "output/raw_g4sim.root";
  std::string outfilename = "decayCorr_1M_Geom-P1_muplus.root";
  std::string detname = "SiL3";
  double det_resolution = 30.9; // keV
  double det_threshold = 35; // keV
  double det_k_factor = 1.0;

  R15b_MuPlusEDep(filename, outfilename, detname, det_resolution, det_threshold, det_k_factor);
}
