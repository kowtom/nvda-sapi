# Common library for SAPI to NVDA bridge
add_library(nvda_sapi_bridge SHARED
    nvda_sapi_bridge.cpp
    nvda_sapi_bridge.h
    nvda_client.cpp
    nvda_client.h
    sapi_voice.cpp
    sapi_voice.h
    sapi_minimal.h
    sapi_guids.cpp
    nvda_sapi_bridge.def
)

target_compile_definitions(nvda_sapi_bridge PRIVATE
    UNICODE
    _UNICODE
    WIN32_LEAN_AND_MEAN
)

# Link required Windows libraries
target_link_libraries(nvda_sapi_bridge PRIVATE
    ole32
    oleaut32
    user32
)

# Set output name based on architecture
set_target_properties(nvda_sapi_bridge PROPERTIES
    OUTPUT_NAME "nvda_sapi${ARCH_SUFFIX}"
    PREFIX ""
)

# Install the DLL
install(TARGETS nvda_sapi_bridge
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
