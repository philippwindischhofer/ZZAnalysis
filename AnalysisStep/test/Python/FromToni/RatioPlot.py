#!/usr/bin/env python

import ROOT
from ROOT import *

ROOT.gROOT.SetBatch()
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptTitle(0)

lumi = 41.37

def DrawRatioPlot(name, xaxis_title, yaxis_title, c, data, MC, MCUp, MCDn):
	c.Divide(0,2,0,0)
	c.cd(1)
	
	max = 0.
	for bin in range (data.GetSize() - 2):
		if ( data.GetBinContent(bin + 1) > max):
			max = data.GetBinContent(bin + 1)

	MC.SetMaximum(1.4*max)

	MC.SetFillColor(kOrange + 1)
	MC.GetYaxis().SetTitle(yaxis_title)
	MC.Draw("HIST")

	data.SetLineColor(ROOT.kBlack)
	data.SetMarkerStyle(20)
	data.SetMarkerSize(0.6)
	data.Draw("p E1 X0 SAME")

	c.cd(2)

	Ratio    = TH1F("Ratio","Ratio",MC.GetSize() - 2, MC.GetXaxis().GetXmin(),MC.GetXaxis().GetXmax())
	sigma_up = TH1F("sigma_up","sigma_up",MC.GetSize() - 2, MC.GetXaxis().GetXmin(),MC.GetXaxis().GetXmax())
	sigma_dn = TH1F("sigma_dn","sigma_dn",MC.GetSize() - 2, MC.GetXaxis().GetXmin(),MC.GetXaxis().GetXmax())
	Ratio.GetXaxis().SetTitle(xaxis_title)
	Ratio.GetYaxis().SetTitle("(Data/MC) - 1")

	for bin in range (MC.GetSize() - 2):
		if (MC.GetBinContent(bin + 1) == 0):
			Ratio   .SetBinContent(bin + 1, 0)
			Ratio   .SetBinError(bin + 1, 0)
			sigma_up.SetBinContent(bin + 1, 0)
			sigma_dn.SetBinContent(bin + 1, 0)
		else:
			Ratio   .SetBinContent(bin + 1, float(data.GetBinContent(bin + 1))/MC.GetBinContent(bin + 1))
			Ratio   .SetBinError  (bin + 1, 1./(data.GetBinContent(bin + 1))**0.5)
			sigma_up.SetBinContent(bin + 1, float(MCUp.GetBinContent(bin + 1))/MC.GetBinContent(bin + 1))
			sigma_dn.SetBinContent(bin + 1, float(MCDn.GetBinContent(bin + 1))/MC.GetBinContent(bin + 1))

	for bin in range (Ratio.GetSize() - 2):
		temp = Ratio.GetBinContent(bin + 1)
		Ratio.SetBinContent( bin + 1, temp - 1)

	for bin in range (sigma_up.GetSize() - 2):
		temp = sigma_up.GetBinContent(bin + 1)
		sigma_up.SetBinContent( bin + 1, temp - 1)

	for bin in range (sigma_dn.GetSize() - 2):
		temp = sigma_dn.GetBinContent(bin + 1)
		sigma_dn.SetBinContent( bin + 1, temp - 1)

	sigma_up.SetFillColor(ROOT.kGray)
	sigma_up.SetLineColor(ROOT.kGray)
	sigma_up.Draw("HIST SAME")
	sigma_dn.SetFillColor(ROOT.kGray)
	sigma_dn.SetLineColor(ROOT.kGray)
	sigma_dn.Draw("HIST SAME")
	sigma_up.SetMaximum(1.0)
	sigma_up.SetMinimum(-1.0)
	Ratio.SetMarkerStyle(20)
	Ratio.SetMarkerSize(0.6)
	Ratio.Draw("p E1 X0 SAME")

	c.SaveAs(name+".pdf")
	c.SaveAs(name+".png")

	c.Clear()



DIR = "/data_CMS/cms/wind/"

f1 = ROOT.TFile.Open("/data_CMS/cms/wind/MC_180218/DYJetsToLL_M50/ZZ4lAnalysis.root")
tDY = f1.Get("ZTree/candTree")
SumWeight = (f1.Get("ZTree/Counters")).GetBinContent(1)

f11 = ROOT.TFile.Open("/data_CMS/cms/wind/MC_180218/TTTo2L2Nu/ZZ4lAnalysis.root")
tTTbar = f11.Get("ZTree/candTree")
SumWeight_tt = (f11.Get("ZTree/Counters")).GetBinContent(1)

f2 = ROOT.TFile.Open("/data_CMS/cms/wind/MC_180218/AllData/ZZ4lAnalysis.root")
tdata = f2.Get("ZTree/candTree")


