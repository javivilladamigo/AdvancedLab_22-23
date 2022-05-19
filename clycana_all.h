//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Oct 30 10:27:35 2020 by ROOT version 6.09/01
// from TTree caenData/caenData
// found on file: CLYC_252Cf.root
//////////////////////////////////////////////////////////

#ifndef clycana_h
#define clycana_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <TCutG.h>
#include <TH1.h>
#include <TH2.h>
// Header file for the classes stored in the TTree if any.

class clycana {
public :
   TTree          *fChain_PSA;      //!pointer to the analyzed TTree or TChain
   //TTree          *fChain_TAC;    //!pointer to the analyzed TTree or TChain
   TTree          *fChain_AMP;      //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent_PSA;    //!current Tree number in a TChain
   //Int_t           fCurrent_TAC;  //!current Tree number in a TChain
   Int_t           fCurrent_AMP;    //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   //Double_t        Timestamp_TAC;
   //Int_t           Energy_TAC;
   //Int_t           Samples_TAC[5000];
   Double_t        Timestamp_AMP;
   Int_t           Energy_AMP;
   Int_t           Samples_AMP[5000];
   Double_t        Timestamp_PSA;
   Int_t           Energy_PSA;
   Int_t           Samples_PSA[5000];

   // List of branches
   //TBranch        *b_Timestamp_TAC;   //!
   //TBranch        *b_Energy_TAC;   //!
   //TBranch        *b_Samples_TAC;   //!
   TBranch        *b_Timestamp_AMP;   //!
   TBranch        *b_Energy_AMP;   //!
   TBranch        *b_Samples_AMP;   //!
   TBranch        *b_Timestamp_PSA;   //!
   TBranch        *b_Energy_PSA;   //!
   TBranch        *b_Samples_PSA;   //!

   clycana(TTree *tree_PSA=0, TTree *tree_AMP=0);
   virtual ~clycana();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry_PSA(Long64_t entry_PSA);
   //virtual Int_t    GetEntry_TAC(Long64_t entry_TAC);
   virtual Int_t    GetEntry_AMP(Long64_t entry_AMP); 
   
   virtual Long64_t LoadTree_PSA(Long64_t entry_PSA);
   //virtual Long64_t LoadTree_TAC(Long64_t entry_TAC);
   virtual Long64_t LoadTree_AMP(Long64_t entry_AMP);
   virtual void     Init(TTree *tree_PSA, TTree *tree_AMP);
   virtual void     Loop(Long64_t);
   virtual Bool_t   Notify();
   virtual void     Show_PSA(Long64_t entry_PSA = -1);
   //virtual void     Show_TAC(Long64_t entry_TAC = -1);
   virtual void     Show_AMP(Long64_t entry_AMP = -1);
};

#endif

#ifdef clycana_cxx
clycana::clycana(TTree *tree_PSA, TTree *tree_AMP) : fChain_PSA(0), fChain_AMP(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree_PSA == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/Users/javi/Documents/Padova/2/AdvancedLab/AdvancedLab_22-23/data/UNFILTERED/CH_2@DT5725_1701_Data_60Co_252Cf_CLYC_ML_newOpticGrease.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/Users/javi/Documents/Padova/2/AdvancedLab/AdvancedLab_22-23/data/UNFILTERED/CH_2@DT5725_1701_Data_60Co_252Cf_CLYC_ML_newOpticGrease.root");
      }
      f->GetObject("caenData_AMP", tree_PSA);

   }
 

/*
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree_TAC == 0) {
      TFile *g = (TFile*)gROOT->GetListOfFiles()->FindObject("/Users/javi/Documents/Padova/2/AdvancedLab/AdvancedLab_22-23/data/UNFILTERED/CH_2@DT5725_1701_Data_60Co_252Cf_CLYC_ML_newOpticGrease.root");
      if (!g || !g->IsOpen()) {
         g = new TFile("/Users/javi/Documents/Padova/2/AdvancedLab/AdvancedLab_22-23/data/UNFILTERED/CH_0@DT5725_1701_Data_60Co_252Cf_CLYC_ML_newOpticGrease.root");
      }
      g->GetObject("caenData_TAC", tree_TAC);

   }
*/



// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree_AMP == 0) {
      TFile *h = (TFile*)gROOT->GetListOfFiles()->FindObject("/Users/javi/Documents/Padova/2/AdvancedLab/AdvancedLab_22-23/data/20220517/252Cf_CLYC_both_COINCIDENCE/FILTERED/CH_0@DT5725_1701_Data_252Cf_CLYC_both_COINCIDENCE_CH4_1.root");
      if (!h || !h->IsOpen()) {
         h = new TFile("/Users/javi/Documents/Padova/2/AdvancedLab/AdvancedLab_22-23/data/20220517/252Cf_CLYC_both_COINCIDENCE/FILTERED/CH_0@DT5725_1701_Data_252Cf_CLYC_both_COINCIDENCE_CH4_1.root");
      }
      h->GetObject("caenData_AMP", tree_AMP);

   }
   Init(tree_PSA, tree_AMP);

}

