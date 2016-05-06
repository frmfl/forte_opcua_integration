/*******************************************************************************
 * Copyright (c) 2015-2016 Florian Froschermeier <florian.froschermeier@tum.de>
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Florian Froschermeier
 *      - initial integration of the OPC-UA protocol
 *******************************************************************************/


#include "opcuahandler.h"
#include <string.h>
#include <cstdbool>

using namespace forte::com_infra;

DEFINE_SINGLETON(COPC_UA_Handler);

const int COPC_UA_Handler::scmUADataTypeMapping[] = {
		/* Datatype mapping of IEC61131 types to OPC-UA types according
		 * to OPC UA standard specification release 1.0,
		 * "Information Model" Table 26, Section 5.2 Datatypes;
		 */

		UA_TYPES_VARIANT, //e_ANY,
		UA_TYPES_BOOLEAN, //e_BOOL,
		UA_TYPES_SBYTE, //e_SINT,
		UA_TYPES_INT16,	//e_INT
		UA_TYPES_INT32, //e_DINT
		UA_TYPES_INT64, //e_LINT
		UA_TYPES_BYTE, //e_USINT,
		UA_TYPES_UINT16, //e_UINT
		UA_TYPES_UINT32, //e_UDINT
		UA_TYPES_UINT64, //e_ULINT
		UA_TYPES_BYTE, //e_BYTE
		UA_TYPES_UINT16, //e_WORD
		UA_TYPES_UINT32, //e_DWORD
		UA_TYPES_UINT64, //e_LWORD
		UA_TYPES_DATETIME, //e_DATE,
		UA_TYPES_DATETIME, //e_TIME_OF_DAY,
		UA_TYPES_DATETIME, //e_DATE_AND_TIME,
		UA_TYPES_DOUBLE, //e_TIME, //until here simple Datatypes
		UA_TYPES_FLOAT, //e_REAL
		UA_TYPES_DOUBLE, //e_LREAL
		UA_TYPES_STRING, //e_STRING
		UA_TYPES_STRING //e_WSTRING,

		//TODO implement mapping for following datatypes.
		//e_DerivedData,
		//e_DirectlyDerivedData,
		//e_EnumeratedData,
		//e_SubrangeData,
		//e_ARRAY, //according to the compliance profile
		//e_STRUCT,
		//e_External = 256, // Base for CIEC_ANY based types outside of the forte base
		//e_Max = 65535 // Guarantees at least 16 bits - otherwise gcc will optimizes on some platforms
};

void COPC_UA_Handler::configureUAServer() {
	m_server_config = UA_ServerConfig_standard;
	m_server_config.enableUsernamePasswordLogin = false;
	m_server_config.networkLayersSize = 1;
	m_server_config.logger = Logger_Stdout;
	m_server_networklayer = UA_ServerNetworkLayerTCP(UA_ConnectionConfig_standard, 16664);
	m_server_config.networkLayers = &m_server_networklayer;
}


void COPC_UA_Handler::createUAServer(){
	mOPCUAServer = UA_Server_new(m_server_config);	// create the server specified in config

}

COPC_UA_Handler::COPC_UA_Handler() : m_server_config(), m_server_networklayer(){

	configureUAServer(); 	// configure a standard server
	createUAServer();	// create an OPC_UA Server with specified configuration
	setServerRunning();		// set server loop flag

	if(!isAlive()){
		//thread is not running start it
		start();
	}

	// OPTION: add a namespace in xml format to the server containing the application configuration.
	//UA_Server_addExternalNamespace()
}

COPC_UA_Handler::~COPC_UA_Handler() {
	stopServerRunning();
	UA_Server_delete(mOPCUAServer);
	m_server_networklayer.deleteMembers(&m_server_networklayer);
}

void COPC_UA_Handler::run(){
	UA_StatusCode retVal = UA_Server_run(mOPCUAServer, mbServerRunning);	// server keeps iterating as long as running is true;
	DEVLOG_INFO("UA_Server run status code %s", retVal);
}

