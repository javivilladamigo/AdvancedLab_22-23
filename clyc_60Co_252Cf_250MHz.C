#define clyc_60Co_252Cf_cxx
#include "clyc_60Co_252Cf_250MHz.h"
#include <TH2.h> 
#include <TStyle.h> 
#include <TCanvas.h> 
#include <TMath.h> 
#include <TSpline.h> 
#include <iostream> 

void clyc_60Co_252Cf::Loop(Long64_t nbEntries = -1)
{


  //  In a ROOT session, you can do:
  //    root >  .L clyc_60Co_252Cf.C
  //    root >  clyc_60Co_252Cf t
  //    root >  t.GetEntry(12); // Fill t data members with entry number 12
  //    root >  t.Show();       // Show values of entry 12
  //    root >  t.Show(16);     // Read and show values of entry 16
  //    root >  t.Loop();       // Loop on all entries
  //

  //  This is the loop skeleton where:
  //    jentry is the global entry number in the chain
  //    ientry is the entry number in the current Tree
  //  Note that the argument to GetEntry must be:
  //    jentry for TChain::GetEntry
  //    ientry for TTree::GetEntry and TBranch::GetEntry
  //
  //  To read only selected branches, Insert statements like:
  //
  //    METHOD1:
  //      fChain->SetBranchStatus("*", 0);  // disable all branches
  //      fChain->SetBranchStatus("branchname", 1);  // activate branchname
  //
  //    METHOD2: replace line
  //      fChain->GetEntry(jentry);       //read all branches

  TFile *output = new TFile("output.root", "recreate");
  Double_t charge = 0; Double_t PSA = 0; Double_t Start = 0;
  TGraph *gr;
  TSpline3 *signal_spline3;

  TTree *result = new TTree("result", "result");
  result->Branch("PSA", &PSA, "PSA/D");
  result->Branch("charge", &charge, "charge/D");
  result->SetAutoSave(10000000);



  TH1D *signal_total                  = new TH1D("signal_total", "signal_total", 500, 0, 500);
  TH1D *signal_temp                   = new TH1D("signal_temp", "signal_temp", 500, 0, 500);
  TH1D *signal_gammas                 = new TH1D("signal_gammas", "signal_gammas", 500, 0, 500);
  TH1D *signal_neutrons               = new TH1D("signal_neutrons", "signal_neutrons", 500, 0, 500);
  TH1D *h_energy                      = new TH1D("h_energy", "Energy gamma calibrated", 2000, 0, 2000);

  Long64_t sum[500];                  for (UShort_t z = 0; z < 500; z++) sum[z] = 0;
  Long64_t sum_gammas[500];           for (UShort_t z = 0; z < 500; z++) sum_gammas[z] = 0;
  Long64_t sum_neutrons[500];         for (UShort_t z = 0; z < 500; z++) sum_neutrons[z] = 0;

  Long64_t gammas = 0; Long64_t neutrons = 0; Long64_t protons = 0; Long64_t alfas = 0; 
  
  int start = 0;	
  double int_short, int_long; 
  double max_70, max_20, time_70, time_20;

  TH2F* histo_psd                     = new TH2F("histo_psd", "PSD", 1000, 0., 1000000., 1000, 0, 1);
  TH2F* histo_psd_energy              = new TH2F("histo_psd_energy", "PSD", 8000, 0., 8000., 1000, 0, 1);

  Long64_t var[500];                  for (UShort_t z = 0; z < 500; z++) var[z] = 0;
  Long64_t var_gammas[500];           for (UShort_t z = 0; z < 500; z++) var_gammas[z] = 0;
  Long64_t var_neutrons[500];         for (UShort_t z = 0; z < 500; z++) var_neutrons[z] = 0;

  Long64_t sigma_total[500];          for (UShort_t z = 0; z < 500; z++) sigma_total[z] = 0;
  Long64_t sigma_gammas[500];         for (UShort_t z = 0; z < 500; z++) sigma_gammas[z] = 0;
  Long64_t sigma_neutrons[500];       for (UShort_t z = 0; z < 500; z++) sigma_neutrons[z] = 0;

  Long64_t avg[500];                  for (UShort_t z = 0; z < 500; z++) avg[z] = 0;
  Long64_t avg_gammas[500];           for (UShort_t z = 0; z < 500; z++) avg_gammas[z] = 0;
  Long64_t avg_neutrons[500];         for (UShort_t z = 0; z < 500; z++) avg_neutrons[z] = 0;
  Long64_t count  = 0;




  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;

  Long64_t maxEntries = (nbEntries == -1) ? fChain->GetEntries() : nbEntries;
  maxEntries = min(maxEntries, fChain->GetEntries());



  cout << "Will treat " << maxEntries << " events" << std::endl;
  for (Long64_t jentry = 0; jentry < maxEntries; jentry++)
  {
    int_short = 0;
    int_long = 0;
    Long64_t ientry = LoadTree(jentry);
    if (ientry  <  0) break;

    nb = fChain->GetEntry(jentry); nbytes += nb;


    /* I dont think this is doing anything relevant (?)
    for (UShort_t i = 10; i < 500; i++)
    {
      if (Samples_AMP[i] >= 30 && Samples_AMP[i+1] >= 30 && Samples_AMP[i+2] >= 30 && Samples_AMP[i+3] >= 30 && Samples_AMP[i+4] >= 30)
      {
        start = i; Start = start; //cout << start <<  "value of sample "  << Samples_AMP[i] <<  endl;
        break;
      }
    }
    */
	


    /////////////////////////////////// PART to MODIFY ///////////////////////////////////
    // cout << start << endl;
    int long_max = start + 250;  //play with
    int short_max = start + 60; //play with
    double psd = 0;
    /////////////////////////////////// END PART to MODIFY ///////////////////////////////

    
    for (UShort_t i = start; i < (long_max + 1); i++) // compute int_long and int_short for every entry (trace)
    {
		  int_long +=  Samples_AMP[i];

      if (i < (short_max+1))
      {
        int_short +=  Samples_AMP[i];
      }

    }


    psd = 1.0*(int_long-int_short)/int_long;
    charge = int_long;

    histo_psd->Fill(charge, psd);


    result->Fill();
    gr->Write();
    output->Write();
  }
}















  // These are just drawings of polygons
  /*
  TCutG *neutroni = new TCutG("neutroni", 13);
  neutroni->SetVarX("Ene_gamma");
  neutroni->SetVarY("PSA");
  neutroni->SetPoint(0, 572.377, 0.815576); // neutroni->SetPoint(0, 472.377, 0.815576);
  neutroni->SetPoint(1, 569.249, 0.793297); //neutroni->SetPoint(1, 469.249, 0.793297);
  neutroni->SetPoint(2, 562.367, 0.725225); // neutroni->SetPoint(2, 462.367, 0.725225);
  neutroni->SetPoint(3, 2769.55, 0.732032);
  neutroni->SetPoint(4, 4472.78, 0.719984);
  neutroni->SetPoint(5, 6163.32, 0.693481);
  neutroni->SetPoint(6, 7338.11, 0.68182);
  neutroni->SetPoint(7, 7739.26, 0.680759);
  neutroni->SetPoint(8, 7825.21, 0.77087);
  neutroni->SetPoint(9, 6765.04, 0.813275);
  neutroni->SetPoint(10, 4828.99, 0.8131);
  neutroni->SetPoint(11, 3280.5, 0.826096);
  neutroni->SetPoint(12, 1650.43, 0.818576);
  neutroni->SetPoint(13, 631.908, 0.821764);
  neutroni->SetPoint(14, 572.377, 0.815576); // neutroni->SetPoint(14, 472.377, 0.815576);

   
   
   
   
 
  TCutG *alfa = new TCutG("alfa", 13);
  alfa->SetVarX("Ene_gamma");
  alfa->SetVarY("TAC_ene");
  alfa->SetPoint(0, 3071.95, 7633);
  alfa->SetPoint(1, 1273.2, 6130.54);
  alfa->SetPoint(2, 819.604, 5280.79);
  alfa->SetPoint(3, 809.176, 3778.33);
  alfa->SetPoint(4, 887.383, 1943.35);
  alfa->SetPoint(5, 1200.21, 1647.78);
  alfa->SetPoint(6, 1622.52, 1832.51);
  alfa->SetPoint(7, 4521.38, 4997.54);
  alfa->SetPoint(8, 4985.4, 5613.3);
  alfa->SetPoint(9, 5293.01, 6807.88);
  alfa->SetPoint(10, 4724.71, 7719.21);
  alfa->SetPoint(11, 3947.86, 7867);
  alfa->SetPoint(12, 3071.95, 7633);
   
  TCutG *protoni =  new TCutG("protoni", 9);
  protoni->SetVarX("Ene_gamma");
  protoni->SetVarY("TAC_ene");
  protoni->SetPoint(0, 1080.29, 1081.28);
  protoni->SetPoint(1, 6048.71, 6067.51);
  protoni->SetPoint(2, 7065.9, 6974.68);
  protoni->SetPoint(3, 7681.95, 6025.32);
  protoni->SetPoint(4, 1120.34, 455.696);
  protoni->SetPoint(5, 819.484, 12.6582);
  protoni->SetPoint(6, 446.991, -8.43888);
  protoni->SetPoint(7, 905.444, 772.152);
  protoni->SetPoint(8, 1080.29, 1081.28);
  
   
  TCutG *gamma = new TCutG("gamma", 13);
  gamma->SetVarX("Ene_gamma");
  gamma->SetVarY("PSA");
  gamma->SetPoint(0, 504.298, 0.690506);
  gamma->SetPoint(1, 747.851, 0.674684);
  gamma->SetPoint(2, 1020.06, 0.663608);
  gamma->SetPoint(3, 2782.23, 0.647785);
  gamma->SetPoint(4, 7796.56, 0.636709);
  gamma->SetPoint(5, 7739.26, 0.589241);
  gamma->SetPoint(6, 3255.01, 0.568671);
  gamma->SetPoint(7, 1879.66, 0.560759);
  gamma->SetPoint(8, 704.871, 0.537025);
  gamma->SetPoint(9, 275.072, 0.502215);
  gamma->SetPoint(10, 146.132, 0.516456);
  gamma->SetPoint(11, 289.398, 0.704747);
  gamma->SetPoint(12, 504.298, 0.690506);
  */