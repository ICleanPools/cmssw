#include <string>
#include <vector>

#include "DQM/L1TMonitor/interface/L1TStage2EMTF.h"

const double PI = 3.14159265358979323846;

L1TStage2EMTF::L1TStage2EMTF(const edm::ParameterSet& ps)
    : daqToken(consumes<l1t::EMTFDaqOutCollection>(ps.getParameter<edm::InputTag>("emtfSource"))),
      hitToken(consumes<l1t::EMTFHitCollection>(ps.getParameter<edm::InputTag>("emtfSource"))),
      trackToken(consumes<l1t::EMTFTrackCollection>(ps.getParameter<edm::InputTag>("emtfSource"))),
      muonToken(consumes<l1t::RegionalMuonCandBxCollection>(ps.getParameter<edm::InputTag>("emtfSource"))),
      monitorDir(ps.getUntrackedParameter<std::string>("monitorDir", "")),
      verbose(ps.getUntrackedParameter<bool>("verbose", false)) {}

L1TStage2EMTF::~L1TStage2EMTF() {}

void L1TStage2EMTF::dqmBeginRun(const edm::Run& r, const edm::EventSetup& c) {}

void L1TStage2EMTF::beginLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&) {}

void L1TStage2EMTF::bookHistograms(DQMStore::IBooker& ibooker, const edm::Run&, const edm::EventSetup&) {

  ibooker.setCurrentFolder(monitorDir);

  // DAQ Output Monitor Elements
  emtfErrors = ibooker.book1D("emtfErrors", "EMTF Errors", 6, 0, 6);
  emtfErrors->setAxisTitle("Error Type (Corruptions Not Implemented)", 1);
  emtfErrors->setAxisTitle("Number of Errors", 2);
  emtfErrors->setBinLabel(1, "Corruptions", 1);
  emtfErrors->setBinLabel(2, "Synch. Err.", 1);
  emtfErrors->setBinLabel(3, "Synch. Mod.", 1);
  emtfErrors->setBinLabel(4, "BX Mismatch", 1);
  emtfErrors->setBinLabel(5, "Time Misalign.", 1);
  emtfErrors->setBinLabel(6, "FMM != Ready", 1);

  // Hit (LCT) Monitor Elements
  int n_xbins, nWGs, nHSs;
  std::string name, label;
  std::vector<std::string> suffix_name = {"42", "41", "32", "31", "22", "21", "13", "12", "11b", "11a"};
  std::vector<std::string> suffix_label = {"4/2", "4/1", "3/2", "3/1", " 2/2", "2/1", "1/3", "1/2", "1/1b", "1/1a"};

  cscLCTBX = ibooker.book2D("cscLCTBX", "CSC LCT BX", 8, -3, 5, 20, 0, 20);
  cscLCTBX->setAxisTitle("BX", 1);
  for (int xbin = 1, xbin_label = -3; xbin <= 8; ++xbin, ++xbin_label) {
    cscLCTBX->setBinLabel(xbin, std::to_string(xbin_label), 1);
  }
  for (int ybin = 1; ybin <= 10; ++ybin) {
    cscLCTBX->setBinLabel(ybin, "ME-" + suffix_label[ybin - 1], 2);
    cscLCTBX->setBinLabel(19 - ybin, "ME+" + suffix_label[ybin - 1], 2);
  }

  ibooker.setCurrentFolder(monitorDir + "/CSCInput");

  for (int hist = 0, i = 0; hist < 20; ++hist, i = hist % 10) {

    if (hist < 10) {
      name = "MENeg" + suffix_name[i];
      label = "ME-" + suffix_label[i];
    } else {
      name = "MEPos" + suffix_name[9 - i];
      label = "ME+" + suffix_label[9 - i];
    }

    if (hist < 6 || hist > 13) {
      n_xbins = (i % 2) ? 18 : 36;
    } else {
      n_xbins = 36;
    }

    if (hist > 7 || hist < 11){
      nWGs = 48;
    } else if (hist == 6 || hist == 8 || hist == 11 || hist == 13){
      nWGs = 96;
    } else if (hist == 3 || hist == 16){
      nWGs = 32;
    } else if (hist == 4 || hist == 15){
      nWGs = 112;
    } else {
      nWGs = 64;
    }

    if (hist == 6 || hist == 13 || hist == 8 || hist == 10){
      nHSs = 128;
    } else if (hist == 9 || hist == 10){
      nHSs = 96;
    } else {
      nHSs = 160;
    }

    cscLCTStrip[hist] = ibooker.book1D("cscLCTStrip" + name, "CSC Halfstrip " + label, nHSs, 0, nHSs);
    cscLCTStrip[hist]->setAxisTitle("Cathode Halfstrip, " + label, 1);

    cscLCTWire[hist] = ibooker.book1D("cscLCTWire" + name, "CSC Wiregroup " + label, nWGs, 0, nWGs);
    cscLCTWire[hist]->setAxisTitle("Anode Wiregroup, " + label, 1);

    cscChamberStrip[hist] = ibooker.book2D("cscChamberStrip" + name, "CSC Halfstrip " + label, n_xbins, 1, 1+n_xbins, nHSs, 0, nHSs);
    cscChamberStrip[hist]->setAxisTitle("Chamber, " + label, 1);
    cscChamberStrip[hist]->setAxisTitle("Cathode Halfstrip", 2);

    cscChamberWire[hist] = ibooker.book2D("cscChamberWire" + name, "CSC Wiregroup " + label, n_xbins, 1, 1+n_xbins, nWGs, 0, nWGs);
    cscChamberWire[hist]->setAxisTitle("Chamber, " + label, 1);
    cscChamberWire[hist]->setAxisTitle("Anode Wiregroup", 2);

    for (int bin = 1; bin <= n_xbins; ++bin) {
      cscChamberStrip[hist]->setBinLabel(bin, std::to_string(bin), 1);
      cscChamberWire[hist]->setBinLabel(bin, std::to_string(bin), 1);
    }
  }

  ibooker.setCurrentFolder(monitorDir);

  cscLCTOccupancy = ibooker.book2D("CSC LCT Occupancy", "CSC Chamber Occupancy", 54, 1, 55, 10, -5, 5);
  cscLCTOccupancy->setAxisTitle("Sector (CSCID 1-9 Unlabelled)", 1);
  for (int bin = 1; bin <= 46; bin += 9) {
    cscLCTOccupancy->setBinLabel(bin, std::to_string(bin % 8), 1);
  }
  cscLCTOccupancy->setBinLabel(1, "ME-4", 2);
  cscLCTOccupancy->setBinLabel(2, "ME-3", 2);
  cscLCTOccupancy->setBinLabel(3, "ME-2", 2);
  cscLCTOccupancy->setBinLabel(4, "ME-1b", 2);
  cscLCTOccupancy->setBinLabel(5, "ME-1a", 2);
  cscLCTOccupancy->setBinLabel(6, "ME+1a", 2);
  cscLCTOccupancy->setBinLabel(7, "ME+1b", 2);
  cscLCTOccupancy->setBinLabel(8, "ME+2", 2);
  cscLCTOccupancy->setBinLabel(9, "ME+3", 2);
  cscLCTOccupancy->setBinLabel(10, "ME+4", 2);

  // Track Monitor Elements
  emtfnTracks = ibooker.book1D("emtfnTracks", "Number of EMTF Tracks per Event", 11, 0, 11);
  for (int bin = 1; bin <= 10; ++bin) {
    emtfnTracks->setBinLabel(bin, std::to_string(bin - 1), 1);
  }
  emtfnTracks->setBinLabel(11, "Overflow", 1);

  emtfTracknHits = ibooker.book1D("emtfTracknHits", "Number of Hits per EMTF Track", 5, 0, 5);
  for (int bin = 1; bin <= 5; ++bin) {
    emtfTracknHits->setBinLabel(bin, std::to_string(bin - 1), 1);
  }

  emtfTrackBX = ibooker.book2D("emtfTrackBX", "EMTF Track Bunch Crossing", 12, -6, 6, 8, -3, 5);
  emtfTrackBX->setAxisTitle("Sector (Endcap)", 1);
  for (int i = 0; i < 6; ++i) {
    emtfTrackBX->setBinLabel(i + 1, std::to_string(6 - i) + " (-)", 1);
    emtfTrackBX->setBinLabel(12 - i, std::to_string(6 - i) + " (+)", 1);
  }
  emtfTrackBX->setAxisTitle("Track BX", 2);
  for (int bin = 1, i = -3; bin <= 8; ++bin, ++i) {
    emtfTrackBX->setBinLabel(bin, std::to_string(i), 2);
  }

  emtfTrackPt = ibooker.book1D("emtfTrackPt", "EMTF Track p_{T}", 256, 1, 257);
  emtfTrackPt->setAxisTitle("Track p_{T} [GeV]", 1);

  emtfTrackEta = ibooker.book1D("emtfTrackEta", "EMTF Track #eta", 100, -2.5, 2.5);
  emtfTrackEta->setAxisTitle("Track #eta", 1);

  emtfTrackPhi = ibooker.book1D("emtfTrackPhi", "EMTF Track #phi", 126, -3.15, 3.15);
  emtfTrackPhi->setAxisTitle("Track #phi", 1);

  emtfTrackPhiHighQuality = ibooker.book1D("emtfTrackPhiHighQuality", "EMTF High Quality #phi", 126, -3.15, 3.15);
  emtfTrackPhiHighQuality->setAxisTitle("Track #phi (High Quality)", 1);

  emtfTrackOccupancy = ibooker.book2D("emtfTrackOccupancy", "EMTF Track Occupancy", 100, -2.5, 2.5, 126, -3.15, 3.15);
  emtfTrackOccupancy->setAxisTitle("#eta", 1);
  emtfTrackOccupancy->setAxisTitle("#phi", 2);

  emtfTrackMode = ibooker.book1D("emtfTrackMode", "EMTF Track Mode", 16, 0, 16);
  emtfTrackMode->setAxisTitle("Mode", 1);

  emtfTrackQuality = ibooker.book1D("emtfTrackQuality", "EMTF Track Quality", 16, 0, 16);
  emtfTrackQuality->setAxisTitle("Quality", 1);

  emtfTrackQualityVsMode = ibooker.book2D("emtfTrackQualityVsMode", "EMTF Track Quality vs Mode", 16, 0, 16, 16, 0, 16);
  emtfTrackQualityVsMode->setAxisTitle("Mode", 1);
  emtfTrackQualityVsMode->setAxisTitle("Quality", 2);

  for (int bin = 1; bin <= 16; ++bin) {
    emtfTrackMode->setBinLabel(bin, std::to_string(bin - 1), 1);
    emtfTrackQuality->setBinLabel(bin, std::to_string(bin - 1), 1);
    emtfTrackQualityVsMode->setBinLabel(bin, std::to_string(bin - 1), 1);
    emtfTrackQualityVsMode->setBinLabel(bin, std::to_string(bin - 1), 2);
  }

  // Regional Muon Candidate Monitor Elements
  ibooker.setCurrentFolder(monitorDir + "/MuonCand");

  emtfMuonBX = ibooker.book1D("emtfMuonBX", "EMTF Muon Cand BX", 7, -3, 4);
  emtfMuonBX->setAxisTitle("BX", 1);
  for (int bin = 1, bin_label = -3; bin <= 7; ++bin, ++bin_label) {
    emtfMuonBX->setBinLabel(bin, std::to_string(bin_label), 1);
  }

  emtfMuonhwPt = ibooker.book1D("emtfMuonhwPt", "EMTF Muon Cand p_{T}", 512, 0, 512);
  emtfMuonhwPt->setAxisTitle("Hardware p_{T}", 1);

  emtfMuonhwEta = ibooker.book1D("emtfMuonhwEta", "EMTF Muon Cand #eta", 460, -230, 230);
  emtfMuonhwEta->setAxisTitle("Hardware #eta", 1);

  emtfMuonhwPhi = ibooker.book1D("emtfMuonhwPhi", "EMTF Muon Cand #phi", 125, -20, 105);
  emtfMuonhwPhi->setAxisTitle("Hardware #phi", 1);

  emtfMuonhwQual = ibooker.book1D("emtfMuonhwQual", "EMTF Muon Cand Quality", 16, 0, 16);
  emtfMuonhwQual->setAxisTitle("Quality", 1);
  for (int bin = 1; bin <= 16; ++bin) {
    emtfMuonhwQual->setBinLabel(bin, std::to_string(bin - 1), 1);
  }
}