void COPC_UA_Handler::enableHandler(void){
	start();
}

void COPC_UA_Handler::disableHandler(void){
	COPC_UA_Handler::stopServerRunning();
	end();
}

void COPC_UA_Handler::setPriority(int){
	//currently we are doing nothing here.
	//TODO We should adjust the thread priority.
}

int COPC_UA_Handler::getPriority(void) const{
	//the same as for setPriority
	return 0;
}

UA_Server * COPC_UA_Handler::getServer(){
	return mOPCUAServer;
}


void COPC_UA_Handler::setServerRunning(){
	*mbServerRunning = UA_TRUE;
}

void COPC_UA_Handler::stopServerRunning(){
	*mbServerRunning = UA_FALSE;
}


void COPC_UA_Handler::registerNode(){
}


/*
 * Get Function Block Node Id from the pointer to a CFunctionBlock.
 * Method is used to check if a not to the pointed function block already
 * exists in the address space of the OPC-UA Server.
 */

UA_StatusCode COPC_UA_Handler::getFBNodeId(const CFunctionBlock* pCFB, UA_NodeId* returnFBNodeId){
	UA_StatusCode retVal = UA_STATUSCODE_GOOD;
	const char* FBInstanceName = pCFB->getInstanceName();	// Name of the SourcePoint function block
	UA_NodeId FBNodeId = UA_NODEID_STRING_ALLOC(1, FBInstanceName);		// Create new FBNodeId from c string

	UA_NodeId* returnNodeId = UA_NodeId_new();
	retVal = UA_Server_readNodeId(mOPCUAServer, FBNodeId, returnNodeId);		// read node of given ID
	if(retVal != UA_STATUSCODE_GOOD){
		return retVal;		// reading not successful
	}else{
		retVal = UA_NodeId_copy(returnNodeId, returnFBNodeId);	// reading successful, return NodeId
	};
	return retVal;
}


UA_StatusCode COPC_UA_Handler::getSPNodeId(const CFunctionBlock *pCFB, SConnectionPoint& sourceRD, UA_NodeId* returnSPNodeId){
	UA_StatusCode retVal = UA_STATUSCODE_GOOD;

	// Reading the node without reference to parent node id, unknown if this works.
	//FIXME needs further testing with OPC_UA Address Space Browser and example node
	const SFBInterfaceSpec* sourceFBInterface = pCFB->getFBInterfaceSpec();

	CStringDictionary::TStringId SPNameId = sourceFBInterface->m_aunDONames[sourceRD.mPortId];
	const char * SPName = CStringDictionary::getInstance().get(SPNameId);
	UA_NodeId SPNodeId = UA_NODEID_STRING_ALLOC(1, SPName);

	UA_NodeId* returnNodeId = UA_NodeId_new();
	retVal = UA_Server_readNodeId(mOPCUAServer,SPNodeId, returnNodeId);		// read node of given ID
	if(retVal != UA_STATUSCODE_GOOD){
		return retVal;		// reading not successful
	}else{
		retVal = UA_NodeId_copy(returnNodeId, returnSPNodeId);	// reading successful, return NodeId
	};
	return retVal;
}

/* Function creates an address space object node defined by a given pointer to
 * a control function block. If creation successful the NodeId is returned otherwise
 * UA_StatusCode from node creation with error message.
 */
