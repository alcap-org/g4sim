#include "R15b_TransferMatrix.C"

void R15b_Si16b_TransferMatrix() {

  std::vector<Arm> arms;
  Arm right_arm;
  Arm left_arm;
  //  right_arm.recoil_factor = 1.0 - recoil_fraction;
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
  right_arm.thin_saturation_energy = -1;//7050;//-1; // no saturation
  
  right_arm.thin_k_factor = 1.0;//1.5;
  right_arm.resolution_hists.push_back(new TH1F("resHist_SiR1_1", "", 500,-250,250));
  right_arm.resolution_hists.push_back(new TH1F("resHist_SiR1_2", "", 500,-250,250));
  right_arm.resolution_hists.push_back(new TH1F("resHist_SiR1_3", "", 500,-250,250));
  right_arm.resolution_hists.push_back(new TH1F("resHist_SiR1_4", "", 500,-250,250));
  right_arm.thick_exists = true;
  right_arm.thick_energy_resolution = new TF1("res_SiR2", "TMath::Gaus(x, 0, 32.7)", -250, 250);
  right_arm.thick_threshold = 200;
  right_arm.thick_k_factor = 1.0;//0.8;//1.4;
  right_arm.thick_saturation_energy = -1;//17020;//-1; // no saturation
  right_arm.third_exists = true;
  right_arm.third_energy_resolution = new TF1("res_SiR3", "TMath::Gaus(x, 0, 63.8)", -250, 250);
  right_arm.third_threshold = 300;
  right_arm.third_saturation_energy = -1;//17430;//-1; // no saturation
  right_arm.third_k_factor = 1.0;//0.8;//0.8;
  right_arm.two_layer_response = NULL;
  right_arm.three_layer_response = NULL;
  right_arm.middle_layer_response = NULL;

  //  left_arm.recoil_factor = right_arm.recoil_factor;
  left_arm.detname = "SiL";
  //  left_arm.thin_exists = false;
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

  //  left_arm.thick_exists = true;
  //  left_arm.thick_energy_resolution = new TF1("res_SiL3", "TMath::Gaus(x, 0, 31.6)", -250, 250);
  //  left_arm.thick_threshold = 180;
  //  left_arm.thick_k_factor = 1.0;
  //  left_arm.thick_saturation_energy = -1; // no saturation
  left_arm.thick_exists = false;
  left_arm.third_exists = false;
  left_arm.third_threshold = 0;
  left_arm.two_layer_response = NULL;
  left_arm.three_layer_response = NULL;
  left_arm.middle_layer_response = NULL;

  arms.push_back(right_arm);
  arms.push_back(left_arm);

  
  const int n_particles = 1;//4;
  std::string particles[n_particles] = {"alpha"};//{"proton"};//, "deuteron", "triton", "alpha"};
  //  std::string sample_sizes[n_particles] = {"10M", "10M", "10M", "12M"};
  std::string sample_sizes[n_particles] = {"100M"};//, "10M", "10M", "12M"};

  const int n_settings = 1;//5;
  std::string settings[n_settings] = {""};//{""};//, "higherE", "lowerE", "closerSiR", "furtherSiR"};

  const int n_bin_widths = 1;
  double bin_widths[n_bin_widths] = {0.1};//{1, 0.5};
  std::string BinWs[n_bin_widths] = {"100keV"};//{"1MeV", "500keV"};
  
  for (int i_particle = 0; i_particle < n_particles; ++i_particle) {
    std::string particle = particles[i_particle];
    double recoil_fraction = 0.0; // no recoil for the passive analyses
    std::string sample_size = sample_sizes[i_particle];
    //  std::string particle = "He3";
    //  std::string particle = "Li6";
    //  std::string particle = "He6";

    for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
      std::string setting = settings[i_setting];

      std::string filename = "output/R15b_" + sample_size + "_Geom-P5";
      if (setting != "") {
      	filename += "-" + setting;
      }
      filename += "_" + particle + "s-flat.root";
      //      filename = "output/raw_g4sim.root";
    //  std::string filename = "output/R15b_10M_Geom-P5_protons_new_wBugFix.root";
    //  std::string outfilename = "R15b_Si16b_response-matrix_500keV-bins_protons.root";
    //  std::string filename = "output/raw_g4sim.root";
    //  std::string filename = "output/R15b_" + sample_size + "_Geom-P5_" + particle + "s-flat_retune.root";
    //  std::string filename = "output/R15b_" + sample_size + "_Geom-P5_" + particle + "s-flat_newGaps.root";
    //  std::string filename = "output/R15b_" + sample_size + "_Geom-P5_" + particle + "s-flat_higherE.root";
    //  std::string filename = "output/R15b_" + sample_size + "_Geom-P5_" + particle + "s-flat_halfDL.root";
    //  std::string filename = "output/R15b_100k_Geom-P5_" + particle + "s-flat_activeTgt.root";
  
    //  std::string outfilename = "dump.root";
    //  std::string outfilename = "respMatrix_1M_Geom-P5_proton.root";
    //  std::string outfilename = "respMatrix_1M_Geom-P5_" + particle + ".root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5-lowestE_" + particle + ".root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5-highestE_" + particle + ".root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5-1mmCloserSiR_" + particle + ".root";
    //  std::string outfilename = "respMatrix_1M_Geom-P5_" + particle + "-sw.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5-flipX_" + particle + ".root";
    //    std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_retune_SiL1-11--15.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_500keVBins.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_1000keVBins.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_1000keVBins_higherE.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_1000keVBins_lowerE.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_1000keVBins_closerSiR.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_1000keVBins_furtherSiR.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_1000keVBins_cutObsE.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_1000keVBins_truth.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_1000keVBins_newGaps.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_1000keVBins_halfDL.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_1250keVBins.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_2000keVBins.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_retune.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_retune_500keVBins.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_noSiR3.root";
    //  std::string outfilename = "respMatrix_100k_Geom-P5_" + particle + "_activeTgt.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_1000keVBins_saturation.root";
    //  std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_1000keVBins_withRecoil.root";

      std::string tgtvolname = "SiL1";
    double max_axis = 25;
    if (particle == "alpha") {
      max_axis = 30;
    }

    for (int i_bin_width = 0; i_bin_width < n_bin_widths; ++i_bin_width) {
      double bin_width = bin_widths[i_bin_width];
      std::string BinW = BinWs[i_bin_width];
      if (bin_width == 0.5 && setting != "") {
	continue; // only want 500 keV bins for central setting
      }

      std::string outfilename = "respMatrix_" + sample_size + "_Geom-P5_" + particle + "_" + BinW + "Bins";
      if (setting != "") {
	outfilename += "_" + setting;
      }
      outfilename += ".root";

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

      R15b_TransferMatrix(filename, outfilename, particle, tgtvolname, max_axis, arms, bin_width);//, 1, 16, 0, 10000000, 10000000);
    }

    /*
      outfilename = "respMatrix_100k_Geom-P5_combined_activeTgt.root";
      int min_volID = 0; // not actually used
      int max_volID = 16; // not actually used
      
      double min_true_kinetic_energy = 0;
      double max_true_kinetic_energy = 100000000;
      const int n_particles = 4;
      std::string particles[n_particles] = {"proton", "deuteron", "triton", "alpha"};
      double fractions[n_particles] = {0.45, 0.28, 0.08, 0.19};
      int n_total_entries = 100000;
      
      for (int i_particle = 0; i_particle < n_particles; ++i_particle) {
      std::string particle = particles[i_particle];
      double fraction = fractions[i_particle];
      
      std::string filename = "output/R15b_100k_Geom-P5_" + particle + "s-flat_activeTgt.root";
      int n_entries = n_total_entries*fraction;
      R15b_TransferMatrix(filename, outfilename, particle, tgtvolname, max_axis, arms, min_volID, max_volID, min_true_kinetic_energy, max_true_kinetic_energy, n_entries, 1.0);
      }
    */
    }
  }
}