leadingJet_Eta_data     = TH1F("leadingJet_Eta_data","leadingJet_Eta_data",      47,-4.7,4.7)
leadingJet_Eta_MC       = TH1F("leadingJet_Eta_MC","leadingJet_Eta_MC",          47,-4.7,4.7)
leadingJet_Eta_MCup     = TH1F("leadingJet_Eta_MCup","leadingJet_Eta_MCup",      47,-4.7,4.7)
leadingJet_Eta_MCdn     = TH1F("leadingJet_Eta_MCdn","leadingJet_Eta_MCdn",      47,-4.7,4.7)

nJetsPt30_data     = TH1I("nJetsPt30_data","nJetsPt30_data",      6,0,6)
nJetsPt30_MC       = TH1I("nJetsPt30_MC","nJetsPt30_MC",          6,0,6)
nJetsPt30_MCup     = TH1I("nJetsPt30_MCup","nJetsPt30_MCup",      6,0,6)
nJetsPt30_MCdn     = TH1I("nJetsPt30_MCdn","nJetsPt30_MCdn",      6,0,6)

PFMET_data     = TH1F("PFMET_data","PFMET_data",      20,0,200)
PFMET_MC       = TH1F("PFMET_MC","PFMET_MC",          20,0,200)
PFMET_MCup     = TH1F("PFMET_MCup","PFMET_MCup",      20,0,200)
PFMET_MCdn     = TH1F("PFMET_MCdn","PFMET_MCdn",      20,0,200)

ZMass_data     = TH1F("ZMass_data","ZMass_data",      40,70,110)
ZMass_MC       = TH1F("ZMass_MC","ZMass_MC",          40,70,110)


for event in tDY:
	nJetsUp = 0
	nJetsDn = 0
	highest_up = 0.
	highest_dn = 0.
	i_up = 0
	i_dn = 0
	if (event.LepPt.at(0) < 30 or event.LepPt.at(1) < 30. or abs(event.LepLepId.at(0))!=11):
		continue
	if (event.ZMass < 70 or event.ZMass > 110):
		continue
	for jet in range(event.JetPt.size()):
		if (event.JetJERUp.at(jet)*(1+event.JetSigma.at(jet)) > highest_up):
			highest_up = event.JetJERUp.at(jet)*(1+event.JetSigma.at(jet))
			i_up = jet
		if (event.JetJERDown.at(jet)*(1-event.JetSigma.at(jet)) > highest_dn):
			highest_dn = event.JetJERDown.at(jet)*(1-event.JetSigma.at(jet))
			i_dn = jet
		if ( event.JetJERUp.at(jet)*(1+event.JetSigma.at(jet)) > 30.):
			nJetsUp += 1
		if ( event.JetJERDown.at(jet)*(1-event.JetSigma.at(jet)) > 30.):
			nJetsDn += 1

	if (event.JetPt.size() > 0 and event.JetPt.at(0) > 30.):
		leadingJet_Eta_MC.Fill(event.JetEta.at(0),event.overallEventWeight*event.xsec*1000*lumi/SumWeight)
	if (event.JetPt.size() > 0 and event.JetJERUp.at(i_up)*(1+event.JetSigma.at(i_up)) > 30.):
		leadingJet_Eta_MCup.Fill(event.JetEta.at(i_up),event.overallEventWeight*event.xsec*1000*lumi/SumWeight)
	if (event.JetPt.size() > 0 and event.JetJERDown.at(i_dn)*(1-event.JetSigma.at(i_up)) > 30.):
		leadingJet_Eta_MCdn.Fill(event.JetEta.at(i_dn),event.overallEventWeight*event.xsec*1000*lumi/SumWeight)

	nJetsPt30_MC.Fill(event.nCleanedJetsPt30,event.overallEventWeight*event.xsec*1000*lumi/SumWeight)
	nJetsPt30_MCup.Fill(nJetsUp,event.overallEventWeight*event.xsec*1000*lumi/SumWeight)
	nJetsPt30_MCdn.Fill(nJetsDn,event.overallEventWeight*event.xsec*1000*lumi/SumWeight)

	PFMET_MC.Fill(event.PFMET,event.overallEventWeight*event.xsec*1000*lumi/SumWeight)
	PFMET_MCup.Fill(event.PFMET_jesUp,event.overallEventWeight*event.xsec*1000*lumi/SumWeight)
	PFMET_MCdn.Fill(event.PFMET_jesDn,event.overallEventWeight*event.xsec*1000*lumi/SumWeight)

	ZMass_MC.Fill(event.ZMass,event.overallEventWeight*event.xsec*1000*lumi/SumWeight)

