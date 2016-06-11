#############################################################################
# OPC_UA Protocol Com Layer
#############################################################################

# CMake for Publish Subscribe Function Blocks
forte_add_module(OPC_UA "OPC_UA Function Blocks")

set(OPCUA_FB_PATH "61499_FBs")

forte_add_sourcefile_hcpp(${OPCUA_FB_PATH}/AOPCUANodeID ${OPCUA_FB_PATH}/GEN_SUBSCRIBEOPCUA ${OPCUA_FB_PATH}/GEN_PUBLISHOPCUA ${OPCUA_FB_PATH}/OPCUA_NodeID)

forte_add_definition(-std=c++11)

forte_add_network_layer(OPC_UA OFF "opc_ua" COPC_UA_Layer opcua_layer "Enable OPC_UA Com Layer")

# path to open62541 OPC-UA server library
SET(FORTE_COM_OPC_UA_DIR "" CACHE PATH "Path to the OPC_UA (open62541 library) build folder (leave empty for installed source code)")

# add cmake flag for external init namespace that is compiled into the server
option(FORTE_COM_OPC_UA_ENABLE_INIT_NAMESPACE "Add an externally constructed INIT server namespace to the build" ON)

# enable external init namespace flag in source code
if(FORTE_COM_OPC_UA_ENABLE_INIT_NAMESPACE)
    add_definitions(-DOPC_UA_ENABLED_INIT_NAMESPACE)
endif()

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(FORTE_COM_OPC_UA)
	forte_add_sourcefile_hcpp( opcua_layer opcuahandler )
	if(EXISTS ${FORTE_COM_OPC_UA_DIR})
	    if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
    		forte_add_include_directories( ${FORTE_COM_OPC_UA_DIR})
    		forte_add_link_directories( ${FORTE_COM_OPC_UA_DIR})
	       	forte_add_link_library(libopen62541.so)
	    elseif("${FORTE_ARCHITECTURE}" STREQUAL "Win32")
    		forte_add_include_directories( ${FORTE_COM_OPC_UA_DIR})
    		forte_add_link_directories( ${FORTE_COM_OPC_UA_DIR})
	    endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
	else(EXISTS ${FORTE_COM_OPC_UA_DIR})
		message("No OPC_UA directory given")
    endif(EXISTS ${FORTE_COM_OPC_UA_DIR})
endif(FORTE_COM_OPC_UA)
