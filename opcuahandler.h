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


#ifndef SRC_MODULES_OPC_UA_OPCUAHANDLER_H_
#define SRC_MODULES_OPC_UA_OPCUAHANDLER_H_

#include <open62541/build/open62541.h>
#include <singlet.h>
#include <thread.h>
#include <extevhan.h>
#include <conn.h>
#include "funcbloc.h"
#include <stdio.h>
#include "../../arch/devlog.h"

//struct UA_Server;	//forward declaration
//struct SConnectionPoint;
//struct SFBInterfaceSpec;


class COPC_UA_Handler : public CExternalEventHandler, public CThread{
	DECLARE_SINGLETON(COPC_UA_Handler);

public:


	/* functions needed for the external event handler interface */
	void enableHandler(void);
	void disableHandler(void);
	void setPriority(int pa_nPriority);
	int getPriority(void) const;

	UA_Server * getServer(void);
	UA_Client * getClient(void);

	UA_StatusCode getFBNodeId(const CFunctionBlock *pCFB, UA_NodeId* returnFBNodeId);		// get function block (FB) NodeId
	UA_StatusCode getSPNodeId(const CFunctionBlock *pCFB, SConnectionPoint& sourceRD, UA_NodeId* returnSPNodeId);		// get source point (SP) NodeId
	UA_StatusCode createUAVarNode(const CFunctionBlock* pCFB, SConnectionPoint& sourceRD, UA_NodeId * returnVarNodeId);	// create variable node from SourcePoint Node Id
	UA_StatusCode createUAObjNode(const CFunctionBlock* pCFB, UA_NodeId * returnObjNodeId);	// create object node from Parent Function Block Node Id

protected:

	//UA_StatusCode runServer(void);
	//UA_StatusCode stop(void);
	//FIXME pass a pointer to a struct containing the node information
	//FIXME add a struct entry containing the type of not that shall be created
	//UA_StatusCode addServerNode();
	/*1. What node shall be created
	 * 2. what are the parent nodes
	 * 3. Will the address space exist from startup
	 * 4. How to then retrieve that node data from the server
	 * 5. check if node exists, then only update the value
	 * 6. otherwise create the node.
	 * 7. Does node exist? ->NO -> create the address space.
	 *
	 * Important:
	 * From the first node that is to be created, construct the parents node first.
	 * Then the parents node of the parents node, such that slowly the complete addres space is built.
	 * To send a value on INIT is also important such that the first value is not empty until the publisher
	 * publishes once.
	 */

private:
	// OPC_UA Server and configuration
	UA_Server * mOPCUAServer;
	UA_ServerConfig m_server_config;

	// OPC_UA Client and configuration
	volatile UA_Boolean* mbServerRunning = new UA_Boolean(UA_TRUE);
	UA_ServerNetworkLayer m_server_networklayer;
	//static UA_StatusCode mStatusCode;


	void setServerRunning();
	void stopServerRunning();
	//static forte::com_infra::EComResponse m_eComResponse;
	void configureUAServer();
	void createUAServer();

	// implementation of thread.h virtual method start
	virtual void run();

	void registerNode();
};

#endif /* SRC_MODULES_OPC_UA_OPCUAHANDLER_H_ */
