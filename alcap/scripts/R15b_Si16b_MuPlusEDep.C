#include "R15b_MuPlusEDep.C"

void R15b_Si16b_MuPlusEDep() {

  //  std::string filename = "output/R15b_1M_Geom-P4_muplus_w1475umSiL3.root";
  //  std::string filename = "output/R15b_1M_Geom-P4_muplus_w1200umSiL3.root";
  std::string filename = "output/R15b_100k_Geom-P5_muplus.root";
  std::string outfilename = "decayCorr_100k_Geom-P5.root";

  std::string detname = "SiL1";//"SiL3";
  double det_resolution = 30.9; // keV
  double det_threshold = 140; // keV
  double det_k_factor = 1.0;
  std::string tgtvolname = "SiL1";
  double x_max = 25000;

  R15b_MuPlusEDep(filename, outfilename, detname, det_resolution, det_threshold, det_k_factor, tgtvolname, x_max);
}
