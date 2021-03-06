 
/*
   @file ReactionHVQMNR_LHCb_7TeV_beauty.cc
   @date 2017-01-02
   @author Oleksandr Zenaiev (oleksandr.zenaiev@desy.de)
   Created by  AddReaction.py on 2017-01-02

   Derived from ReactionBaseHVQMNR where basic stuff for HVQMNR calculation is implemented.
   This class implements calculation for LHCb beauty measurement at 7 TeV 
   [JHEP 1308 (2013) 117] [arXiv:1306.3663]
*/

#include "ReactionHVQMNR_LHCb_7TeV_beauty.h"
#include "xfitter_cpp.h"
#include <TF1.h>
#include <TH2D.h>
#include <TMath.h>

// the class factories
extern "C" ReactionHVQMNR_LHCb_7TeV_beauty* create() {
  return new ReactionHVQMNR_LHCb_7TeV_beauty();
}


// initialize at the start of the computation
int ReactionHVQMNR_LHCb_7TeV_beauty::initAtStart(const string &s)
{
  // ignore provided terminfo (s): all needed information has been set already 
  // via setDatasetParamters(int dataSetID, map<string,string> pars)
  
  // ******************************************************************
  // perform initialisation and pre-calculation
  // ******************************************************************
  // protection against overdoing
  if(_isInitAtStart)
    return 0;
  _isInitAtStart = true;
  //printf("ReactionHVQMNR_LHCb_7TeV_beauty::initAtStart()\n");

  // check HF scheme
  CheckHFScheme();
    
  // read needed theory parameters
  UpdateParameters();
  PrintParameters();

  // stereing parameters for this calculation (modify only if you understand what you are doing)
  Steering steer;
  steer.ptmin = 0.001;
  steer.ptmax = 70.0;
  steer.npt   = 35;
  steer.nptsm = 500;
  steer.ymin  = 2.0;
  steer.ymax  = 5.0;
  steer.ny    = 50;
  steer.nsfnb = 500;
  steer.nx3   = 125;
  steer.nx4   = 125;
  steer.nbz   = 100;
    
  DefaultInit(steer, _pars.mb, _mnr, _frag, _grid, _gridSmoothed);
  //if(_debug)
    printf("ReactionHVQMNR_LHCb_7TeV_beauty::initAtStart(): at initialisation mb = %f\n", _pars.mb);
  // MNR (parton-level calculation)
  _mnr.SetDebug(_debug);
  _mnr.fC_sh = TMath::Power(7000.0, 2.0); // centre-of-mass energy squared
  _mnr.CalcConstants();
  // fragmentation
  _frag.SetDebug(_debug);
  // add needed final states
  _frag.AddOut(MNR::Frag::GetFragFunction(0, "", _pars.fragpar_b), MNR::Frag::fM_bch);
  _frag.AddOut(MNR::Frag::GetFragFunction(0, "", _pars.fragpar_b), MNR::Frag::fM_bzero);
  _frag.AddOut(MNR::Frag::GetFragFunction(0, "", _pars.fragpar_b), MNR::Frag::fM_bs);

  // set binning for cross-section histograms
  _hCalculatedXSec.resize(3);
  for(int f = 0; f < 3; f++)
    _hCalculatedXSec[f] = new TH2D;
  int nbin_y = 5;
  double bin_y[6] = { 2.0, 2.5, 3.0, 3.5, 4.0, 4.5 };
  // B+
  int nbin_pt_bch = 27;
  double bin_pt_bch[28] = {0.0,0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.0,8.5,9.0,9.5,10.0,10.5,11.5,12.5,14.0,16.5,23.5,40.0};
  _hCalculatedXSec[0]->SetBins(nbin_pt_bch, bin_pt_bch, nbin_y, bin_y);
  // B0
  int nbin_pt_bzero = 19;
  double bin_pt_bzero[20] = {0.0,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.0,9.0,10.0,11.5,14.0,19.5,40.0};
  _hCalculatedXSec[1]->SetBins(nbin_pt_bzero, bin_pt_bzero, nbin_y, bin_y);
  // Bs
  int nbin_pt_bs = 15;
  double bin_pt_bs[16] = {0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,13.0,15.0,19.0,40.0};
  _hCalculatedXSec[2]->SetBins(nbin_pt_bs, bin_pt_bs, nbin_y, bin_y);
      
  return 0;
}


// perform calculation (this is done once per iteration)
void ReactionHVQMNR_LHCb_7TeV_beauty::initAtIteration() 
{
  // protection against overdoing
  // TODO: remove this trick
  //if(_ifcncount_last == cfcn_.ifcncount)
  //  return;
  //_ifcncount_last = cfcn_.ifcncount;

  //printf("ReactionHVQMNR_LHCb_7TeV_beauty::initAtIteration() %d \n", cfcn_.ifcncount);

  // read needed MINUIT parameters (enough to be done once per iteration)
  UpdateParameters();
  if(_debug)
    PrintParameters();

  // update parameters and perform calculation
  _mnr.SetScaleCoef(_pars.mf_A_b, _pars.mf_B_b, _pars.mf_C_b, _pars.mr_A_b, _pars.mr_B_b, _pars.mr_C_b);
  _mnr.CalcXS(&_grid, _pars.mb);
  MNR::Grid::InterpolateGrid(&_grid, &_gridSmoothed, _pars.mb);
  for(int f = 0; f < 3; f++)
    _frag.GetFF(f)->SetParameter(1, _pars.fragpar_b);
  _frag.CalcCS(&_gridSmoothed, _pars.mb, _hCalculatedXSec);
}


// main function to compute results at an iteration
int ReactionHVQMNR_LHCb_7TeV_beauty::compute(int dataSetID, valarray<double> &val, map<string, valarray<double> > &err)
{
  // TODO move to core xFitter
  //initAtIteration();
  //printf("ReactionHVQMNR_LHCb_7TeV_beauty::compute() %d\n", dataSetID);
  
  // get histogramm with cross sections for needed dataset
  DataSet& ds = _dataSets[dataSetID];
  TH2D* histXSec = NULL;
  if(ds.FinalState == "bch")
    histXSec = _hCalculatedXSec[0];
  else if(ds.FinalState == "bzero")
    histXSec = _hCalculatedXSec[1];
  else if(ds.FinalState == "bs")
    histXSec = _hCalculatedXSec[2];
  else
    error(16123005, "F: in HVQMNR_LHCb_7tev_beauty(): unknown FinalState" + ds.FinalState);

  // match bins and fill cross section array
  bool diffPt = true;
  bool diffY = true;
  for(unsigned int i = 0; i < ds.BinsYMin->size(); i++)
  {
    val[i] = FindXSecPtYBin(histXSec, (*ds.BinsYMin)[i], (*ds.BinsYMax)[i], (*ds.BinsPtMin)[i], (*ds.BinsPtMax)[i], diffPt, diffY);
    // normalise to another bin if needed
    if(ds.NormY == 1)
      val[i] = val[i] / FindXSecPtYBin(histXSec, (*ds.BinsYMinRef)[i], (*ds.BinsYMaxRef)[i], (*ds.BinsPtMin)[i], (*ds.BinsPtMax)[i], diffPt, diffY);
    // ... or scale to fragmentation fraction
    else
      val[i] = val[i] * ds.FragFraction;
  }
        
  return 0;
}
