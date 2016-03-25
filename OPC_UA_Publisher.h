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

#ifndef _OPC_UA_PUBLISHER_H_
#define _OPC_UA_PUBLISHER_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_any.h>

#ERROR type contains variables of type ANY. Please check the usage of these variables as we can not gurantee correct usage on export!

#include <forte_wstring.h>
#include "ANY_ADAPTER.h"

class FORTE_OPC_UA_Publisher: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_OPC_UA_Publisher)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_WSTRING &Server() {
    return *static_cast<CIEC_WSTRING*>(getDI(0));
  };

  CIEC_ANY &PARAM() {
    return *static_cast<CIEC_ANY*>(getDI(1));
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

   FORTE_FB_DATA_ARRAY(2, 2, 2, 1);

  void executeEvent(int pa_nEIID);

public:
  FUNCTION_BLOCK_CTOR(FORTE_OPC_UA_Publisher){
  };

  virtual ~FORTE_OPC_UA_Publisher(){};

};

#endif //close the ifdef sequence from the beginning of the file

