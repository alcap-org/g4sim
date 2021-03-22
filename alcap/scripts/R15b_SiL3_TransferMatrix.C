#include "R15b_TransferMatrix.C"

void R15b_SiL3_TransferMatrix() {

  // //  std::string particle = "proton";
  // //  std::string particle = "deuteron";
  // //  std::string particle = "triton";
  // std::string particle = "alpha";
  
  // //  std::string filename = "output/raw_g4sim.root";
  // std::string filename = "output/R15b_1M_Geom-P1_" + particle + "s-flat.root";
  // //  std::string filename = "output/R15b_1M_Geom-P1_" + particle + "s-SWSpec.root";
  
  // //  std::string outfilename = "dump.root";
  // //  std::string outfilename = "protonCorr_1M_Geom-P1_proton-flat.root";
  // //  std::string outfilename = "protonCorr_1M_Geom-P1-3mm_proton-flat_Thresh0-1MeV.root";
  // //  std::string outfilename = "protonCorr_1M_Geom-P1_proton-flat_Thresh0-5MeV.root";
  // //  std::string outfilename = particle + "Corr_1M_Geom-P1_" + particle + "-sw.root";
  // std::string outfilename = particle + "Corr_1M_Geom-P1_" + particle + "-flat_Thresh0-1MeV.root";
  // //  std::string outfilename = "combinedCorr_1M_Geom-P1_combined-flat_Thresh0-1MeV_cutObsE.root";
  // //  std::string outfilename = "combinedCorr_1M_Geom-P1_combined-flat_Thresh0-1MeV_noPID.root";
  // //  std::string outfilename = "combinedCorr_1M_Geom-P1_combined-flat_Thresh0-1MeV_missingAlpha.root";
  // //  std::string outfilename = "combinedCorr_1M_Geom-P1_combined-flat_Thresh0-1MeV_TCutG.root";
  // //  std::string outfilename = "combinedCorr_1M_Geom-P1_combined-flat_Thresh1-4MeV.root";
  // //  std::string outfilename = "combinedCorr_1M_Geom-P1_combined-flat_Thresh0-1MeV_fractions.root";
  // //  std::string outfilename = "combinedCorr_1M_Geom-P1_combined-sw.root";

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
  left_arm.hELoss = NULL;
  
  Arm right_arm;
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
  right_arm.third_exists = false;
  //  right_arm.third_energy_resolution = new TF1("res_SiR3", "TMath::Gaus(x, 0, 63.8)", -250, 250);
  //  right_arm.third_threshold = 300;
  //  right_arm.third_saturation_energy = -1; // no saturation
  //  right_arm.third_k_factor = 1.0;//0.8;//0.8;
  right_arm.two_layer_response = NULL;
  right_arm.three_layer_response = NULL;
  right_arm.middle_layer_response = NULL;
  right_arm.hELoss = NULL;
  
  arms.push_back(left_arm);
  //  arms.push_back(right_arm);
  
  std::string tgtvolname = "SiL3";
  double max_axis = 50;
  //  R15b_TransferMatrix(filename, outfilename, particle, tgtvolname, max_axis, arms);

  int min_volID = 0; // not actually used
  int max_volID = 16; // not actually used

  double min_true_kinetic_energy = 0;//1400;//0;
  double max_true_kinetic_energy = 100000000;
  const int n_particles = 4;
  std::string particles[n_particles] = {"proton", "deuteron", "triton", "alpha"};
  //  double fractions[n_particles] = {0.484, 0.283, 0.060, 0.173};
  //  double fractions[n_particles] = {0.536, 0.263, 0.035, 0.166};
  //  double fractions[n_particles] = {0.45, 0.28, 0.08, 0.19};
  //  double fractions[n_particles] = {0.54, 0.24, 0.07, 0.15}; // cutObsE
  //  double fractions[n_particles] = {0.43, 0.32, 0.07, 0.18}; // no PID
  //  double fractions[n_particles] = {0.36, 0.22, 0.06, 0.36}; // missingAlpha

  // old numbers
  //  double fractions[n_particles] = {0.436, 0.341, 0.047, 0.176}; // TCutG (central value)
  //  double fractions[n_particles] = {0.492, 0.319, 0.033, 0.156}; // TCutG (low syst.)
  //  double fractions[n_particles] = {0.380, 0.363, 0.061, 0.196}; // TCutG (high syst.)

  // new numbers
  //  std::string outfilename = "combinedCorr_1M_Geom-P1_combined-flat_Thresh0-1MeV.root";
  //  double fractions[n_particles] = {0.451, 0.319, 0.048, 0.182}; // TCutG (central value)
  //  std::string outfilename = "combinedLowCorr_1M_Geom-P1_combinedLow-flat_Thresh0-1MeV.root";
  //  double fractions[n_particles] = {0.509, 0.297, 0.033, 0.161}; // TCutG (low syst.)
  //  std::string outfilename = "combinedHighCorr_1M_Geom-P1_combinedHigh-flat_Thresh0-1MeV.root";
  //  double fractions[n_particles] = {0.393, 0.341, 0.063, 0.203}; // TCutG (high syst.)

  // even newer numbers
  // HigherECorr and LowerECorr are for the MuStopPos systematics
  //  std::string outfilename = "combinedCorr_1M_Geom-P1_combined-flat_Thresh0-1MeV.root";
  //  std::string outfilename = "combinedHigherECorr_1M_Geom-P1_combinedHigherE-flat_Thresh0-1MeV.root";
  //  std::string outfilename = "combinedLowerECorr_1M_Geom-P1_combinedLowerE-flat_Thresh0-1MeV.root";
  //  double fractions[n_particles] = {0.443, 0.323, 0.049, 0.185}; // TCutG (central value)
  //  std::string outfilename = "combinedLowCorr_1M_Geom-P1_combinedLow-flat_Thresh0-1MeV.root";
  //  double fractions[n_particles] = {0.509, 0.304, 0.026, 0.161}; // TCutG (low syst.)
  //  std::string outfilename = "combinedHighCorr_1M_Geom-P1_combinedHigh-flat_Thresh0-1MeV.root";
  //  double fractions[n_particles] = {0.379, 0.323, 0.050, 0.248}; // TCutG (high syst.)

  // // even newer numbers again (6th Feb 2020)
  // // HigherECorr and LowerECorr are for the MuStopPos systematics
  // std::string outfilename = "combinedCorr_1M_Geom-P1_combined-flat_Thresh0-1MeV.root";
  // //  std::string outfilename = "combinedHigherECorr_1M_Geom-P1_combinedHigherE-flat_Thresh0-1MeV.root";
  // //  std::string outfilename = "combinedLowerECorr_1M_Geom-P1_combinedLowerE-flat_Thresh0-1MeV.root";
  // double fractions[n_particles] = {0.453, 0.343, 0.054, 0.150}; // TCutG (central value)
  // //  std::string outfilename = "combinedLowCorr_1M_Geom-P1_combinedLow-flat_Thresh0-1MeV.root";
  // //  double fractions[n_particles] = {0.546, 0.309, 0.031, 0.114}; // TCutG (low syst.)
  // //  std::string outfilename = "combinedHighCorr_1M_Geom-P1_combinedHigh-flat_Thresh0-1MeV.root";
  // //  double fractions[n_particles] = {0.354, 0.386, 0.071, 0.189}; // TCutG (high syst.)

  // final numbers (2nd July 2020)
  // minor updates (23rd July 2020)
  // more minor updates (7th December 2020)
  const int n_fractions = 7;
  std::string fractions[n_fractions] = {"proton", "deuteron", "triton", "alpha", "combined", "combinedLow", "combinedHigh"}; // HigherECorr and LowerECorr are for the MuStopPos systematics
  double fraction_values[n_fractions][n_particles] = {
    {1.000, 0.000, 0.000, 0.000}, // protons
    {0.000, 1.000, 0.000, 0.000}, // deuterons
    {0.000, 0.000, 1.000, 0.000}, // tritons
    {0.000, 0.000, 0.000, 1.000}, // alphas
    //    {0.513, 0.288, 0.048, 0.152}, // (central value)
    //    {0.584, 0.271, 0.035, 0.111}, // (low syst.)
    //    {0.449, 0.288, 0.060, 0.204}  // (high syst.)
    {0.504, 0.293, 0.049, 0.154}, // (central value)
    {0.564, 0.287, 0.036, 0.113}, // (low syst.)
    {0.444, 0.293, 0.056, 0.207}  // (high syst.)
  };

  const int n_muon_positions = 3;
  std::string muon_positions[n_muon_positions] = {"", "higherE", "lowerE"};

  int n_total_entries = 1000000;

  for (int i_muon_position = 0; i_muon_position < n_muon_positions; ++i_muon_position) {
    std::string muon_position = muon_positions[i_muon_position];
  
    for (int i_fraction = 0; i_fraction < n_fractions; ++i_fraction) {
      std::string fraction = fractions[i_fraction];
      std::string outfilename = fraction + "Corr_1M_Geom-P1_" + fraction + "-flat_Thresh0-1MeV_";
      if (muon_position != "") {
	outfilename +=  muon_position;
      }
      outfilename += "_MeV.root";

      // Reset response matrices
      for (std::vector<Arm>::iterator i_arm = arms.begin(); i_arm != arms.end(); ++i_arm) {
	if ( (*i_arm).two_layer_response != NULL) {
	  delete (*i_arm).two_layer_response;
	  delete (*i_arm).three_layer_response;
	  delete (*i_arm).middle_layer_response;
	  
	  (*i_arm).two_layer_response = NULL;
	  (*i_arm).three_layer_response = NULL;
	  (*i_arm).middle_layer_response = NULL;
	}
      }

      for (int i_particle = 0; i_particle < n_particles; ++i_particle) {
	std::string particle = particles[i_particle];
	double fraction_value = fraction_values[i_fraction][i_particle];
    
	std::string filename = "output/R15b_1M_Geom-P1";
	if (muon_position != "") {
	  filename += "-" + muon_position;
	}
	filename += "_" + particle + "s-flat.root";
      //    std::string filename = "output/R15b_1M_Geom-P1_" + particle + "s-SWSpec.root";
	int n_entries = n_total_entries*fraction_value;
	double bin_width = 0.1;
	R15b_TransferMatrix(filename, outfilename, particle, tgtvolname, max_axis, arms, bin_width, min_volID, max_volID, min_true_kinetic_energy, max_true_kinetic_energy, n_entries, 1.0);
      }
    }
  }
  
  /*
  const int n_ranges = 8;
  double min_true_kinetic_energies[n_ranges] = {0, 3500, 4500, 5500, 6500, 7500, 8500, 9500};
  double max_true_kinetic_energies[n_ranges] = {3500, 4500, 5500, 6500, 7500, 8500, 9500, 50000};
  float proton_fractions[n_ranges] = {1.00, 0.73, 0.60, 0.55, 0.50, 0.48, 0.40, 0.40};

  for (int i_range = 0; i_range < n_ranges; ++i_range) {
  
    double min_true_kinetic_energy = min_true_kinetic_energies[i_range];
    double max_true_kinetic_energy = max_true_kinetic_energies[i_range];
    float proton_fraction = proton_fractions[i_range];
    float deuteron_fraction = 1 - proton_fraction;

    std::string particle = "deuteron";
    std::string filename = "output/R15b_1M_Geom-P1_" + particle + "s-flat.root";
    int n_deuteron_entries = -1;//420000;
    R15b_TransferMatrix(filename, outfilename, particle, tgtvolname, max_axis, arms, min_volID, max_volID, min_true_kinetic_energy, max_true_kinetic_energy, n_deuteron_entries, deuteron_fraction);
    
    int deuteron_total = left_arm.n_middle_layer_hits + left_arm.n_middle_layer_misses;
    particle = "proton";
    filename = "output/R15b_1M_Geom-P1_" + particle + "s-flat.root";
    int n_proton_entries = -1;//580000;
    //  int n_proton_entries = (deuteron_total / 0.27)*0.73;
    R15b_TransferMatrix(filename, outfilename, particle, tgtvolname, max_axis, arms, min_volID, max_volID, min_true_kinetic_energy, max_true_kinetic_energy, n_proton_entries, proton_fraction);
  }
  */
}
