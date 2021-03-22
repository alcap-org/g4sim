#include "R15b_MuPlusEDep.C"

void R15b_Ti50_MuPlusEDep() {

  //  std::string filename = "output/R15b_1M_Geom-P4_muplus_w1475umSiL3.root";
  //  std::string filename = "output/R15b_1M_Geom-P4_muplus_w1200umSiL3.root";
  std::string filename = "output/raw_g4sim.root";
  std::string outfilename = "dump.root";
  std::string detname = "SiL3";
  double det_resolution = 30.9; // keV
  double det_threshold = 140; // keV
  double det_k_factor = 0.85;//1.0;

  R15b_MuPlusEDep(filename, outfilename, detname, det_resolution, det_threshold, det_k_factor);
}