void L1TStage2EMTF::analyze(const edm::Event& e, const edm::EventSetup& c) {

  if (verbose) edm::LogInfo("L1TStage2EMTF") << "L1TStage2EMTF: analyze..." << std::endl;

  // DAQ Output
  edm::Handle<l1t::EMTFDaqOutCollection> DaqOutCollection;
  e.getByToken(daqToken, DaqOutCollection);

  for (std::vector<l1t::EMTFDaqOut>::const_iterator DaqOut = DaqOutCollection->begin(); DaqOut != DaqOutCollection->end(); ++DaqOut) {
    const l1t::emtf::MECollection* MECollection = DaqOut->PtrMECollection();
    for (std::vector<l1t::emtf::ME>::const_iterator ME = MECollection->begin(); ME != MECollection->end(); ++ME) {
      if (ME->SE()) emtfErrors->Fill(1);
      if (ME->SM()) emtfErrors->Fill(2);
      if (ME->BXE()) emtfErrors->Fill(3);
      if (ME->AF()) emtfErrors->Fill(4);
    }

    const l1t::emtf::EventHeader* EventHeader = DaqOut->PtrEventHeader();
    if (!EventHeader->Rdy()) emtfErrors->Fill(5);
  }

  // Hits (LCTs)
  edm::Handle<l1t::EMTFHitCollection> HitCollection;
  e.getByToken(hitToken, HitCollection);

  for (std::vector<l1t::EMTFHit>::const_iterator Hit = HitCollection->begin(); Hit != HitCollection->end(); ++Hit) {
    int endcap = Hit->Endcap();
    int sector = Hit->Sector();
    int station = Hit->Station();
    int ring = Hit->Ring();
    int cscid = Hit->CSC_ID();
    int chamber = Hit->Chamber();
    int strip = Hit->Strip();
    int wire = Hit->Wire();

    int hist_index = 0;
    int cscid_offset = (sector - 1) * 9;

    // The following logic determines the index of the monitor element
    // to which a hit belongs, exploiting the symmetry of the endcaps.
    if (station == 1) {
      if (ring == 4) {
        strip -= 128;
        hist_index = 9;
      } else if (ring == 1) {
        hist_index = 8;
      } else if (ring == 2) {
        hist_index = 7;
      } else if (ring == 3) {
        hist_index = 6;
      }
    } else if (ring == 1) {
      if (station == 2) {
        hist_index = 5;
      } else if (station == 3) {
        hist_index = 3;
      } else if (station == 4) {
        hist_index = 1;
      }
    } else if (ring == 2) {
      if (station == 2) {
        hist_index = 4;
      } else if (station == 3) {
        hist_index = 2;
      } else if (station == 4) {
        hist_index = 0;
      }
    }

    if (endcap > 0) hist_index = 19 - hist_index;
    
    if (Hit->Is_CSC() == true && Hit->Neighbor() == false){
      cscLCTBX->Fill(Hit->BX(), hist_index);

      cscLCTStrip[hist_index]->Fill(strip);
      cscLCTWire[hist_index]->Fill(wire);

      cscChamberStrip[hist_index]->Fill(chamber, strip);
      cscChamberWire[hist_index]->Fill(chamber, wire);

      if (Hit->Subsector() == 1) {
        cscLCTOccupancy->Fill(cscid + cscid_offset, endcap * (station - 0.5));
      } else {
        cscLCTOccupancy->Fill(cscid + cscid_offset, endcap * (station + 0.5));
      }
    }
  }

  // Tracks
  edm::Handle<l1t::EMTFTrackCollection> TrackCollection;
  e.getByToken(trackToken, TrackCollection);

  int nTracks = TrackCollection->size();

  if (nTracks <= 10) {
    emtfnTracks->Fill(nTracks);
  } else {
    emtfnTracks->Fill(10);
  }

  for (std::vector<l1t::EMTFTrack>::const_iterator Track = TrackCollection->begin(); Track != TrackCollection->end(); ++Track) {
    int endcap = Track->Endcap();
    int sector = Track->Sector();
    float eta = Track->Eta();
    float phi_glob_rad = Track->Phi_glob() * PI / 180.;
    int mode = Track->Mode();
    int quality = Track->GMT_quality();

    emtfTracknHits->Fill(Track->NumHits());
    emtfTrackBX->Fill(endcap * (sector - 0.5), Track->BX());
    emtfTrackPt->Fill(Track->Pt());
    emtfTrackEta->Fill(eta);
    emtfTrackPhi->Fill(phi_glob_rad);
    emtfTrackOccupancy->Fill(eta, phi_glob_rad);
    emtfTrackMode->Fill(mode);
    emtfTrackQuality->Fill(quality);
    emtfTrackQualityVsMode->Fill(mode, quality);
    if (mode == 15) emtfTrackPhiHighQuality->Fill(phi_glob_rad);
   }

  // Regional Muon Candidates
  edm::Handle<l1t::RegionalMuonCandBxCollection> MuonBxCollection;
  e.getByToken(muonToken, MuonBxCollection);

  for (int itBX = MuonBxCollection->getFirstBX(); itBX <= MuonBxCollection->getLastBX(); ++itBX) {
    for (l1t::RegionalMuonCandBxCollection::const_iterator Muon = MuonBxCollection->begin(itBX); Muon != MuonBxCollection->end(itBX); ++Muon) {
      emtfMuonBX->Fill(itBX);
      emtfMuonhwPt->Fill(Muon->hwPt());
      emtfMuonhwEta->Fill(Muon->hwEta());
      emtfMuonhwPhi->Fill(Muon->hwPhi());
      emtfMuonhwQual->Fill(Muon->hwQual());
    }
  }
}

