#include <iostream>
#include <algorithm>
#include <ctime>

#include "TStopwatch.h"
#include "TMath.h"
#include "TRandom2.h"

#include <TApplication.h>
#include <TCanvas.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TAxis.h>

using namespace std;

const int npass = 100000;
const int maxint = 20;
const int minsize = 20;
const int maxsize = 500;
const int increment = 10;
const int arraysize = (maxsize-minsize)/10 + 1;


template<typename T> 
struct Compare { 

   Compare(const T *  d) : fData(d) {}

   bool operator()(int i1, int i2) { 
      return fData[i1] > fData[i2];
   }

   const T * fData; 
};

template <typename T> void testSort(const int n, double& tTMath, double& tStd)
{
   T k[n];

   T index[n];
   TStopwatch t; 

   TRandom2 r( time( 0 ) );
   for ( Int_t i = 0; i < n; i++) {
      k[i] = (T) r.Integer( maxint ); 
//      cout << k[i] << ' ' << endl;
   }

   t.Start(); 
   for (int j = 0; j < npass; ++j) { 
//    for(Int_t i = 0; i < n; i++) { index[i] = i; }
      TMath::Sort(n,k,index,kTRUE);  
   }
   t.Stop(); 
   tTMath = t.RealTime();
   cout << "\nTMath Sort\n";
   cout << "TMath::Sort time :\t\t " << t.RealTime() << endl;

   cout << "\nSort using indices" << endl;
  
   t.Start(); 
   for (int j = 0; j < npass; ++j) { 
      for(Int_t i = 0; i < n; i++) { index[i] = i; }
      std::sort(index,index+n, Compare<T>(k) );
   }
   t.Stop(); 
   tStd = t.RealTime();
   std::cout << "std::sort using indices:\t " << t.RealTime() << std::endl;
}

void stdsort() 
{
   double tM[ arraysize ];
   double tS[ arraysize ];
   double index[ arraysize ];

   //cout << (maxsize-minsize)/10 + 1 << endl;

   for ( int i = minsize; i <= maxsize; i += increment)
   {
      testSort<Int_t>(i, tM[(i-minsize)/10], tS[(i-minsize)/10]);
      index[(i-minsize)/10] = i;
   }

   for ( int i = minsize; i <= maxsize; i += increment)
      cout << tM[(i-minsize)/10] << ' ' << tS[(i-minsize)/10] << endl;

   TCanvas* c1 = new TCanvas("c1", "Comparision of Sorting Time", 600, 400);
   TH2F* hpx = new TH2F("hpx", "Comparision of Sorting Time", arraysize, minsize, maxsize, arraysize, 0,tM[arraysize-1]);
   hpx->SetStats(kFALSE);
   hpx->Draw();
   
   TGraph* gM = new TGraph(arraysize, index, tM);
   gM->SetLineColor(2);
   gM->SetLineWidth(3);
   gM->SetTitle("TMath::Sort()");
   gM->Draw("SAME");

   TGraph* gS = new TGraph(arraysize, index, tS);
   gS->SetLineColor(3);
   gS->SetLineWidth(3);
   gS->SetTitle("std::sort()");
   gS->Draw("SAME");

   TLegend* legend = new TLegend(0.15,0.72,0.4,0.86);
   legend->AddEntry(gM, "TMath::Sort()");
   legend->AddEntry(gS, "std::sort()");
   legend->Draw();

   hpx->GetXaxis()->SetTitle("Array Size");
   hpx->GetYaxis()->SetTitle("Time");
   

   c1->Show();

}

int main(int argc, char **argv)
{
   TApplication theApp("App",&argc,argv);
   stdsort();
   theApp.Run();

   return 0;
}
