

const int Nnu           = 4;
const char* namenu[Nnu] = {"nue","nuebar","numu","numubar"};

const int Nint            = 7;
const char* nameint[Nint] = {"tot","pBepi_HARP","pBepi_NoHARP","pBeK","pBenuc","incmes","other"};

const char* legint[Nint] = {"Total","pBe->#pi^{+} (HARP)","pBe->#pi^{#pm} (No-HARP)","pBe->K","pBe->nucl","Inc. meson","Other"};

const int col[Nint]       = {1, kRed-2, kOrange-3, kBlue-2, 28, kYellow+2,kCyan+2};

void SetHistos(TH1D* hhIn, int icol){
  hhIn->SetLineColor(icol);
  hhIn->SetLineWidth(4);
  hhIn->SetStats(0);
  hhIn->GetXaxis()->SetRangeUser(0,4);
  hhIn->GetYaxis()->SetRangeUser(0,2.25);
  hhIn->GetXaxis()->SetTitle("Neutrino energy (GeV)");
  hhIn->GetYaxis()->SetTitle("<hadronic interactions>/ #nu");
}

void Plotting(){

  // Getting
  TFile* fIn = new TFile("test.root","read");
  TH1D* hflux[Nnu];
  TH1D* hhad[Nnu][Nint];
  for(int ii=0;ii<Nnu;ii++){
    hflux[ii] = (TH1D*)fIn->Get(Form("%s/hflux_%s",namenu[ii],namenu[ii]));
    for(int jj=0;jj<Nint;jj++){
      hhad[ii][jj] = (TH1D*)fIn->Get(Form("%s/hhad_%s_%s",namenu[ii],namenu[ii],nameint[jj]));
      hhad[ii][jj]->Divide(hflux[ii]);
    }
  }
  
  //Plotting #nu_{#mu}
  TCanvas* cnumu = new TCanvas();
  TLegend* lnumu = new TLegend(0.65,0.57,0.84,0.88);
  lnumu->SetBorderSize(0);
  
  for(int jj=0;jj<Nint;jj++){
    SetHistos(hhad[2][jj], col[jj]);
    //
    if(jj==0)hhad[2][jj]->Draw("hist");
    else     hhad[2][jj]->Draw("histsame");
    //
    lnumu->AddEntry(hhad[2][jj],legint[jj],"l");
  }
  lnumu->Draw();
  
  //Plotting #nu_{e}
  TCanvas* cnue = new TCanvas();
  TLegend* lnue = new TLegend(0.65,0.57,0.84,0.88);
  lnue->SetBorderSize(0);
  
  for(int jj=0;jj<Nint;jj++){
    SetHistos(hhad[0][jj], col[jj]);
    //
    if(jj==0)hhad[0][jj]->Draw("hist");
    else    hhad[0][jj]->Draw("histsame");
    //
    lnue->AddEntry(hhad[0][jj],legint[jj],"l");
  }
  lnue->Draw();
 
 //Plotting #bar{#nu}_{e}
  TCanvas* cnueb = new TCanvas();
  TLegend* lnueb = new TLegend(0.65,0.57,0.84,0.88);
  lnueb->SetBorderSize(0);
  
  for(int jj=0;jj<Nint;jj++){
    SetHistos(hhad[1][jj], col[jj]);
    //
    if(jj==0)hhad[1][jj]->Draw("hist");
    else     hhad[1][jj]->Draw("histsame");
    //
    lnueb->AddEntry(hhad[1][jj],legint[jj],"l");
  }
  lnueb->Draw();
  
}
