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



DEFINE_SINGLETON(COPC_UA_Handler);

const int COPC_UA_Handler::scmUADataTypeMapping[] = {
		0, //e_ANY,
		UA_TYPES_BOOLEAN, //e_BOOL,
//		e_SINT, e_INT, e_DINT, e_LINT, e_USINT, e_UINT, e_UDINT, e_ULINT, e_BYTE, e_WORD, e_DWORD, e_LWORD, e_DATE, e_TIME_OF_DAY, e_DATE_AND_TIME, e_TIME, //until here simple Datatypes
//		      e_REAL,
//		      e_LREAL,
//		      e_STRING,
//		      e_WSTRING
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

	/*
	char *fb_name;
	strcpy(fb_name, NodeAttr->fb_name);

	char *var_name;
	strcpy(var_name, NodeAttr->port_name);

	char *var_id;
	strcpy(var_id, NodeAttr->port_id);

	UA_Client *myClient = getClient();


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

	//createObjectNode
	//nodetype depends on the return value of the client. If parent node already exists, then construct
	//variable node. If variable node exists, do nothing.
	/*switch (nodetype)
	{
	case "object":
	   Aktion1
	   break;
	case "variable":
	   Aktion2
	   break;

	default:
	   Aktion4
	}
	 */
	/*

	UA_Server_addVariableNode(UA_Server *server, const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName, const UA_NodeId typeDefinition,
			const UA_VariableAttributes attr, UA_InstantiationCallback *instantiationCallback, UA_NodeId *outNewNodeId)

					UA_Server_addObjectNode(UA_Server *server, const UA_NodeId requestedNewNodeId,
							const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId,
							const UA_QualifiedName browseName, const UA_NodeId typeDefinition,
							const UA_ObjectAttributes attr, UA_InstantiationCallback *instantiationCallback, UA_NodeId *outNewNodeId)

	UA_VariableAttributes attr;
	UA_VariableAttributes_init(&attr);
	UA_Int32 myInteger = 42;
	UA_Variant_setScalar(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
	attr.description = UA_LOCALIZEDTEXT("en_US","the answer");
	attr.displayName = UA_LOCALIZEDTEXT("en_US","the answer");


	UA_NodeId objNodeId = UA_NODEID_STRING(1, "system.node");
	UA_NodeId_init(&objNodeId);
	UA_QualifiedName



	//createVariableNode
	 */
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


	// Careful container!
	/*CStringDictionary::TStringId sourceFBTypeNameId = sourceFB->getFBTypeId();
						const char * sourceFBTypeName = CStringDictionary::getInstance().get(sourceFBTypeNameId);
						//FIXME Retrieve System name

						CStringDictionary::TStringId sourceFBNameId = sourceFB->getInstanceNameId();
						const char* sourceFBName = sourceFB->getInstanceName();

						SFBInterfaceSpec* sourceFBInterface = sourceFB->getFBInterfaceSpec();

						CStringDictionary::TStringId sourceRDNameId = sourceFBInterface->m_aunDONames[sourceRD.mPortId];
						const char * sourceRDName = CStringDictionary::getInstance().get(sourceRDNameId);

						CStringDictionary::TStringId sourceRDTypeNameId = sourceFBInterface->m_aunDODataTypeNames[sourceRD.mPortId];
						const char * sourceRDTypeName = CStringDictionary::getInstance().get(sourceRDTypeNameId);

						char message[128];
						sprintf(message,"%s %s %s\n", sourceFBName, sourceFBTypeName, sourceRDName, sourceRDTypeName);
						DEVLOG_INFO(message);

						char *fb_name;
						strcpy(fb_name, sourceFBName);

						char *fb_typename;
						strcpy(fb_typename, sourceFBTypeName);

						char *var_name;
						strcpy(var_name, sourceRDName);

						char *var_typename;
						strcpy(var_typename, sourceRDTypeName);

						char varBrowse_id[64];
						sprintf(varBrowse_id, "%s.%s", fb_name, var_name);

						COPC_UA_Handler::getInstance().NodeAttr.fb_name = fb_name;
						COPC_UA_Handler::getInstance().NodeAttr.fb_typename = fb_typename;
						COPC_UA_Handler::getInstance().NodeAttr.port_name = var_name;
						COPC_UA_Handler::getInstance().NodeAttr.port_typename = var_typename;
	 */

}


