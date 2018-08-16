/***************************************************************************
  *  Copyright S. V. Paulauskas 2014-2016                                   *
  *                                                                        *
  *  This program is free software: you can redistribute it and/or modify  *
  *  it under the terms of the GNU General Public License as published by  *
  *  the Free Software Foundation, version 3.0 License.                    *
  *                                                                        *
  *  This program is distributed in the hope that it will be useful,       *
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
  *  GNU General Public License for more details.                          *
  *                                                                        *
  *  You should have received a copy of the GNU General Public License     *
  *  along with this program.  If not, see <http://www.gnu.org/licenses/>. *
  **************************************************************************
*/
/** \file test.cpp
 *  \brief A test code to test the filtering
 *  \author S. V. Paulauskas
 *  \date 23 April 2014
 *
 *  This code is based off of the IGOR macro energy.ipf
 *  written by H. Tan of XIA LLC and parts of the nscope
 *  program written at the NSCL written by C.Prokop.
 *
 */
// Adapted by JJ van Zyl, June 2018
// Compile with:
// g++ signal_trace.cpp TraceFilter.cpp `root-config --libs --cflags --glibs` -o signal_trace
// provide input signal parameters (file: sig_input.dat), as well as trace parameters  (file: trc_params.dat) 
// 
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include <TH1F.h>
#include <TFile.h>
#include <TRandom3.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TTree.h>

#include "TraceFilter.hpp"

using namespace std;

TFile hfile("test-out.root","RECREATE","Demo ROOT file with histogram");
TRandom3 *rgaus = new TRandom3(0);

// ------------------------------------------------------------------------------
double noise(double val, double dval)        //rgaus is [-1,1]
{
//   	TRandom3 *rgaus = new TRandom3(0);
	double rg = 1.*rgaus->Gaus(0,1);
	val = val + dval*rg;         // +- dval [mm]
    return val;
}

// ------------------------------------------------------------------------------
double Tfilter(vector<double> &trc) {
//double Tfilter(double readY[], int NN) {
/*
    vector<double> trc;
	for(int i=0;i<NN;i++) trc.push_back(readY[i]);

    vector<double> trc;
    double readY;

    ifstream ifs(infile, ios::in | ios::binary );

    while ( ifs ) {
    	ifs.read( reinterpret_cast<char*>(&readY), sizeof(double) );
        trc.push_back(readY);
    }
*/ 
/*
    ifstream infile("piledupTrc.dat");
    if(!infile) {
        cerr << "Cannot open input file. Try again, son." << endl;
        exit(1);
    } else {
        while(infile) {
            if (isdigit(infile.peek())) {
                int junk1;
                infile >> junk1;
                trc.push_back(junk1);
            } else  infile.ignore(1000,'\n');
        }
    }
    infile.close();
*/
    		
    //times in ns, sampling in ns/S, and thresh in ADC units
    unsigned int adc = 10; // in ns/S
    double tl = 200, tg = 20;
    unsigned int thresh = 300;
    double el = 400, eg = 200, tau = 25000;
    int trc_flag = 0;	// flag for trace output file
	static bool trcout = false;	// flag if trace output was already written once
	
    ifstream trcfile("trc_params.dat");
    if(!trcfile) {
        cerr << "Cannot open trace parameter input file. Try again, son." << endl;
        exit(1);
    } else 
          trcfile >> adc >> tau >> tl >> tg >> thresh >> el >> eg >> trc_flag;
    trcfile.close();
    
    TrapFilterParameters trigger(tl, tg, thresh);
    TrapFilterParameters energy(el, eg, tau);

//    double eLen = energy.GetSize() / adc;

    TraceFilter filter(adc, trigger, energy);
    filter.SetVerbose(false);
   
    if(filter.CalcFilters(&trc) != 0)
        exit(0);
        
    if(trc_flag && !trcout) {
    	int j=0;
	    vector<double> trig  = filter.GetTriggerFilter();
		TH1D *hTrc = new TH1D("hTrc","Trigger filter trace; time [ns]",trig.size(),0,trig.size());

		ofstream output("trig.dat");
		cout<<"...trig.dat output file written..."<<endl;
    	if(output) {
        	for(const auto &i : trig) {
        	    output << i << endl;
				hTrc->SetBinContent(j,i);
				j++;
			}
		}	    
    	output.close();
    	trcout = true;
    }

    double trcEn = filter.GetEnergy();
    return trcEn;
    
/*
    //The energy sum information
    double sumL = 20489;
    double sumG = 10040;
    double sumT = 16508;
    //double pE   = 1179;

    double pb = 3780.7283;
    double pbPerSamp = pb / eLen;

    vector<double> esums  = filter.GetEnergySums();
    vector<double> coeffs = filter.GetEnergyFilterCoefficients();
*/
/*
    cout << endl << "Calculations using my coefficients" << endl
         << "----------------------------" << endl;
    cout << "Trc Energy Sums : ";
    for(const auto &i : esums)
        cout << i << " ";
    cout << endl;
    
    cout << "Trace Energy: " << trcEn << endl;
    cout << "Esums Energy Calc 1 : " 
         << sumL*coeffs[0]+sumG*coeffs[1]+sumT*coeffs[2]-pbPerSamp
         << endl;
    cout << "Esums Energy Calc 2 : " 
         << (sumL-pbPerSamp)*coeffs[0]+(sumG-pbPerSamp)*coeffs[1]+(sumT-pbPerSamp)*coeffs[2]
         << endl;
    cout << "Esums Energy Calc 3 : " 
         << sumL*coeffs[0]+sumG*coeffs[1]+sumT*coeffs[2]
         << endl;
    cout << "Esums Energy Calc 4 : " 
         << (sumL-pb)*coeffs[0]+(sumG-pb)*coeffs[1]+(sumT-pb)*coeffs[2]
         << endl;
    cout << "Esums Energy Calc 5 : " 
         << (sumL/(el*adc))*coeffs[0]+(sumG/(el*adc))*coeffs[1]+(sumT/(el*adc))*coeffs[2]
         << endl;
    cout << "Esums Energy Calc 6 : " 
         << (sumL/(el*adc))+(sumG/(el*adc))+(sumT/(el*adc))
         << endl;

    double b1 = exp(-1/(tau*adc));
    double a0 = pow(b1,el*adc)/(pow(b1,el*adc)-1.0);
    double ag = 1.0;
    double a1 = -1.0/(pow(b1,el*adc)-1.0);
    cout << endl << "Calculations using C. Prokop's coefficients" << endl
         << "----------------------------" << endl;
    cout << "Coeffs: " << endl << "  a0 = " << a0 << endl << "  ag = " << ag << endl 
         << "  a1 = " << a1 << endl;
    cout << "Esums Energy Calc 1 : " 
         << (sumL)*a0+(sumG)*ag+(sumT)*a1 - pb
         << endl;
    cout << "Esums Energy Calc 2 : " 
         << (sumL-pb)*a0+(sumG-pb)*ag+(sumT-pb)*a1
         << endl;
*/
}

