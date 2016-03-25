/*
 * opcuahandler.cpp
 *
 *  Created on: 02.03.2016
 *      Author: florian
 */

#include "opcuahandler.h"

#include <cstdbool>


UA_StatusCode statCode;

DEFINE_SINGLETON(COPC_UA_Handler);
//UA_ServerNetworkLayer()


COPC_UA_Handler::COPC_UA_Handler(){

	UA_ServerConfig server_config = UA_ServerConfig_standard;
	UA_ServerNetworkLayer server_networklayer = UA_ServerNetworkLayerTCP(UA_ConnectionConfig_standard, 16664);
	server_config.enableUsernamePasswordLogin = false;
	server_config.enableAnonymousLogin = true;
	UA_Logger logger;
	server_config.logger = logger;
	server_config.networkLayers = &server_networklayer;
	mOPCUAServer = UA_Server_new(server_config);
	mbServerRunning = static_cast<UA_Boolean *>(false);
}

COPC_UA_Handler::~COPC_UA_Handler() {
	UA_Server_delete(mOPCUAServer);
}

UA_Server * COPC_UA_Handler::getServer(){
	return mOPCUAServer;
}


void COPC_UA_Handler::run(void){
	*mbServerRunning = true;
	//nThreads = 1;
	//UA_StatusCode retval = UA_Server_run(mOPCUAServer, nThreads, mbServerRunning);

	//UA_StatusCode retval = UA_Server_run(mOPCUAServer, mbServerRunning);//return statCode;
	// need a status code mapping.
	//FIXME add forward declaration of enum to obtain server status information
}

void COPC_UA_Handler::stop(void){
	*mbServerRunning = false;
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