UA_StatusCode COPC_UA_Handler::getSPNodeId(const CFunctionBlock *pCFB, SConnectionPoint& sourceRD, UA_NodeId* returnSPNodeId){
	UA_StatusCode retVal = UA_STATUSCODE_GOOD;

	// Important Comment: Reading the node without reference to parent node id, unknown if this works.
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


// ((CIEC_BOOL &) paDataPoint).operator bool());

void COPC_UA_Handler::updateNodeValue(UA_NodeId * pNodeId, CIEC_ANY &paDataPoint){
	UA_Variant NodeValue;		// new Variant object
	UA_Variant_init(&NodeValue);

	UA_Variant_setScalarCopy(&NodeValue, static_cast<const void *>(paDataPoint.getConstDataPtr()),
			&UA_TYPES[scmUADataTypeMapping[paDataPoint.getDataTypeID()]]);
    UA_Server_writeValue(mOPCUAServer, *pNodeId, NodeValue);


	switch (paDataPoint.getDataTypeID()){
	case CIEC_ANY::e_BOOL:
		UA_Variant_setScalarCopy(&NodeValue, &((CIEC_BOOL &) paDataPoint).operator bool(), &UA_TYPES[UA_TYPES_BOOLEAN]);
		//UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
	case CIEC_ANY::e_SINT:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_SINT &) paDataPoint).operator signed char(), &UA_TYPES[UA_TYPES_SBYTE]);
		//UA_Variant_setScalarCopy(pNodeValue, paDataPoint.getConstDataPtr(), &UA_TYPES[UA_TYPES_SBYTE]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
	case CIEC_ANY::e_USINT:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_BYTE &) paDataPoint).operator unsigned char(), &UA_TYPES[UA_TYPES_BYTE]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
	case CIEC_ANY::e_INT:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_INT &) paDataPoint).operator short int(), &UA_TYPES[UA_TYPES_INT16]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
	case CIEC_ANY::e_UINT:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_UINT &) paDataPoint).operator unsigned short int(), &UA_TYPES[UA_TYPES_UINT16]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
	case CIEC_ANY::e_DINT:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_DINT &) paDataPoint).operator int(), &UA_TYPES[UA_TYPES_INT32]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
	case CIEC_ANY::e_UDINT:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_UDINT &) paDataPoint).operator unsigned int(), &UA_TYPES[UA_TYPES_UINT32]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
		/*
	case CIEC_ANY::e_LINT:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_L &) paDataPoint). &UA_TYPES[UA_TYPES_INT64]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
	case CIEC_ANY::e_ULINT:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_ &) paDataPoint), &UA_TYPES[UA_TYPES_UINT64]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
		 */
	case CIEC_ANY::e_BYTE:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_BYTE &) paDataPoint).operator unsigned char(), &UA_TYPES[UA_TYPES_BYTE]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
	case CIEC_ANY::e_WORD:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_WORD &) paDataPoint).operator unsigned short int(), &UA_TYPES[UA_TYPES_UINT16]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
	case CIEC_ANY::e_DWORD:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_DWORD &) paDataPoint).operator unsigned int(), &UA_TYPES[UA_TYPES_UINT32]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
		/*#ifdef FORTE_USE_64BIT_DATATYPES
	case CIEC_ANY::e_LWORD:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_LWORD &) paDataPoint), &UA_TYPES[UA_TYPES_UINT64]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
#endif
	case CIEC_ANY::e_REAL:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_UINT &) paDataPoint), &UA_TYPES[UA_TYPES_FLOAT]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
	case CIEC_ANY::e_LREAL:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_UINT &) paDataPoint), &UA_TYPES[UA_TYPES_DOUBLE]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
	case CIEC_ANY::e_STRING:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_UINT &) paDataPoint), &UA_TYPES[UA_TYPES_STRING]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
	case CIEC_ANY::e_WSTRING:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_UINT &) paDataPoint), &UA_TYPES[UA_TYPES_STRING]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
		//  case CIEC_ANY::e_CHAR:		not defined in forte
		//  case CIEC_ANY::e_WCHAR:		not defined in forte
	case CIEC_ANY::e_DATE_AND_TIME:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_UINT &) paDataPoint), &UA_TYPES[UA_TYPES_DATETIME]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
	case CIEC_ANY::e_DATE:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_UINT &) paDataPoint), &UA_TYPES[UA_TYPES_DATETIME]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
	case CIEC_ANY::e_TIME_OF_DAY:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_UINT &) paDataPoint), &UA_TYPES[UA_TYPES_DATETIME]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
	case CIEC_ANY::e_TIME:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_UINT &) paDataPoint), &UA_TYPES[UA_TYPES_DOUBLE]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
	case CIEC_ANY::e_ANY:
		UA_Variant_setScalarCopy(pNodeValue, &((CIEC_UINT &) paDataPoint), &UA_TYPES[UA_NS0ID_BASEDATATYPE]);
		UA_Server_writeValue(mOPCUAServer, *pNodeId, *pNodeValue);
		break;
		 */
	default:
		break;
	}
}




