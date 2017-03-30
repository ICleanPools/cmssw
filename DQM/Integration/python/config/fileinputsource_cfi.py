import FWCore.ParameterSet.Config as cms

source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/001AF1D0-6BE9-E511-9A8D-02163E0143FE.root", 
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/005BCAC3-72E9-E511-B002-02163E014310.root",
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/00E3C803-BBE9-E511-A99C-02163E01465A.root",
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/00EFE5C2-6EE9-E511-ABB3-02163E013417.root",
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/04159D15-95E9-E511-B73F-02163E014176.root",
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/04E4A62F-93E9-E511-83A3-02163E0134CD.root",
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/04F7F2D7-72E9-E511-9534-02163E014310.root",
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/06077D63-78E9-E511-9D8A-02163E014310.root",
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/061EFCD5-97E9-E511-8764-02163E0143FE.root",
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/063C23DD-82E9-E511-9020-02163E011FCE.root",        
        #"file:/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/DQMTest/MinimumBias__RAW__v1__165633__1CC420EE-B686-E011-A788-0030487CD6E8.root",
        #"/store/data/Commissioning2014/Cosmics/RAW//v3/000/224/380/00000/E05943D1-1227-E411-BB8E-02163E00F0C4.root"
        #"/store/data/Commissioning2014/Cosmics/RAW/v3/000/224/380/00000/68FDADE5-1227-E411-8AA6-02163E00A10C.root"
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/00A396A0-3C92-E611-9B93-02163E0145AD.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/00CEE8A0-3C92-E611-B6DE-02163E01185B.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/0208FF04-EB91-E611-965C-02163E01389B.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/0212579C-3C92-E611-B0ED-02163E014348.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/029C87AA-3C92-E611-956C-02163E014380.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/02DA1D23-4092-E611-AA04-FA163E35234A.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/02E9BAA7-3C92-E611-95C6-02163E01198B.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/04213E9F-EC91-E611-BBE3-02163E013425.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/0432FCA5-EE91-E611-8E1E-02163E011970.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/043FB294-3C92-E611-A4CA-FA163E2C38F6.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/0453FA53-EA91-E611-981F-02163E0140EB.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/047A8447-4092-E611-B151-02163E0118E6.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/067F96A8-EE91-E611-9FC2-FA163EB6B8B4.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/06B3158E-3C92-E611-84E2-FA163EA0D418.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/06BCCC0B-EB91-E611-BE14-02163E0118A5.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/06C9C586-3C92-E611-81A5-FA163ECD93DC.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/083B8F9E-3C92-E611-A9A3-02163E0146CA.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/086F7994-3C92-E611-8014-FA163E073540.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/0870D7A6-EE91-E611-B822-02163E011AB4.root",
        "/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/08FE108A-EE91-E611-A031-FA163EE1765D.root",

    )
)

maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)

# Parameters for runType
import FWCore.ParameterSet.VarParsing as VarParsing
import sys
from dqmPythonTypes import *

options = VarParsing.VarParsing("analysis")

options.register(
    "runkey",
    "cosmic_run",
    VarParsing.VarParsing.multiplicity.singleton,
    VarParsing.VarParsing.varType.string,
    "Run Keys of CMS"
)

options.parseArguments()

# Fix to allow scram to compile
#if len(sys.argv) > 1:
#  options.parseArguments()

runType = RunType()
if not options.runkey.strip():
    options.runkey = "pp_run"

runType.setRunType(options.runkey.strip())
