/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: NodeConfig
 *** Description: null
 *** Version: 
 *************************************************************************/

#include "NodeConfig.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "NodeConfig_gen.cpp"
#endif

DEFINE_ADAPTER_TYPE(FORTE_NodeConfig, g_nStringIdNodeConfig)

const CStringDictionary::TStringId FORTE_NodeConfig::scm_anDataInputNames[] = {g_nStringIdParam, g_nStringIdNodeId, g_nStringIdBrowseName, g_nStringIdParentNodeId, g_nStringIdRefTypeId, g_nStringIdObjTypeId};

const CStringDictionary::TStringId FORTE_NodeConfig::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING};

const TDataIOID FORTE_NodeConfig::scm_anEIWith[] = {0, 1, 2, 3, 4, 5, 255};
const TForteInt16 FORTE_NodeConfig::scm_anEIWithIndexes[] = {0};
const TForteInt16 FORTE_NodeConfig::scm_anEOWithIndexes[] = {-1};
const SFBInterfaceSpec FORTE_NodeConfig::scm_stFBInterfaceSpecSocket = {
  0,  0,  0,  0,
  0,  0,   0, 0,  6,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0,  0, 0,
  0, 0
};

const SFBInterfaceSpec FORTE_NodeConfig::scm_stFBInterfaceSpecPlug = {
  0,  0,  0,  0,
  0,  0,   scm_anEIWith, scm_anEIWithIndexes,  0,  0, 0, 
  6,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0, 0
};



