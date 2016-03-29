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

	TDataConnectionPtr pao_DIConns = getCommFB()->m_apoDIConns;
	for(int i = 0; i<=numData; i++){
		if(!pao_DIConns->isConnected()){
			DEVLOG_INFO("OPC_UA Publisher DIs not connected\n");
		}else{
			DEVLOG_INFO("OPC_UA Publisher DIs are connected, linked list not empty\n");
			// loop over all DI-ports and retrieve Source FB Information
			SConnectionPoint& sourceRD = pao_DIConns[i].getSourceId();

			CFunctionBlock *sourceFB = sourceRD.mFB;
			CStringDictionary::TStringId sourceFBTypeId = sourceFB->getFBTypeId();
			const char * sourceFBTypeName = CStringDictionary::getInstance().get(sourceFBTypeId);
			//FIXME Retrieve System name

			CStringDictionary::TStringId sourceFBNameId = sourceFB->getInstanceNameId();
			const char* sourceFBName = sourceFB->getInstanceName();

			SFBInterfaceSpec* sourceFBInterface = sourceFB->getFBInterfaceSpec();

			CStringDictionary::TStringId sourceRDNameId = sourceFBInterface->m_aunDONames[sourceRD.mPortId];
			const char * sourceRDName = CStringDictionary::getInstance().get(sourceRDNameId);

			CStringDictionary::TStringId sourceRDTypeNameId = sourceFBInterface->m_aunDODataTypeNames[sourceRD.mPortId];
			const char * sourceRDTypeName = CStringDictionary::getInstance().get(sourceRDTypeNameId);


			char message[128];
			sprintf(message,"%s %s %s\n", sourceFBName, sourceRDName, sourceRDTypeName);
			DEVLOG_INFO(message);

			char *fb_id;
			strcpy(fb_id, sourceFBName);

			char *var_id;
			strcpy(var_id, sourceRDName);

			char varBrowse_id[64];
			sprintf(varBrowse_id, "%s.%s", fb_id, var_id);

			UA_Client* client = COPC_UA_Handler::getInstance().startupOPC_UA_Client();
			UA_Client * clientInstance = COPC_UA_Handler::getInstance().getClient();
			COPC_UA_Handler::getInstance().connectOPC_UA_Client();

			UA_Client *client = UA_Client_new(UA_ClientConfig_standard, Logger_Stdout);
			UA_StatusCode retval = UA_Client_connect(client, ClientNetworkLayerTCP_connect, "opc.tcp://localhost:16664");

			if(retval != UA_STATUSCODE_GOOD) {
				UA_Client_delete(client);
				sendOutputEvent(scm_nEventCNFID);
				break;
			}

			char node_id_string[64];
			int node_found = 0;


			UA_BrowseRequest bReq;
			UA_BrowseRequest_init(&bReq);
			bReq.requestedMaxReferencesPerNode = 0;
			bReq.nodesToBrowse = UA_BrowseDescription_new();
			bReq.nodesToBrowseSize = 1;
			bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER); //browse objects folder
			bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; //return everything

					UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);

					for (int i = 0; i < bResp.resultsSize; ++i) {
						for (int j = 0; j < bResp.results[i].referencesSize; ++j) {
							UA_ReferenceDescription *ref = &(bResp.results[i].references[j]);

							sprintf(node_id_string, "%.*s", ref->nodeId.nodeId.identifier.string.length, ref->nodeId.nodeId.identifier.string.data);
							if(!strcmp(fb_id, node_id_string)){
								node_found = 1;
							}
						}
					}

					UA_BrowseRequest_deleteMembers(&bReq);
					UA_BrowseResponse_deleteMembers(&bResp);

					if(!node_found){
						DEVLOG_INFO("OPC UA FB OBJECT create\n");

						/* New Object */
						UA_NodeId fbNode_id = createFBNode(client, fb_id);
						/* New Variable */
						UA_NodeId varNode_id = createVariableNode(client, fbNode_id, fb_id, var_id);
					}else{
						UA_NodeId fbNode_id = UA_NODEID_STRING_ALLOC(1, fb_id);

						int var_found = 0;

						UA_BrowseRequest bReq;
						UA_BrowseRequest_init(&bReq);
						bReq.requestedMaxReferencesPerNode = 0;
						bReq.nodesToBrowse = UA_BrowseDescription_new();
						bReq.nodesToBrowseSize = 1;
						bReq.nodesToBrowse[0].nodeId = fbNode_id;
						bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; //revarBrowse_idturn everything

						UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);

						for (int i = 0; i < bResp.resultsSize; ++i) {
							for (int j = 0; j < bResp.results[i].referencesSize; ++j) {
								UA_ReferenceDescription *ref = &(bResp.results[i].references[j]);

								sprintf(node_id_string, "%.*s", ref->nodeId.nodeId.identifier.string.length, ref->nodeId.nodeId.identifier.string.data);
								if(!strcmp(varBrowse_id, node_id_string)){
									var_found = 1;
								}
							}
						}

						UA_BrowseRequest_deleteMembers(&bReq);
						UA_BrowseResponse_deleteMembers(&bResp);

						if(var_found){
							printf("Variable node found\n");

							UA_StatusCode retval = UA_Client_deleteNode(
									client,
									UA_NODEID_STRING_ALLOC(1, varBrowse_id),
									false
							);

							if(retval == UA_STATUSCODE_GOOD )
								printf("Variable removed %s \n", varBrowse_id);
							else
								printf("Error removing variable : %x\n", retval);
						}
						UA_NodeId varNode_id = createVariableNode(client, fbNode_id, fb_id, var_id);
					}



					UA_Client_disconnect(client);
					UA_Client_delete(client);


		}else{
			DEVLOG_INFO("OPC UA not - connected\n");
		}

		sendOutputEvent(scm_nEventCNFID);
		break;
	}
}
for(i = 0; i < numData; ++i){
	m_apoDIConns[i] = 0;
	createDataPoint(&pnDataIds, pa_acFBVarsData);
	pa_acFBVarsData += sizeof(CIEC_ANY);
}



}



EComResponse COPC_UA_Layer::sendData(void *paData, unsigned int paSize){
	EComResponse retVal = e_ProcessDataOk;

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
		//for(int i = 0; i < paNumData; i++){
		//	if(0 != paLayerParameter){
		//		nextParam = strchr()
		//	}
		//m_ppstOPCUAItem;
		//}


		//1. Create Struct for each item
		//2. Create array of structs holding these items
		//2.1 or maybe do a typecast and inherit the SD's data value to the struct member
		//3. register a datapoint, maybe this does not work with OPC_UA

		//CIEC_ANY * pa_FB_DataInputs =
		//CIEC_ANY aPortIDs [paNumData];
		CIEC_ANY aPortIDs;

		for(int i = 0; i<=paNumData; i++){
			TPortId portID = TPortId(i);
			aPortIDs = getCommFB()->getDIFromPortId(portID);

		}



		return retVal;
	}
}



