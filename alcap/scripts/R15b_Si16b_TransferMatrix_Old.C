// NB this assumes you are working on a MC run where protons were the initial particle
// generated inside the target

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TF1.h"

#include "RooUnfold.h"

#include <iostream>
#include <cmath>

extern TSystem* gSystem;

struct Arm {
  std::string detname;
  RooUnfoldResponse* two_layer_response;
  int n_two_layer_hits;
  int n_two_layer_misses;
  RooUnfoldResponse* three_layer_response;
  int n_three_layer_hits;
  int n_three_layer_misses;
  RooUnfoldResponse* middle_layer_response;
  int n_middle_layer_hits;
  int n_middle_layer_misses;
  double total_thin_edep;
  double total_thick_edep;
  bool thick_stopped;
  double total_third_edep;
  bool third_stopped;

  TH2F* hEvdE_stopped;
  TH2F* hEvdE_notstopped;
  TH2F* hEvdE_ThreeLayer_stopped;
  TH2F* hEvdE_ThreeLayer_notstopped;
  TH2F* hSingleDetAxes_TwoLayer_stopped;
  TH2F* hSingleDetAxes_TwoLayer_notstopped;
  TH2F* hSingleDetAxes_ThreeLayer_12_stopped;
  TH2F* hSingleDetAxes_ThreeLayer_12_notstopped;
  TH2F* hSingleDetAxes_ThreeLayer_23_stopped;
  TH2F* hSingleDetAxes_ThreeLayer_23_notstopped;
  TH2F* hSingleDetAxes_ThreeLayer_13_stopped;
  TH2F* hSingleDetAxes_ThreeLayer_13_notstopped;

  std::vector<TF1*> thin_energy_resolutions;
  std::vector<TH1F*> resolution_hists;
  double thin_threshold;
  double thin_k_factor;
  TF1* thick_energy_resolution;
  double thick_k_factor;
  double thick_threshold;
  TF1* third_energy_resolution;
  double third_k_factor;
  double third_threshold;

  bool thin_exists;
  bool thick_exists;
  bool third_exists;
};

struct LayerHit {
  std::string layerID;
  std::string arm;
  int track_ID;
  double time;
  double edep;
  //  double px, py, pz;
  int parent_pid;
  std::string ovolName;
  std::vector<std::string> volNames;
  std::vector<double> edeps;
  std::vector<std::string> particleNames;
  int stopped;
};
  

