
// To run:
// > root -l 
// root> .L plotting.C
// root> plotting(input_root_file)
// root> .q

const int Nhad         = 2;
const char* nhad[Nhad] = {"pip","kap"};

void SetHistos(TH2D* hhIn){
  hhIn->SetStats(0);
  hhIn->GetXaxis()->SetRangeUser(0,31);
  hhIn->GetYaxis()->SetRangeUser(0,1000);
  hhIn->GetXaxis()->SetTitle("Momentum (GeV/c)");
  hhIn->GetYaxis()->SetTitle("Angle (mrad)");
}

void plotting(const char* cfIn, const char* ctag){

  TFile* fIn = new TFile(cfIn,"read");
  TH2D* hhad[Nhad];
  for(int i=0;i<Nhad;i++){
    hhad[i] = (TH2D*)fIn->Get(Form("h%s",nhad[i]));
    SetHistos(hhad[i]);
  }

  TCanvas* can[Nhad];
  for(int i=0;i<Nhad;i++){
    can[i] = new TCanvas();
    hhad[i]->Draw("colz");
    can[i]->SaveAs(Form("yield_%s_%s.pdf",nhad[i],ctag));
  }
  
}
