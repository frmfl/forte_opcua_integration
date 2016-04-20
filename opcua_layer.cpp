/*
 * opcua_layer.cpp
 *
 *  Created on: 01.03.2016
 *      Author: florian
 */

#include "opcua_layer.h"
#include "opcuahandler.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace forte::com_infra;


COPC_UA_Layer::COPC_UA_Layer(CComLayer * pa_poUpperLayer, CCommFB * pa_poComFB) : CComLayer(pa_poUpperLayer, pa_poComFB){
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
	EComResponse retValEcom = e_InitOk;
	UA_StatusCode retVal = UA_STATUSCODE_GOOD;
	if(0 == paNumData){
		//handle pure event message
		retValEcom = e_InitInvalidId;
		// implement handling for pure event connections
		DEVLOG_ERROR("OPC_UA Publisher no SD Ports, pure Event messages\n");
		retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;

	} else{
		// allocate memory for an array of pointers to pointers pointing to values of type NodeId
		st_ParentChildNodeId.ppNodeId_ParentFB = new UA_NodeId *[paNumData];
		st_ParentChildNodeId.ppNodeId_SrcPoint = new UA_NodeId *[paNumData];

		memset(st_ParentChildNodeId.ppNodeId_ParentFB, 0, sizeof(UA_NodeId *) * paNumData);		//!< initialize pointer memory: multiply size of struct with amount
		memset(st_ParentChildNodeId.ppNodeId_SrcPoint, 0, sizeof(UA_NodeId *) * paNumData);		//!< initialize pointer memory: multiply size of struct with amount

		/* check for FBParent existence
		 * create FBParent Node
		 * check for SourcePoint Node existing
		 * create SourcePoint Node
		 */

		for(int i = 2; i < (paNumData+2); i++){
			const SFBInterfaceSpec* pstInterfaceSpec = getCommFB()->getFBInterfaceSpec();
			const CStringDictionary::TStringId paDINameId = pstInterfaceSpec->m_aunDINames[i];
			const char* myname = CStringDictionary::getInstance().get(paDINameId);
			DEVLOG_INFO("%s with port ID %i and name %s found\n",CStringDictionary::getInstance().get(getCommFB()->getFBTypeId()), i, myname);

			const CDataConnection* pC_DIConn = getCommFB()->getDIConnection(paDINameId); 	//!< pointer to a Connection Object

			// retrieve connection source point (SP)
			//const SConnectionPoint& rst_sourceRD = const_cast<const CDataConnection*>(pC_DIConn)->getSourceId();
			const SConnectionPoint& rst_sourceRD(pC_DIConn->getSourceId());

			const CFunctionBlock *sourceFB = rst_sourceRD.mFB;	// pointer to Parent Function Block
			UA_NodeId* returnFBNodeId = UA_NodeId_new();

			// check if Function Block is present in the Address Space otherwise create it
			UA_StatusCode retValgetNode = COPC_UA_Handler::getInstance().getFBNodeId(sourceFB, returnFBNodeId);

			if(retValgetNode == UA_STATUSCODE_GOOD){
				// Parent object node (FB Node) was present in the UA Address Space.
				DEVLOG_INFO("Function block node already present in the Address space.\n");
				st_ParentChildNodeId.ppNodeId_ParentFB[i] = returnFBNodeId;
				retVal = UA_STATUSCODE_GOOD;

			}else{
				// Parent object node (FB Node) was not present in the UA Address space -> create it.
				DEVLOG_INFO("Parent object node was not present in AS, create it.\n");
				UA_NodeId * returnObjNodeId = UA_NodeId_new();
				UA_StatusCode retValcreateObjNode = COPC_UA_Handler::getInstance().createUAObjNode(sourceFB, returnObjNodeId);

				if(retValcreateObjNode == UA_STATUSCODE_GOOD){
					// Node creation successful
					returnObjNodeId->identifier;
					//DEVLOG_INFO("Object node %s successfully created.\n",returnObjNodeId->identifier);		//FIXME add objectnode identifier here
					DEVLOG_INFO("Object node successfully created.\n"); //FIXME convert returnObjNode identifier to string ->then output
					st_ParentChildNodeId.ppNodeId_ParentFB[i] = returnObjNodeId;
					retVal = UA_STATUSCODE_GOOD;

				}else{
					// Node creation not successful
					DEVLOG_ERROR("Error creating node %s\n", retValcreateObjNode);
					retVal = retValcreateObjNode;

				}
			}

			UA_NodeId* returnSPNodeId = UA_NodeId_new();
			// check if Variable Node is present in Address Space otherwise create it.
			retValgetNode = COPC_UA_Handler::getInstance().getSPNodeId(sourceFB, const_cast<SConnectionPoint&>(rst_sourceRD), returnSPNodeId);
			//retValgetNode = COPC_UA_Handler::getInstance().getSPNodeId(sourceFB, sourceRD, returnSPNodeId);

			if(retValgetNode == UA_STATUSCODE_GOOD){
				// SourcePoint node (SP Node) was present in the UA Address Space.
				DEVLOG_INFO("SP node already present in the Address space.\n");
				st_ParentChildNodeId.ppNodeId_SrcPoint[i] = returnSPNodeId;
				retVal = UA_STATUSCODE_GOOD;

			}else{
				// Create SourcePort Node (Publishers SD Port) in UA Server Address Space
				// pass Function Block pointer
				// pass SourcePort reference
				UA_NodeId * returnVarNodeId = UA_NodeId_new();
				UA_StatusCode retValcreateVarNode = COPC_UA_Handler::getInstance().createUAVarNode(sourceFB, const_cast<SConnectionPoint&>(rst_sourceRD), returnVarNodeId);

				if(retValcreateVarNode == UA_STATUSCODE_GOOD){
					// Node creation successful
					DEVLOG_INFO("Object node %s successfully created.\n",returnVarNodeId->identifier);		//FIXME add variablenode identifier here
					st_ParentChildNodeId.ppNodeId_SrcPoint[i] = returnVarNodeId;
					retVal = UA_STATUSCODE_GOOD;

				}else{
					// Node creation not successful
					DEVLOG_ERROR("Error creating node %s\n", retValcreateVarNode);
					retVal = retValcreateVarNode;

				}
			}

		}
	}
	//FIXME: mapping from UA_StatusCode to EComResponse type!
	retValEcom = e_InitOk;
	return retValEcom;
}



/*



	SFBInterfaceSpec* sourceFBInterface = sourceFB->getFBInterfaceSpec();

				CStringDictionary::TStringId sourceRDNameId = sourceFBInterface->m_aunDONames[sourceRD.mPortId];
				const char * sourceRDName = CStringDictionary::getInstance().get(sourceRDNameId);

				CStringDictionary::TStringId sourceRDTypeNameId = sourceFBInterface->m_aunDODataTypeNames[sourceRD.mPortId];
				const char * sourceRDTypeName = CStringDictionary::getInstance().get(sourceRDTypeNameId);

				char message[128];
				sprintf(message,"%s %s %s\n", sourceFBName, sourceFBTypeName, sourceRDName, sourceRDTypeName);
				DEVLOG_INFO(message);

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
 */


EComResponse COPC_UA_Layer::sendData(void *paData, unsigned int paSize){



	EComResponse retVal = e_ProcessDataOk;
	/*
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
	 */
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






