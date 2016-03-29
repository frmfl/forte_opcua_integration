/*
 * opcuahandler.cpp
 *
 *  Created on: 02.03.2016
 *      Author: florian
 */

#include "opcuahandler.h"
#include "devlog.h"
#include <string.h>

#include <cstdbool>


UA_StatusCode statCode;

DEFINE_SINGLETON(COPC_UA_Handler);

UA_ServerConfig COPC_UA_Handler::configureUAServer() {
	m_server_config = UA_ServerConfig_standard;
	m_server_config.enableUsernamePasswordLogin = false;
	m_server_config.networkLayersSize = 1;
	m_server_config.logger = Logger_Stdout;
	m_server_networklayer = UA_ServerNetworkLayerTCP(UA_ConnectionConfig_standard, 16664);
	m_server_config.networkLayers = m_server_networklayer;
	return m_server_config;
}

UA_ClientConfig COPC_UA_Handler::configureUAClient(){
	m_client_config = UA_ClientConfig_standard;
	return m_client_config;
}

void COPC_UA_Handler::createUAClient(UA_ClientConfig m_client_config){
	mOPCUAClient = UA_Client_new(m_client_config, Logger_Stdout);
}

void COPC_UA_Handler::createUAServer(UA_ServerConfig m_server_config){
	mOPCUAServer = UA_Server_new(m_server_config);	// create the server specified in config
}

UA_Server * COPC_UA_Handler::getServer(){
	return mOPCUAServer;
}

UA_Client * COPC_UA_Handler::getClient(){
	return mOPCUAClient;
}

//UA_ServerNetworkLayer()


COPC_UA_Handler::COPC_UA_Handler() : m_server_config(0), m_server_networklayer(0), mbServerRunning(0) {
	configureUAServer(); 	// configure a standard server
	createUAServer(m_server_config);	// create
	runUAServer();
	UA_StatusCode retVal = UA_Server_run(mOPCUAServer, mbServerRunning);	// server keeps iterating as long as running is true
	DEVLOG_INFO("UA_Server run status code %s", retVal);

	//add a namespace in xml format to the server containing the application configuration.
	//UA_Server_addExternalNamespace()

}

COPC_UA_Handler::~COPC_UA_Handler() {
	stopUAServer();
	UA_Server_delete(mOPCUAServer);
	m_server_networklayer.deleteMembers(m_server_networklayer);
	destroyUAClient(getClient());
}




void COPC_UA_Handler::runUAServer(){
	*mbServerRunning = UA_Boolean (true);
	//	char mylogmsg = "Value of boolean 'running' ";
	//	logMessage(E_INFO, &mylogmsg, (char*) mbServerRunning);

	//UA_StatusCode retval = UA_Server_run(mOPCUAServer, nThreads, mbServerRunning);
	//FIXME add forward declaration of enum to obtain server status information
}

void COPC_UA_Handler::stopUAServer(){
	*mbServerRunning = UA_Boolean (false);
	//	char mylogmsg = "Value of boolean 'stop' ";
	//	logMessage(E_INFO, &mylogmsg, (char*) mbServerRunning);
	//nThreads = 1;
	//UA_StatusCode statCode;
	//UA_StatusCode retval = UA_Server_run(mOPCUAServer, mbServerRunning);
	//return statCode;
	//FIXME add forward declaration of enum to obtain server status information
}

UA_StatusCode COPC_UA_Handler::connectUAClient(UA_Client* client){
	UA_StatusCode retVal;
	retVal = UA_Client_connect(client, UA_ClientConnectionTCP, "opc.tcp://localhost:16664");
	return retVal;
}

void COPC_UA_Handler::destroyUAClient(UA_Client *client){
	UA_Client_disconnect(client);
	UA_Client_delete(client);
}


void COPC_UA_Handler::registerNode(){
	//createObjectNode

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


	UA_Server_addVariableNode(UA_Server *server, const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName, const UA_NodeId typeDefinition,
			const UA_VariableAttributes attr, UA_InstantiationCallback *instantiationCallback, UA_NodeId *outNewNodeId)


			UA_Server_addObjectNode(mOPCUAServer, );


	UA_NodeId newVarNodeId = UA_NODEID_STRING(1, "publisher_DI_node");
	UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
	UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
	UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME(1, "the answer");

	UA_Server_readNodeId();

	UA_Server_addVariableNode(mOPCUAServer, newVarNodeId, parentNodeId,
			parentReferenceNodeId, myIntegerName,
			UA_NODEID_NULL, attr, NULL, NULL);

	UA_VariableAttributes attr;
	UA_VariableAttributes_init(&attr);
	UA_Int32 myInteger = 42;
	UA_Variant_setScalar(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
	attr.description = UA_LOCALIZEDTEXT("en_US","the answer");
	attr.displayName = UA_LOCALIZEDTEXT("en_US","the answer");


	UA_NodeId objNodeId = UA_NODEID_STRING(1, "system.node");
	UA_NodeId_init(&objNodeId);
	UA_QualifiedName

	UA_Server_addObjectNode(UA_Server *server, const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName, const UA_NodeId typeDefinition,
			const UA_ObjectAttributes attr, UA_InstantiationCallback *instantiationCallback, UA_NodeId *outNewNodeId)


	//createVariableNode




}

void COPC_UA_Handler::enableHandler(void){

	//FIXME do nothing
}

void COPC_UA_Handler::disableHandler(void){

	//FIXME implement
}

void COPC_UA_Handler::setPriority(int pa_nPriority){

	//FIXME to implement
}

int COPC_UA_Handler::getPriority(void) const{

	return 0;
}


