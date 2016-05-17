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

#ifndef _OPCUA_NODECONFIG_H_
#define _OPCUA_NODECONFIG_H_

#include <basicfb.h>
#include <forte_bool.h>
#include <forte_string.h>
#include "NodeConfig.h"

class FORTE_OPCUA_NodeConfig: public CBasicFB{
  DECLARE_FIRMWARE_FB(FORTE_OPCUA_NodeConfig)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  };

  CIEC_BOOL &CaseParam() {
    return *static_cast<CIEC_BOOL*>(getDI(1));
  };

  CIEC_STRING &AttNodeId() {
    return *static_cast<CIEC_STRING*>(getDI(2));
  };

  CIEC_STRING &AttBrowseName() {
    return *static_cast<CIEC_STRING*>(getDI(3));
  };

  CIEC_STRING &AttParentNodeId() {
    return *static_cast<CIEC_STRING*>(getDI(4));
  };

  CIEC_STRING &AttRefTypeId() {
    return *static_cast<CIEC_STRING*>(getDI(5));
  };

  CIEC_STRING &AttObjTypeDef() {
    return *static_cast<CIEC_STRING*>(getDI(6));
  };

  static const TEventID scm_nEventINITID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventINITOID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SAdapterInstanceDef scm_astAdapterInstances[];

  FORTE_NodeConfig& ANodeConfig() {
    return (*static_cast<FORTE_NodeConfig*>(m_apoAdapters[0]));
  };
  static const int scm_nANodeConfigAdpNum = 0;
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_BASIC_FB_DATA_ARRAY(1, 7, 0, 0, 1);
  void alg_SNDNODE(void);
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateState = 1;

  void enterStateSTART(void);
  void enterStateState(void);

  virtual void executeEvent(int pa_nEIID);

public:
  FORTE_OPCUA_NodeConfig(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : 
       CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId,
              0, m_anFBConnData, m_anFBVarsData){
  };

  virtual ~FORTE_OPCUA_NodeConfig(){};

};

#endif //close the ifdef sequence from the beginning of the file

