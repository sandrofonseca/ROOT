// @(#)root/proofx:$Id$
// Author: Sangsu Ryu 22/06/2010

/*************************************************************************
 * Copyright (C) 1995-2005, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TProofBenchRunCPU                                                    //
//                                                                      //
// CPU-intensive PROOF benchmark test generates events and fill 1, 2,   //
// or 3-D histograms. No I/O activity is involved.                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TProofBenchRunCPU.h"
#include "TProofBenchMode.h"
#include "TProofNode.h"
#include "TFile.h"
#include "TFileCollection.h"
#include "TFileInfo.h"
#include "TProof.h"
#include "TString.h"
#include "TDSet.h"
#include "Riostream.h"
#include "TMap.h"
#include "TEnv.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TProfile.h"
#include "TLegend.h"
#include "TKey.h"
#include "TRegexp.h"
#include "TPerfStats.h"
#include "TQueryResult.h"
#include "TMath.h"
#include "TStyle.h"

ClassImp(TProofBenchRunCPU)

//______________________________________________________________________________
TProofBenchRunCPU::TProofBenchRunCPU(TProofBenchRun::EHistType histtype,
                                     Int_t nhists,
                                     TString filename,
                                     Option_t* foption, 
                                     TProof* proof,
                                     Int_t maxnworkers,
                                     Long64_t nevents,
                                     Int_t ntries,
                                     Int_t start,
                                     Int_t stop,
                                     Int_t step,
                                     Int_t draw,
                                     Int_t debug):
fProof(0),
fHistType(histtype),
fNHists(nhists),
fNEvents(nevents),
fNTries(ntries),
fMaxNWorkers(0),
fStart(start),
fStop(stop),
fStep(step),
fDraw(draw),
fDebug(debug),
fFile(0),
fDirProofBench(0),
fWritable(0),
fNodes(0),
fPerfStats(0),
fListPerfProfiles(0),
fCPerfProfiles(0),
fName(0)
{
   //Default constructor
   fProof=proof?proof:gProof;

   if (filename.Length()){
      OpenFile(filename.Data(), foption);
   }

   fName="CPU"+GetNameStem();

   if (maxnworkers>0){
      SetMaxNWorkers(maxnworkers);
   }
   else{
      SetMaxNWorkers("1x");
   }

   if (stop==-1){
      fStop=fMaxNWorkers;
   }

   FillNodeInfo();

   fPerfStats=new TList();

   gEnv->SetValue("Proof.StatsTrace",1);
   gStyle->SetOptStat(0);

}

//______________________________________________________________________________
TProofBenchRunCPU::~TProofBenchRunCPU()
{
   // Destructor
   fProof=0;
   fDirProofBench=0;
   if (fFile){
      fFile->Close();
      delete fFile;
   }
   if (fNodes) delete fNodes;  //fNodes is the owner of its members
   if (fListPerfProfiles) delete fListPerfProfiles;
   if (fCPerfProfiles) delete fCPerfProfiles;
} 

//______________________________________________________________________________
void TProofBenchRunCPU::Run(Long64_t nevents,
                            Int_t ntries,
                            Int_t start,
                            Int_t stop,
                            Int_t step,
                            Int_t debug,
                            Int_t draw)
{
   // Run benchmark
   // Input parameters
   //   nevents:   Number of events to run per file. When it is -1, use data member fNEvents.
   //   start: Start scan with 'start' workers. When it is -1, use data member fNTries.
   //   stop: Stop scan at 'stop' workers. When it is -1 , use data member fStop.
   //   step: Scan every 'step' workers. When it is -1, use data member fStep.
   //   debug: debug switch. When it is -1, use data member fDebug.
   //   draw: draw switch. When it is -1, use data member fDraw.
   // Returns
   //    Nothing

   if (!fProof){
      Error("RunBenchmark", "Proof not set");
      return;
   }

   nevents=-1?fNEvents:nevents;
   ntries=-1?fNTries:ntries;
   start=-1?fStart:start;
   stop=-1?fStop:stop;
   step=-1?fStep:step;
   debug=-1?fDebug:debug;
   draw=-1?fDraw:draw;

   if (!fListPerfProfiles){
      fListPerfProfiles=new TList();
   }

   Int_t quotient=(stop-start)/step;
   Int_t ndiv=quotient+1;
   Int_t ns_min=start;
   Int_t ns_max=quotient*step+start;

   //find perfstat profile
   TString profile_perfstat_event_name=BuildProfileName("hProf", "PerfStat_Event");
   TProfile* profile_perfstat_event=(TProfile*)(fListPerfProfiles->FindObject(profile_perfstat_event_name.Data()));

   //book one if one does not exists yet or reset the existing one
   if (!profile_perfstat_event){
      TString profile_perfstat_event_title=BuildProfileTitle("Profile", "PerfStat Event");
      profile_perfstat_event= new TProfile(profile_perfstat_event_name, profile_perfstat_event_title, ndiv, ns_min-0.5, ns_max+0.5);

      profile_perfstat_event->GetXaxis()->SetTitle("Number of Slaves");
      profile_perfstat_event->GetYaxis()->SetTitle("#times10^{3} Events/sec");
      profile_perfstat_event->SetMarkerStyle(21);

      fListPerfProfiles->Add(profile_perfstat_event);
   }
   else{
      profile_perfstat_event->Reset();
   }

   //file queryresult profile
   TString profile_queryresult_event_name=BuildProfileName("hProf", "QueryResult_Event");
   TProfile* profile_queryresult_event=(TProfile*)(fListPerfProfiles->FindObject(profile_queryresult_event_name.Data()));
 
   if (!profile_queryresult_event){
      TString profile_queryresult_event_title=BuildProfileTitle("Profile", "QueryResult Event");
      TProfile* profile_queryresult_event=new TProfile(profile_queryresult_event_name, profile_queryresult_event_title, ndiv, ns_min-0.5, ns_max+0.5);

      profile_queryresult_event->GetXaxis()->SetTitle("Number of Slaves");
      profile_queryresult_event->GetYaxis()->SetTitle("#times10^{3} Events/sec");
      profile_queryresult_event->SetMarkerStyle(22);

      fListPerfProfiles->Add(profile_queryresult_event);
   }
   else{
       profile_queryresult_event->Reset();
   }

   //get pad
   if (!fCPerfProfiles){
      fCPerfProfiles=new TCanvas("CPerfProfiles");
   }
   //divide the canvas as many as the number of profiles in the list
   Int_t nprofiles=fListPerfProfiles->GetSize();
   if (nprofiles<=2){
      fCPerfProfiles->Divide(nprofiles);
   }
   else{
      Int_t nside = (Int_t)TMath::Sqrt((Float_t)nprofiles);
      nside = (nside*nside<nprofiles)?nside+1:nside;
      fCPerfProfiles->Divide(nside,nside);
   }

   TString perfstats_name = "PROOF_PerfStats";

   SetParameters();

   //Delete the list of performance statistics trees
   fPerfStats->Delete();

   for (Int_t nactive=start; nactive<=stop; nactive+=step) {
      fProof->SetParallel(nactive);
      for (Int_t j=0; j<ntries; j++) {

         Int_t npad=1; //pad number

         TTime starttime = gSystem->Now();
         TTime endtime = gSystem->Now();

         fProof->Process("TSelHist", nevents);

         TList* l = fProof->GetOutputList();

         //save perfstats
         TTree* t = dynamic_cast<TTree*>(l->FindObject(perfstats_name.Data()));
         if (t) {

            FillPerfStatProfiles(t, profile_perfstat_event, nactive);
            fCPerfProfiles->cd(npad++);
            profile_perfstat_event->Draw();
            gPad->Update();

            t->SetDirectory(fDirProofBench);

            //build up new name
            TString newname=BuildNewPatternName(perfstats_name, nactive, j);
            t->SetName(newname);

            fPerfStats->Add(t);

            if (debug && fWritable){
               fDirProofBench->cd();
               t->Write();
            }
         } else {
            Error("RunBenchmark", "tree %s not found", perfstats_name.Data());
         }
         
         // Performance measures from TQueryResult

         TQueryResult* queryresult=fProof->GetQueryResult();  
         TDatime qr_start=queryresult->GetStartTime(); 
         TDatime qr_end=queryresult->GetEndTime(); 
         Float_t qr_init=queryresult->GetInitTime(); 
         Float_t qr_proc=queryresult->GetProcTime(); 
         //Float_t qr_usedcpu=queryresult->GetUsedCPU(); 
    
         Long64_t qr_entries=queryresult->GetEntries();

         // Calculate event rate
         Double_t qr_eventrate=qr_entries/Double_t(qr_init+qr_proc);

         // Build profile name
         TString profile_queryresult_event_name=BuildProfileName("hProf", "QueryResult_Event");
         // Get profile
         TProfile* profile_queryresult_event=(TProfile*)(fListPerfProfiles->FindObject(profile_queryresult_event_name.Data()));

         // Fill and draw
         if (profile_queryresult_event){
            profile_queryresult_event->Fill(nactive, qr_eventrate);
            fCPerfProfiles->cd(npad++);
            profile_queryresult_event->Draw();
            gPad->Update();
         }
         else{
            Error("Run", "Profile not found: %s", profile_queryresult_event_name.Data());
         }
      }//for iterations
   }//for number of workers
}

//______________________________________________________________________________
void TProofBenchRunCPU::FillPerfStatProfiles(TTree* t, TProfile* profile, Int_t nactive)
{

   // Fill performance profiles using tree 't'(PROOF_PerfStats).
   // Input parameters
   //    t: Proof output tree (PROOF_PerfStat) containing performance statistics.
   //    profile: Profile to be filled up with information from tree 't'.
   //    nactive: Number of active workers processed the query.
   // Return
   //    Nothing

   Int_t nevents_holder;
   Int_t bytes_holder;
   Float_t time_holder;

   // extract timing information
   TPerfEvent pe;
   TPerfEvent* pep = &pe;
   t->SetBranchAddress("PerfEvents",&pep);
   Long64_t entries = t->GetEntries();
   Double_t start(0), end(0);
   //Bool_t started=kFALSE;
      
   Long64_t nevents_kPacket=0;
   Long64_t nevents_kRate=0;
   Long64_t bytesread_kPacket=0;
   Long64_t bytesread_kRate=0;
   for (Long64_t k=0; k<entries; k++) {
      t->GetEntry(k);

      //Printf("k:%lld fTimeStamp=%lf fEvtNode=%s pe.fType=%d fSlaveName=%s fNodeName=%s fFileName=%s fFileClass=%s fSlave=%s fEventsProcessed=%lld fBytesRead=%lld fLen=%lld fLatency=%lf fProcTime=%lf fCpuTime=%lf fIsStart=%d fIsOk=%d",k, pe.fTimeStamp.GetSec() + 1e-9*pe.fTimeStamp.GetNanoSec(), pe.fEvtNode.Data(), pe.fType, pe.fSlaveName.Data(), pe.fNodeName.Data(), pe.fFileName.Data(), pe.fFileClass.Data(), pe.fSlave.Data(), pe.fEventsProcessed, pe.fBytesRead, pe.fLen, pe.fLatency, pe.fProcTime, pe.fCpuTime, pe.fIsStart, pe.fIsOk);

      if (pe.fType==TVirtualPerfStats::kPacket){
         nevents_kPacket+=pe.fEventsProcessed;
         bytesread_kPacket+=pe.fBytesRead;
      }
      if (pe.fType==TVirtualPerfStats::kRate){
         //printf("adding pe.fEventsProcessed=%lld\n", pe.fEventsProcessed);
         nevents_kRate+=pe.fEventsProcessed;
         bytesread_kRate+=pe.fBytesRead;
      }
        
         ///if (!started) {
          //  if (pe.fType==TVirtualPerfStats::kPacket) {
          //     start = pe.fTimeStamp.GetSec()
          //             + 1e-9*pe.fTimeStamp.GetNanoSec()
          //             - pe.fProcTime;
          //     started=kTRUE;
          //  }
         //} else {
         //   if (pe.fType==TVirtualPerfStats::kPacket) {
         //      end = pe.fTimeStamp.GetSec()
         //            + 1e-9*pe.fTimeStamp.GetNanoSec();
         //   }
         //}
         //skip information from workers
      if (pe.fEvtNode.Contains(".")) continue;
      if (pe.fType==TVirtualPerfStats::kStart) start= pe.fTimeStamp.GetSec()+1e-9*pe.fTimeStamp.GetNanoSec();
      if (pe.fType==TVirtualPerfStats::kStop) end= pe.fTimeStamp.GetSec()+1e-9*pe.fTimeStamp.GetNanoSec();
   }
     
   //if (nevents_kPacket!=fNEvents){
   //  Error("BuildTimingTree", "Number of events processed is different from the number of events in the file");
   // return 0;
   //}

   nevents_holder=nevents_kPacket;
   bytes_holder=bytesread_kPacket;
   time_holder = end-start;

   Double_t event_rate;

   event_rate=nevents_holder/time_holder/1000.; 
   profile->Fill(Double_t(nactive), event_rate);

   //if (fWritable){
   //   fProfEvent->Write();
   //}
   return;
}

//______________________________________________________________________________
void TProofBenchRunCPU::Print(Option_t* option)const{

   Printf("Name=%s", fName.Data());
   if (fProof) fProof->Print(option);
   Printf("fHistType=%s%s", "k", GetNameStem().Data());
   Printf("fNHists=%d", fNHists);
   Printf("fNEvents=%lld", fNEvents);
   Printf("fNTries=%d", fNTries);
   Printf("fMaxNWorkers=%d", fMaxNWorkers);
   Printf("fStart=%d", fStart);
   Printf("fStop=%d", fStop);
   Printf("fStep=%d", fStep);
   Printf("fDraw=%d", fDraw);
   Printf("fDebug=%d", fDebug);
   if (fFile){
       fFile->Print(option);
       fFile->ls(option);
   }
   else{
      Printf("No file open");
   }
   if (fDirProofBench){
      Printf("fDirProofBench=%s", fDirProofBench->GetPath());
   }
   if (fNodes) fNodes->Print(option);
   if (fPerfStats) fPerfStats->Print(option);
   if (fListPerfProfiles) fListPerfProfiles->Print(option);
   if (fCPerfProfiles){
      Printf("Performance Profiles Canvas: Name=%s Title=%s",
              fCPerfProfiles->GetName(), fCPerfProfiles->GetTitle());
   }
}

//______________________________________________________________________________
void TProofBenchRunCPU::DrawPerfProfiles()
{
   // Get canvas
   if (!fCPerfProfiles){
      fCPerfProfiles=new TCanvas("CPerfProfiles");
   }

   fCPerfProfiles->Clear();

   // Divide the canvas as many as the number of profiles in the list
   Int_t nprofiles=fListPerfProfiles->GetSize();
   if (nprofiles<=2){
      fCPerfProfiles->Divide(nprofiles);
   }
   else{
      Int_t nside = (Int_t)TMath::Sqrt((Float_t)nprofiles);
      nside = (nside*nside<nprofiles)?nside+1:nside;
      fCPerfProfiles->Divide(nside,nside);
   }

   Int_t npad=1;
   TIter nxt(fListPerfProfiles);
   TProfile* profile=0;
   while ((profile=(TProfile*)(nxt()))){
      fCPerfProfiles->cd(npad++);
      profile->Draw();
      gPad->Update();
   }
   return; 
}

//______________________________________________________________________________
void TProofBenchRunCPU::SetHistType(TProofBenchRun::EHistType histtype)
{
   fHistType=histtype;
   fName=GetNameStem()+"CPU";
}

//______________________________________________________________________________
void TProofBenchRunCPU::SetNHists(Int_t nhists)
{
   fNHists=nhists;
}

//______________________________________________________________________________
void TProofBenchRunCPU::SetNEvents(Long64_t nevents)
{
   fNEvents=nevents;
}

//______________________________________________________________________________
void TProofBenchRunCPU::SetNTries(Int_t ntries)
{
   fNTries=ntries;
}
   
//______________________________________________________________________________
void TProofBenchRunCPU::SetMaxNWorkers(Int_t maxnworkers)
{
  fMaxNWorkers=maxnworkers;
}

//______________________________________________________________________________
void TProofBenchRunCPU::SetMaxNWorkers(TString sworkers)
{

   // Set the maximum number of workers for benchmark test
   // Input parameters
   //    sworkers: can be "1x", "2x" and so on, where total number of workers is set 
   //              to 1*no_total_workers, 2*no_total_workers respectively.
   //              For now only "1x" is supported
   // Returns
   //    Nothing

   sworkers.ToLower();
   sworkers.Remove(TString::kTrailing, ' ');
   if (fProof){
      if (sworkers.Contains("x")){//nx
         TList* lslave=fProof->GetListOfSlaveInfos();
         Int_t nslaves=lslave->GetSize();  //number of slave workers regardless of its status, active or inactive
         sworkers.Remove(TString::kTrailing, 'x');
         Int_t mult=sworkers.Atoi();
         fMaxNWorkers=mult*nslaves; //this number to be parameterized in the future
      }
   }
   else{
      Error("SetMaxNWorkers", "Proof not set, doing nothing");
   }
   return;
}

//______________________________________________________________________________
void TProofBenchRunCPU::SetStart(Int_t start){
   fStart=start;
}

//______________________________________________________________________________
void TProofBenchRunCPU::SetStop(Int_t stop){
   fStop=stop;
}

//______________________________________________________________________________
void TProofBenchRunCPU::SetStep(Int_t step){
   fStep=step;
}

//______________________________________________________________________________
void TProofBenchRunCPU::SetDraw(Int_t draw)
{
   fDraw=draw;
}

//______________________________________________________________________________
void TProofBenchRunCPU::SetDebug(Int_t debug)
{
   fDebug=debug;
}

//______________________________________________________________________________
TFile* TProofBenchRunCPU::OpenFile(const char* filename,
                             Option_t* option,
                             const char* ftitle,
                             Int_t compress)
{
   // Opens a file which output profiles and/or intermediate files (trees, histograms when debug is set)
   // are to be written to. Makes a directory named "ProofBench" if possible and changes to the directory.
   // If directory ProofBench already exists, change to the directory. If the directory can not be created,
   // make a directory Rint:/ProofBench and change to the directory.
   // Input parameters:
   //    filename: Name of the file to open
   //    option: Option for TFile::Open(...) function
   //    ftitle: Title parameter for TFile::Open(...) function
   //    compress: Compression parameter for TFile::Open(...) function
   // Returns:
   //    Open file if a file is already open
   //    New file just opened
   //    0 when open fails;

   TString sfilename(filename);
   sfilename.Remove(TString::kBoth, ' '); //remove leading and trailing white space(s)
   sfilename.Remove(TString::kBoth, '\t');//remove leading and trailing tab character(s)

   //if (sfilename.Length()<1){
   //   return fFile;
   //}

   TString soption(option);
   soption.ToLower();

   if (fFile){
      Error("OpenFile", "File alaredy open; %s; Close it before open another file", fFile->GetName());
      return fFile;
   }

   TDirectory* dirsav=gDirectory;
   fFile=new TFile(sfilename, option, ftitle, compress);

   if (fFile->IsZombie()){//open failed
      Error("FileOpen", "Cannot open file: %s", sfilename.Data());
      fFile->Close();
      fFile=0;
      dirsav->cd();
      return 0;
   }
   else{//open succeeded
      fFile->mkdir("ProofBench");
      fFile->cd("ProofBench");
      SetDirProofBench(gDirectory);

      TString soption=fFile->GetOption();
      soption.ToLower();
      if (soption.Contains("create") || soption.Contains("update")){
         fWritable=1;
      }
      return fFile;
   }
}

//______________________________________________________________________________
void TProofBenchRunCPU::SetDirProofBench(TDirectory* dir)
{
   fDirProofBench=dir;
}

//______________________________________________________________________________
TProofBenchRun::EHistType TProofBenchRunCPU::GetHistType()const
{
   return fHistType;
}

//______________________________________________________________________________
Int_t TProofBenchRunCPU::GetNHists()const
{
   return fNHists;
}

//______________________________________________________________________________
Int_t TProofBenchRunCPU::GetNTries()const
{
   return fNTries;
}

//______________________________________________________________________________
Long64_t TProofBenchRunCPU::GetNEvents()const
{
   return fNEvents;
}

//______________________________________________________________________________
Int_t TProofBenchRunCPU::GetMaxNWorkers()const
{
   return fMaxNWorkers;
}

//______________________________________________________________________________
Int_t TProofBenchRunCPU::GetStart()const
{
   return fStart;
}

//______________________________________________________________________________
Int_t TProofBenchRunCPU::GetStop()const
{
   return fStop;
}

//______________________________________________________________________________
Int_t TProofBenchRunCPU::GetStep()const
{
   return fStep;
}

//______________________________________________________________________________
Int_t TProofBenchRunCPU::GetDraw()const
{
   return fDraw;
}

//______________________________________________________________________________
Int_t TProofBenchRunCPU::GetDebug()const
{
   return fDebug;
}

//______________________________________________________________________________
TFile* TProofBenchRunCPU::GetFile()const
{
   return fFile;
}

//______________________________________________________________________________
TDirectory* TProofBenchRunCPU::GetDirProofBench() const
{
   return fDirProofBench;
}

//______________________________________________________________________________
TList* TProofBenchRunCPU::GetListOfPerfStats()const
{
   return fPerfStats;
}  

//______________________________________________________________________________
TList* TProofBenchRunCPU::GetListPerfProfiles()const
{
   return fListPerfProfiles;
}  

//______________________________________________________________________________
TCanvas* TProofBenchRunCPU::GetCPerfProfiles() const
{
   return fCPerfProfiles;
}

//______________________________________________________________________________
const char* TProofBenchRunCPU::GetName()const
{
   return fName.Data();
}

//______________________________________________________________________________
TString TProofBenchRunCPU::GetNameStem()const
{
   TString namestem;
   switch (fHistType){
   case TProofBenchRun::kHist1D:
      namestem="Hist1D";
      break;
   case TProofBenchRun::kHist2D:
      namestem="Hist2D";
      break;
   case TProofBenchRun::kHist3D:
      namestem="Hist3D";
      break;
   case TProofBenchRun::kHistAll:
      namestem="HistAll";
      break;
   default:
      break;
   }
    return namestem;
}

//______________________________________________________________________________
Int_t TProofBenchRunCPU::FillNodeInfo()
{
   // Re-Generate the list of worker node info (fNodes)
   // (the existing info is always removed)
   // Return
   //     0 if ok
   //    <0 otherwise

   if (!fProof){
      Error("FillNodeInfo", "proof not set, doing nothing");
      return -1;
   }

   if (fNodes) {
      fNodes->SetOwner(kTRUE);
      SafeDelete(fNodes);
   }
   fNodes = new TList;
   fNodes->SetOwner();  //fNodes is owner of members
   // Get info
   TList *wl = fProof->GetListOfSlaveInfos();
   if (!wl) {
      Error("FillNodeInfo", "could not get information about workers!");
      return -2;
   }

   TIter nxwi(wl);
   TSlaveInfo *si = 0;
   TProofNode *ni = 0;
   while ((si = (TSlaveInfo *) nxwi())) {
      if (!(ni = (TProofNode *) fNodes->FindObject(si->GetName()))) {
         ni = new TProofNode(si->GetName(), si->GetSysInfo().fPhysRam);
         fNodes->Add(ni);
      } else {
         ni->AddWrks(1);
      }
   }
   // Notify
   Info("FillNodeInfo","%d physically different mahcines found", fNodes->GetSize());
   // Done
   return 0;
}

//______________________________________________________________________________
Int_t TProofBenchRunCPU::SetParameters(){
   if (!fProof){
      Error("SetParameters", "proof not set; Doing nothing");
      return 1;
   }

   fProof->SetParameter("PROOF_BenchmarkHistType", Int_t(fHistType));
   fProof->SetParameter("PROOF_BenchmarkNHists", fNHists);
   fProof->SetParameter("PROOF_BenchmarkDraw", Int_t(fDraw));
   return 0;
}

//______________________________________________________________________________
Int_t TProofBenchRunCPU::DeleteParameters(){
   if (!fProof){
      Error("DeleteParameters", "proof not set; Doing nothing");
      return 1;
   }
   fProof->DeleteParameters("PROOF_BenchmarkHistType");
   fProof->DeleteParameters("PROOF_BenchmarkNHists");
   fProof->DeleteParameters("PROOF_BenchmarkDraw");
   return 0;
}

//______________________________________________________________________________
TString TProofBenchRunCPU::BuildPatternName(const TString& objname, const TString& delimiter)
{
   TString newname(objname);
   newname+=delimiter;
   newname+=GetName();
   return newname;
}

//______________________________________________________________________________
TString TProofBenchRunCPU::BuildNewPatternName(const TString& objname, Int_t nactive, Int_t tries, const TString& delimiter)
{
   TString newname(BuildPatternName(objname, delimiter));
   newname+=delimiter;
   newname+=nactive;
   newname+="Slaves";
   newname+=delimiter;
   newname+="Run";
   newname+=tries;
   return newname;
}

//______________________________________________________________________________
TString TProofBenchRunCPU::BuildProfileName(const TString& objname, const TString& type, const TString& delimiter)
{
   TString newname(BuildPatternName(objname, delimiter));
   newname+=delimiter;
   newname+=type;
   return newname;
}

//______________________________________________________________________________
TString TProofBenchRunCPU::BuildProfileTitle(const TString& objname, const TString& type, const TString& delimiter)
{
   TString newname(BuildPatternName(objname, delimiter));
   newname+=delimiter;
   newname+=type;
   return newname;
}