/*enum EComResponse{
  e_Nothing = 0,
  e_InitOk = e_InitPositive | e_Ok,
  e_ProcessDataOk = e_ProcessDataPositive | e_Ok,
  e_InitInvalidId = e_InitNegative | e_InvalidId,
  e_InitTerminated = e_InitNegative | e_Terminated,
  e_ProcessDataInvalidObject = e_ProcessDataNegative | e_InvalidObject,
  e_ProcessDataDataTypeError = e_ProcessDataNegative | e_DataTypeError,
  e_ProcessDataInhibited = e_ProcessDataNegative | e_Inhibited,
  e_ProcessDataNoSocket = e_ProcessDataNegative | e_NoSocket,
  e_ProcessDataSendFailed = e_ProcessDataNegative | e_SendFailed,
  e_ProcessDataRecvFaild = e_ProcessDataNegative | e_RecvFailed

 */


/*
 * static UA_StatusCode
readLedStatus(void *handle, UA_NodeId nodeid, UA_Boolean sourceTimeStamp,
              const UA_NumericRange *range, UA_DataValue *value) {
    if(range)
        return UA_STATUSCODE_BADINDEXRANGEINVALID;

    value->hasValue = true;
    UA_StatusCode retval = UA_Variant_setScalarCopy(&value->value, &ledStatus,
                                                    &UA_TYPES[UA_TYPES_BOOLEAN]);

    if(retval != UA_STATUSCODE_GOOD)
        return retval;

    if(sourceTimeStamp) {
        value->sourceTimestamp = UA_DateTime_now();
        value->hasSourceTimestamp = true;
    }
    return UA_STATUSCODE_GOOD;
}
 */

/*
 * /* LED control for rpi */
//    if(access("/sys/class/leds/led0/trigger", F_OK ) != -1 ||
//       access("/sys/class/leds/led0/brightness", F_OK ) != -1) {
//        if((triggerFile = fopen("/sys/class/leds/led0/trigger", "w")) &&
//           (ledFile = fopen("/sys/class/leds/led0/brightness", "w"))) {
//            //setting led mode to manual
//            fprintf(triggerFile, "%s", "none");
//            fflush(triggerFile);
//
//            //turning off led initially
//            fprintf(ledFile, "%s", "1");
//            fflush(ledFile);
//
//            // add node with the LED status data source
//            UA_DataSource ledStatusDataSource = (UA_DataSource) {
//                .handle = NULL, .read = readLedStatus, .write = writeLedStatus};
//            UA_VariableAttributes_init(&v_attr);
//            v_attr.description = UA_LOCALIZEDTEXT("en_US","status LED");
//            v_attr.displayName = UA_LOCALIZEDTEXT("en_US","status LED");
//            const UA_QualifiedName statusName = UA_QUALIFIEDNAME(0, "status LED");
//            UA_Server_addDataSourceVariableNode(server, UA_NODEID_NULL,
//                                                UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
//                                                UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), statusName,
//                                                UA_NODEID_NULL, v_attr, ledStatusDataSource, NULL);
//        } else
//            UA_LOG_WARNING(logger, UA_LOGCATEGORY_USERLAND,
//                           "[Raspberry Pi] LED file exist, but is not accessible (try to run server with sudo)");
//    }
/*
 *
 * There are four mechanisms for callbacks from the node-based information model
 * into userspace:
 *
 * - Datasources for variable nodes, where the variable content is managed
 *   externally
 * - Value-callbacks for variable nodes, where userspace is notified when a
 *   read/write occurs
 * - Object lifecycle management, where a user-defined constructor and
 *   destructor is added to an object type
 * - Method callbacks, where a user-defined method is exposed in the information
 *   model

 */


