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

#ifndef _GEN_PUBLISH_OPCUA_H_
#define _GEN_PUBLISH_OPCUA_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_any.h>

#ERROR type contains variables of type ANY. Please check the usage of these variables as we can not gurantee correct usage on export!

#include <forte_wstring.h>
#include "ANY_ADAPTER.h"

class FORTE_GEN_PUBLISH_OPCUA: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_GEN_PUBLISH_OPCUA)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  };

  CIEC_WSTRING &ID() {
    return *static_cast<CIEC_WSTRING*>(getDI(1));
  };

  CIEC_ANY &SD_1() {
    return *static_cast<CIEC_ANY*>(getDI(2));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  };

  CIEC_WSTRING &STATUS() {
    return *static_cast<CIEC_WSTRING*>(getDO(1));
  };

  static const TEventID scm_nEventINITID = 0;
  static const TEventID scm_nEventREQID = 1;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventINITOID = 0;
  static const TEventID scm_nEventCNFID = 1;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SAdapterInstanceDef scm_astAdapterInstances[];

  FORTE_ANY_ADAPTER& ANY_ADAPTER() {
    return (*static_cast<FORTE_ANY_ADAPTER*>(m_apoAdapters[0]));
  };
  static const int scm_nANY_ADAPTERAdpNum = 0;
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(2, 3, 2, 1);

  void executeEvent(int pa_nEIID);

public:
  FUNCTION_BLOCK_CTOR(FORTE_GEN_PUBLISH_OPCUA){
  };

  virtual ~FORTE_GEN_PUBLISH_OPCUA(){};

};

#endif //close the ifdef sequence from the beginning of the file

