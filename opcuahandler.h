/*
 * opcuahandler.h
 *
 *  Created on: 02.03.2016
 *      Author: florian
 */

#ifndef SRC_MODULES_OPC_UA_OPCUAHANDLER_H_
#define SRC_MODULES_OPC_UA_OPCUAHANDLER_H_

#include <open62541/build/open62541.h>
#include <singlet.h>
#include <thread.h>
#include <extevhan.h>

struct UA_Server;	//forward declaration


class COPC_UA_Handler : public CExternalEventHandler, public CThread{
	DECLARE_SINGLETON(COPC_UA_Handler);

public:
    /* functions needed for the external event handler interface */
    void enableHandler(void);
    void disableHandler(void);
    void setPriority(int pa_nPriority);
    int getPriority(void) const;
    UA_Server * getServer(void);
    void registerNode();

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
    //struct opcua_server * mOPCUAServer;
    UA_Server * mOPCUAServer;
    volatile UA_Boolean* mbServerRunning;
    UA_ServerConfig *m_server_config;
    UA_ServerNetworkLayer *m_server_networklayer;
    //static UA_StatusCode mStatusCode;

    void run();
    void stop();
    //static forte::com_infra::EComResponse m_eComResponse;
	void configureServer();
};

#endif /* SRC_MODULES_OPC_UA_OPCUAHANDLER_H_ */