void R15b_Si16b_TransferMatrix_Refactor() {

  //  std::string filename = "output/R15b_10M_Geom-P5_protons-restricted-dir.root";
  //  std::string filename = "output/R15b_10M_Geom-P5_protons.root";
  //  std::string filename = "output/R15b_10M_Geom-P5_protons_shallower.root";
  //  std::string filename = "output/R15b_10M_Geom-P5_protons_shallower-still.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_protons.root";

  //  std::string filename = "output/raw_g4sim.root";
  //  std::string filename = "output/R15b_10M_Geom-P5_protons_new.root";
  //  std::string filename = "output/R15b_10M_Geom-P5_protons_new_wBugFix.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_protons_new_SiR3Thickness-1475um.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_protons_new_SiR3Thickness-100um.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_protons_new_SiDeadLayers-AllSiR.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_protons_new_SiAlDeadLayers-AllSiR.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_protons_new_SiAlDeadLayers-AllSiR_SiR3-1400umThick.root";
  //    std::string filename = "output/R15b_1M_Geom-P5_protons_new_SiR3-1360umThick.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_protons_new_SiR3-1000umThick.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_protons_new_SiR3-750umThick.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_protons_new_SiR3-800umThick.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_protons_new_SiR3-850umThick.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_electron_new.root";
  //  std::string filename = "output/R15b_10M_Geom-P5_deuterons_new.root";
  //  std::string filename = "output/R15b_10M_Geom-P5_deuterons_new_wBugFix.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_deuterons_new_WideERange.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_tritons_new_wBugFix.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_alphas_new_wBugFix.root";
  //  std::string filename = "output/R15b_10M_Geom-P5_protons_new_wBugFix.root";
  std::string filename = "output/R15b_10M_Geom-P5_protons-SWSpec.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_electrons-45-55MeV.root";

  //  std::string outfilename = "R15b_Si16b_response-matrix_500keV-bins_protons_new_wBugFix.root";
  std::string outfilename = "R15b_Si16b_response-matrix_500keV-bins_protons-SWSpec.root";
  //  std::string outfilename = "R15b_Si16b_response-matrix_500keV-bins_deuterons_new_wBugFix.root";
  //  std::string outfilename = "R15b_Si16b_response-matrix_500keV-bins_tritons_new_wBugFix.root";
  //  std::string outfilename = "R15b_Si16b_response-matrix_500keV-bins_alphas_new_wBugFix.root";
  //  std::string outfilename = "R15b_Si16b_response-matrix_500keV-bins_electrons_new.root";
  //  std::string outfilename = "R15b_Si16b_response-matrix_500keV-bins_proton_combineds_new_wBugFix.root";
  //  std::string outfilename = "R15b_Si16b_response-matrix_500keV-bins_electrons_new.root";
  //  std::string outfilename = "dump.root";
  
  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  double u = 931.494*1000; // convert to keV
  double electron_mass = 5.48579909070e-4*u;
  double proton_mass = 1.007276466879*u; // convert to keV
  double deuteron_mass = 2.01410178*u;
  double triton_mass = 3.0160492*u;
  double alpha_mass = 4.001506179125*u;
  double mass = proton_mass;

  double bin_width = 500;
  //  double energy_high = 25000;
  //  double energy_low = 0;
  //  int n_bins = (energy_high - energy_low) / bin_width;
  double observed_energy_low = 0;
  double observed_energy_high = 30000;
  int n_observed_bins = (observed_energy_high - observed_energy_low) / bin_width;
  double true_energy_low = 0;
  double true_energy_high = 30000;
  //  double true_energy_low = 45000; // for electrons
  //  double true_energy_high = 55000;
  int n_true_bins = (true_energy_high - true_energy_low) / bin_width;


  std::vector<Arm> arms;
  Arm right_arm;
  Arm left_arm;
  right_arm.detname = "SiR";
  right_arm.thin_exists = true;
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_1", "TMath::Gaus(x, 0, 42.7)", -250, 250));
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_2", "TMath::Gaus(x, 0, 37.7)", -250, 250));
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_3", "TMath::Gaus(x, 0, 40.7)", -250, 250));
  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_4", "TMath::Gaus(x, 0, 43.0)", -250, 250));
  right_arm.thin_threshold = 100;
  right_arm.thin_k_factor = 1.0;//1.5;
  right_arm.resolution_hists.push_back(new TH1F("resHist_SiR1_1", "", 500,-250,250));
  right_arm.resolution_hists.push_back(new TH1F("resHist_SiR1_2", "", 500,-250,250));
  right_arm.resolution_hists.push_back(new TH1F("resHist_SiR1_3", "", 500,-250,250));
  right_arm.resolution_hists.push_back(new TH1F("resHist_SiR1_4", "", 500,-250,250));
  right_arm.thick_exists = true;
  right_arm.thick_energy_resolution = new TF1("res_SiR2", "TMath::Gaus(x, 0, 32.7)", -250, 250);
  right_arm.thick_threshold = 200;
  right_arm.thick_k_factor = 1.0;//0.8;//1.4;
  right_arm.third_exists = true;
  right_arm.third_energy_resolution = new TF1("res_SiR3", "TMath::Gaus(x, 0, 63.8)", -250, 250);
  right_arm.third_threshold = 300;
  right_arm.third_k_factor = 1.0;//0.8;//0.8;
  
  left_arm.detname = "SiL";
  left_arm.thin_exists = false;
  left_arm.thick_exists = true;
  left_arm.thick_energy_resolution = new TF1("res_SiL3", "TMath::Gaus(x, 0, 31.6)", -250, 250);
  left_arm.thick_threshold = 180;
  left_arm.thick_k_factor = 1.0;
  left_arm.third_exists = false;

  arms.push_back(right_arm);
  arms.push_back(left_arm);
  for (std::vector<Arm>::iterator i_arm = arms.begin(); i_arm != arms.end(); ++i_arm) {
    std::string responsename = (*i_arm).detname + "_two_layer_response";
    (*i_arm).two_layer_response = new RooUnfoldResponse(n_observed_bins,observed_energy_low,observed_energy_high, n_true_bins,true_energy_low,true_energy_high, responsename.c_str());
    (*i_arm).n_two_layer_hits = 0;
    (*i_arm).n_two_layer_misses = 0;
    responsename = (*i_arm).detname + "_three_layer_response";
    (*i_arm).three_layer_response = new RooUnfoldResponse(n_observed_bins,observed_energy_low,observed_energy_high, n_true_bins,true_energy_low,true_energy_high, responsename.c_str());
    (*i_arm).n_three_layer_hits = 0;
    (*i_arm).n_three_layer_misses = 0;
    responsename = (*i_arm).detname + "_middle_layer_response";
    (*i_arm).middle_layer_response = new RooUnfoldResponse(n_observed_bins,observed_energy_low,observed_energy_high, n_true_bins,true_energy_low,true_energy_high,responsename.c_str());
    (*i_arm).n_middle_layer_hits = 0;
    (*i_arm).n_middle_layer_misses = 0;

	

    double bin_width = 10;
    double total_energy_high = 25000;
    double total_energy_low = 0;
    int n_total_bins = (total_energy_high - total_energy_low) / bin_width;
    double thin_energy_high = 20000;
    double thin_energy_low = 0;
    int n_thin_bins = (thin_energy_high - thin_energy_low) / bin_width;

    std::string histname = (*i_arm).detname + "_EvdE_stopped";
    (*i_arm).hEvdE_stopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_thin_bins,thin_energy_low,thin_energy_high);
    histname = (*i_arm).detname + "_EvdE_notstopped";
    (*i_arm).hEvdE_notstopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_thin_bins,thin_energy_low,thin_energy_high);

    histname = (*i_arm).detname + "_EvdE_ThreeLayer_stopped";
    (*i_arm).hEvdE_ThreeLayer_stopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_thin_bins,thin_energy_low,thin_energy_high);
    histname = (*i_arm).detname + "_EvdE_ThreeLayer_notstopped";
    (*i_arm).hEvdE_ThreeLayer_notstopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_thin_bins,thin_energy_low,thin_energy_high);

    histname = (*i_arm).detname + "_SingleDetAxes_TwoLayer_stopped";
    (*i_arm).hSingleDetAxes_TwoLayer_stopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_thin_bins,thin_energy_low,thin_energy_high);
    histname = (*i_arm).detname + "_SingleDetAxes_TwoLayer_notstopped";
    (*i_arm).hSingleDetAxes_TwoLayer_notstopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_thin_bins,thin_energy_low,thin_energy_high);
    histname = (*i_arm).detname + "_SingleDetAxes_ThreeLayer_12_stopped";
    (*i_arm).hSingleDetAxes_ThreeLayer_12_stopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_thin_bins,thin_energy_low,thin_energy_high);
    histname = (*i_arm).detname + "_SingleDetAxes_ThreeLayer_12_notstopped";
    (*i_arm).hSingleDetAxes_ThreeLayer_12_notstopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_thin_bins,thin_energy_low,thin_energy_high);
    histname = (*i_arm).detname + "_SingleDetAxes_ThreeLayer_23_stopped";
    (*i_arm).hSingleDetAxes_ThreeLayer_23_stopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_thin_bins,thin_energy_low,thin_energy_high);
    histname = (*i_arm).detname + "_SingleDetAxes_ThreeLayer_23_notstopped";
    (*i_arm).hSingleDetAxes_ThreeLayer_23_notstopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_thin_bins,thin_energy_low,thin_energy_high);
    histname = (*i_arm).detname + "_SingleDetAxes_ThreeLayer_13_stopped";
    (*i_arm).hSingleDetAxes_ThreeLayer_13_stopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_thin_bins,thin_energy_low,thin_energy_high);
    histname = (*i_arm).detname + "_SingleDetAxes_ThreeLayer_13_notstopped";
    (*i_arm).hSingleDetAxes_ThreeLayer_13_notstopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_thin_bins,thin_energy_low,thin_energy_high);

  }


  std::vector<std::string>* particleName = 0;
  std::vector<std::string>* volName = 0;
  std::vector<double>* edep = 0;
  std::vector<int>* stopped = 0;
  double i_px = 0;
  double i_py = 0;
  double i_pz = 0;
  std::vector<int>* tid = 0;
  std::vector<double>* t = 0;
  std::vector<double>* px = 0;
  std::vector<double>* py = 0;
  std::vector<double>* pz = 0;
  std::vector<std::string>* ovolName = 0;
  std::vector<int>* ppid = 0;
  std::vector<double>* opx = 0;
  std::vector<double>* opy = 0;
  std::vector<double>* opz = 0;
  std::vector<std::string>* oprocess = 0;
  std::vector<int>* volID = 0;
  
  TBranch* br_particleName = tree->GetBranch("M_particleName");
  TBranch* br_volName = tree->GetBranch("M_volName");
  TBranch* br_edep = tree->GetBranch("M_edep");
  TBranch* br_stopped = tree->GetBranch("M_stopped");
  TBranch* br_i_px = tree->GetBranch("i_px");
  TBranch* br_i_py = tree->GetBranch("i_py");
  TBranch* br_i_pz = tree->GetBranch("i_pz");
  TBranch* br_tid = tree->GetBranch("M_tid");
  TBranch* br_t = tree->GetBranch("M_t");
  TBranch* br_px = tree->GetBranch("M_px");
  TBranch* br_py = tree->GetBranch("M_py");
  TBranch* br_pz = tree->GetBranch("M_pz");
  TBranch* br_ovolName = tree->GetBranch("M_ovolName");
  TBranch* br_ppid = tree->GetBranch("M_ppid");
  TBranch* br_opx = tree->GetBranch("M_opx");
  TBranch* br_opy = tree->GetBranch("M_opy");
  TBranch* br_opz = tree->GetBranch("M_opz");
  TBranch* br_oprocess = tree->GetBranch("M_oprocess");
  TBranch* br_volID = tree->GetBranch("M_volID");
  
  br_particleName->SetAddress(&particleName);
  br_volName->SetAddress(&volName);
  br_edep->SetAddress(&edep);
  br_stopped->SetAddress(&stopped);
  br_i_px->SetAddress(&i_px);
  br_i_py->SetAddress(&i_py);
  br_i_pz->SetAddress(&i_pz);
  br_tid->SetAddress(&tid);
  br_t->SetAddress(&t);
  br_px->SetAddress(&px);
  br_py->SetAddress(&py);
  br_pz->SetAddress(&pz);
  br_ovolName->SetAddress(&ovolName);
  br_ppid->SetAddress(&ppid);
  br_opx->SetAddress(&opx);
  br_opy->SetAddress(&opy);
  br_opz->SetAddress(&opz);
  br_oprocess->SetAddress(&oprocess);
  br_volID->SetAddress(&volID);
  
  int n_entries = tree->GetEntries();
  for (int iEntry = 0; iEntry < n_entries; ++iEntry) {
    tree->GetEvent(iEntry);
      
    if (iEntry % 10000 == 0) {
      std::cout << iEntry << " / " << n_entries << std::endl;
    }

    std::vector<LayerHit*> layer_hits;
    double true_kinetic_energy = 0;
  
    // Calculate true energy here
    double true_px = i_px*1e3;
    double true_py = i_py*1e3;
    double true_pz = i_pz*1e3;
    double squared_mom = true_px*true_px + true_py*true_py + true_pz*true_pz;
    double total_energy = std::sqrt(squared_mom + mass*mass);
    true_kinetic_energy = total_energy - mass;

    //      std::cout << "Entry #" << iEntry << ": true kinetic energy = " << true_kinetic_energy << std::endl;
    bool didnt_start_in_target = false;
    for (int iElement = 0; iElement < particleName->size();  ++iElement) {

      std::string i_particleName = particleName->at(iElement);
      //      if (i_particleName != "proton") { // only want protons
	//	std::cout << i_particleName << " " << tid->at(iElement) << " " << edep->at(iElement)*1e6 << " " << t->at(iElement) <<std::endl;
      //	continue;
      //      }
      std::string i_volName = volName->at(iElement);
      std::string i_ovolName = ovolName->at(iElement);
      int i_tid = tid->at(iElement);
      int i_volID = volID->at(iElement);
      
      if (i_tid == 1 && i_ovolName.find("Target")==std::string::npos) { // only want events that started in the target
	//	std::cout << "ACtually started in " << i_ovolName << std::endl;
	didnt_start_in_target = true;
	break;
      }

      // Loop through the arms and record all energy deposits that have hit in any of the Si detectors
      for (std::vector<Arm>::iterator i_arm = arms.begin(); i_arm != arms.end(); ++i_arm) {
	std::string third_monname = i_arm->detname+"3";
	std::string thick_monname = i_arm->detname+"2";
	std::string thin_monname = i_arm->detname+"1";
	if (i_arm->detname == "SiL") {
	  thick_monname = "SiL3";
	  third_monname = "SiL2";
	}

	if (i_volName == thick_monname){ // && stopped->at(iElement) == 1) {
	  LayerHit* layer_hit = new LayerHit();
	  layer_hit->layerID = "thick";
	  layer_hit->arm = i_arm->detname;
	  layer_hit->track_ID = i_tid;
	  layer_hit->time = t->at(iElement);
	  layer_hit->edep = (edep->at(iElement)*1e6 + i_arm->thick_energy_resolution->GetRandom())*i_arm->thick_k_factor; // convert to keV and smear
	  layer_hit->ovolName = i_ovolName;
	  for (int jElement = 0; jElement < particleName->size(); ++jElement) {
	    layer_hit->volNames.push_back(volName->at(jElement));
	    layer_hit->edeps.push_back(edep->at(jElement)*1e6);
	    layer_hit->particleNames.push_back(particleName->at(jElement));
	  }
	  layer_hit->stopped = stopped->at(iElement);
	  layer_hits.push_back(layer_hit);
	}
	else if (i_volName == thin_monname) {
	  LayerHit* layer_hit = new LayerHit();
	  layer_hit->layerID = "thin";
	  layer_hit->arm = i_arm->detname;
	  layer_hit->track_ID = i_tid;
	  layer_hit->time = t->at(iElement);
	  double smear = (i_arm->thin_energy_resolutions).at(i_volID)->GetRandom();
	  (i_arm->resolution_hists).at(i_volID)->Fill(smear);
	  layer_hit->edep = (edep->at(iElement)*1e6 + smear)*i_arm->thin_k_factor; // convert to keV
	  layer_hit->ovolName = i_ovolName;
	  for (int jElement = 0; jElement < particleName->size(); ++jElement) {
	    layer_hit->volNames.push_back(volName->at(jElement));
	    layer_hit->edeps.push_back(edep->at(jElement)*1e6);
	    layer_hit->particleNames.push_back(particleName->at(jElement));
	  }
	  layer_hit->stopped = stopped->at(iElement);
	  layer_hits.push_back(layer_hit);
	}
	else if (i_volName == third_monname){ // && stopped->at(iElement) == 1) {
	  LayerHit* layer_hit = new LayerHit();
	  layer_hit->layerID = "third";
	  layer_hit->arm = i_arm->detname;
	  layer_hit->track_ID = i_tid;
	  layer_hit->time = t->at(iElement);
	  layer_hit->edep = (edep->at(iElement)*1e6 + i_arm->third_energy_resolution->GetRandom())*i_arm->third_k_factor; // convert to keV and smear
	  layer_hit->ovolName = i_ovolName;
	  for (int jElement = 0; jElement < particleName->size(); ++jElement) {
	    layer_hit->volNames.push_back(volName->at(jElement));
	    layer_hit->edeps.push_back(edep->at(jElement)*1e6);
	    layer_hit->particleNames.push_back(particleName->at(jElement));
	  }
	  layer_hit->stopped = stopped->at(iElement);
	  layer_hits.push_back(layer_hit);
	}
      }
    }

    if (didnt_start_in_target) {
      continue;
    }

    // After looping through the elements, we will hopefully have a list of layer hits
    for (std::vector<Arm>::iterator i_arm = arms.begin(); i_arm != arms.end(); ++i_arm) {
      i_arm->total_thin_edep = i_arm->total_thick_edep = i_arm->total_third_edep = 0;
      i_arm->thick_stopped = false;
      i_arm->third_stopped = false;
      for (std::vector<LayerHit*>::iterator i_layer_hit = layer_hits.begin(); i_layer_hit != layer_hits.end(); ++i_layer_hit) {
	if (i_arm->detname == (*i_layer_hit)->arm) {
	  if ( (*i_layer_hit)->layerID == "thin") {
	    i_arm->total_thin_edep += (*i_layer_hit)->edep;
	  }
	  else if ( (*i_layer_hit)->layerID == "thick") {
	    i_arm->total_thick_edep += (*i_layer_hit)->edep;
	    if ( (*i_layer_hit)->stopped == 1 && (*i_layer_hit)->track_ID == 1) {
	      i_arm->thick_stopped = true;
	    }
	  }
	  else if ( (*i_layer_hit)->layerID == "third") {
	    i_arm->total_third_edep += (*i_layer_hit)->edep;
	    if ( (*i_layer_hit)->stopped == 1 && (*i_layer_hit)->track_ID == 1) {
	      i_arm->third_stopped = true;
	    }
	  }
	}
      }
    }

    for (std::vector<Arm>::iterator i_arm = arms.begin(); i_arm != arms.end(); ++i_arm) {

      double observed_E = i_arm->total_thin_edep + i_arm->total_thick_edep;
      bool three_layer_hit = false;
      //      if (observed_E > 0 && i_arm->thick_stopped == true) {
      if (i_arm->total_thin_edep>i_arm->thin_threshold && i_arm->total_thick_edep>i_arm->thick_threshold && i_arm->total_third_edep<i_arm->third_threshold) {
	i_arm->two_layer_response->Fill(observed_E, true_kinetic_energy); // this arm saw the proton
	i_arm->three_layer_response->Fill(observed_E, true_kinetic_energy); // this arm saw the proton
	//	i_arm->two_layer_response->Fill(true_kinetic_energy, true_kinetic_energy - observed_E); // this arm saw the proton
	i_arm->n_two_layer_hits++;
	i_arm->n_three_layer_hits++;
      }
      else if (i_arm->total_thin_edep>i_arm->thin_threshold && i_arm->total_thick_edep>i_arm->thick_threshold && i_arm->total_third_edep>i_arm->third_threshold) {
	// also seen in three layer
	three_layer_hit = true;
	i_arm->three_layer_response->Fill(observed_E+i_arm->total_third_edep, true_kinetic_energy);
	i_arm->n_three_layer_hits++;
      }
      else {
	i_arm->two_layer_response->Miss(true_kinetic_energy); // otherwise it didn't
	i_arm->n_two_layer_misses++;
	
	if (!three_layer_hit) {
	  i_arm->three_layer_response->Miss(true_kinetic_energy);
	  i_arm->n_three_layer_misses++;
	}
      }
	
      //      if (i_arm->total_thin_edep<i_arm->thin_threshold && i_arm->total_thick_edep>i_arm->thick_threshold && i_arm->total_third_edep>i_arm->third_threshold) {
      if (i_arm->total_thick_edep>i_arm->thick_threshold) {
	i_arm->middle_layer_response->Fill(i_arm->total_thick_edep, true_kinetic_energy);
	i_arm->n_middle_layer_hits++;
      }
      else {//if ( (i_arm->total_thin_edep>i_arm->thin_threshold && i_arm->total_thick_edep>i_arm->thick_threshold && i_arm->total_third_edep<i_arm->third_threshold)
	//		|| (i_arm->total_thin_edep>i_arm->thin_threshold && i_arm->total_thick_edep>i_arm->thick_threshold && i_arm->total_third_edep>i_arm->third_threshold) ) {
	i_arm->middle_layer_response->Miss(true_kinetic_energy);
	i_arm->n_middle_layer_misses++;
      }

      // Fill a bunch of plots
      if (i_arm->total_thin_edep>i_arm->thin_threshold && i_arm->total_thick_edep>i_arm->thick_threshold) {
	if (i_arm->thick_stopped == true) {
	  i_arm->hEvdE_stopped->Fill(observed_E, i_arm->total_thin_edep);
	  i_arm->hSingleDetAxes_TwoLayer_stopped->Fill(i_arm->total_thick_edep, i_arm->total_thin_edep);
	}
	if (i_arm->third_stopped == true) {
	  i_arm->hEvdE_ThreeLayer_stopped->Fill(observed_E+i_arm->total_third_edep, i_arm->total_thin_edep+i_arm->total_thick_edep);
	  i_arm->hSingleDetAxes_ThreeLayer_12_stopped->Fill(i_arm->total_thick_edep, i_arm->total_thin_edep);
	  i_arm->hSingleDetAxes_ThreeLayer_23_stopped->Fill(i_arm->total_third_edep, i_arm->total_thick_edep);
	  i_arm->hSingleDetAxes_ThreeLayer_13_stopped->Fill(i_arm->total_third_edep, i_arm->total_thin_edep);
	}
	if (i_arm->thick_stopped == false && i_arm->third_stopped == false) {
	  i_arm->hEvdE_notstopped->Fill(observed_E, i_arm->total_thin_edep);
	  i_arm->hEvdE_ThreeLayer_notstopped->Fill(observed_E+i_arm->total_third_edep, i_arm->total_thin_edep+i_arm->total_thick_edep);
	  i_arm->hSingleDetAxes_TwoLayer_notstopped->Fill(i_arm->total_thick_edep, i_arm->total_thin_edep);
	  i_arm->hSingleDetAxes_ThreeLayer_12_notstopped->Fill(i_arm->total_thick_edep, i_arm->total_thin_edep);
	  i_arm->hSingleDetAxes_ThreeLayer_23_notstopped->Fill(i_arm->total_third_edep, i_arm->total_thick_edep);
	  i_arm->hSingleDetAxes_ThreeLayer_13_notstopped->Fill(i_arm->total_third_edep, i_arm->total_thin_edep);
	}
      }
    }
      /*      else if (!i_arm->thin_exists && i_arm->thick_exists && !i_arm->third_exists) { // only one layer
	double observed_E = i_arm->total_thick_edep;
	if (i_arm->total_thick_edep>i_arm->thick_threshold) {
	  i_arm->two_layer_response->Fill(observed_E, true_kinetic_energy); // this arm saw the proton
	  i_arm->n_hits++;
	}
	else {
	  i_arm->two_layer_response->Miss(true_kinetic_energy); // otherwise it didn't
	  i_arm->n_misses++;
	}
      }
      */
      /*      if ( observed_E < 0.5*true_kinetic_energy ) {
	std::cout << "Entry #" << iEntry << std::endl;
	for (std::vector<LayerHit*>::iterator i_layer_hit = layer_hits.begin(); i_layer_hit != layer_hits.end(); ++i_layer_hit) {
	  std::cout << "Hit i: arm = " << (*i_layer_hit)->arm << ", layer = " << (*i_layer_hit)->layerID << ", track ID = " << (*i_layer_hit)->track_ID << ", time = " << (*i_layer_hit)->time << ", ovol = " << (*i_layer_hit)->ovolName << ", edep = " << (*i_layer_hit)->edep << ", volNames = ";
	  for (int i_vol = 0; i_vol < (*i_layer_hit)->volNames.size(); ++i_vol) {
	    std::cout << (*i_layer_hit)->volNames.at(i_vol) << " (" << (*i_layer_hit)->particleNames.at(i_vol) << ", " << (*i_layer_hit)->edeps.at(i_vol) << ") ";
	  }
	  std::cout << std::endl;
	}
      }
      */

    for (std::vector<LayerHit*>::iterator i_layer_hit = layer_hits.begin(); i_layer_hit != layer_hits.end(); ++i_layer_hit) {
      delete *i_layer_hit;
    }
  }
  
  TFile* out_file = new TFile(outfilename.c_str(), "RECREATE");
  for (std::vector<Arm>::iterator i_arm = arms.begin(); i_arm != arms.end(); ++i_arm) {
    TH2* hResponse = i_arm->two_layer_response->Hresponse();

    hResponse->SetXTitle("Observed E [keV]");
    hResponse->SetYTitle("True E [keV]");
    i_arm->two_layer_response->Write();
    i_arm->three_layer_response->Write();
    i_arm->middle_layer_response->Write();
    std::cout << i_arm->detname << ": two layer: " << i_arm->n_two_layer_hits << " hits and " << i_arm->n_two_layer_misses << " misses" << std::endl;
    std::cout << i_arm->detname << ": three layer: " << i_arm->n_three_layer_hits << " hits and " << i_arm->n_three_layer_misses << " misses" << std::endl;
    std::cout << i_arm->detname << ": middle layer: " << i_arm->n_middle_layer_hits << " hits and " << i_arm->n_middle_layer_misses << " misses" << std::endl;

    i_arm->hEvdE_stopped->Write();
    i_arm->hEvdE_notstopped->Write();
    i_arm->hEvdE_ThreeLayer_stopped->Write();
    i_arm->hEvdE_ThreeLayer_notstopped->Write();

    i_arm->hSingleDetAxes_TwoLayer_stopped->Write();
    i_arm->hSingleDetAxes_ThreeLayer_12_stopped->Write();
    i_arm->hSingleDetAxes_ThreeLayer_23_stopped->Write();
    i_arm->hSingleDetAxes_ThreeLayer_13_stopped->Write();
    i_arm->hSingleDetAxes_TwoLayer_notstopped->Write();
    i_arm->hSingleDetAxes_ThreeLayer_12_notstopped->Write();
    i_arm->hSingleDetAxes_ThreeLayer_23_notstopped->Write();
    i_arm->hSingleDetAxes_ThreeLayer_13_notstopped->Write();
    
    int n_channels = i_arm->resolution_hists.size();
    for (int i_channel = 0; i_channel < n_channels; ++i_channel) {
      (i_arm->resolution_hists).at(i_channel)->Write();
    }
  }
  out_file->Close();
}
