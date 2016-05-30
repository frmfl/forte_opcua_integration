/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: AOPCUANodeID
 *** Description: null
 *** Version: 
 *************************************************************************/

#include "AOPCUANodeID.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "AOPCUANodeID_gen.cpp"
#endif

DEFINE_ADAPTER_TYPE(FORTE_AOPCUANodeID, g_nStringIdAOPCUANodeID)

const CStringDictionary::TStringId FORTE_AOPCUANodeID::scm_anDataInputNames[] = {g_nStringIdnsIndex, g_nStringIdidentifierType, g_nStringIdidentifier};

const CStringDictionary::TStringId FORTE_AOPCUANodeID::scm_anDataInputTypeIds[] = {g_nStringIdUINT, g_nStringIdUSINT, g_nStringIdANY};

const TDataIOID FORTE_AOPCUANodeID::scm_anEIWith[] = {0, 1, 2, 255};
const TForteInt16 FORTE_AOPCUANodeID::scm_anEIWithIndexes[] = {0};
const TForteInt16 FORTE_AOPCUANodeID::scm_anEOWithIndexes[] = {-1};
const SFBInterfaceSpec FORTE_AOPCUANodeID::scm_stFBInterfaceSpecSocket = {
  0,  0,  0,  0,
  0,  0,   0, 0,  3,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0,  0, 0,
  0, 0
};

const SFBInterfaceSpec FORTE_AOPCUANodeID::scm_stFBInterfaceSpecPlug = {
  0,  0,  0,  0,
  0,  0,   scm_anEIWith, scm_anEIWithIndexes,  0,  0, 0, 
  3,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0, 0
};



