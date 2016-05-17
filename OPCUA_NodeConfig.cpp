/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: OPCUA_NodeConfig
 *** Description: Basic Function Block Type
 *** Version: 
 ***     0.0: 2016-05-09/4DIAC-IDE - 4DIAC-Consortium - 
 *************************************************************************/

#include "OPCUA_NodeConfig.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "OPCUA_NodeConfig_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_OPCUA_NodeConfig, g_nStringIdOPCUA_NodeConfig)

const CStringDictionary::TStringId FORTE_OPCUA_NodeConfig::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdCaseParam, g_nStringIdAttNodeId, g_nStringIdAttBrowseName, g_nStringIdAttParentNodeId, g_nStringIdAttRefTypeId, g_nStringIdAttObjTypeDef};

const CStringDictionary::TStringId FORTE_OPCUA_NodeConfig::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING};

const TForteInt16 FORTE_OPCUA_NodeConfig::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_OPCUA_NodeConfig::scm_anEIWith[] = {0, 1, 2, 3, 4, 5, 6, 255};
const CStringDictionary::TStringId FORTE_OPCUA_NodeConfig::scm_anEventInputNames[] = {g_nStringIdINIT};

const TForteInt16 FORTE_OPCUA_NodeConfig::scm_anEOWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId FORTE_OPCUA_NodeConfig::scm_anEventOutputNames[] = {g_nStringIdINITO};

const SAdapterInstanceDef FORTE_OPCUA_NodeConfig::scm_astAdapterInstances[] = {
{g_nStringIdNodeConfig, g_nStringIdANodeConfig, true }};

const SFBInterfaceSpec FORTE_OPCUA_NodeConfig::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  0, 0,  7,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0,  0, 0,
  1,scm_astAdapterInstances};

void FORTE_OPCUA_NodeConfig::alg_SNDNODE(void){
/*  Nodes exist in UA Address Space: 
 * Subscriber only required NodeId for subscription target.
 */
if((CaseParam() == 0)){
ANodeConfig().Param() = CaseParam();
ANodeConfig().NodeId() = AttNodeId();
}
else{
/*  Nodes do not exist in UA Address Space: 
 * Subscriber requires all Node attributes to create
 * Node in UA Address Space and subscribe to it.
 */
ANodeConfig().Param() = CaseParam();
ANodeConfig().NodeId() = AttNodeId();
ANodeConfig().BrowseName() = AttBrowseName();
ANodeConfig().ParentNodeId() = AttBrowseName();
ANodeConfig().RefTypeId() = AttRefTypeId();
ANodeConfig().ObjTypeId() = AttObjTypeDef();
};
}


void FORTE_OPCUA_NodeConfig::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void FORTE_OPCUA_NodeConfig::enterStateState(void){
  m_nECCState = scm_nStateState;
  alg_SNDNODE();
  sendOutputEvent( scm_nEventINITOID);
}

void FORTE_OPCUA_NodeConfig::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(m_nECCState){
      case scm_nStateSTART:
        if(scm_nEventINITID == pa_nEIID)
          enterStateState();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateState:
        if(1)
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      default:
      DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 1.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; //0 is always the initial state
        break;
    }
    pa_nEIID = cg_nInvalidEventID;  // we have to clear the event after the first check in order to ensure correct behavior
  }while(bTransitionCleared);
}


