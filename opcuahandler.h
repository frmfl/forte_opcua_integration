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
    //UA_StatusCode runServer(void);
    //UA_StatusCode stop(void);

private:
    //struct opcua_server * mOPCUAServer;
    UA_Server * mOPCUAServer;
    volatile UA_Boolean* mbServerRunning;
    //static UA_StatusCode mStatusCode;

    void run();
    void stop();
    //static forte::com_infra::EComResponse m_eComResponse;

};

#endif /* SRC_MODULES_OPC_UA_OPCUAHANDLER_H_ */
