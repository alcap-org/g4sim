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
  TH2F* hEvdE;
  std::vector<double> thin_energy_thresholds;
  double thick_energy_threshold;
  std::vector<TF1*> thin_energy_resolutions;
  TF1* thick_energy_resolution;

  double total_thin_edep;
  double total_thick_edep;

  TH1F* hThinEnergy_noVeto;
  TH1F* hThinEnergy_Veto;
  TH1F* hThickEnergy_noVeto;
  TH1F* hThickEnergy_Veto;
  TH1F* hBeamPosition;
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
  int volID;
  int stopped;
  std::string particleName;
  double ekin;
  std::string oprocess;
};
  
bool operator<(const LayerHit& lhs, const LayerHit& rhs) {
  return (lhs.time < rhs.time);
}

void R15b_EvdEPlot(std::string filename, std::vector<Arm>& arms) {

  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");
  
  double bin_width = 10;//100;
  //  double energy_high = 25000;
  //  double energy_low = 0;
  //  int n_bins = (energy_high - energy_low) / bin_width;
  double total_energy_high = 10000;
  double total_energy_low = 0;
  int n_total_bins = (total_energy_high - total_energy_low) / bin_width;
  double thin_energy_high = 10000;
  double thin_energy_low = 0;
  int n_thin_bins = (thin_energy_high - thin_energy_low) / bin_width;


  for (std::vector<Arm>::iterator i_arm = arms.begin(); i_arm != arms.end(); ++i_arm) {
    std::string histname = "hEvdE_" + (*i_arm).detname;
    (*i_arm).hEvdE = new TH2F(histname.c_str(), "", n_total_bins,total_energy_low,total_energy_high, n_thin_bins,thin_energy_low,thin_energy_high);
    (*i_arm).hEvdE->SetMarkerStyle(kFullDotMedium);
    (*i_arm).hEvdE->SetXTitle("E_{1} + E_{2} [keV]");
    (*i_arm).hEvdE->SetYTitle("E_{1} [keV]");
    (*i_arm).hEvdE->SetStats(true);

    histname = "hThickEnergy_noVeto_" + (*i_arm).detname;
    (*i_arm).hThickEnergy_noVeto = new TH1F(histname.c_str(), "", n_total_bins,total_energy_low,total_energy_high);
    (*i_arm).hThickEnergy_noVeto->SetXTitle("E_{1} [keV]");
    (*i_arm).hThickEnergy_noVeto->SetStats(true);

    histname = "hThickEnergy_Veto_" + (*i_arm).detname;
    (*i_arm).hThickEnergy_Veto = new TH1F(histname.c_str(), "", n_total_bins,total_energy_low,total_energy_high);
    (*i_arm).hThickEnergy_Veto->SetXTitle("E_{1} [keV]");
    (*i_arm).hThickEnergy_Veto->SetStats(true);

    histname = "hThinEnergy_noVeto_" + (*i_arm).detname;
    (*i_arm).hThinEnergy_noVeto = new TH1F(histname.c_str(), "", n_thin_bins,thin_energy_low,thin_energy_high);
    (*i_arm).hThinEnergy_noVeto->SetXTitle("E_{1} [keV]");
    (*i_arm).hThinEnergy_noVeto->SetStats(true);
    
    histname = "hThinEnergy_Veto_" + (*i_arm).detname;
    (*i_arm).hThinEnergy_Veto = new TH1F(histname.c_str(), "", n_thin_bins,thin_energy_low,thin_energy_high);
    (*i_arm).hThinEnergy_Veto->SetXTitle("E_{1} [keV]");
    (*i_arm).hThinEnergy_Veto->SetStats(true);

    histname = "hBeamPosition_" + (*i_arm).detname;
    int n_bins = (*i_arm).thin_energy_resolutions.size();
    (*i_arm).hBeamPosition = new TH1F(histname.c_str(), "", n_bins,1,n_bins+1);
    (*i_arm).hBeamPosition->SetXTitle("Strip Number");
    (*i_arm).hBeamPosition->SetStats(true);
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
  std::vector<double>* ekin = 0;

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
  TBranch* br_ekin = tree->GetBranch("M_ekin");
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
  br_ekin->SetAddress(&ekin);
  br_oprocess->SetAddress(&oprocess);
  br_volID->SetAddress(&volID);

  int n_entries = tree->GetEntries();
  for (int iEntry = 0; iEntry < n_entries; ++iEntry) {
    tree->GetEvent(iEntry);
      
    if (iEntry % 10000 == 0) {
      std::cout << iEntry << " / " << n_entries << std::endl;
    }

    std::vector<LayerHit> layer_hits;
    for (int iElement = 0; iElement < particleName->size();  ++iElement) {

      std::string i_particleName = particleName->at(iElement);
      //      if (i_particleName != particle_name) { // only want protons
      //      	//	std::cout << i_particleName << " " << tid->at(iElement) << " " << edep->at(iElement)*1e6 << " " << t->at(iElement) <<std::endl;
      //      	continue;
      //      }
      std::string i_volName = volName->at(iElement);
      std::string i_ovolName = ovolName->at(iElement);
      double i_edep = edep->at(iElement)*1e6; // convert to keV
      int i_volID = volID->at(iElement);
      int i_stopped = stopped->at(iElement);
      double i_ekin = ekin->at(iElement)*1e6; // convert to keV
      // Loop through the arms and record all protons that have hit any of the Si detectors
      for (std::vector<Arm>::iterator i_arm = arms.begin(); i_arm != arms.end(); ++i_arm) {
	std::string thick_monname = i_arm->detname+"2";
	if (i_arm->detname=="SiL") {
	  thick_monname = i_arm->detname+"3";
	}
	std::string thin_monname = i_arm->detname+"1";
	if (i_arm->detname=="SiT") {
	  thin_monname = i_arm->detname;
	  thick_monname = "";
	}

	if (i_volName == thick_monname) {
	  LayerHit layer_hit;
	  layer_hit.layerID = "thick";
	  layer_hit.arm = i_arm->detname;
	  layer_hit.track_ID = tid->at(iElement);
	  layer_hit.time = t->at(iElement);
	  layer_hit.edep = i_edep;
	  layer_hit.ovolName = i_ovolName;
	  layer_hit.volID = i_volID;
	  layer_hit.stopped = i_stopped;
	  layer_hit.particleName = i_particleName;
	  layer_hit.ekin = i_ekin;
	  layer_hit.oprocess = oprocess->at(iElement);
	  layer_hits.push_back(layer_hit);

	  /*
	  if (layer_hit.oprocess == "muMinusCaptureAtRest") {
	    std::cout << i_arm->detname << " Thick Hit: " << layer_hit.particleName << " " << layer_hit.oprocess << " " << layer_hit.ovolName << ", " << layer_hit.time << " ns, " << layer_hit.edep << " keV" << std::endl;
	  }
	  */
	}
	else if (i_volName == thin_monname) {
	  LayerHit layer_hit;
	  layer_hit.layerID = "thin";
	  layer_hit.arm = i_arm->detname;
	  layer_hit.track_ID = tid->at(iElement);
	  layer_hit.time = t->at(iElement);
	  layer_hit.edep = i_edep;
	  layer_hit.ovolName = i_ovolName;
	  layer_hit.volID = i_volID;
	  layer_hit.stopped = i_stopped;
	  layer_hit.particleName = i_particleName;
	  layer_hit.ekin = i_ekin;
	  layer_hit.oprocess = oprocess->at(iElement);
	  layer_hits.push_back(layer_hit);

	  /*
	  if (layer_hit.oprocess == "muMinusCaptureAtRest") {
	    std::cout << i_arm->detname << " Thin Hit: " << layer_hit.particleName << " " << layer_hit.oprocess << " " << layer_hit.ovolName << ", " << layer_hit.time << " ns, " << layer_hit.edep << " keV" << std::endl;
	  }
	  */
	}
      }
    }

    // Time order the layer hits
    std::sort(layer_hits.begin(), layer_hits.end());
    
    // After looping through the elements, we will hopefully have a list of layer hits
    for (std::vector<Arm>::iterator i_arm = arms.begin(); i_arm != arms.end(); ++i_arm) {
      bool trackID_1_hit = false;
      bool thin_stopped = false;
      bool thick_stopped = false;
      double thin_time = -1;
      double thick_time = -1;
      std::string thin_particle;
      std::string thick_particle;
      std::string thin_ovol;
      std::string thick_ovol;
      std::string thin_oprocess;
      std::string thick_oprocess;
      double thin_ekin = -1;
      double thick_ekin = -1;
      i_arm->total_thin_edep = 0;
      i_arm->total_thick_edep = 0;
      int thin_volID = -1;
      // For our electronics, we have 5000 ns long pulse islands where we take the largest peak
      // We want to keep track of the edeps for all peaks and then choose after the fact
      std::vector<double> thin_edeps;
      std::vector<double> thick_edeps;
      std::vector<std::string> thin_particles;
      std::vector<std::string> thick_particles;
      std::vector<double> thin_times;
      std::vector<double> thick_times;
      std::vector<std::string> thin_oprocesses;
      std::vector<std::string> thick_oprocesses;
      std::vector<double> thin_ekins;
      std::vector<double> thick_ekins;
      std::vector<int> thin_volIDs;
      //      std::vector<int> thick_volIDs;
      double tpi_length = 5000;
      double merge_time = 1;
      //      std::cout << "N Layer Hits = " << layer_hits.size() << std::endl;
      for (std::vector<LayerHit>::iterator i_layer_hit = layer_hits.begin(); i_layer_hit != layer_hits.end(); ++i_layer_hit) {
	//	std::cout << i_layer_hit->arm << " " << i_arm->detname << std::endl;
	if (i_layer_hit->arm == i_arm->detname) {
	  if (i_layer_hit->track_ID == 1) {
	    trackID_1_hit = true;
	  }
	  
	  if (i_layer_hit->layerID == "thin") {
	    //	    std::cout << "Thin Time = " << thin_time << std::endl;
	    if (thin_time < 0) {
	      thin_time = i_layer_hit->time;
	      thin_volID = i_layer_hit->volID;
	      thin_particle = i_layer_hit->particleName;
	      thin_ovol = i_layer_hit->ovolName;
	      thin_oprocess = i_layer_hit->oprocess;
	      thin_ekin = i_layer_hit->ekin;
	      thin_volID = i_layer_hit->volID;

	      std::cout << "First hit (" << i_layer_hit->layerID << ": " << i_layer_hit->particleName << " " << i_layer_hit->edep << " keV, " << i_layer_hit->time << " ns " << i_layer_hit->ovolName << ", " << i_layer_hit->ekin << " kin keV" << std::endl;
	    }
	    else if (thin_time > tpi_length) {
	      continue;
	    }
	    if (std::fabs(thin_time - i_layer_hit->time) < merge_time) { // sum edeps with this time of first hit
	    //	    std::cout << "Entry #" << iEntry << ": " <<i_layer_hit->layerID << " " << i_layer_hit->arm << " " << i_layer_hit->track_ID << " " << i_layer_hit->edep << " keV, " << i_layer_hit->time << " ns, " << i_layer_hit->ovolName << std::endl;
	      std::cout << "Merging (" << i_layer_hit->layerID << ": " << i_layer_hit->particleName << " " << i_layer_hit->edep << " keV, " << i_layer_hit->time << " ns " << i_layer_hit->ovolName << ", " << i_layer_hit->ekin << " kin keV" << std::endl;;
	      i_arm->total_thin_edep += i_layer_hit->edep;
	    }
	    else {
	      thin_edeps.push_back(i_arm->total_thin_edep);
	      thin_particles.push_back(thin_particle);
	      thin_oprocesses.push_back(thin_oprocess);
	      thin_times.push_back(thin_time);
	      thin_ekins.push_back(thin_ekin);
	      thin_volIDs.push_back(thin_volID);
	      
	      thin_time = i_layer_hit->time;
	      thin_particle = i_layer_hit->particleName;
	      i_arm->total_thin_edep = i_layer_hit->edep;
	      thin_oprocess = i_layer_hit->oprocess;
	      thin_ekin = i_layer_hit->ekin;
	      thin_volID = i_layer_hit->volID;

	      std::cout << "New hit (" << i_layer_hit->layerID << ": " << i_layer_hit->particleName << " " << i_layer_hit->edep << " keV, " << i_layer_hit->time << " ns " << i_layer_hit->ovolName << ", " << i_layer_hit->ekin << " kin keV" << std::endl;;
	    }
	    if (i_layer_hit->stopped==1 && i_layer_hit->track_ID==1) {
	      thin_stopped = true;
	    }
	  }
	  if (i_layer_hit->layerID == "thick") {
	    if (thick_time < 0) {
	      thick_time = i_layer_hit->time;
	      thick_particle = i_layer_hit->particleName;
	      thick_ovol = i_layer_hit->ovolName;
	      thick_oprocess = i_layer_hit->oprocess;
	      thick_ekin = i_layer_hit->ekin;
	      std::cout << "First hit (" << i_layer_hit->layerID << ": " << i_layer_hit->particleName << " " << i_layer_hit->edep << " keV, " << i_layer_hit->time << " ns " << i_layer_hit->ovolName << ", " << i_layer_hit->ekin << " kin keV" << std::endl;
	    }
	    else if (thick_time > tpi_length) {
	      continue;
	    }
	    if (std::fabs(thick_time - i_layer_hit->time) < merge_time) { // sum edeps with this time of first hit
	      std::cout << "Merging (" << i_layer_hit->layerID << ": " << i_layer_hit->particleName << " " << i_layer_hit->edep << " keV, " << i_layer_hit->time << " ns " << i_layer_hit->ovolName << ", " << i_layer_hit->ekin << " kin keV" << std::endl;;
	      i_arm->total_thick_edep += i_layer_hit->edep;
	      //	      std::cout << "New Edep = " << i_arm->total_thick_edep << std::endl;
	    }
	    else {
	      thick_edeps.push_back(i_arm->total_thick_edep);
	      thick_particles.push_back(thick_particle);
	      thick_times.push_back(thick_time);
	      thick_oprocesses.push_back(thick_oprocess);
	      thick_ekins.push_back(thick_ekin);
	      
	      thick_time = i_layer_hit->time;
	      thick_particle = i_layer_hit->particleName;
	      i_arm->total_thick_edep = i_layer_hit->edep;
	      thick_oprocess = i_layer_hit->oprocess;
	      thick_ekin = i_layer_hit->ekin;
	      std::cout << "New hit (" << i_layer_hit->layerID << ": " << i_layer_hit->particleName << " " << i_layer_hit->edep << " keV, " << i_layer_hit->time << " ns " << i_layer_hit->ovolName << ", " << i_layer_hit->ekin << " kin keV" << std::endl;;
	    }

	    if (i_layer_hit->stopped==1 && i_layer_hit->track_ID==1) {
	      thick_stopped = true;
	    }
	  }
	}
      }

      //      std::cout << "N Thin = " << thin_edeps.size() << ", N Thick = " << thick_edeps.size() << std::endl;
      if (thin_edeps.size()>0) {
	size_t thin_element = std::max_element(thin_edeps.begin(), thin_edeps.end()) - thin_edeps.begin();
	i_arm->total_thin_edep = thin_edeps.at(thin_element);
	thin_particle = thin_particles.at(thin_element);
	thin_time = thin_times.at(thin_element);
	thin_oprocess = thin_oprocesses.at(thin_element);
	thin_ekin = thin_ekins.at(thin_element);
	thin_volID = thin_volIDs.at(thin_element);
	//	std::cout << "Chosen Thin: " << thin_particle << " " << thin_oprocess << " " << thin_time << " ns, " << i_arm->total_thin_edep << " keV" << std::endl;
	//	for (size_t i_element = 0; i_element < thin_edeps.size(); ++i_element) {
	//	  std::cout << thin_edeps.at(i_element) << ", " << thin_particles.at(i_element) << std::endl;
	//	}
      }
      if (thick_edeps.size()>0) {
	size_t thick_element = std::max_element(thick_edeps.begin(), thick_edeps.end()) - thick_edeps.begin();
	i_arm->total_thick_edep = thick_edeps.at(thick_element);
	thick_particle = thick_particles.at(thick_element);
	thick_time = thick_times.at(thick_element);
	thick_oprocess = thick_oprocesses.at(thick_element);
	thick_ekin = thick_ekins.at(thick_element);

	//	std::cout << "Chosen Thick: " << thick_particle << " " << thick_oprocess << " " << thick_time << " ns, " << i_arm->total_thick_edep << " keV" << std::endl;
	//	for (size_t i_element = 0; i_element < thick_edeps.size(); ++i_element) {
	//	  std::cout << thick_edeps.at(i_element) << " kev, " << thick_times.at(i_element) << " ns, " << thick_particles.at(i_element) << std::endl;
	//	}
      }
      //      if (trackID_1_hit == false) { // want at least one layer hit to be from the particle we wnat
      //      	continue;
      //      }
      
      bool thick_hit = false;
      bool thin_hit = false;
      if (i_arm->total_thick_edep > 0) {
	//	std::cout << "Total Thick: " << i_arm->total_thick_edep << std::endl;
	i_arm->total_thick_edep += i_arm->thick_energy_resolution->GetRandom();
	if(i_arm->total_thick_edep >= i_arm->thick_energy_threshold && std::fabs(thick_time)<10) {// && thick_stopped) {
	  thick_hit = true;
	}
      }
      if (i_arm->total_thin_edep > 0) {
	//	std::cout << "Total Thin: " << i_arm->total_thin_edep << std::endl;
	if (thin_volID>=0) {
	  i_arm->total_thin_edep += (i_arm->thin_energy_resolutions).at(thin_volID)->GetRandom();
	  if(i_arm->total_thin_edep >= (i_arm->thin_energy_thresholds).at(thin_volID) && std::fabs(thin_time)<10) {
	    thin_hit = true;
	    if (i_arm->total_thin_edep > 200 && i_arm->total_thin_edep<1200 && std::fabs(thin_time)<10) {
	      //	      std::cout << "Thin: " << thin_particle << " " << i_arm->total_thin_edep << " keV " << thin_oprocess << " " << thin_volID << std::endl;
	      i_arm->hBeamPosition->Fill(thin_volID+1);
	    }
	  }
	}
      }

      
      double thin_E = i_arm->total_thin_edep;
      double thick_E = i_arm->total_thick_edep;
      double total_E = thin_E + thick_E;

      if (thin_hit && thin_particle=="mu-") {
	//	if (i_arm->detname=="SiL") {
	std::cout << "** Thin " << i_arm->detname << ": " << thin_particle << " " << i_arm->total_thin_edep << " keV "<< thin_oprocess << " " << thin_ekin << " keV " << thin_time << " ns" << std::endl;
	//	}
	i_arm->hThinEnergy_noVeto->Fill(thin_E);
      }
      if (thick_hit && thick_particle=="mu-") {
	//	if (i_arm->detname=="SiL") {
	std::cout << "** Thick " << i_arm->detname << ": " << thick_particle << " " << i_arm->total_thick_edep << " keV "<< thick_oprocess << " " << thick_ekin << " keV " << thick_time << " ns" << std::endl;
	  //	}
	i_arm->hThickEnergy_noVeto->Fill(thick_E);
      }
      
      if (!thick_hit || !thin_hit) {
	if (thin_hit && !thick_hit) {
	  i_arm->hThinEnergy_Veto->Fill(thin_E);
	  //	  std::cout << "Thin: " << thin_particle << " " << i_arm->total_thin_edep << " keV " << thin_oprocess << " " << thin_ekin << " keV" << std::endl;
	}
	continue; // missing a hit in one layer
      }

      if (std::fabs(thin_time - thick_time) > 200) { // layer coincidence
	i_arm->hThinEnergy_Veto->Fill(thin_E);
	//	std::cout << "Thin: " << thin_particle << " " << i_arm->total_thin_edep << " keV " << thin_oprocess << " " << thin_ekin << " keV, Thick: " << thick_particle << " " << i_arm->total_thick_edep << " keV "<< thick_oprocess << " " << thick_ekin << " keV" << std::endl;
	continue;
      }

      //      if (!(thin_particle=="mu-" && thick_particle=="mu-")) { // require both muons
      //      if (!(thin_particle=="mu-" && thick_particle!="mu-")) { // require thin muons and thick anything but a muon
      //      if (!(thin_particle=="mu-" && thin_stopped==true)) { // require muon stop in thin
      //      if (!(thin_particle=="mu-" && thick_particle=="e-")) { // require thin muon and thick electron
      //      if (!(thin_particle=="mu-" && thick_particle=="proton")) { // require thin muon and thick proton
      //      if (!(thin_particle=="proton" && thick_particle=="proton")) { // require both hits are protons
      //      if (!(thin_particle=="e-" && thick_particle=="e-")) { // require both hits are protons
      //      if (!(thin_particle!="mu-" && thick_particle=="mu-")) { // require thick muon and thin anything but a muon
      //	continue;
      //      }
      /*
      if (thick_stopped != want_tgt_hit) { // if we don't have the thick stopped we want...
	if (!thin_stopped) { // ...check if we have stopped in the thin
	  continue;
	}
      }
      */
      //      if (i_arm->total_thick_edep>10000) {
      //	for (size_t i_element = 0; i_element < thick_edeps.size(); ++i_element) {
      //	  std::cout << thick_edeps.at(i_element) << ", " << thick_particles.at(i_element) << std::endl;
      //	}
      //      }
      //      std::cout << "Thin: " << thin_particle << " " << i_arm->total_thin_edep << " keV " << thin_oprocess << " " << thin_ekin << " keV, Thick: " << thick_particle << " " << i_arm->total_thick_edep << " keV "<< thick_oprocess << " " << thick_ekin << " keV" << std::endl;
      /*
      if ( (i_layer_hit->layerID != (*j_layer_hit)->layerID) && // want hits in opposite layers
	   (i_layer_hit->arm == (*j_layer_hit)->arm) && // want the same arm
	   (i_layer_hit->track_ID == (*j_layer_hit)->track_ID) //&& // want the same track ID
	   //	     (i_layer_hit->track_ID == 1)// && // and it's a primary proton
	   //	     (i_layer_hit->ovolName.find("Target")!=std::string::npos && (*j_layer_hit)->ovolName == i_layer_hit->ovolName) // want them to have originated in the target
	   ) {

	total_E = i_layer_hit->edep + (*j_layer_hit)->edep;
	if (i_layer_hit->layerID == "thin") {
	  thin_E = i_layer_hit->edep;
	}
	else {
	  thin_E = (*j_layer_hit)->edep;
	}
      }*/
      i_arm->hEvdE->Fill(total_E, thin_E);
      i_arm->hThickEnergy_Veto->Fill(thick_E);
    }

    // delete all the layer_hits
    layer_hits.clear();
  }

  for (std::vector<Arm>::iterator i_arm = arms.begin(); i_arm != arms.end(); ++i_arm) {	
    //    i_arm->hEvdE->Draw("COLZ");
    i_arm->hEvdE->SetDirectory(0);
    i_arm->hThinEnergy_Veto->SetDirectory(0);
    i_arm->hThinEnergy_noVeto->SetDirectory(0);
    i_arm->hThickEnergy_noVeto->SetDirectory(0);
    i_arm->hThickEnergy_Veto->SetDirectory(0);
    i_arm->hBeamPosition->SetDirectory(0);
  }
  file->Close();
}
