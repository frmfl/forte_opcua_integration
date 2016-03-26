/*
 * opcuahandler.cpp
 *
 *  Created on: 02.03.2016
 *      Author: florian
 */

#include "opcuahandler.h"
#include "devlog.h"

#include <cstdbool>


UA_StatusCode statCode;

DEFINE_SINGLETON(COPC_UA_Handler);
//UA_ServerNetworkLayer()


COPC_UA_Handler::COPC_UA_Handler() : m_server_config(0), m_server_networklayer(0), mbServerRunning(true) {

	*m_server_config = UA_ServerConfig_standard;
	m_server_networklayer = UA_ServerNetworkLayerTCP(UA_ConnectionConfig_standard, 16664);
	m_server_config->enableUsernamePasswordLogin = false;
	m_server_config->networkLayersSize = 1;
	m_server_config->logger = Logger_Stdout;
	m_server_config->networkLayers = &m_server_networklayer;
	mOPCUAServer = UA_Server_new(*m_server_config);	// create the server specified in config
	UA_StatusCode retVal = UA_Server_run(mOPCUAServer, mbServerRunning);	// server keeps iterating as long as running is true
	UA_LOG_INFO()
	//add a namespace in xml format to the server containing the application configuration.
	//UA_Server_addExternalNamespace()
}

COPC_UA_Handler::~COPC_UA_Handler() {
	this->stop();
	UA_Server_delete(mOPCUAServer);
	m_server_networklayer->deleteMembers(m_server_networklayer);


}

UA_Server * COPC_UA_Handler::getServer(){
	return mOPCUAServer;
}


void COPC_UA_Handler::run(){
	mbServerRunning = static_cast<UA_Boolean *>(true);
	//nThreads = 1;
	//UA_StatusCode retval = UA_Server_run(mOPCUAServer, nThreads, mbServerRunning);
	//FIXME add forward declaration of enum to obtain server status information
}

void COPC_UA_Handler::stop(){
	mbServerRunning = static_cast<UA_Boolean *>(false);
	//nThreads = 1;
	UA_StatusCode statCode;
	//UA_StatusCode retval = UA_Server_run(mOPCUAServer, mbServerRunning);
	//return statCode;
	//FIXME add forward declaration of enum to obtain server status information

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


