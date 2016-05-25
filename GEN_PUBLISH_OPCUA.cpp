/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: GEN_PUBLISH_OPCUA
 *** Description: Data Publication Service
 *** Version: 
 ***     0.1: 2001-04-24/JHC - Rockwell Automation - XML version
 ***     0.0: 1998-12-08/JHC - Rockwell Automation - SRC version
 *************************************************************************/

#include "GEN_PUBLISH_OPCUA.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_PUBLISH_OPCUA_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_GEN_PUBLISH_OPCUA, g_nStringIdGEN_PUBLISH_OPCUA)

const CStringDictionary::TStringId FORTE_GEN_PUBLISH_OPCUA::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdID, g_nStringIdSD_1};

const CStringDictionary::TStringId FORTE_GEN_PUBLISH_OPCUA::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdANY};

const CStringDictionary::TStringId FORTE_GEN_PUBLISH_OPCUA::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId FORTE_GEN_PUBLISH_OPCUA::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 FORTE_GEN_PUBLISH_OPCUA::scm_anEIWithIndexes[] = {0, 3};
const TDataIOID FORTE_GEN_PUBLISH_OPCUA::scm_anEIWith[] = {0, 1, 255, 0, 2, 255};
const CStringDictionary::TStringId FORTE_GEN_PUBLISH_OPCUA::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdREQ};

const TDataIOID FORTE_GEN_PUBLISH_OPCUA::scm_anEOWith[] = {0, 1, 255, 0, 1, 255};
const TForteInt16 FORTE_GEN_PUBLISH_OPCUA::scm_anEOWithIndexes[] = {0, 3, -1};
const CStringDictionary::TStringId FORTE_GEN_PUBLISH_OPCUA::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdCNF};

const SAdapterInstanceDef FORTE_GEN_PUBLISH_OPCUA::scm_astAdapterInstances[] = {
{g_nStringIdANY_ADAPTER, g_nStringIdANY_ADAPTER, false }};

const SFBInterfaceSpec FORTE_GEN_PUBLISH_OPCUA::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  3,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  1,scm_astAdapterInstances};


void FORTE_GEN_PUBLISH_OPCUA::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventINITID:
#error add code for INIT event!
/*
  do not forget to send output event, calling e.g.
      sendOutputEvent(scm_nEventCNFID);
*/
      break;
    case scm_nEventREQID:
#error add code for REQ event!
/*
  do not forget to send output event, calling e.g.
      sendOutputEvent(scm_nEventCNFID);
*/
      break;
  }
}



