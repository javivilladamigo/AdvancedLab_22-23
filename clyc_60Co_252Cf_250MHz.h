//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Nov 2615:30:48 2020 by ROOT version 6.09/01
// from TTree data/data
// found on file: correlated_tree.root
//////////////////////////////////////////////////////////

#ifndef clyc_60Co_252Cf_h
#define clyc_60Co_252Cf_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class clyc_60Co_252Cf {
public :
      TTree          *fChain;   // !pointer to the analyzed TTree or TChain
      Int_t           fCurrent; // !current Tree number in a TChain

   
      // Fixed size dimensions of array or collections stored in the TTree if any.

      // Declaration of leaf types

      Double_t    Timestamp_AMP;
      Double_t    Energy_AMP;
      Int_t       Samples_AMP[500];

      // List of branches
      TBranch     *b_Timestamp_AMP;   //!
      TBranch     *b_Energy_AMP;   //!
      TBranch     *b_Samples_AMP;   //!

      clyc_60Co_252Cf(TTree *tree=0);
      virtual ~clyc_60Co_252Cf();
      virtual Int_t     Cut(Long64_t entry);
      virtual Int_t     GetEntry(Long64_t entry);
      virtual Long64_t  LoadTree(Long64_t entry);
      virtual void      Init(TTree *tree);
      virtual void      Loop(Long64_t);
      virtual Bool_t    Notify();
      virtual void      Show(Long64_t entry = -1);
};

#endif

#ifdef clyc_60Co_252Cf_cxx
clyc_60Co_252Cf::clyc_60Co_252Cf(TTree *tree = 0) : fChain(0) 
{
   // if parameter tree is not specified (or zero), connect the file
   // used to generate this class and read the Tree.
   if (tree == 0) {
      //TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("correlated_tree_24.root");
      // if (!f || !f->IsOpen()) {
      //    f = new TFile("correlated_tree_24.root");

      // }
      TChain *dchain = new TChain("caenData_AMP","");

      dchain->Add("/Users/javi/Documents/Padova/2/AdvancedLab/AdvancedLab_22-23/data/UNFILTERED/CH_0@DT5725_1701_Data_60Co_252Cf_CLYC_ML_newOpticGrease.root");
      dchain->Add("/Users/javi/Documents/Padova/2/AdvancedLab/AdvancedLab_22-23/data/UNFILTERED/CH_2@DT5725_1701_Data_60Co_252Cf_CLYC_ML_newOpticGrease.root");

      tree = dchain;
   }
   Init(tree);
}

clyc_60Co_252Cf::~clyc_60Co_252Cf()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t clyc_60Co_252Cf::GetEntry(Long64_t entry)
{
   // Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t clyc_60Co_252Cf::LoadTree(Long64_t entry)
{
   // Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void clyc_60Co_252Cf::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Timestamp_AMP", &Timestamp_AMP, &b_Timestamp_AMP);
   fChain->SetBranchAddress("Energy_AMP", &Energy_AMP, &b_Energy_AMP);
   fChain->SetBranchAddress("Samples_AMP", &Samples_AMP, &b_Samples_AMP);

   Notify();
}

Bool_t clyc_60Co_252Cf::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void clyc_60Co_252Cf::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t clyc_60Co_252Cf::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef clyc_60Co_252Cf_cxx
