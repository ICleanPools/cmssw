#include <string>

#include "DQM/L1TMonitor/interface/L1TdeStage2EMTF.h"


L1TdeStage2EMTF::L1TdeStage2EMTF(const edm::ParameterSet& ps)
    : dataToken(consumes<l1t::RegionalMuonCandBxCollection>(ps.getParameter<edm::InputTag>("dataSource"))),
      emulToken(consumes<l1t::RegionalMuonCandBxCollection>(ps.getParameter<edm::InputTag>("emulSource"))),
      hitToken(consumes<l1t::EMTFHitCollection>(ps.getParameter<edm::InputTag>("hitSource"))),
      monitorDir(ps.getUntrackedParameter<std::string>("monitorDir", "")),
      verbose(ps.getUntrackedParameter<bool>("verbose", false)) {}

L1TdeStage2EMTF::~L1TdeStage2EMTF() {}

void L1TdeStage2EMTF::dqmBeginRun(const edm::Run& r, const edm::EventSetup& c) {}

void L1TdeStage2EMTF::beginLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&) {}

void L1TdeStage2EMTF::bookHistograms(DQMStore::IBooker& ibooker, const edm::Run&, const edm::EventSetup&) {

  ibooker.setCurrentFolder(monitorDir);

  emtfComparenMuonsEvent = ibooker.book2D("emtfComparenMuonsEvent", "Number of EMTF Muon Cands per Event", 12, 0, 12, 12, 0, 12);
  for (int axis = 1; axis <= 2; ++axis) {
    std::string axisTitle = (axis == 1) ? "Data" : "Emulator";
    emtfComparenMuonsEvent->setAxisTitle(axisTitle, axis);
    for (int bin = 1; bin <= 12; ++bin) {
      std::string binLabel = (bin == 12) ? "Overflow" : std::to_string(bin - 1);
      emtfComparenMuonsEvent->setBinLabel(bin, binLabel, axis);
    }
  }

  emtfDataBX = ibooker.book1D("emtfDataBX", "EMTF Muon Cand BX", 7, -3, 4);
  emtfDataBX->setAxisTitle("BX", 1);

  emtfEmulBX = ibooker.book1D("emtfEmulBX", "EMTF Emulated Muon Cand BX", 7, -3, 4);
  emtfEmulBX->setAxisTitle("BX", 1);

  for (int bin = 1, bin_label = -3; bin <= 7; ++bin, ++bin_label) {
    emtfDataBX->setBinLabel(bin, std::to_string(bin_label), 1);
    emtfEmulBX->setBinLabel(bin, std::to_string(bin_label), 1);
  }

  emtfDatahwPt = ibooker.book1D("emtfDatahwPt", "EMTF Muon Cand p_{T}", 512, 0, 512);
  emtfDatahwPt->setAxisTitle("Hardware p_{T}", 1);

  emtfEmulhwPt = ibooker.book1D("emtfEmulhwPt", "EMTF Emulated Muon Cand p_{T}", 512, 0, 512);
  emtfEmulhwPt->setAxisTitle("Hardware p_{T}", 1);

  emtfDatahwEta = ibooker.book1D("emtfDatahwEta", "EMTF Muon Cand #eta", 460, -230, 230);
  emtfDatahwEta->setAxisTitle("Hardware #eta", 1);

  emtfEmulhwEta = ibooker.book1D("emtfEmulhwEta", "EMTF Emulated Muon Cand #eta", 460, -230, 230);
  emtfEmulhwEta->setAxisTitle("Hardware #eta", 1);

  emtfDatahwPhi = ibooker.book1D("emtfDatahwPhi", "EMTF Muon Cand #phi", 125, -20, 105);
  emtfDatahwPhi->setAxisTitle("Hardware #phi", 1);

  emtfEmulhwPhi = ibooker.book1D("emtfEmulhwPhi", "EMTF Emulated Muon Cand #phi", 125, -20, 105);
  emtfEmulhwPhi->setAxisTitle("Hardware #phi", 1);

  emtfDatahwQual = ibooker.book1D("emtfDatahwQual", "EMTF Muon Cand Quality", 16, 0, 16);
  emtfDatahwQual->setAxisTitle("Quality", 1);

  emtfEmulhwQual = ibooker.book1D("emtfEmulhwQual", "EMTF Emulated Muon Cand Quality", 16, 0, 16);
  emtfEmulhwQual->setAxisTitle("Quality", 1);

  for (int bin = 1; bin <= 16; ++bin) {
    emtfDatahwQual->setBinLabel(bin, std::to_string(bin - 1), 1);
    emtfEmulhwQual->setBinLabel(bin, std::to_string(bin - 1), 1);
  }

// RPC Histograms - Added by Preston

  std::vector<std::string> suffix = {"4/2","3/2","2/2","1/3","1/2"};

  emtfRPCHitBX = ibooker.book2D("emtfRPCHitBX", "EMTF RPC Hit BX", 7, -3, 4, 10, 0, 10);
  emtfRPCHitBX->setAxisTitle("BX",1);
  for (int xbin = 1; xbin < 8; ++xbin)
    emtfRPCHitBX->setBinLabel(xbin, std::to_string(xbin-4), 1);
  for (int ybin = 1; ybin < 6; ++ybin) {
    emtfRPCHitBX->setBinLabel(ybin, "RE-" + suffix[ybin - 1],2);
    emtfRPCHitBX->setBinLabel(11 - ybin, "RE+" + suffix[ybin - 1],2);
  }

  emtfRPCBX = ibooker.book1D("emtfRPCBX", "EMTF RPC BX", 7, -3, 4);
  emtfRPCBX->setAxisTitle("BX",1);
  for (int bin = 1; bin < 8; ++bin)
    emtfRPCBX->setBinLabel(bin, std::to_string(bin-4), 1);

  emtfRPCSectorP = ibooker.book1D("emtfRPCSectorP", "Positive Endcap RPC Sector", 6, 1, 7);
  emtfRPCSectorP->setAxisTitle("Sector", 1);
  for (int bin = 1; bin < 7; ++bin)
    emtfRPCSectorP->setBinLabel(bin, std::to_string(bin), 1);

  emtfRPCSectorN = ibooker.book1D("emtfRPCSectorN", "Negative Endcap RPC Sector", 6, 1, 7);
  emtfRPCSectorN->setAxisTitle("Sector", 1);
  for (int bin = 1; bin < 7; ++bin)
    emtfRPCSectorN->setBinLabel(bin, std::to_string(bin), 1);

  emtfRPCSSSP = ibooker.book1D("emtfRPCSSSP", "Positive Endcap RPC Sector and Subsector", 36, 7, 43);
  emtfRPCSSSP->setAxisTitle("Sector/Subsector", 1);
  for (int sect = 1; sect < 7; ++sect) {
    emtfRPCSSSP->setBinLabel(6*(sect-1)+1, std::to_string(sect) + "/1", 1);
    emtfRPCSSSP->setBinLabel(6*(sect-1)+4, std::to_string(sect) + "/4", 1);
  }

  emtfRPCSSSN = ibooker.book1D("emtfRPCSSSN", "Negative Endcap RPC Sector and Subsector", 36, 7, 43);
  emtfRPCSSSN->setAxisTitle("Sector/Subsector", 1);
  for (int sect = 1; sect < 7; ++sect) {
    emtfRPCSSSN->setBinLabel(6*(sect-1)+1, std::to_string(sect) + "/1", 1);
    emtfRPCSSSN->setBinLabel(6*(sect-1)+4, std::to_string(sect) + "/4", 1);
  }

  // Comparison plots reserved for updated emulator.
  /*emtfComparehwPt = ibooker.book2D("emtfComparehwPt", "EMTF Muon Cand p_{T}", 512, 0, 512, 512, 0, 512);
  emtfComparehwPt->setAxisTitle("Hardware p_{T}", 1);
  emtfComparehwPt->setAxisTitle("Emulator Hardware p_{T}", 2);

  emtfComparehwEta = ibooker.book2D("emtfComparehwEta", "EMTF Muon Cand #eta", 460, -230, 230, 460, -230, 230);
  emtfComparehwEta->setAxisTitle("Hardware #eta", 1);
  emtfComparehwEta->setAxisTitle("Emulator Hardware #eta", 2);

  emtfComparehwPhi = ibooker.book2D("emtfComparehwPhi", "EMTF Muon Cand #phi", 125, -20, 105, 125, -20, 105);
  emtfComparehwPhi->setAxisTitle("Hardware #phi", 1);
  emtfComparehwPhi->setAxisTitle("Emulator Hardware #phi", 2);

  emtfComparehwQual = ibooker.book2D("emtfComparehwQual", "EMTF Muon Cand Quality", 16, 0, 16, 16, 0, 16);
  for (int axis = 1; axis <= 2; ++axis) {
    std::string axisTitle = (axis == 1) ? "Quality" : "Emulator Quality";
    emtfComparehwQual->setAxisTitle(axisTitle, axis);
    for (int bin = 1; bin <= 16; ++bin) {
      emtfComparehwQual->setBinLabel(bin, std::to_string(bin - 1), axis);
    }
  }*/
}