UA_StatusCode COPC_UA_Handler::createUAObjNode(const CFunctionBlock* pCFB, UA_NodeId * returnObjNodeId){

	// retrieve parent function block name
	CStringDictionary::TStringId sourceFBNameId = pCFB->getInstanceNameId();
	const char* srcFBName = CStringDictionary::getInstance().get(sourceFBNameId);

	// set UA NodeId attributes
	UA_NodeId newObjNodeId = UA_NODEID_STRING_ALLOC(1, srcFBName);
	UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
	UA_NodeId parentReferenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
	UA_QualifiedName objBrowseName = UA_QUALIFIEDNAME_ALLOC(0, srcFBName);
	UA_NodeId objTypeDefinition = UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE);

	UA_ObjectAttributes obj_attr;
	UA_ObjectAttributes_init(&obj_attr);
	char dispName[32];
	sprintf(dispName, "FB-%s\n", srcFBName);
	obj_attr.displayName = UA_LOCALIZEDTEXT("en_US", dispName);
	char descpName[64];
	sprintf(descpName, "Object node of FB-%s, origin: Publisher\n", srcFBName);

	obj_attr.description =  UA_LOCALIZEDTEXT("en_US", descpName);
	UA_NodeId * returnNodeId = UA_NodeId_new();
	UA_StatusCode retVal = UA_Server_addObjectNode(
			mOPCUAServer,                 // server
			newObjNodeId,              	  // requestedNewNodeId
			parentNodeId,                 // parentNodeId
			parentReferenceTypeId,        // referenceTypeId
			objBrowseName,                // browseName
			objTypeDefinition,               // typeDefinition
			obj_attr,                     // Variable attributes
			NULL,                         // instantiation callback
			returnNodeId);			  	  // return Node Id

	if(retVal == UA_STATUSCODE_GOOD){
		DEVLOG_INFO("UA-Server AddressSpace: New Object Node - %s added.\n", dispName);
		retVal = UA_NodeId_copy(returnNodeId, returnObjNodeId);
	}else{
		DEVLOG_INFO("UA-Server AddressSpace: Adding Object Node %s failed. Message: %x\n", dispName, retVal);
	}
	return retVal;
}

/* For a given connection SourcePoint between two 61499 FBs add
 * a variable Node to the OPC_UA address space.
 * Node is described by the name of the port and the name of the parent function block.
 * If creation successful the NodeId is returned otherwise
 * UA_StatusCode from node creation with error message.
 */
UA_StatusCode COPC_UA_Handler::createUAVarNode(const CFunctionBlock* pCFB, SConnectionPoint& sourceRD, UA_NodeId * returnVarNodeId){
	// retrieve parent function block name
	CStringDictionary::TStringId sourceFBNameId = pCFB->getInstanceNameId();
	const char* srcFBName = CStringDictionary::getInstance().get(sourceFBNameId);

	// get parent function block interface
	const SFBInterfaceSpec* sourceFBInterface = pCFB->getFBInterfaceSpec();

	// retrieve source port name
	CStringDictionary::TStringId SPNameId = sourceFBInterface->m_aunDONames[sourceRD.mPortId];
	const char * SPName = CStringDictionary::getInstance().get(SPNameId);

	// set UA NodeId attributes
	UA_NodeId newVarNodeId = UA_NODEID_STRING_ALLOC(1,SPName);
	UA_NodeId parentNodeId = UA_NODEID_STRING_ALLOC(1, srcFBName);
	UA_NodeId parentReferenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
	char browsename[32];
	sprintf(browsename, "%s\n", SPName);
	UA_QualifiedName varBrowseName = UA_QUALIFIEDNAME(1, browsename);
	UA_NodeId typeDefinition = UA_NODEID_NULL;

	// attribute value
	UA_UInt32 myInteger = 42;

	// create variable attributes
	UA_VariableAttributes var_attr;
	UA_VariableAttributes_init(&var_attr);

	char display[32];
	sprintf(display, "SD-%s\n", SPName);

	var_attr.displayName = UA_LOCALIZEDTEXT("en_US", display);
	var_attr.description = UA_LOCALIZEDTEXT("en_US", "SD port of Publisher");
	UA_Variant_setScalar(&var_attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);

	// add UA Variable Node to the server address space
	UA_NodeId * returnNodeId = UA_NodeId_new();
	UA_StatusCode retVal = UA_Server_addVariableNode(
			mOPCUAServer,                 // server
			newVarNodeId,              	  // requestedNewNodeId
			parentNodeId,                 // parentNodeId
			parentReferenceTypeId,        // referenceTypeId
			varBrowseName,                // browseName
			typeDefinition,               // typeDefinition
			var_attr,                     // Variable attributes
			NULL,                         // instantiation callback
			returnNodeId);			  	  // return Node Id


	if(retVal == UA_STATUSCODE_GOOD){
		DEVLOG_INFO("UA-Server AddressSpace: New Variable Node - %s added.\n", browsename);
		retVal = UA_NodeId_copy(returnNodeId, returnVarNodeId);
	}else{
		DEVLOG_INFO("UA-Server AddressSpace: Adding Variable Node %s failed. Message: %x\n", browsename, retVal);
	}
	return retVal;
}

