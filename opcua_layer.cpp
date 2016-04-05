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

	return retVal;


}


EComResponse COPC_UA_Layer::createItems(CIEC_ANY *paDataArray, int paNumData, char* paLayerParameter){
	EComResponse retVal = e_InitOk;
	if(0 == paNumData){
		//handle pure event message
		retVal = e_InitInvalidId;
		break;
	} else{
		// allocate memory for an array of pointers to pointers pointing to values of type NodeId
		st_ParentChildNodeId.ppNodeId_ParentFB = new UA_NodeId *[paNumData];
		st_ParentChildNodeId.ppNodeId_SrcPoint = new UA_NodeId *[paNumData];

		memset(st_ParentChildNodeId.ppNodeId_ParentFB, 0, sizeof(UA_NodeId *) * paNumData);		//!< initialize pointer memory: multiply size of struct with amount
		memset(st_ParentChildNodeId.ppNodeId_SrcPoint, 0, sizeof(UA_NodeId *) * paNumData);		//!< initialize pointer memory: multiply size of struct with amount

		char* nextParam;

		/* check for FBParent existence
		 * create FBParent Node
		 * check for SourcePoint Node existing
		 * create SourcePoint Node
		 */

		TDataConnectionPtr pao_DIConns = getCommFB()->m_apoDIConns;
		for(int i = 0; i < paNumData; i++){
			if(!pao_DIConns->isConnected()){
				DEVLOG_INFO("OPC_UA Publisher DIs not connected\n");
			}else{
				DEVLOG_INFO("OPC_UA Publisher DIs are connected, linked list not empty\n");

				// loop over all SD-ports and retrieve Source FB Information
				SConnectionPoint& sourceRD = pao_DIConns[i].getSourceId();

				CFunctionBlock *sourceFB = sourceRD.mFB;	// pointer to Parent Function Block
				UA_NodeId* returnFBNodeId = UA_NodeId_new();

				// check if Function Block is present in the Address Space otherwise create it
				UA_StatusCode retValgetNode = COPC_UA_Handler::getInstance().getFBNodeId(sourceFB, returnFBNodeId);

				if(retValgetNode != UA_STATUSCODE_GOOD){
					//create the parent object node in the address space
					UA_StatusCode retValcreateNode = COPC_UA_Handler::getInstance().createUAObjNode();
					st_ParentChildNodeId.ppNodeId_ParentFB[i] = returnFBNodeId;

				}else{
					//create the child node
					st_ParentChildNodeId.ppNodeId_ParentFB[i] = returnFBNodeId;
				}


				/************************************************************/


				SFBInterfaceSpec* sourceFBInterface = sourceFB->getFBInterfaceSpec();

				CStringDictionary::TStringId sourceRDNameId = sourceFBInterface->m_aunDONames[sourceRD.mPortId];
				const char * sourceRDName = CStringDictionary::getInstance().get(sourceRDNameId);

				CStringDictionary::TStringId sourceRDTypeNameId = sourceFBInterface->m_aunDODataTypeNames[sourceRD.mPortId];
				const char * sourceRDTypeName = CStringDictionary::getInstance().get(sourceRDTypeNameId);

				char message[128];
				sprintf(message,"%s %s %s\n", sourceFBName, sourceFBTypeName, sourceRDName, sourceRDTypeName);
				DEVLOG_INFO(message);

				/*************************************************************/

				//This calls the handler
				COPC_UA_Handler::getInstance().registerNode(&NodeAttr);

				registerDataPoint(paLayerParameter, "Coment");

				if(0 != mSFPItem[i]){
					//write the initial value to the SFP server so that the data type of the item gets set
					CEclipseSCADASFPHandler::updateDataPoint(mSFPItem[i], paDataArray[i]);
				}else {
					retVal = e_InitInvalidId;
					break;
				}



				retVal = e_InitInvalidId;
				break;
			}
		}


		//1. Create Struct for each item
		//2. Create array of structs holding these items
		//2.1 or maybe do a typecast and inherit the SD's data value to the struct member
		//3. register a datapoint, maybe this does not work with OPC_UA

		//1. Get SD's source Port FB and ID Name
		//2. Check if Nodes exist
		//3. Create Nodes
		//4. Save Nodes to array and initialize with current value




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