void L1TdeStage2EMTF::analyze(const edm::Event& e, const edm::EventSetup& c) {

  if (verbose) edm::LogInfo("L1TdeStage2EMTF") << "L1TdeStage2EMTF: analyze..." << std::endl;

  edm::Handle<l1t::RegionalMuonCandBxCollection> dataMuons;
  e.getByToken(dataToken, dataMuons);

  edm::Handle<l1t::RegionalMuonCandBxCollection> emulMuons;
  e.getByToken(emulToken, emulMuons);

  emtfComparenMuonsEvent->Fill(dataMuons->size(), emulMuons->size());

  for (int itBX = dataMuons->getFirstBX(); itBX <= dataMuons->getLastBX(); ++itBX) {
    for (l1t::RegionalMuonCandBxCollection::const_iterator dataMuon = dataMuons->begin(itBX); dataMuon != dataMuons->end(itBX); ++dataMuon) {
      emtfDataBX->Fill(itBX);
      emtfDatahwPt->Fill(dataMuon->hwPt());
      emtfDatahwEta->Fill(dataMuon->hwEta());
      emtfDatahwPhi->Fill(dataMuon->hwPhi());
      emtfDatahwQual->Fill(dataMuon->hwQual());
    }
  }

  for (int itBX = emulMuons->getFirstBX(); itBX <= emulMuons->getLastBX(); ++itBX) {
    for (l1t::RegionalMuonCandBxCollection::const_iterator emulMuon = emulMuons->begin(itBX); emulMuon != emulMuons->end(itBX); ++emulMuon) {
      emtfEmulBX->Fill(itBX);
      emtfEmulhwPt->Fill(emulMuon->hwPt());
      emtfEmulhwEta->Fill(emulMuon->hwEta());
      emtfEmulhwPhi->Fill(emulMuon->hwPhi());
      emtfEmulhwQual->Fill(emulMuon->hwQual());
    }
  }

// RPC Histograms - Added by Preston

  edm::Handle<l1t::EMTFHitCollection> HitCollection;
  e.getByToken(hitToken, HitCollection);

  for (std::vector<l1t::EMTFHit>::const_iterator Hit = HitCollection->begin(); Hit != HitCollection->end(); ++Hit) {
    int station = Hit->Station();
    int ring = Hit->Ring();

    int hist_index = 0;

    switch (station) {
      case 1: switch (ring) {
        case 2: hist_index = 4; break;
        case 3: hist_index = 3; break;
      } break;
      case 2: hist_index = 2; break;
      case 3: hist_index = 1; break;
      case 4: hist_index = 0; break;
    }

    if (Hit->Is_RPC()){
      if (Hit->Endcap() > 0){
        hist_index = 9 - hist_index;
        emtfRPCSectorP->Fill(Hit->Sector());
        emtfRPCSSSP->Fill(6*Hit->Sector() + Hit->Subsector());
      } else {
        emtfRPCSectorN->Fill(Hit->Sector());
        emtfRPCSSSN->Fill(6*Hit->Sector() + Hit->Subsector());
      }
      if (Hit->BC0()){
        emtfRPCHitBX->Fill(Hit->BX(), hist_index);
        emtfRPCBX->Fill(Hit->BX());
      }
    }
  }
}

