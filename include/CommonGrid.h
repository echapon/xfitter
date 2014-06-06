/**
  @class CommonGrid

  @brief A class for general grid representation

  Eh, what does it do?

  @author A.Sapronov <sapronov@ifh.de>

  @version 1.0

  @date 2014/03/25
 */
#ifndef CommonGrid_h
#define CommonGrid_h 1

#include <list>
#include <map>
#include <string>
#include <valarray>
#include <vector>

#include "appl_grid/appl_grid.h"

using namespace std;
using namespace appl;

struct tHyperBin {
  double *b;
  int ngb;
  appl::grid *g;
};

class CommonGrid {
 private:
  //! Hidden default constructor
  CommonGrid(){};
 public:
  ~CommonGrid();
  //! Proper constructor
  /*!
   \param grid_type the type of grid
   \param grid_source shows from where the grid should be read
  */
  CommonGrid(const string & grid_type, const string &grid_source);

  //! Selects if we have a proton-antiproton collision
  void SetCollisions(int ppbar) {_ppbar = (ppbar == 1);};
  void SetDynamicScale(float dynscale) {_dynamicscale = dynscale;};
  //! Check that the data and grid bins are consistent
  int checkBins(vector<int> &bin_flags, vector<vector<double> > &data_bins);

  //! Convolute the PDFs and return the cross section
  std::vector<double> vconvolute(const int iorder, const double mur, const double muf);

  //! Set custom CKM matrix for APPLgrid
  /*!
    \param v_ckm the 3x3 CMK matrix to be set in the APPLgrid

    The CKM matrix values used in APPLgrids calculations can be updated
    here.
   */
  int setCKM(const vector<double> &v_ckm);
 
 private:
  //! Read the applgrid
  int readAPPLgrid(const string &grid_source);
  //! Read the virtual grid
  int readVirtGrid(const string &grid_source);

 private:
  int _ndim;
  /*!
   The _flag is a binary value:
    - first bin: 0/1 - standard/normalization grid
    - second bin: 0/1 - applgrid/virtgrid selection.
   For example, if datafile requests 'virtgrid_norm' option, the flag
   will be '3'
  */
  unsigned short _flag;
  vector<tHyperBin> _hbins;

  /// ppbar PDF
  bool _ppbar;

  /// bin-by-bin dynamic scale
  float _dynamicscale;
};

#endif
