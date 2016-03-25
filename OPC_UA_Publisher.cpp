/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: OPC_UA_Publisher
 *** Description: Service Interface Function Block Type
 *** Version: 
 ***     0.0: 2016-02-25/4DIAC-IDE - 4DIAC-Consortium - 
 *************************************************************************/

#include "OPC_UA_Publisher.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "OPC_UA_Publisher_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_OPC_UA_Publisher, g_nStringIdOPC_UA_Publisher)

const CStringDictionary::TStringId FORTE_OPC_UA_Publisher::scm_anDataInputNames[] = {g_nStringIdServer, g_nStringIdPARAM};

const CStringDictionary::TStringId FORTE_OPC_UA_Publisher::scm_anDataInputTypeIds[] = {g_nStringIdWSTRING, g_nStringIdANY};

const CStringDictionary::TStringId FORTE_OPC_UA_Publisher::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId FORTE_OPC_UA_Publisher::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 FORTE_OPC_UA_Publisher::scm_anEIWithIndexes[] = {0, 3};
const TDataIOID FORTE_OPC_UA_Publisher::scm_anEIWith[] = {0, 1, 255, 0, 255};
const CStringDictionary::TStringId FORTE_OPC_UA_Publisher::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdREQ};

const TDataIOID FORTE_OPC_UA_Publisher::scm_anEOWith[] = {0, 1, 255, 0, 1, 255};
const TForteInt16 FORTE_OPC_UA_Publisher::scm_anEOWithIndexes[] = {0, 3, -1};
const CStringDictionary::TStringId FORTE_OPC_UA_Publisher::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdCNF};

const SAdapterInstanceDef FORTE_OPC_UA_Publisher::scm_astAdapterInstances[] = {
{g_nStringIdANY_ADAPTER, g_nStringIdANY_ADAPTER, false }};

const SFBInterfaceSpec FORTE_OPC_UA_Publisher::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  2,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  1,scm_astAdapterInstances};


void FORTE_OPC_UA_Publisher::executeEvent(int pa_nEIID){
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



