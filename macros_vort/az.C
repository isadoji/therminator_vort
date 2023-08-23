{
TFile * ther0 =new TFile("pola.root");
TTree * arbolT0 = (TTree *)ther0->Get("pol");
float xmax,xmin;
xmin=0;
xmax=TMath::Pi();

TH1F * h0 = new TH1F("h0", "h0", 100,xmin,xmax);
TH1F * h0r = new TH1F("h0r", "h0r", 100,xmin,xmax);
TH1F * ptH = new TH1F("ptH", "ptH",100,xmin,xmax);


arbolT0->Project("h0", "TMath::ACos(x/sqrt(x*x+y*y))", "(Sy-(St*e-Sx*px-Sy*py-Sz*pz)*py/(e*(e+m)))*(anti==1)");
arbolT0->Project("h0r", "TMath::ACos(x/sqrt(x*x+y*y))", "sqrt(St*e-(Sx*Sx+Sy*Sy+Sz*Sz))");
ptH->Divide(h0,h0r);


TCanvas *c1 = new TCanvas("c1", "pt particles distribution",50,50,500,500);
 gStyle->SetOptStat(false);
 gStyle->SetOptTitle(0);
 gStyle->SetPalette(1);
 c1->SetRightMargin(0.0465116);
 c1->SetTopMargin(0.39916);
 c1->SetFillColor(0);
 TPad *pt_1 = new TPad("pt_1", "pt_1",0.0,0.0,0.98,0.98);
 pt_1->Draw();
 pt_1->cd();
 //pt_1->Range(-20.6795,-0.0133333,105.496,0.062549);
 pt_1->SetFillColor(0);
 pt_1->SetBorderSize(2);
 pt_1->SetLeftMargin(0.17);
 pt_1->SetRightMargin(0.05);
 pt_1->SetTopMargin(0.1);
 pt_1->SetBottomMargin(0.15);
 pt_1->SetFrameLineWidth(2);
 pt_1->SetFrameLineWidth(2);
 
 ptH->Draw();
 ptH->SetMarkerColor(2);
 ptH->SetLineColor(2);
 ptH->SetMarkerStyle(24);
 ptH->SetMarkerSize(0.7);
 ptH->SetTitle("Polarizacion en y para #Lambda");
 ptH->GetXaxis()->SetTitle("#phi ");
 ptH->GetXaxis()->CenterTitle(true);
 ptH->GetXaxis()->SetTitleSize(0.05);
 ptH->GetXaxis()->SetLabelSize(0.045);
 ptH->GetXaxis()->SetTitleOffset(1.3);
 ptH->GetYaxis()->SetTitle("P_{y}");
 ptH->GetYaxis()->CenterTitle(true);
 ptH->GetYaxis()->SetTitleSize(0.05);
 ptH->GetYaxis()->SetLabelSize(0.045);
 ptH->GetYaxis()->SetTitleOffset(1.5);


 TLegend *leg1 = new TLegend(0.79,0.79,0.91,0.89);
 leg1->SetTextFont(62);
 leg1->SetTextSize(0.033);
 leg1->SetLineColor(0);
 leg1->SetLineStyle(0);
 leg1->SetLineWidth(1);
 leg1->SetFillColor(0);
 leg1->SetFillStyle(1001);
 //leg1->AddEntry("", "Valor de #omega","");
 leg1->AddEntry("","Polarizacion","");
 leg1->AddEntry("","en y para #Lambda","");
 leg1->AddEntry(ptH,"archivo","p");
 leg1->Draw();


}
