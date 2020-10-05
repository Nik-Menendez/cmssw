#ifndef _Validation_SiTrackerPhase2V_Phase2ITValidateRecHit_h
#define _Validation_SiTrackerPhase2V_Phase2ITValidateRecHit_h
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"
#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/DQMStore.h"

// DQM Histograming
class PixelDigiSimLink;
class SimTrack;
class SimHit;
class TrackerTopology;
class TrackerGeometry;
class TrackerDigiGeometryRecord;
class TrackerTopologyRcd;
class Phase2ITValidateRecHit : public DQMEDAnalyzer {
public:
  explicit Phase2ITValidateRecHit(const edm::ParameterSet&);
  ~Phase2ITValidateRecHit() override;
  void bookHistograms(DQMStore::IBooker& ibooker, edm::Run const& iRun, edm::EventSetup const& iSetup) override;
  void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) override;
  void dqmBeginRun(const edm::Run&, const edm::EventSetup&) override;
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void fillITHistos(const edm::Event& iEvent,
                    const TrackerHitAssociator& associateRecHit,
                    const std::vector<edm::Handle<edm::PSimHitContainer>>& simHits,
                    const std::map<unsigned int, SimTrack>& selectedSimTrackMap);

  void bookLayerHistos(DQMStore::IBooker& ibooker, unsigned int det_id, std::string& subdir);

  edm::ParameterSet config_;
  TrackerHitAssociator::Config trackerHitAssociatorConfig_;
  const double simtrackminpt_;
  std::string geomType_;
  const edm::EDGetTokenT<SiPixelRecHitCollection> tokenRecHitsIT_;
  const edm::EDGetTokenT<edm::SimTrackContainer> simTracksToken_;
  std::vector<edm::EDGetTokenT<edm::PSimHitContainer>> simHitTokens_;
  const edm::ESGetToken<TrackerGeometry, TrackerDigiGeometryRecord> geomToken_;
  const edm::ESGetToken<TrackerTopology, TrackerTopologyRcd> topoToken_;
  const TrackerGeometry* tkGeom_ = nullptr;
  const TrackerTopology* tTopo_ = nullptr;
  struct RecHitME {
    // use TH1D instead of TH1F to avoid stauration at 2^31
    // above this increments with +1 don't work for float, need double
    MonitorElement* deltaX = nullptr;
    MonitorElement* deltaY = nullptr;
    MonitorElement* pullX = nullptr;
    MonitorElement* pullY = nullptr;
    MonitorElement* deltaX_eta = nullptr;
    MonitorElement* deltaY_eta = nullptr;
    MonitorElement* pullX_eta = nullptr;
    MonitorElement* pullY_eta = nullptr;
    //For rechits matched to simhits from highPT tracks
    MonitorElement* numberRecHitsprimary = nullptr;
    MonitorElement* pullX_primary;
    MonitorElement* pullY_primary;
    MonitorElement* deltaX_primary;
    MonitorElement* deltaY_primary;
  };
  std::map<std::string, RecHitME> layerMEs_;
};
#endif
