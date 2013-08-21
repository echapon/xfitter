#ifndef  DataSet_h
#define  DataSet_h

#include <TString.h>
#include <TObject.h>
#include <iostream>
#include <TGraphErrors.h>
#include <TObjArray.h>
#include <vector>
#include <TObjString.h>
#include <TOrdCollection.h>
#include <map>
#include <TH1F.h>

using std::cout;
using std::cerr;
using std::endl;
using std::map;
using std::vector;

struct SubPlot {
  TString* fTitle;
  TString* fPlotOption;
  TGraphErrors* fDUnc;
  TGraphErrors* fDTot;
  TGraphErrors* fTheo;
  TGraphErrors* fTMod;
  TH1F* fHistogram;
  vector <float> Bins1, Bins2, Data, Uncor, Toterr, Th, Thshift, Pull;
  bool fXlog;
  bool fYlog;
  SubPlot(const char* descriptor);
  virtual ~SubPlot();
  void AddPoint(double x, double bin1, double bin2, double data, double uncorrerr, double toterr, double theory, double theory_mod, double pull);
  void PrepareHistogram(bool RatioToData);
};


class  DataSet {
 private:    
  TString* fName;
  Int_t fSetId;
  map<int, SubPlot*> fSubPlots;

 public: 
  DataSet();
  DataSet(Int_t SetId, const Char_t* name);
  virtual ~DataSet();
  Int_t inline GetSetId() {return fSetId;};
  inline const Char_t* GetName() {return fName->Data();}
  
  void AddNewPlot(const char* descriptor);
  void AddPoint(const char* s, double bin1, double bin2, double data, double uncorrerr, double toterr, double theory, double theory_mod, double pull);
  inline int GetNSubPlots() {return fSubPlots.size();}
  
  TH1F* GetHistogram(int i, bool RatioToData);
  TGraphErrors* GetDataUnc(int i);
  TGraphErrors* GetDataTot(int i);
  TGraphErrors* GetTheo(int i);
  TGraphErrors* GetTMod(int i);
  bool          GetXlog(int i);
  bool          GetYlog(int i);
  vector <float>  getbins1(int idx);
  vector <float>  getbins2(int idx);
  vector <float>  getdata(int idx);
  vector <float>  getuncor(int idx);
  vector <float>  gettoterr(int idx);
  vector <float>  gettheory(int idx);
  vector <float>  gettheoryshifted(int idx);
  vector <float>  getpulls(int idx);
  
 private:
  SubPlot* GetSubPlot(int idx);
  
      };

#endif