void COPC_UA_Handler::registerNodeCallBack(UA_NodeId *paNodeId, forte::com_infra::CComLayer *paLayer){
	UA_ValueCallback callback = {static_cast<void *>(paLayer), NULL, onWrite};
	UA_Server_setVariableNode_valueCallback(mOPCUAServer, paNodeId, callback);
}

void COPC_UA_Handler::onWrite(void *h, const UA_NodeId nodeid, const UA_Variant *data,
                    const UA_NumericRange *range) {
    UA_LOG_INFO(logger, UA_LOGCATEGORY_USERLAND, "onWrite; handle: %i", (uintptr_t)h);
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


/*
 typedef struct {
    void *handle;
    void (*onRead)(void *handle, const UA_NodeId nodeid,
                   const UA_Variant *data, const UA_NumericRange *range);
    void (*onWrite)(void *handle, const UA_NodeId nodeid,
                    const UA_Variant *data, const UA_NumericRange *range);
} UA_ValueCallback;

UA_StatusCode
UA_Server_setVariableNode_valueCallback(UA_Server *server, const UA_NodeId nodeId,
                                        const UA_ValueCallback callback);
 */

sfp_item_set_write_handler (
    struct sfp_item * item,
    void
    (*handle_write) (
	struct sfp_item * item,
	struct sfp_variant * value,
	int32_t operation_id,
	struct sfp_strategy * strategy,
	void
	(*handle_result) (struct sfp_strategy* strategy, int32_t operation_id,
			  struct sfp_error_information* error),
	void * ctx),
    void * ctx)
{
  item->handle_write_ctx = ctx;
  item->handle_write = handle_write;
}






bool COPC_UA_Handler::readBackDataPoint(const struct sfp_variant *paValue, CIEC_ANY &paDataPoint){
	bool retVal = true;
	switch (paDataPoint.getDataTypeID()){
	case CIEC_ANY::e_BOOL:
		if(VT_BOOLEAN == paValue->type){
			((CIEC_BOOL &) paDataPoint) = (paValue->payload.val_boolean != 0 );
		}else{
			retVal = false;
		}
		break;
	case CIEC_ANY::e_SINT:
	case CIEC_ANY::e_INT:
	case CIEC_ANY::e_DINT:
	case CIEC_ANY::e_USINT:
	case CIEC_ANY::e_UINT:
	case CIEC_ANY::e_BYTE:
	case CIEC_ANY::e_WORD:
		//Everything smaller or equal 32Bit will be handled here
		if(VT_INT32 == paValue->type){
			((CIEC_DINT &) paDataPoint) = paValue->payload.val_int;
		}else{
			retVal = false;
		}
		break;
	case CIEC_ANY::e_LINT:
	case CIEC_ANY::e_UDINT:
	case CIEC_ANY::e_ULINT:
	case CIEC_ANY::e_DWORD:
	case CIEC_ANY::e_LWORD:
		//Everything needing more than 32Bit signed integer will be handled here
		if(VT_INT64 == paValue->type){
			((CIEC_LINT &) paDataPoint) = paValue->payload.val_long;
		}else{
			retVal = false;
		}
	case CIEC_ANY::e_REAL:
		if(VT_DOUBLE == paValue->type){
			((CIEC_REAL &) paDataPoint) = static_cast<CIEC_REAL::TValueType>(paValue->payload.val_double);
		}else{
			retVal = false;
		}
		break;
	case CIEC_ANY::e_LREAL:
		if(VT_DOUBLE == paValue->type){
			((CIEC_LREAL &) paDataPoint) = paValue->payload.val_double;
		}else{
			retVal = false;
		}
		break;
	case CIEC_ANY::e_STRING:
	case CIEC_ANY::e_WSTRING:
		if(VT_BOOLEAN == paValue->type){
			((CIEC_ANY_STRING &) paDataPoint) = paValue->payload.val_string;
		}else{
			retVal = false;
		}
		break;
	default:
		//TODO think on how to handle these e_DATE, e_TIME_OF_DAY, e_DATE_AND_TIME, e_TIME
		retVal = false;
		break;
	}
	return retVal;
}







