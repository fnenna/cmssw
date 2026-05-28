import FWCore.ParameterSet.Config as cms

PFMuonAlgoParameters = cms.PSet(
    # Parameters for Muon ID and post cleaning
    maxDPtOPt = cms.double(1.0),
    trackQuality = cms.string('highPurity'),
    ptErrorScale = cms.double(8.0),

    # Cleaning and rejection thresholds
    eventFractionForCleaning = cms.double(0.5),
    minPtForPostCleaning = cms.double(20.0),
    eventFactorForCosmics = cms.double(10.0),
    metSignificanceForCleaning = cms.double(3.0),
    metSignificanceForRejection = cms.double(4.0),
    metFactorForCleaning = cms.double(4.0),
    eventFractionForRejection = cms.double(0.8),
    metFactorForRejection = cms.double(4.0),
    metFactorForHighEta = cms.double(25.0),
    ptFactorForHighEta = cms.double(2.0),
    metFactorForFakes = cms.double(4.0),
    
    # Punch-through parameters
    minMomentumForPunchThrough = cms.double(100.0),
    minEnergyForPunchThrough = cms.double(100.0),
    punchThroughFactor = cms.double(3.0),
    punchThroughMETFactor = cms.double(4.0),
    
    # Cosmic rejection
    cosmicRejectionDistance = cms.double(1.0),
    
    # Consider ME0 in the algorithm (Phase-2)
    hasME0 = cms.bool(False) 
)

from Configuration.Eras.Modifier_phase2_GE0_cff import phase2_GE0
phase2_GE0.toModify(PFMuonAlgoParameters, hasME0 = True)
