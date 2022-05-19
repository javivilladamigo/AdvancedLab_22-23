#define clycana_cxx
#include "clycana_all.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iomanip>

void clycana::Loop(Long64_t nbEntries = -1)
{
  //   In a ROOT session, you can do:
  //      root> .L clycana.C
  //      root> clycana t
  //      root> t.GetEntry(12); // Fill t data members with entry number 12
  //      root> t.Show();       // Show values of entry 12
  //      root> t.Show(16);     // Read and show values of entry 16
  //      root> t.Loop();       // Loop on all entries
  //

  //     This is the loop skeleton where:
  //    jentry is the global entry number in the chain
  //    ientry is the entry number in the current Tree
  //  Note that the argument to GetEntry must be:
  //    jentry for TChain::GetEntry
  //    ientry for TTree::GetEntry and TBranch::GetEntry
  //
  //       To read only selected branches, Insert statements like:
  // METHOD1:
  //    fChain->SetBranchStatus("*",0);  // disable all branches
  //    fChain->SetBranchStatus("branchname",1);  // activate branchname
  // METHOD2: replace line
  //    fChain->GetEntry(jentry);       //read all branches
  //by  b_branchname->GetEntry(ientry); //read only this branch
  if (fChain_PSA == 0) return;
 //  
 // TCanvas *C1 = new TCanvas("Canvas", "Canvas");
 // C1 -> Divide(1,2);
 // C1 -> cd(1);
  
  TFile *correlated_tree = new TFile("correlatedTree.root", "recreate");
  TH1D *signal=new TH1D("signal", "signal",5000,0,5000);
  Double_t AMP_amp=0; Int_t Samples_PSA[5000];
  Long64_t jentry_PSA=0;  Long64_t jentry_AMP=0; 
 

  TTree *data = new TTree("data", "data");
  data->Branch("Timestamp_AMP",&Timestamp_AMP,"TAC_amp/D");
  data->Branch("AMP_amp",   &AMP_amp   ,"AMP_amp/D");
  data->Branch("Samples_PSA", Samples_PSA  ,"Samples_PSA[5000]/I");
  data->SetAutoSave(10000);


  Long64_t nentries_AMP = fChain_AMP->GetEntriesFast();
  Long64_t nentries_PSA = fChain_PSA->GetEntriesFast();
  Long64_t nbytes_PSA = 0, nb_PSA = 0; Long64_t nbytes_AMP = 0, nb_AMP = 0; double previous_tsd;
  
  for (jentry_AMP = 0; jentry_AMP < nentries_AMP; jentry_AMP++) // main loop
  {
    for (int r = 3; r < 500; r++) Samples_PSA[r] = 0.;
    if (jentry_AMP % 3000==0 ) { cout << "percentage completed " << 100.*jentry_AMP/nentries_AMP << " %"<< endl<< std::setw(2) << std::setfill('0'); cout.flush(); }
      

    Long64_t ientry_AMP = LoadTree_AMP(jentry_AMP);
    if (ientry_AMP < 0) break;
    Long64_t ientry_PSA = LoadTree_PSA(jentry_PSA);

    nb_AMP = fChain_AMP->GetEntry(jentry_AMP);   nbytes_AMP += nb_AMP;
    AMP_amp = Energy_AMP;

    jentry_PSA = jentry_AMP;

    
    

    if (Energy_AMP>2000.)
    {
      fChain_AMP->GetEntry(jentry_AMP);
      previous_tsd = abs(Timestamp_AMP - Timestamp_PSA);

      while (abs(Timestamp_AMP - Timestamp_PSA)<1E23 && jentry_AMP <= nentries_AMP && abs(Timestamp_AMP - Timestamp_PSA) <= previous_tsd) // AMP forward search
      {
        previous_tsd = abs(Timestamp_AMP - Timestamp_PSA);

        if(abs(Timestamp_AMP - Timestamp_PSA)<1.1E6)
        {
          //AMP_amp = Energy_AMP; 
          cout << jentry_AMP << "   " << Timestamp_AMP << "   "  << AMP_amp << " AMP Time stamp difference forward = " << Timestamp_AMP - Timestamp_PSA << endl;
        }

        jentry_AMP += 1;
        jentry_AMP = LoadTree_AMP(jentry_AMP);
        fChain_AMP->GetEntry(jentry_AMP);
        nbytes_AMP += nb_AMP;
        
        if (jentry_AMP < 0) break;
      }
      
      data->Fill();
    }
  }
correlated_tree->Write();
//signal->Draw();
}