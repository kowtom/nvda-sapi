cmake_minimum_required(VERSION 3.15)
project(nvda-sapi VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the TTS engine DLL
add_library(nvda-sapi SHARED
    src/NvdaSapiEngine.cpp
    src/NvdaSapiEngine.def
    src/dllmain.cpp
)

target_include_directories(nvda-sapi PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/nvdaControllerClient
)

# Link against required Windows libraries
target_link_libraries(nvda-sapi PRIVATE
    ole32
    oleaut32
    uuid
    ${CMAKE_CURRENT_SOURCE_DIR}/nvdaControllerClient/nvdaControllerClient.lib
)

# Set output name
set_target_properties(nvda-sapi PROPERTIES
    OUTPUT_NAME "nvda-sapi"
    PREFIX ""
)

# Installation
install(TARGETS nvda-sapi
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)

install(FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/register.bat
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/unregister.bat
    DESTINATION bin
)