clycana::~clycana()
{
   if (!fChain_PSA) return;
   delete fChain_PSA->GetCurrentFile();
}

Int_t clycana::GetEntry_PSA(Long64_t entry_PSA)
{
// Read contents of entry.
   if (!fChain_PSA) return 0;
   return fChain_PSA->GetEntry(entry_PSA);
}

/*
Int_t clycana::GetEntry_TAC(Long64_t entry_TAC)
{
// Read contents of entry.
   if (!fChain_TAC) return 0;
   return fChain_TAC->GetEntry(entry_TAC);
}
*/

Int_t clycana::GetEntry_AMP(Long64_t entry_AMP)
{
// Read contents of entry.
   if (!fChain_AMP) return 0;
   return fChain_AMP->GetEntry(entry_AMP);
}

Long64_t clycana::LoadTree_PSA(Long64_t entry_PSA)
{
// Set the environment to read one entry
   if (!fChain_PSA) return -5;
   Long64_t centry_PSA = fChain_PSA->LoadTree(entry_PSA);
   if (centry_PSA < 0) return centry_PSA;
   if (fChain_PSA->GetTreeNumber() != fCurrent_PSA) {
      fCurrent_PSA = fChain_PSA->GetTreeNumber();
      Notify();
   }
   return centry_PSA;

}
/*
Long64_t clycana::LoadTree_TAC(Long64_t entry_TAC)
{
// Set the environment to read one entry
   if (!fChain_TAC) return -5;
   Long64_t centry_TAC = fChain_TAC->LoadTree(entry_TAC);
   if (centry_TAC < 0) return centry_TAC;
   if (fChain_TAC->GetTreeNumber() != fCurrent_TAC) {
      fCurrent_TAC = fChain_TAC->GetTreeNumber();
      Notify();
   }
   return centry_TAC;
}
*/

Long64_t clycana::LoadTree_AMP(Long64_t entry_AMP)
{
// Set the environment to read one entry
   if (!fChain_AMP) return -5;
   Long64_t centry_AMP = fChain_AMP->LoadTree(entry_AMP);
   if (centry_AMP < 0) return centry_AMP;
   if (fChain_AMP->GetTreeNumber() != fCurrent_AMP) {
      fCurrent_AMP = fChain_AMP->GetTreeNumber();
      Notify();
   }
   return centry_AMP;

}

void clycana::Init(TTree *tree_PSA, TTree *tree_AMP)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree_PSA) return;
   fChain_PSA = tree_PSA;
   fCurrent_PSA = -1;
   fChain_PSA->SetMakeClass(1);

   fChain_PSA->SetBranchAddress("Timestamp_AMP", &Timestamp_PSA, &b_Timestamp_PSA);
   fChain_PSA->SetBranchAddress("Energy_AMP", &Energy_PSA, &b_Energy_PSA);
   fChain_PSA->SetBranchAddress("Samples_AMP", &Samples_PSA, &b_Samples_PSA);
   Notify();
/*
if (!tree_TAC) return;
   fChain_TAC = tree_TAC;
   fCurrent_TAC = -1;
   fChain_TAC->SetMakeClass(1);

   fChain_TAC->SetBranchAddress("Timestamp_TAC", &Timestamp_TAC, &b_Timestamp_TAC);
   fChain_TAC->SetBranchAddress("Energy_TAC", &Energy_TAC, &b_Energy_TAC);
   fChain_TAC->SetBranchAddress("Samples_TAC", Samples_TAC, &b_Samples_TAC);
   Notify();
*/

if (!tree_AMP) return;
   fChain_AMP = tree_AMP;
   fCurrent_AMP = -1;
   fChain_AMP->SetMakeClass(1);

   fChain_AMP->SetBranchAddress("Timestamp_AMP", &Timestamp_AMP, &b_Timestamp_AMP);
   fChain_AMP->SetBranchAddress("Energy_AMP", &Energy_AMP, &b_Energy_AMP);
   fChain_AMP->SetBranchAddress("Samples_AMP", Samples_AMP, &b_Samples_AMP);
   Notify();

}

Bool_t clycana::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void clycana::Show_PSA(Long64_t entry_PSA)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain_PSA) return;
   fChain_PSA->Show(entry_PSA);
}
/*
void clycana::Show_TAC(Long64_t entry_TAC)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain_TAC) return;
   fChain_TAC->Show(entry_TAC);
}
*/
void clycana::Show_AMP(Long64_t entry_AMP)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain_AMP) return;
   fChain_AMP->Show(entry_AMP);
}
Int_t clycana::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef clycana_cxx
