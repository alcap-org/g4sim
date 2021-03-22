#include "R15b_MuPlusEDep.C"

void R15b_SiL3_MuPlusEDep() {

  //  std::string filename = "output/R15b_10M_Geom-P1_muplus.root";
  //  std::string filename = "output/R15b_1M_Geom-P1_muplus.root";
  //  std::string filename = "output/raw_g4sim.root";
  //  std::string outfilename = "decayCorr_1M_Geom-P1_muplus.root";

  // std::string filename = "output/R15b_1M_Geom-P1-higherE_muplus.root";
  // std::string outfilename = "decayCorr_1M_Geom-P1-higherE_muplus.root";
  // std::string filename = "output/R15b_1M_Geom-P1-lowerE_muplus.root";
  // std::string outfilename = "decayCorr_1M_Geom-P1LowerE_muplus.root";

  const int n_settings = 3;
  std::string settings[n_settings] = {"", "-higherE", "-lowerE"};

  for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
    std::string setting = settings[i_setting];
    std::string filename = "output/R15b_1M_Geom-P1" + setting + "_muplus.root" ;
    std::string outfilename = "decayCorr_1M_Geom-P1" + setting + "_muplus_KFactor0-85_MeV.root";

    std::string detname = "SiL3";
    double det_resolution = 30.9; // keV
    double det_threshold = 35; // keV
    double det_k_factor = 0.85;
    
    R15b_MuPlusEDep(filename, outfilename, detname, det_resolution, det_threshold, det_k_factor, detname);
  }
}
