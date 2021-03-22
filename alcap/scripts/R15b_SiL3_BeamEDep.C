#include "R15b_BeamEDep.C"

void R15b_SiL3_BeamEDep() {

  //  std::string filename = "output/raw_g4sim.root";
  std::string filename = "output/R15b_200k_Geom-P1_muon-beam-sim_SF1-12-nominal.root";
  //  std::string filename = "output/R15b_200k_Geom-P1_muon-beam-sim_SF1-12-lowerE.root";
  //  std::string filename = "output/R15b_200k_Geom-P1_muon-beam-sim_SF1-12-higherE.root";
  std::string outfilename = "dump.root";
  //  std::string detname = "Target";
  std::string detname = "SiL3";
  double det_resolution = 30.9; // keV
  double det_threshold = 100;//35; // keV
  double det_k_factor = 1.0;

  R15b_BeamEDep(filename, outfilename, detname, det_resolution, det_threshold, det_k_factor);
}
