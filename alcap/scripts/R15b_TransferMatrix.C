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
  int thin_channel;
  double total_thick_edep;
  bool thick_stopped;
  double total_third_edep;
  bool third_stopped;

  TH2F* hEvdE_TwoLayer_stopped;
  TH2F* hEvdE_TwoLayer_notstopped;
  TH2F* hEvdE_ThreeLayer_stopped;
  TH2F* hEvdE_ThreeLayer_notstopped;
  TH2F* hSingleDetAxes_TwoLayer_stopped;
  TH2F* hSingleDetAxes_TwoLayer_notstopped;
  TH2F* hSingleDetAxes_ThreeLayer12_stopped;
  TH2F* hSingleDetAxes_ThreeLayer12_notstopped;
  TH2F* hSingleDetAxes_ThreeLayer23_stopped;
  TH2F* hSingleDetAxes_ThreeLayer23_notstopped;
  TH2F* hSingleDetAxes_ThreeLayer13_stopped;
  TH2F* hSingleDetAxes_ThreeLayer13_notstopped;

  std::vector<TF1*> thin_energy_resolutions;
  std::vector<TH1F*> resolution_hists;
  std::vector<double> thin_thresholds;
  double thin_saturation_energy;
  double thin_k_factor;
  TF1* thick_energy_resolution;
  double thick_k_factor;
  double thick_threshold;
  double thick_saturation_energy;
  TF1* third_energy_resolution;
  double third_k_factor;
  double third_threshold;
  double third_saturation_energy;

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
  std::string particleName;
  std::vector<std::string> volNames;
  std::vector<double> edeps;
  std::vector<std::string> particleNames;
  int stopped;
  int channel;
};
  

