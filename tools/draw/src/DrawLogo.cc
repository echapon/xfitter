#include "DrawLogo.h"

#include "logo.xpm"

#include <TImage.h>
#include <TLatex.h>
#include <TEnv.h>
#include <TCanvas.h>

#include <string>
#include <iostream>
#include <sys/stat.h>

#include <CommandParser.h>

//label position
float labx;
float laby;
float xratio;

using namespace std;
TPad * DrawLogo(string pos)
{
  string ver = VERSION;

  TImage *logo = TImage::Create();
  logo->SetImageBuffer(logo_xpm, TImage::kXpm);
  if (!logo) 
    {
      cout << "Error, Could not find logo" << endl;
      return 0;
    }

  logo->SetConstRatio(1);
  logo->SetImageQuality(TAttImage::kImgBest);

  //Draw xFitter version on logo
  if (opts.version)
    {
      TString fp = gEnv->GetValue("Root.TTFontPath", "");
      TString font = fp + "/BlackChancery.ttf";
      //Check the font file exists
      struct stat st;
      if (stat(font,&st) != 0) //backup, arial font
	font = fp + "/DroidSansFallback.ttf";
      if (stat(font,&st) != 0)
	{
	  cout << "Warning, cannot find font: " << font << "; xFitter version cannot be drawn on logo "<<endl;
	  opts.version = false;
	}
      else 
	logo->DrawText(200, 510, ver.c_str(), 80, 0, 
		       font, TImage::kShadeBelow);
    }

  float dx = 0.0768 * 1.5;
  float dy = 0.0597 * 1.5;

  float x, y;
  x = 1-rmarg-0.01;
  y = 1-tmarg-0.01;
  if (pos == "bc")
    {
      x = 0.74;
      y = 0.12 + dy;
    }

  TPad * logopad = new TPad("logopad", "", x-dx, y-dy, x, y);
  logopad->SetBorderSize(0);
  logopad->Draw();
  logopad->cd();
  logo->Draw("");

  return logopad;
}

void CMS()
{
  TLatex l; //l.SetTextAlign(12);
  l.SetNDC();
  l.SetTextFont(42);
  l.SetTextSize(xratio*0.04);
  l.DrawLatex(xratio*(lmarg+0.05), 1-tmarg+0.01, "CMS");
}

void CMSpreliminary()
{
  TLatex l; //l.SetTextAlign(12);
  l.SetNDC();
  l.SetTextFont(42);
  l.SetTextSize(xratio*0.04);
  l.DrawLatex(xratio*(lmarg+0.05), 1-tmarg+0.01, "CMS Preliminary");
}

void ATLAS()
{
  TLatex l; //l.SetTextAlign(12);
  l.SetNDC();
  l.SetTextFont(72);
  l.SetTextSize(xratio*0.04);
  l.DrawLatex(xratio*(labx+0.05), laby-0.05, "ATLAS");
}

void ATLASpreliminary()
{
  TLatex p; 
  p.SetNDC();
  p.SetTextFont(42);
  p.SetTextSize(xratio*0.04);
  p.DrawLatex(xratio*(labx+0.19), laby-0.05, "Preliminary");
}

void ATLASinternal()
{
  TLatex p; 
  p.SetNDC();
  p.SetTextFont(42);
  p.SetTextSize(xratio*0.04);
  p.DrawLatex(xratio*(labx+0.19), laby-0.05, "Internal");
}

//CDF Run II Preliminary
TPaveText * CDFIIpreliminary()
{
  TPaveText *cdfii = new TPaveText(xratio*0.45, 1-tmarg+0.02, xratio*0.95, 0.96, "NDC");
  cdfii->AddText("CDF Run II Preliminary");
  cdfii->SetTextAlign(12);
  cdfii->SetTextFont(62);
  cdfii->SetTextSize(xratio*0.04);
  cdfii->SetBorderSize(0);
  cdfii->SetFillColor(0);
  return cdfii;
}

void DrawLabels(string pos)
{
  labx = lmarg;
  laby = 1-tmarg;
  if (pos.find("ur") != string::npos)
    if (opts.atlas)
      labx = 1. - rmarg - 0.19;
    else
      labx = 1. - rmarg - 0.19 - 0.20;

  if (pos.find("bc") != string::npos)
    {
      labx = lmarg + 0.3;
      laby = bmarg + 0.05 + 0.03;
    }

  xratio = 1.;
  if (pos.find("half") != string::npos)
    xratio = 0.5;
    
  if (opts.cms)
    CMS();
  if (opts.cmspreliminary)
    CMSpreliminary();
  if (opts.atlasinternal)
    {
      ATLAS();
      ATLASinternal();
    }
  else if (opts.atlaspreliminary)
    {
      ATLAS();
      ATLASpreliminary();
    }
  else if (opts.atlas)
    ATLAS();
  if (opts.cdfiipreliminary)
    CDFIIpreliminary()->Draw();
}