for event in tTTbar:
	nJetsUp = 0
	nJetsDn = 0
	highest_up = 0.
	highest_dn = 0.
	i_up = 0
	i_dn = 0
	if (event.LepPt.at(0) < 30 or event.LepPt.at(1) < 30. or abs(event.LepLepId.at(0))!=11):
		continue
	if (event.ZMass < 70 or event.ZMass > 110):
		continue
	for jet in range(event.JetPt.size()):
		if (event.JetJERUp.at(jet)*(1+event.JetSigma.at(jet)) > highest_up):
			highest_up = event.JetJERUp.at(jet)*(1+event.JetSigma.at(jet))
			i_up = jet
		if (event.JetJERDown.at(jet)*(1-event.JetSigma.at(jet)) > highest_dn):
			highest_dn = event.JetJERDown.at(jet)*(1-event.JetSigma.at(jet))
			i_dn = jet
		if ( event.JetJERUp.at(jet)*(1+event.JetSigma.at(jet)) > 30.):
			nJetsUp += 1
		if ( event.JetJERDown.at(jet)*(1-event.JetSigma.at(jet)) > 30.):
			nJetsDn += 1

	if (event.JetPt.size() > 0 and event.JetPt.at(0) > 30.):
		leadingJet_Eta_MC.Fill(event.JetEta.at(0),event.overallEventWeight*event.xsec*1000*lumi/SumWeight_tt)
	if (event.JetPt.size() > 0 and event.JetJERUp.at(i_up)*(1+event.JetSigma.at(i_up)) > 30.):
		leadingJet_Eta_MCup.Fill(event.JetEta.at(i_up),event.overallEventWeight*event.xsec*1000*lumi/SumWeight_tt)
	if (event.JetPt.size() > 0 and event.JetJERDown.at(i_dn)*(1-event.JetSigma.at(i_up)) > 30.):
		leadingJet_Eta_MCdn.Fill(event.JetEta.at(i_dn),event.overallEventWeight*event.xsec*1000*lumi/SumWeight_tt)

	nJetsPt30_MC.Fill(event.nCleanedJetsPt30,event.overallEventWeight*event.xsec*1000*lumi/SumWeight_tt)
	nJetsPt30_MCup.Fill(nJetsUp,event.overallEventWeight*event.xsec*1000*lumi/SumWeight_tt)
	nJetsPt30_MCdn.Fill(nJetsDn,event.overallEventWeight*event.xsec*1000*lumi/SumWeight_tt)

	PFMET_MC.Fill(event.PFMET,event.overallEventWeight*event.xsec*1000*lumi/SumWeight_tt)
	PFMET_MCup.Fill(event.PFMET_jesUp,event.overallEventWeight*event.xsec*1000*lumi/SumWeight_tt)
	PFMET_MCdn.Fill(event.PFMET_jesDn,event.overallEventWeight*event.xsec*1000*lumi/SumWeight_tt)

	ZMass_MC.Fill(event.ZMass,event.overallEventWeight*event.xsec*1000*lumi/SumWeight_tt)



tdata.Draw("JetEta[0] >> leadingJet_Eta_data","nCleanedJetsPt30 > 0 && (ZMass > 70 && ZMass < 110) && (LepPt[0] > 30 && LepPt[1] > 30) && abs(LepLepId[0])==11")
tdata.Draw("nCleanedJetsPt30 >> nJetsPt30_data","(ZMass > 70 && ZMass < 110) && (LepPt[0] > 30 && LepPt[1] > 30) && abs(LepLepId[0])==11")
tdata.Draw("PFMET >> PFMET_data","(ZMass > 70 && ZMass < 110) && (LepPt[0] > 30 && LepPt[1] > 30)&& abs(LepLepId[0])==11")
tdata.Draw("ZMass >> ZMass_data","(ZMass > 70 && ZMass < 110) && (LepPt[0] > 30 && LepPt[1] > 30)&& abs(LepLepId[0])==11")

c = ROOT.TCanvas()

DrawRatioPlot(DIR+"leadingJet_Eta", "leading jet #eta", "Events/0.2", c, leadingJet_Eta_data, leadingJet_Eta_MC, leadingJet_Eta_MCup, leadingJet_Eta_MCdn)
DrawRatioPlot(DIR+"nCleanedJetsPt30", "# Jets Pt > 30 GeV", "Events", c, nJetsPt30_data, nJetsPt30_MC, nJetsPt30_MCup, nJetsPt30_MCdn)
DrawRatioPlot(DIR+"PFMET", "PFMET", "Events/10 GeV", c, PFMET_data, PFMET_MC, PFMET_MCup, PFMET_MCdn)
DrawRatioPlot(DIR+"ZMass", "ZMass", "Events/1 GeV", c, ZMass_data, ZMass_MC, ZMass_MC, ZMass_MC)




