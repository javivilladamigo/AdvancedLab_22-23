#include <string>
#include <iostream>

void resolution(char const *fileName = "data/60Co_CLYC_ML.Spe", Int_t npeaks = 2, Float_t xMin = 80, Float_t xMax = 1024)
{
    gROOT->SetStyle("Default");
	gStyle->SetCanvasColor(0);
	gStyle->SetStatBorderSize(1);
	gStyle->SetOptFit(0);
	gStyle->SetOptStat(0);
	gStyle->SetPalette(0);

    Int_t bins = 8192; //number of bins/channels of your histogram 
	Int_t data; //variable where to store data
    TH1I *hist = new TH1I("hist", "", bins, 0, bins); // data histogram

    //Reading nbins/lines with data
    ifstream *file = new ifstream(fileName);
    for(Int_t i=0; i<12; i++)
    {
        file->ignore(256, '\n'); //skipping the header of the ascii file (12 lines in this case)
    } 
    for(Int_t i=0;i<bins;i++)
    { 
        *file >> data;
        hist->SetBinContent(i, data);
    }

    TCanvas *c1 = new TCanvas("c1", "Raw Spectrum");
    hist->GetXaxis()->SetRangeUser(xMin, xMax);
    hist->Draw();

    TSpectrum *ss = new TSpectrum(npeaks); //creating TSpectrum object
    Int_t nfound = ss->Search(hist, 10, "new", 0.1); //searching peaks
    cout << "\n --------- Found " << nfound << " peaks" << endl << endl << endl;
    Double_t *xpeaks;
    xpeaks = (Double_t*) ss->GetPositionX(); //saving the position of the found peaks in an array 


    Double_t xp = 0.;
    Float_t mean = 0., sigma = 0.;
	Float_t resolution[2] = {0., 0.};

    //Sort xpeaks array
    std::sort(xpeaks, xpeaks + nfound);

	//2-step gauss fit to each of the found peaks, calculating the relative resolution afterwards
    TF1 *fun1, *fun2;
    for(Int_t i=0; i<nfound; i++)
    {   

        xp = xpeaks[i];
        cout << " ************ Peak at: " << xp << endl;
        fun1 = new TF1("fun1name", "gaus", xp - 30, xp + 30);
        hist->Fit(fun1,"RQN");
        mean  = fun1->GetParameter(1);
        sigma = fun1->GetParameter(2);
        fun2 = new TF1("fun2name", "gaus", mean - 1.5 * sigma, mean + 1.5 * sigma);
        hist->Fit(fun2, "R+");
        mean  = fun2->GetParameter(1);
        sigma = fun2->GetParameter(2);

        resolution[i]=((2.35 * sigma) / mean) * 100;

        cout << "\nRESULTS:  Mean at: " << mean << " with sigma " << sigma << " , FWHM of " << 2.35 * sigma << " , and resolution of " << resolution[i] << " %" << endl << endl;
    }

    //Fit of the resolution values for each peak to a function Res~1/sqrt(E), and evaluating it at 1 MeV
	Float_t xval[3];
	if(npeaks == 1) xval[0] = 1173; else if(npeaks == 2) {xval[0] = 1173; xval[1] = 1332;};
	TGraph *graph = new TGraph(npeaks, xval, resolution);
	TCanvas *c_graph = new TCanvas("c_graph", "Resolution vs Energy");
	graph->SetMarkerStyle(20);
	graph->Draw("AP");
	TF1 *reso = new TF1("reso","[0]/x+[1]/sqrt(x)+[2]");
	graph->Fit("reso");
	Float_t res_at_one25 = reso->Eval(1250.);
	cout << "\n\n ************** RESOLUTION @ 1.25 MeV:  " << res_at_one25 << " % ************** " << endl << endl;

    //Calibration
	Float_t peaks[npeaks];
	for(Int_t i = 0; i<npeaks; i++) peaks[i] = xpeaks[i];
	TGraph *cal_graph = new TGraph(npeaks, peaks, xval); //calibration: channel vs energy of the found peaks
	cal_graph->SetNameTitle("cal_graph", "Calibration");
	cal_graph->SetMarkerStyle(21);
	cal_graph->SetMarkerColor(4);
	TF1 *cal_fn = new TF1("cal_fn", "[0]*x+[1]"); //defining a first order polynomial function and fit the data
	cal_graph->Fit(cal_fn);
	TCanvas *cal_can = new TCanvas("cal_can", "Calibration");
	cal_graph->Draw();

    //Re-escale the histogram X-axis with the calculated calibration
    Float_t new_end = cal_fn->Eval(bins);
	TH1F *hcal = new TH1F("hcal","hcal",hist->GetNbinsX(), 0, new_end); //create calibrated histo!!
	for(Int_t i=0;i<bins;i++) hcal->SetBinContent(i+1,hist->GetBinContent(i+1)); //fill cal histo with same content!
	TCanvas *cal_histo_can = new TCanvas("cal_histo_can","cal_histo_can");
	hcal->GetXaxis()->SetTitle("Photon energy [MeV]");
	hcal->GetXaxis()->CenterTitle();
    hcal->GetXaxis()->SetRangeUser(cal_fn->Eval(xMin), cal_fn->Eval(xMax));
    hcal->Draw();
}