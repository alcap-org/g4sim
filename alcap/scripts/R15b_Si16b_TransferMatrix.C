// NB this assumes you are working on a MC run where protons were the initial particle
// generated inside the target

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TSystem.h"

#include "RooUnfold.h"

#include <iostream>
#include <cmath>

extern TSystem* gSystem;

struct Arm {
  std::string detname;
  RooUnfoldResponse* response;
  int n_hits;
  int n_misses;
  double total_thin_edep;
  double total_thick_edep;
  bool thick_stopped;
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
  std::string filename = "output/R15b_10M_Geom-P5_protons_new.root";

  std::string outfilename = "R15b_Si16b_response-matrix_100keV-bins_new.root";
  
  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");
  
  double bin_width = 100;
  //  double energy_high = 25000;
  //  double energy_low = 0;
  //  int n_bins = (energy_high - energy_low) / bin_width;
  double observed_energy_high = 25000;
  double observed_energy_low = 0;
  int n_observed_bins = (observed_energy_high - observed_energy_low) / bin_width;
  double true_energy_high = 25000;
  double true_energy_low = 0;
  int n_true_bins = (true_energy_high - true_energy_low) / bin_width;


  std::vector<Arm> arms;
  Arm right_arm;
  Arm left_arm;
  right_arm.detname = "SiR";
  left_arm.detname = "SiL";
  arms.push_back(right_arm);
  //  arms.push_back(left_arm); // don't need SiL arm for Si16b analysis
  for (std::vector<Arm>::iterator i_arm = arms.begin(); i_arm != arms.end(); ++i_arm) {
    std::string responsename = (*i_arm).detname + "_response";
    (*i_arm).response = new RooUnfoldResponse(n_observed_bins,observed_energy_low,observed_energy_high, n_true_bins,true_energy_low,true_energy_high, responsename.c_str());
    (*i_arm).n_hits = 0;
    (*i_arm).n_misses = 0;
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

  double proton_mass = 938.272*1000; // convert to keV
  
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
    double total_energy = std::sqrt(squared_mom + proton_mass*proton_mass);
    true_kinetic_energy = total_energy - proton_mass;

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

      if (i_tid == 1 && i_ovolName.find("Target")==std::string::npos) { // only want events that started in the target
	//	std::cout << "ACtually started in " << i_ovolName << std::endl;
	didnt_start_in_target = true;
	break;
      }

      // Loop through the arms and record all energy deposits that have hit in any of the Si detectors
      for (std::vector<Arm>::iterator i_arm = arms.begin(); i_arm != arms.end(); ++i_arm) {
	std::string thick_monname = i_arm->detname+"2";
	std::string thin_monname = i_arm->detname+"1";

	if (i_volName == thick_monname){ // && stopped->at(iElement) == 1) {
	  LayerHit* layer_hit = new LayerHit();
	  layer_hit->layerID = "thick";
	  layer_hit->arm = i_arm->detname;
	  layer_hit->track_ID = i_tid;
	  layer_hit->time = t->at(iElement);
	  layer_hit->edep = edep->at(iElement)*1e6; // convert to keV
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
	  layer_hit->edep = edep->at(iElement)*1e6; // convert to keV
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
      i_arm->total_thin_edep = i_arm->total_thick_edep = 0;
      i_arm->thick_stopped = false;
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
	}
      }
    }

    for (std::vector<Arm>::iterator i_arm = arms.begin(); i_arm != arms.end(); ++i_arm) {
      double observed_E = i_arm->total_thin_edep + i_arm->total_thick_edep;
      if (observed_E > 0 && i_arm->thick_stopped == true) {
	i_arm->response->Fill(observed_E, true_kinetic_energy); // this arm saw the proton
	i_arm->n_hits++;
      }
      else {
	i_arm->response->Miss(true_kinetic_energy); // otherwise it didn't
	i_arm->n_misses++;
      }

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
    }

    for (std::vector<LayerHit*>::iterator i_layer_hit = layer_hits.begin(); i_layer_hit != layer_hits.end(); ++i_layer_hit) {
      delete *i_layer_hit;
    }
  }
  
  TFile* out_file = new TFile(outfilename.c_str(), "RECREATE");
  for (std::vector<Arm>::iterator i_arm = arms.begin(); i_arm != arms.end(); ++i_arm) {
    TH2* hResponse = i_arm->response->Hresponse();

    hResponse->SetXTitle("Observed E [keV]");
    hResponse->SetYTitle("True E [keV]");
    i_arm->response->Write();
    std::cout << i_arm->detname << ": " << i_arm->n_hits << " hits and " << i_arm->n_misses << " misses" << std::endl;
  }
  out_file->Close();
}
