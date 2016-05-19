/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: NodeConfig
 *** Description: null
 *** Version: 
 *************************************************************************/

#ifndef _NODECONFIG_H_
#define _NODECONFIG_H_

#include <adapter.h>
#include <typelib.h>
#include <forte_bool.h>
#include <forte_string.h>

class FORTE_NodeConfig: public CAdapter{
  DECLARE_ADAPTER_TYPE(FORTE_NodeConfig)

private:
 private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
 public:
  CIEC_BOOL &Param() {
    return *static_cast<CIEC_BOOL*>((isSocket()) ? getDI(0) : getDO(0));
  };

  CIEC_STRING &NodeId() {
    return *static_cast<CIEC_STRING*>((isSocket()) ? getDI(1) : getDO(1));
  };

  CIEC_STRING &BrowseName() {
    return *static_cast<CIEC_STRING*>((isSocket()) ? getDI(2) : getDO(2));
  };

  CIEC_STRING &ParentNodeId() {
    return *static_cast<CIEC_STRING*>((isSocket()) ? getDI(3) : getDO(3));
  };

  CIEC_STRING &RefTypeId() {
    return *static_cast<CIEC_STRING*>((isSocket()) ? getDI(4) : getDO(4));
  };

  CIEC_STRING &ObjTypeId() {
    return *static_cast<CIEC_STRING*>((isSocket()) ? getDI(5) : getDO(5));
  };

 public:
 private:
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
 public:
 private:
  static const TForteInt16 scm_anEOWithIndexes[];
  static const SFBInterfaceSpec scm_stFBInterfaceSpecSocket;

  static const SFBInterfaceSpec scm_stFBInterfaceSpecPlug;

   FORTE_ADAPTER_DATA_ARRAY(0, 0, 6, 0, 0);

public:
  ADAPTER_CTOR(FORTE_NodeConfig){
  };

  virtual ~FORTE_NodeConfig(){};

};

#endif //close the ifdef sequence from the beginning of the file

