/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: ANodeID
 *** Description: null
 *** Version: 
 *************************************************************************/

#include "ANodeID.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "ANodeID_gen.cpp"
#endif

DEFINE_ADAPTER_TYPE(FORTE_ANodeID, g_nStringIdANodeID)

const CStringDictionary::TStringId FORTE_ANodeID::scm_anDataInputNames[] = {g_nStringIdnsIndex, g_nStringIdidentifierType, g_nStringIdidentifier};

const CStringDictionary::TStringId FORTE_ANodeID::scm_anDataInputTypeIds[] = {g_nStringIdUINT, g_nStringIdUSINT, g_nStringIdANY};

const TDataIOID FORTE_ANodeID::scm_anEIWith[] = {0, 1, 2, 255};
const TForteInt16 FORTE_ANodeID::scm_anEIWithIndexes[] = {0};
const TForteInt16 FORTE_ANodeID::scm_anEOWithIndexes[] = {-1};
const SFBInterfaceSpec FORTE_ANodeID::scm_stFBInterfaceSpecSocket = {
  0,  0,  0,  0,
  0,  0,   0, 0,  3,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0,  0, 0,
  0, 0
};

const SFBInterfaceSpec FORTE_ANodeID::scm_stFBInterfaceSpecPlug = {
  0,  0,  0,  0,
  0,  0,   scm_anEIWith, scm_anEIWithIndexes,  0,  0, 0, 
  3,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0, 0
};