void R15b_TransferMatrix(std::string filename, std::string outfilename, std::string particle, std::string tgtvolname, double max_axis, std::vector<Arm>& arms, int n_entries = -1) {

  int debug = 0; // 0, 1 or 2
 
  
  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  double u = 931.494*1000; // convert to keV
  double electron_mass = 5.48579909070e-4*u;
  double proton_mass = 1.007276466879*u; // convert to keV
  double deuteron_mass = 2.01410178*u;
  double triton_mass = 3.0160492*u;
  double alpha_mass = 4.001506179125*u;
  double muon_mass = 0.1134289259*u;
  double mass = 0;
  if (particle == "proton") {
    mass = proton_mass;
  }
  else if (particle == "deuteron") {
    mass = deuteron_mass;
  }
  else if (particle == "triton") {
    mass = triton_mass;
  }
  else if (particle == "alpha") {
    mass = alpha_mass;
  }
  else if (particle == "electron") {
    mass = electron_mass;
  }
  else if (particle == "muon") {
    mass = muon_mass;
  }
  else {
    std::cout << "Error: Particle " << particle << " not supported" << std::endl;
    return;
  }

  double bin_width = 100;
  //  double energy_high = 25000;
  //  double energy_low = 0;
  //  int n_bins = (energy_high - energy_low) / bin_width;
  double observed_energy_low = 0;
  double observed_energy_high = max_axis;
  int n_observed_bins = (observed_energy_high - observed_energy_low) / bin_width;
  double true_energy_low = 0;
  double true_energy_high = max_axis;
  //  double true_energy_low = 45000; // for electrons
  //  double true_energy_high = 55000;
  int n_true_bins = (true_energy_high - true_energy_low) / bin_width;


  for (std::vector<Arm>::iterator i_arm = arms.begin(); i_arm != arms.end(); ++i_arm) {
    // Set-up responses
    if ( (*i_arm).two_layer_response == NULL) {
      std::string responsename = (*i_arm).detname + "_two_layer_response";
      (*i_arm).two_layer_response = new RooUnfoldResponse(n_observed_bins,observed_energy_low,observed_energy_high, n_true_bins,true_energy_low,true_energy_high, responsename.c_str());
      (*i_arm).n_two_layer_hits = 0;
      (*i_arm).n_two_layer_misses = 0;
    }
    if ( (*i_arm).three_layer_response == NULL) {
      std::string responsename = (*i_arm).detname + "_three_layer_response";
      (*i_arm).three_layer_response = new RooUnfoldResponse(n_observed_bins,observed_energy_low,observed_energy_high, n_true_bins,true_energy_low,true_energy_high, responsename.c_str());
      (*i_arm).n_three_layer_hits = 0;
      (*i_arm).n_three_layer_misses = 0;
    }
    if ( (*i_arm).middle_layer_response == NULL) {
      std::string responsename = (*i_arm).detname + "_middle_layer_response";
      (*i_arm).middle_layer_response = new RooUnfoldResponse(n_observed_bins,observed_energy_low,observed_energy_high, n_true_bins,true_energy_low,true_energy_high,responsename.c_str());
      (*i_arm).n_middle_layer_hits = 0;
      (*i_arm).n_middle_layer_misses = 0;
    }
    // Set-up MC truth histograms
    double bin_width = 50;
    double total_energy_low = observed_energy_low;
    double total_energy_high = observed_energy_high;
    int n_total_bins = (total_energy_high - total_energy_low) / bin_width;
    double thin_energy_low = observed_energy_low;;
    double thin_energy_high = observed_energy_high;
    int n_thin_bins = (thin_energy_high - thin_energy_low) / bin_width;

    std::string histname = (*i_arm).detname + "_hEvdE_TwoLayer_stopped";
    (*i_arm).hEvdE_TwoLayer_stopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_thin_bins,thin_energy_low,thin_energy_high);
    (*i_arm).hEvdE_TwoLayer_stopped->SetXTitle("E_{1} + E_{2} [keV]");
    (*i_arm).hEvdE_TwoLayer_stopped->SetYTitle("E_{1} [keV]");
    histname = (*i_arm).detname + "_hEvdE_TwoLayer_notstopped";
    (*i_arm).hEvdE_TwoLayer_notstopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_thin_bins,thin_energy_low,thin_energy_high);
    (*i_arm).hEvdE_TwoLayer_notstopped->SetXTitle("E_{1} + E_{2} [keV]");
    (*i_arm).hEvdE_TwoLayer_notstopped->SetYTitle("E_{1} [keV]");

    histname = (*i_arm).detname + "_EvdE_ThreeLayer_stopped";
    (*i_arm).hEvdE_ThreeLayer_stopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_total_bins,total_energy_low,total_energy_high);
    (*i_arm).hEvdE_ThreeLayer_stopped->SetXTitle("E_{1} + E_{2} + E_{3} [keV]");
    (*i_arm).hEvdE_ThreeLayer_stopped->SetYTitle("E_{1} + E_{2} [keV]");
    histname = (*i_arm).detname + "_EvdE_ThreeLayer_notstopped";
    (*i_arm).hEvdE_ThreeLayer_notstopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_total_bins,total_energy_low,total_energy_high);
    (*i_arm).hEvdE_ThreeLayer_notstopped->SetXTitle("E_{1} + E_{2} + E_{3} [keV]");
    (*i_arm).hEvdE_ThreeLayer_notstopped->SetYTitle("E_{1} + E_{2} [keV]");

    histname = (*i_arm).detname + "_SingleDetAxes_TwoLayer_stopped";
    (*i_arm).hSingleDetAxes_TwoLayer_stopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_thin_bins,thin_energy_low,thin_energy_high);
    (*i_arm).hSingleDetAxes_TwoLayer_stopped->SetXTitle("E_{2} [keV]");
    (*i_arm).hSingleDetAxes_TwoLayer_stopped->SetYTitle("E_{1} [keV]");
    histname = (*i_arm).detname + "_SingleDetAxes_TwoLayer_notstopped";
    (*i_arm).hSingleDetAxes_TwoLayer_notstopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_thin_bins,thin_energy_low,thin_energy_high);
    (*i_arm).hSingleDetAxes_TwoLayer_notstopped->SetXTitle("E_{2} [keV]");
    (*i_arm).hSingleDetAxes_TwoLayer_notstopped->SetYTitle("E_{1} [keV]");
    histname = (*i_arm).detname + "_SingleDetAxes_ThreeLayer12_stopped";
    (*i_arm).hSingleDetAxes_ThreeLayer12_stopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_total_bins,total_energy_low,total_energy_high);
    (*i_arm).hSingleDetAxes_ThreeLayer12_stopped->SetXTitle("E_{2} [keV]");
    (*i_arm).hSingleDetAxes_ThreeLayer12_stopped->SetYTitle("E_{1} [keV]");
    histname = (*i_arm).detname + "_SingleDetAxes_ThreeLayer12_notstopped";
    (*i_arm).hSingleDetAxes_ThreeLayer12_notstopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_total_bins,total_energy_low,total_energy_high);
    (*i_arm).hSingleDetAxes_ThreeLayer12_notstopped->SetXTitle("E_{2} [keV]");
    (*i_arm).hSingleDetAxes_ThreeLayer12_notstopped->SetYTitle("E_{1} [keV]");
    histname = (*i_arm).detname + "_SingleDetAxes_ThreeLayer23_stopped";
    (*i_arm).hSingleDetAxes_ThreeLayer23_stopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_total_bins,total_energy_low,total_energy_high);
    (*i_arm).hSingleDetAxes_ThreeLayer23_stopped->SetXTitle("E_{3} [keV]");
    (*i_arm).hSingleDetAxes_ThreeLayer23_stopped->SetYTitle("E_{2} [keV]");
    histname = (*i_arm).detname + "_SingleDetAxes_ThreeLayer23_notstopped";
    (*i_arm).hSingleDetAxes_ThreeLayer23_notstopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_total_bins,total_energy_low,total_energy_high);
    (*i_arm).hSingleDetAxes_ThreeLayer23_notstopped->SetXTitle("E_{3} [keV]");
    (*i_arm).hSingleDetAxes_ThreeLayer23_notstopped->SetYTitle("E_{2} [keV]");
    histname = (*i_arm).detname + "_SingleDetAxes_ThreeLayer13_stopped";
    (*i_arm).hSingleDetAxes_ThreeLayer13_stopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_total_bins,total_energy_low,total_energy_high);
    (*i_arm).hSingleDetAxes_ThreeLayer13_stopped->SetXTitle("E_{3} [keV]");
    (*i_arm).hSingleDetAxes_ThreeLayer13_stopped->SetYTitle("E_{1} [keV]");
    histname = (*i_arm).detname + "_SingleDetAxes_ThreeLayer13_notstopped";
    (*i_arm).hSingleDetAxes_ThreeLayer13_notstopped = new TH2F(histname.c_str(), histname.c_str(), n_total_bins,total_energy_low,total_energy_high, n_total_bins,total_energy_low,total_energy_high);
    (*i_arm).hSingleDetAxes_ThreeLayer13_notstopped->SetXTitle("E_{3} [keV]");
    (*i_arm).hSingleDetAxes_ThreeLayer13_notstopped->SetYTitle("E_{1} [keV]");
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

  if (n_entries < 0) {
    n_entries = tree->GetEntries();
  }
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

    if (debug>=2) {
      std::cout << "Entry #" << iEntry << ": true kinetic energy = " << true_kinetic_energy << std::endl;
    }
    bool didnt_start_in_target = false;
    for (size_t iElement = 0; iElement < particleName->size();  ++iElement) {

      std::string i_particleName = particleName->at(iElement);
      std::string i_volName = volName->at(iElement);
      std::string i_ovolName = ovolName->at(iElement);
      int i_tid = tid->at(iElement);
      int i_volID = volID->at(iElement);

      //      std::cout << i_particleName << " " << i_volName << " " << i_ovolName << " " << t->at(iElement) << " " << edep->at(iElement)*1e6 << std::endl;
      if (i_tid == 1 && i_ovolName.find(tgtvolname)==std::string::npos) { // only want events that started in the target
	//	std::cout << "Actually started in " << i_ovolName << std::endl;
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

	LayerHit* layer_hit = 0;
	if (i_volName == thick_monname && i_arm->thick_exists){ // && stopped->at(iElement) == 1) {
	  layer_hit = new LayerHit();
	  layer_hit->layerID = "thick";
	  layer_hit->edep = (edep->at(iElement)*1e6 + i_arm->thick_energy_resolution->GetRandom())*i_arm->thick_k_factor; // convert to keV and smear
	}
	else if (i_volName == thin_monname && i_arm->thin_exists) {
	  layer_hit = new LayerHit();
	  layer_hit->layerID = "thin";
	  double smear = (i_arm->thin_energy_resolutions).at(i_volID)->GetRandom();
	  (i_arm->resolution_hists).at(i_volID)->Fill(smear);
	  layer_hit->edep = (edep->at(iElement)*1e6 + smear)*i_arm->thin_k_factor; // convert to keV
	}
	else if (i_volName == third_monname && i_arm->third_exists){ // && stopped->at(iElement) == 1) {
	  layer_hit = new LayerHit();
	  layer_hit->layerID = "third";
	  layer_hit->edep = (edep->at(iElement)*1e6 + i_arm->third_energy_resolution->GetRandom())*i_arm->third_k_factor; // convert to keV and smear
	}

	if (layer_hit) {
	  if (layer_hit->edep<0) {
	    layer_hit->edep = 0;
	  }
	  layer_hit->arm = i_arm->detname;
	  layer_hit->track_ID = i_tid;
	  layer_hit->time = t->at(iElement);
	  layer_hit->ovolName = i_ovolName;
	  layer_hit->particleName = i_particleName;
	  for (size_t jElement = 0; jElement < particleName->size(); ++jElement) {
	    layer_hit->volNames.push_back(volName->at(jElement));
	    layer_hit->edeps.push_back(edep->at(jElement)*1e6);
	    layer_hit->particleNames.push_back(particleName->at(jElement));
	  }
	  layer_hit->stopped = stopped->at(iElement);
	  layer_hit->channel = i_volID;
	  //	  std::cout << "** AE: " << layer_hit->edep << " " << layer_hit->layerID << std::endl;
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
      i_arm->thin_channel = -1;
      i_arm->thick_stopped = false;
      i_arm->third_stopped = false;
      for (std::vector<LayerHit*>::iterator i_layer_hit = layer_hits.begin(); i_layer_hit != layer_hits.end(); ++i_layer_hit) {
	if (i_arm->detname == (*i_layer_hit)->arm) {
	  if (debug>=2) {
	    std::cout << i_arm->detname << " " << (*i_layer_hit)->layerID << " Ch" << (*i_layer_hit)->channel << " tid " << (*i_layer_hit)->track_ID << " " << (*i_layer_hit)->edep << " keV " << (*i_layer_hit)->time << " ns " << (*i_layer_hit)->ovolName << " " << (*i_layer_hit)->particleName << std::endl;
	  }
	  if ( (*i_layer_hit)->layerID == "thin") {
	    i_arm->total_thin_edep += (*i_layer_hit)->edep;
	    if (i_arm->thin_channel == -1) {
	      i_arm->thin_channel = (*i_layer_hit)->channel;
	    }
	    else if (i_arm->thin_channel != (*i_layer_hit)->channel) {
	      std::cout << "WARNING: i_arm->thin_channel is already set to " << i_arm->thin_channel << " and been asked to reset to " << (*i_layer_hit)->channel << std::endl;
	    }
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
      
      double E1 = i_arm->total_thin_edep; double E2 = i_arm->total_thick_edep; double E3 = i_arm->total_third_edep;
      double thin_threshold = 0;
      if (i_arm->thin_channel >= 0) {
	thin_threshold = i_arm->thin_thresholds.at(i_arm->thin_channel);
      }
      if (i_arm->thin_saturation_energy>0 && E1 > i_arm->thin_saturation_energy) {
	E1 = i_arm->thin_saturation_energy;
      }
      if (i_arm->thick_saturation_energy>0 && E2 > i_arm->thick_saturation_energy) {
	E2 = i_arm->thick_saturation_energy;
      }
      if (i_arm->third_saturation_energy>0 && E3 > i_arm->third_saturation_energy) {
	E3 = i_arm->third_saturation_energy;
      }

      double E12 = E1 + E2; double E123 = E1 + E2 + E3;
      if ( (debug==1 && E123>0) || debug>=2) {
	std::cout << "Entry #" << iEntry << ": true kinetic energy = " << true_kinetic_energy << std::endl;
	for (std::vector<LayerHit*>::iterator i_layer_hit = layer_hits.begin(); i_layer_hit != layer_hits.end(); ++i_layer_hit) {
	  std::cout << (*i_layer_hit)->arm << " " << (*i_layer_hit)->layerID << " tid: " << (*i_layer_hit)->track_ID << " edep: " << (*i_layer_hit)->edep << " keV t: " << (*i_layer_hit)->time << " ns ovol:" << (*i_layer_hit)->ovolName << " stopped? " << (*i_layer_hit)->stopped << std::endl;
	}
	std::cout << i_arm->detname << " E1 = " << E1 << " keV, E2 = " << E2 << " keV, E3 = " << E3 << " keV" << std::endl;
	std::cout << i_arm->detname << " E1_th = " << thin_threshold << " keV, E2_th = " << i_arm->thick_threshold << " keV, E3_th = " << i_arm->third_threshold << " keV" << std::endl;
      }
      bool three_layer_hit = false;

      // Two layer arm
      if (i_arm->thin_exists && i_arm->thick_exists && !i_arm->third_exists) {
	if (E1>thin_threshold && E2>i_arm->thick_threshold) {
	  if (i_arm->thick_stopped) {
	    if ( (debug==1 && E123>0) || debug>=2) {
	      std::cout << i_arm->detname <<  " Two Layer Hit" << std::endl;
	    }
	    i_arm->two_layer_response->Fill(E12, true_kinetic_energy); // this arm saw the proton
	    //	i_arm->two_layer_response->Fill(true_kinetic_energy, true_kinetic_energy - E12); // this arm saw the proton
	    i_arm->n_two_layer_hits++;
	    i_arm->hEvdE_TwoLayer_stopped->Fill(E12, E1);
	    i_arm->hSingleDetAxes_TwoLayer_stopped->Fill(E2, E1);
	  }
	  else { // didn't stop in thick layer
	    if ( (debug==1 && E123>0) || debug>=2) {
	      std::cout << i_arm->detname <<  " Two Layer Miss" << std::endl;
	    }
	    i_arm->two_layer_response->Miss(true_kinetic_energy); // otherwise it didn't
	    i_arm->n_two_layer_misses++;
	    i_arm->hEvdE_TwoLayer_notstopped->Fill(E12, E1);
	    i_arm->hSingleDetAxes_TwoLayer_notstopped->Fill(E2, E1);
	  }
	}
	else { // didn't go above threshold
	  if ( (debug==1 && E123>0) || debug>=2) {
	    std::cout << i_arm->detname <<  " Two Layer Miss" << std::endl;
	  }
	  i_arm->two_layer_response->Miss(true_kinetic_energy); // otherwise it didn't
	  i_arm->n_two_layer_misses++;
	}
      }

      // Three layer arm
      if (i_arm->thin_exists && i_arm->thick_exists && i_arm->third_exists) {
	if (E1>thin_threshold && E2>i_arm->thick_threshold) {
	  if (i_arm->thick_stopped) {
	    if ( (debug==1 && E123>0) || debug>=2) {
	      std::cout << i_arm->detname <<  " Two Layer Hit" << std::endl;
	    }
	    i_arm->two_layer_response->Fill(E12, true_kinetic_energy); // this arm saw the proton
	    //	i_arm->two_layer_response->Fill(true_kinetic_energy, true_kinetic_energy - E12); // this arm saw the proton
	    i_arm->n_two_layer_hits++;
	    i_arm->hEvdE_TwoLayer_stopped->Fill(E12, E1);
	    i_arm->hSingleDetAxes_TwoLayer_stopped->Fill(E2, E1);
	  }
	  else { // didn't stop in thick
	    if ( (debug==1 && E123>0) || debug>=2) {
	      std::cout << i_arm->detname <<  " Two Layer Miss" << std::endl;
	    }
	    i_arm->two_layer_response->Miss(true_kinetic_energy); // otherwise it didn't
	    i_arm->n_two_layer_misses++;
	    i_arm->hEvdE_TwoLayer_notstopped->Fill(E12, E1);
	    i_arm->hSingleDetAxes_TwoLayer_notstopped->Fill(E2, E1);
	  }
	}
	else { // didn't go above threhold
	  if ( (debug==1 && E123>0) || debug>=2) {
	    std::cout << i_arm->detname <<  " Two Layer Miss" << std::endl;
	  }
	  i_arm->two_layer_response->Miss(true_kinetic_energy); // otherwise it didn't
	  i_arm->n_two_layer_misses++;
	}

	if (E1>thin_threshold && E2>i_arm->thick_threshold && E3>i_arm->third_threshold) {
	  if (i_arm->third_stopped) {
	    if ( (debug==1 && E123>0) || debug>=2) {
	      std::cout << i_arm->detname <<  " Three Layer Hit" << std::endl;
	    }
	    i_arm->three_layer_response->Fill(E123, true_kinetic_energy); // this arm saw the proton
	    i_arm->n_three_layer_hits++;
	    i_arm->hEvdE_ThreeLayer_stopped->Fill(E123, E12);
	    i_arm->hSingleDetAxes_ThreeLayer12_stopped->Fill(E2, E1);
	    i_arm->hSingleDetAxes_ThreeLayer23_stopped->Fill(E3, E2);
	    i_arm->hSingleDetAxes_ThreeLayer13_stopped->Fill(E3, E1);
	  }
	  else { // didn't stop in third
	    if ( (debug==1 && E123>0) || debug>=2) {
	      std::cout << i_arm->detname <<  " Three Layer Miss" << std::endl;
	    }
	    i_arm->three_layer_response->Miss(true_kinetic_energy);
	    i_arm->n_three_layer_misses++;
	    i_arm->hEvdE_ThreeLayer_notstopped->Fill(E123, E12);
	    i_arm->hSingleDetAxes_ThreeLayer12_notstopped->Fill(E2, E1);
	    i_arm->hSingleDetAxes_ThreeLayer23_notstopped->Fill(E3, E2);
	    i_arm->hSingleDetAxes_ThreeLayer13_notstopped->Fill(E3, E1);

	  }
	}
	else { // didn't go above threshold
	  if ( (debug==1 && E123>0) || debug>=2) {
	    std::cout << i_arm->detname <<  " Three Layer Miss" << std::endl;
	  }
	  i_arm->three_layer_response->Miss(true_kinetic_energy);
	  i_arm->n_three_layer_misses++;

	}
      }

      // Thick Layer only
      if (!i_arm->thin_exists && i_arm->thick_exists && !i_arm->third_exists) {
	if (E2>i_arm->thick_threshold) {
	  i_arm->middle_layer_response->Fill(E2, true_kinetic_energy);
	  i_arm->n_middle_layer_hits++;
	}
	else {//if ( (E1>thin_threshold && E2>i_arm->thick_threshold && E3<i_arm->third_threshold)
	  //		|| (E1>thin_threshold && E2>i_arm->thick_threshold && E3>i_arm->third_threshold) ) {
	  i_arm->middle_layer_response->Miss(true_kinetic_energy);
	  i_arm->n_middle_layer_misses++;
	}
      }
    }

    for (std::vector<LayerHit*>::iterator i_layer_hit = layer_hits.begin(); i_layer_hit != layer_hits.end(); ++i_layer_hit) {
      delete *i_layer_hit;
    }
    layer_hits.clear();
  }

  // Write everything to output
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

    i_arm->hEvdE_TwoLayer_stopped->Write();
    i_arm->hEvdE_TwoLayer_notstopped->Write();
    i_arm->hEvdE_ThreeLayer_stopped->Write();
    i_arm->hEvdE_ThreeLayer_notstopped->Write();

    i_arm->hSingleDetAxes_TwoLayer_stopped->Write();
    i_arm->hSingleDetAxes_ThreeLayer12_stopped->Write();
    i_arm->hSingleDetAxes_ThreeLayer23_stopped->Write();
    i_arm->hSingleDetAxes_ThreeLayer13_stopped->Write();
    i_arm->hSingleDetAxes_TwoLayer_notstopped->Write();
    i_arm->hSingleDetAxes_ThreeLayer12_notstopped->Write();
    i_arm->hSingleDetAxes_ThreeLayer23_notstopped->Write();
    i_arm->hSingleDetAxes_ThreeLayer13_notstopped->Write();
    
    int n_channels = i_arm->resolution_hists.size();
    for (int i_channel = 0; i_channel < n_channels; ++i_channel) {
      (i_arm->resolution_hists).at(i_channel)->Write();
    }
  }
  out_file->Close();
}
