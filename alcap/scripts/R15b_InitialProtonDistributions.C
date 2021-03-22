void R16b_Si16b_InitialProtonDistributions(std::string filename) {

  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  

  tree->Draw("i_y:(sqrt(2)/2)*i_x+(-sqrt(2)/2)*i_z", "", "COLZ");
}