/*
 * Update UA Address Space node value given by the data pointer to an IEC61499 data object.
 * Mapping of IEC61499 to OPC-UA types is performed by scmUADataTypeMapping array.
 */
void COPC_UA_Handler::updateNodeValue(UA_NodeId * pNodeId, CIEC_ANY &paDataPoint){
	UA_Variant NodeValue;		// new Variant object
	UA_Variant_init(&NodeValue);

	UA_Variant_setScalarCopy(&NodeValue, static_cast<const void *>(paDataPoint.getConstDataPtr()),
			&UA_TYPES[scmUADataTypeMapping[paDataPoint.getDataTypeID()]]);
	UA_Server_writeValue(mOPCUAServer, *pNodeId, NodeValue);
}

/* Register a callback routine to a Node in the Address Space that is executed
 * on either write or read access on the node. A handle to the caller communication layer
 * is passed too. This alleviates for the process of searching the
 * originating layer of the external event.
 */
void COPC_UA_Handler::registerNodeCallBack(UA_NodeId *paNodeId, forte::com_infra::CComLayer *paLayer){
	UA_ValueCallback callback = {static_cast<void *>(paLayer), NULL, onWrite};
	UA_StatusCode retVal = UA_Server_setVariableNode_valueCallback(mOPCUAServer, *paNodeId, callback);
}


void COPC_UA_Handler::onWrite(void *handle, const UA_NodeId nodeid, const UA_Variant *pa_pstValue,
		const UA_NumericRange *range){

	  forte::com_infra::CComLayer *layer = static_cast<forte::com_infra::CComLayer *>(handle);

	  EComResponse retVal = layer->recvData(static_cast<void *>(pa_pstValue), 0);


	  if(e_ProcessDataOk == retVal){
	    //only update data in item if data could be read
	    //sfp_item_update_data(pa_pstItem, pa_pstValue, sfp_time_in_millis());
	  }

	  if(e_Nothing != retVal){
	    getInstance().startNewEventChain(layer->getCommFB());
	  }
}


void COPC_UA_Handler::handleWriteNodeCallback(struct sfp_item * pa_pstItem, struct sfp_variant *pa_pstValue, int32_t pa_nOperationID,
		struct sfp_strategy * pa_pstStrategy,
		void (*handle_result)(struct sfp_strategy* strategy, int32_t operation_id, struct sfp_error_information* error),
		void * pa_pvCtx){
	CComLayer *layer = static_cast<CComLayer *>(pa_pvCtx);

	EComResponse retVal = layer->recvData(static_cast<void *>(pa_pstValue), 0);

	if(e_ProcessDataOk == retVal){
		//only update data in item if data could be read
		sfp_item_update_data(pa_pstItem, pa_pstValue, sfp_time_in_millis());
	}

	if(e_Nothing != retVal){
		getInstance().startNewEventChain(layer->getCommFB());
	}

	//  sfp_error_information_new(
	//
	handle_result(pa_pstStrategy, pa_nOperationID, NULL );
}


bool COPC_UA_Handler::readBackDataPoint(const struct sfp_variant *paValue, CIEC_ANY &paDataPoint){
	bool retVal = true;


	return retVal;
}







