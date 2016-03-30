/*
 * opcua_layer.cpp
 *
 *  Created on: 01.03.2016
 *      Author: florian
 */

#include "opcua_layer.h"
#include "opcuahandler.h"
#include <commfb.h>
#include <devlog.h>

using namespace forte::com_infra;


COPC_UA_Layer::COPC_UA_Layer(CComLayer * pa_poUpperLayer, CCommFB * pa_poComFB) : CComLayer(pa_poUpperLayer, pa_poComFB),  m_ppstOPCUAItem(0){
	// constructor list initialization
}


COPC_UA_Layer::~COPC_UA_Layer() {
	// TODO Auto-generated destructor stub
}


void COPC_UA_Layer::closeConnection(){
	//COPC_UA_Handler::getInstance().
}


EComResponse COPC_UA_Layer::openConnection(char * paLayerParameter){
	int numData;
	CIEC_ANY *dataArray;

	if(e_Subscriber == getCommFB()->getComServiceType()){
		numData = getCommFB()->getNumRD();
		dataArray = getCommFB()->getRDs();

	}else{
		numData = getCommFB()->getNumSD();
		dataArray = getCommFB()->getSDs();
	}

	// for OPC_UA also pass the parameters necessary to locate the variable at the correct
	//position in the address space.
	EComResponse retVal = createItems(dataArray, numData, paLayerParameter);

	if(e_InitOk == retVal){
		if(e_Subscriber == getCommFB()->getComServiceType()){
			if(0 == numData){
				numData = 1;
				// for events only, how are events being processed?
			}
		}

	}
	return retVal;


}


EComResponse COPC_UA_Layer::sendData(void *paData, unsigned int paSize){
	EComResponse retVal = e_ProcessDataOk;

	  EComResponse retVal = e_ProcessDataOk;

	  if(0 == paSize){
	    //TODO change to an update now with out the need for a new allocation
		  UA_Server_writeValue()
	    sfp_item_update_data_allocated(*mSFPItem, sfp_variant_new_null(), sfp_time_in_millis ());
	  }else {
	    CIEC_ANY const *SDs(static_cast<TConstIEC_ANYPtr>(paData));
	    for(unsigned int i = 0; i < paSize; i++){
	      CEclipseSCADASFPHandler::updateDataPoint(mSFPItem[i], SDs[i]);  //TODO use common timestamp for all
	    }
	  }
	  return retVal;
	}

	return retVal;
}


EComResponse COPC_UA_Layer::recvData(const void* pav_pvData, unsigned int pa_unSize){
	EComResponse retVal = e_Nothing;
	return retVal;
}


EComResponse COPC_UA_Layer::processInterrupt(void){
	EComResponse retVal = e_Nothing;
	return retVal;
}


EComResponse COPC_UA_Layer::createItems(CIEC_ANY *paDataArray, int paNumData, char* paLayerParameter){
	EComResponse retVal = e_InitOk;
	if(0 == paNumData){
		//handle pure event message
		return retVal;
	} else{
		// empty createItems method
		m_ppstOPCUAItem = new stOPCUAItem *[paNumData]; // this allocates memory for an array of size paNumData to which opc_ua_item points to a pointer that points to the first element of the array.
		memset(m_ppstOPCUAItem, 0, sizeof(stOPCUAItem *) * paNumData);
		char* nextParam;
		//This calls the handler
		//*mSFPItem = CEclipseSCADASFPHandler::getInstance().registerDataPoint(paLayerParameter, "Coment");



		//1. Create Struct for each item
		//2. Create array of structs holding these items
		//2.1 or maybe do a typecast and inherit the SD's data value to the struct member
		//3. register a datapoint, maybe this does not work with OPC_UA


		TDataConnectionPtr pao_DIConns = getCommFB()->m_apoDIConns;
			for(int i = 0; i<=paNumData; i++){
				if(!pao_DIConns->isConnected()){
					DEVLOG_INFO("OPC_UA Publisher DIs not connected\n");
				}else{
					DEVLOG_INFO("OPC_UA Publisher DIs are connected, linked list not empty\n");
					// loop over all DI-ports and retrieve Source FB Information
					SConnectionPoint& sourceRD = pao_DIConns[i].getSourceId();

					CFunctionBlock *sourceFB = sourceRD.mFB;
					UA_NodeId returnFBNodeId = COPC_UA_Handler::getInstance().getFBNode(sourceFB);



					//This calls the handler
					COPC_UA_Handler::getInstance().registerNode(&NodeAttr);
				}
			}

		return retVal;
	}
}



