/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: AOPCUANodeID
 *** Description: null
 *** Version: 
 *************************************************************************/

#ifndef _AOPCUANODEID_H_
#define _AOPCUANODEID_H_

#include <adapter.h>
#include <typelib.h>
#include <forte_usint.h>
#include <forte_any.h>



#include <forte_uint.h>

class FORTE_AOPCUANodeID: public CAdapter{
  DECLARE_ADAPTER_TYPE(FORTE_AOPCUANodeID)

private:
 private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
 public:
  CIEC_UINT &nsIndex() {
    return *static_cast<CIEC_UINT*>((isSocket()) ? getDI(0) : getDO(0));
  };

  CIEC_USINT &identifierType() {
    return *static_cast<CIEC_USINT*>((isSocket()) ? getDI(1) : getDO(1));
  };

  CIEC_ANY &identifier() {
    return *static_cast<CIEC_ANY*>((isSocket()) ? getDI(2) : getDO(2));
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

   FORTE_ADAPTER_DATA_ARRAY(0, 0, 3, 0, 0);

public:
  ADAPTER_CTOR(FORTE_AOPCUANodeID){
  };

  virtual ~FORTE_AOPCUANodeID(){};

};

#endif //close the ifdef sequence from the beginning of the file