int main()
{
//	std::string polname = "data/piledupTrc.dat";
//	std::string logname = "data/logfile.dat";
    int   nruns = 5000;
	float volt0 = 0.662;
	float    dv = 0.02;
	float  errf = 0.02;
    int   TAU_D = 25000;
    int   TAU_R = 200;
	int   basel = 200;
	int     Sps = 10;

	int      x0 = 5000;
	int     ADC = 16384;
	const int NN = (int)(1.5*TAU_D/Sps + x0/Sps);
	float  volt, amp, err;
	double Y=0, X=0, Ytmp;
	double tEn=0;
	bool plot1 = false;
	int onlySig = 0;
	
//    std::string tempfile = "tempdata.dat";

    ifstream infile("sig_input.dat");
    if(!infile) {
        cerr << "Cannot open signal input file. Try again, son." << endl;
        exit(1);
    } else 
          infile >> nruns >> volt0 >> dv >> errf >> TAU_D >> TAU_R >> basel >> Sps >> onlySig;

    infile.close();

//	TFile hfile("test-out.root","RECREATE","Demo ROOT file with histogram");
    TH1D *hE = new TH1D("hE","Energy spectrum; channels; counts/ch",1000,0,16000);
    TH1D *hSig = new TH1D("hSig","Preamp signal; time [ns]; amplitude [mV]",NN,0,NN);

//    TH1D *hTrc = new TH1D("hTrc","Trigger filter trace; time [ns]",NN,0,NN);
/*
    ofstream polfile;
	polfile.open(polname.c_str());
	
    ofstream logfile;
	logfile.open(logname.c_str());
*/
   	vector<double> sigarray;
	for(int j=0; j<nruns; j++) {

		volt = noise(volt0, dv);
		amp = volt*ADC;
		err = amp*errf;
//    	ofstream fout(tempfile, ios::out | ios::binary);

		sigarray.clear();	
		for(int i=0; i<NN; i++) {
			X = i*Sps;
			if(X<x0) Y = noise(basel, err);
			else {
				Ytmp = basel + amp*(exp(-1.*(X-x0)/TAU_D) - exp(-1.*(X-x0)/TAU_R));
				Y = noise(Ytmp, err);	
			}
			if(!plot1) hSig->SetBinContent(i,Y);
			
            sigarray.push_back(Y);
//    		fout.write( reinterpret_cast<char*>(&Y), sizeof(double) );
			if((int)(100.*j/nruns)%5==0) printf("\rOverall progress: %d%%",(int)(100.*j/nruns));
//			logfile << X <<"\t" << Y <<"\n";
//			polfile << Y <<"\n";
		}
	//		fout.close();      
		if(!plot1) plot1=true;

		if(!onlySig) tEn = Tfilter(sigarray);
		else {
			cout<<"\nWriting signal sample - no trace calculation!\n";
			break;
		} 

	    hE->Fill(tEn);
	}

    hfile.cd();
	cout<<"\nWriting root file...\n";
	hfile.Write();
	hfile.Close();

//		polfile.close();
//		cout << polname << " file written with "<< NN << " data points.\n";
	return 0;
}

