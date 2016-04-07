/*
 * opcua_layer.h
 *
 *  Created on: 01.03.2016
 *      Author: florian
 */

#ifndef SRC_MODULES_OPC_UA_OPCUA_LAYER_H_
#define SRC_MODULES_OPC_UA_OPCUA_LAYER_H_

#include <comlayer.h>
#include <forte_any.h>
#include <open62541/build/open62541.h>


class COPC_UA_Layer : public forte::com_infra::CComLayer{
public:
	COPC_UA_Layer(forte::com_infra::CComLayer* pa_poUpperLayer, forte::com_infra::CCommFB* pa_poComFB);
	virtual ~COPC_UA_Layer();

	void closeConnection();
	forte::com_infra::EComResponse sendData(void* pa_pvData, unsigned int pa_unSize);
	forte::com_infra::EComResponse recvData(const void * pa_pvData, unsigned int pa_unSize);
	forte::com_infra::EComResponse processInterrupt();


private:
	forte::com_infra::EComResponse openConnection(char * paLayerParameter);
	struct {UA_NodeId **ppNodeId_ParentFB;	//!< pointer to the array of Parent FunctionBlock NodeIds managed by the OCP_UA Server Stack
			UA_NodeId **ppNodeId_SrcPoint;} st_ParentChildNodeId;	//!< pointer to the array of SourcePoint (SDs) NodeIds managed by the OCP_UA Server Stack

	forte::com_infra::EComResponse createItems(CIEC_ANY *paDataArray, int paNumData, char *paLayerParameter);
};

#endif /* SRC_MODULES_OPC_UA_OPCUA_LAYER_H_ */
