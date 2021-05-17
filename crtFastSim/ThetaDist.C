{
//========= Macro generated from object: h1muonTheta/ muon Theta 
//========= by ROOT version5.18/00b
   
   TH1 *h1muonTheta = new TH1D("h1muonTheta"," muon Theta ",360,0,360);
   h1muonTheta->SetBinContent(111,332);
   h1muonTheta->SetBinContent(112,313);
   h1muonTheta->SetBinContent(113,367);
   h1muonTheta->SetBinContent(114,410);
   h1muonTheta->SetBinContent(115,442);
   h1muonTheta->SetBinContent(116,465);
   h1muonTheta->SetBinContent(117,490);
   h1muonTheta->SetBinContent(118,511);
   h1muonTheta->SetBinContent(119,559);
   h1muonTheta->SetBinContent(120,614);
   h1muonTheta->SetBinContent(121,601);
   h1muonTheta->SetBinContent(122,685);
   h1muonTheta->SetBinContent(123,675);
   h1muonTheta->SetBinContent(124,726);
   h1muonTheta->SetBinContent(125,773);
   h1muonTheta->SetBinContent(126,796);
   h1muonTheta->SetBinContent(127,873);
   h1muonTheta->SetBinContent(128,929);
   h1muonTheta->SetBinContent(129,926);
   h1muonTheta->SetBinContent(130,940);
   h1muonTheta->SetBinContent(131,957);
   h1muonTheta->SetBinContent(132,1048);
   h1muonTheta->SetBinContent(133,1030);
   h1muonTheta->SetBinContent(134,1138);
   h1muonTheta->SetBinContent(135,1109);
   h1muonTheta->SetBinContent(136,1184);
   h1muonTheta->SetBinContent(137,1154);
   h1muonTheta->SetBinContent(138,1276);
   h1muonTheta->SetBinContent(139,1323);
   h1muonTheta->SetBinContent(140,1363);
   h1muonTheta->SetBinContent(141,1344);
   h1muonTheta->SetBinContent(142,1396);
   h1muonTheta->SetBinContent(143,1387);
   h1muonTheta->SetBinContent(144,1517);
   h1muonTheta->SetBinContent(145,1422);
   h1muonTheta->SetBinContent(146,1540);
   h1muonTheta->SetBinContent(147,1708);
   h1muonTheta->SetBinContent(148,1614);
   h1muonTheta->SetBinContent(149,1641);
   h1muonTheta->SetBinContent(150,1679);
   h1muonTheta->SetBinContent(151,1707);
   h1muonTheta->SetBinContent(152,1740);
   h1muonTheta->SetBinContent(153,1776);
   h1muonTheta->SetBinContent(154,1837);
   h1muonTheta->SetBinContent(155,1827);
   h1muonTheta->SetBinContent(156,1836);
   h1muonTheta->SetBinContent(157,1919);
   h1muonTheta->SetBinContent(158,1872);
   h1muonTheta->SetBinContent(159,1885);
   h1muonTheta->SetBinContent(160,2005);
   h1muonTheta->SetBinContent(161,1907);
   h1muonTheta->SetBinContent(162,1976);
   h1muonTheta->SetBinContent(163,1984);
   h1muonTheta->SetBinContent(164,2026);
   h1muonTheta->SetBinContent(165,2113);
   h1muonTheta->SetBinContent(166,2111);
   h1muonTheta->SetBinContent(167,2082);
   h1muonTheta->SetBinContent(168,2003);
   h1muonTheta->SetBinContent(169,2136);
   h1muonTheta->SetBinContent(170,2133);
   h1muonTheta->SetBinContent(171,2112);
   h1muonTheta->SetBinContent(172,2110);
   h1muonTheta->SetBinContent(173,2239);
   h1muonTheta->SetBinContent(174,2257);
   h1muonTheta->SetBinContent(175,2110);
   h1muonTheta->SetBinContent(176,2181);
   h1muonTheta->SetBinContent(177,2217);
   h1muonTheta->SetBinContent(178,2287);
   h1muonTheta->SetBinContent(179,2215);
   h1muonTheta->SetBinContent(180,2140);
   h1muonTheta->SetEntries(100000);
   
   TPaveStats *ptstats = new TPaveStats(0.77939,0.952891,0.979681,0.995717,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(2);
   ptstats->SetFillColor(19);
   ptstats->SetTextAlign(12);
   TText *text = ptstats->AddText("h1muonTheta");
   text->SetTextSize(0.00985011);
   text = ptstats->AddText("Entries = 100000 ");
   text = ptstats->AddText("Mean  =  153.6");
   text = ptstats->AddText("RMS   =  17.71");
   ptstats->SetOptStat(1111);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   h1muonTheta->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(h1muonTheta->GetListOfFunctions());
   h1muonTheta->GetXaxis()->SetRange(109,183);


   //TF1 *f1 = new TF1("f1","[0]*(cos(x*[1]))^1.85",109.0/57.0,180.0/57.0);
   TF1 *f1 = new TF1("f1","[0]*(cos(x*[1]))*(cos(x*[1]))",109.0/57.0,180.0/57.0);
   f1->SetParameter(1,1.0/57.0);
   h1muonTheta->Fit(f1);


   h1muonTheta->Draw("");
}
