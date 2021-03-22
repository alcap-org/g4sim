#include "R15b_BeamEDep.C"

void R15b_Si16b_BeamEDep() {

  const int n_settings = 3;
  std::string setting_names[n_settings] = {"", "higherE", "lowerE"};

  std::string detname = "SiL1";
  double det_resolution = 30.9; // keV
  double det_threshold = 140;//35; // keV
  double det_k_factor = 1.0;

  for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
    std::string setting_name = setting_names[i_setting];
    
    //  std::string filename = "output/raw_g4sim.root";
    std::string filename = "output/R15b_2M_Geom-P5_muon-beam-sim_SF1-02";
    if (setting_name != "") {
      filename += "-" + setting_name;
    }
    filename += ".root";
    //    std::string filename = "output/R15b_2M_Geom-P5_muon-beam-sim_SF1-02-higherE.root";
    //  std::string filename = "output/R15b_2M_Geom-P5_muon-beam-sim_SF1-02-lowerE.root";
    std::string outfilename = "R16b_Si16b_BeamEDep";
    if (setting_name != "") {
      outfilename += "_" + setting_name;
    }
    outfilename += ".root";
    //  std::string detname = "Target";

    R15b_BeamEDep(filename, outfilename, detname, det_resolution, det_threshold, det_k_factor);
  }
}
