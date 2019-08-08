
add_executable(module-tests
        oatpp-websocket/tests.cpp
        oatpp-websocket/app/AsyncController.hpp
        oatpp-websocket/app/AsyncWebSocketListener.hpp
        oatpp-websocket/app/Controller.hpp
        oatpp-websocket/app/WebSocketListener.hpp
        oatpp-websocket/FullAsyncTest.cpp
        oatpp-websocket/FullAsyncTest.hpp
        oatpp-websocket/FullTest.cpp
        oatpp-websocket/FullTest.hpp
)

if (MSVC)
    # The MSVC compiler doesnt comply to c99 thus the OATPP_MACRO_HAS_ARGS macro always expands to 1
    # This screws up the DTO_FIELD and ENDPOINT macros. To force MSVC to c99 compliant mode we need to activate this
    # experimental switch. However, we can't enable it globally since the Windows-SDK isn't prepared for this and
    # throws a whole bunch of errors.
    add_compile_options(/experimental:preprocessor)
endif(MSVC)

set_target_properties(module-tests PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

target_include_directories(module-tests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

if(OATPP_MODULES_LOCATION STREQUAL OATPP_MODULES_LOCATION_EXTERNAL)
    add_dependencies(module-tests ${LIB_OATPP_EXTERNAL})
endif()

add_dependencies(module-tests ${OATPP_THIS_MODULE_NAME})

target_link_oatpp(module-tests)

target_link_libraries(module-tests
        PRIVATE ${OATPP_THIS_MODULE_NAME}
)

## TODO link dependencies here (if some)

add_test(module-tests module-tests)