
add_library(${OATPP_THIS_MODULE_NAME}
        oatpp-websocket/AsyncConnectionHandler.cpp
        oatpp-websocket/AsyncConnectionHandler.hpp
        oatpp-websocket/AsyncWebSocket.cpp
        oatpp-websocket/AsyncWebSocket.hpp
        oatpp-websocket/Config.hpp
        oatpp-websocket/ConnectionHandler.cpp
        oatpp-websocket/ConnectionHandler.hpp
        oatpp-websocket/Connector.cpp
        oatpp-websocket/Connector.hpp
        oatpp-websocket/Frame.cpp
        oatpp-websocket/Frame.hpp
        oatpp-websocket/Handshaker.cpp
        oatpp-websocket/Handshaker.hpp
        oatpp-websocket/SHA1.cpp
        oatpp-websocket/SHA1.hpp
        oatpp-websocket/Utils.cpp
        oatpp-websocket/Utils.hpp
        oatpp-websocket/WebSocket.cpp
        oatpp-websocket/WebSocket.hpp
)

if (MSVC)
    # The MSVC compiler doesnt comply to c99 thus the OATPP_MACRO_HAS_ARGS macro always expands to 1
    # This screws up the DTO_FIELD and ENDPOINT macros. To force MSVC to c99 compliant mode we need to activate this
    # experimental switch. However, we can't enable it globally since the Windows-SDK isn't prepared for this and
    # throws a whole bunch of errors.
    add_compile_options(/experimental:preprocessor)
endif(MSVC)

set_target_properties(${OATPP_THIS_MODULE_NAME} PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

if(OATPP_MODULES_LOCATION STREQUAL OATPP_MODULES_LOCATION_EXTERNAL)
    add_dependencies(${OATPP_THIS_MODULE_NAME} ${LIB_OATPP_EXTERNAL})
endif()

target_link_oatpp(${OATPP_THIS_MODULE_NAME})

target_include_directories(${OATPP_THIS_MODULE_NAME}
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

## TODO link dependencies here (if some)

#######################################################################################################
## install targets

if(OATPP_INSTALL)
    include("../cmake/module-install.cmake")
endif()
