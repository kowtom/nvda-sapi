# Makefile for NVDA SAPI Bridge
# Builds both 32-bit and 64-bit versions using MinGW-w64

# Compiler settings
CXX32 = i686-w64-mingw32-g++
CXX64 = x86_64-w64-mingw32-g++

# Common flags
CXXFLAGS = -O2 -Wall -DUNICODE -D_UNICODE
LDFLAGS = -shared -static-libgcc -static-libstdc++
LIBS = -lole32 -loleaut32 -luuid

# Source files
SRC = src/sapi_nvda_bridge.cpp
DEF = src/nvda_sapi_bridge.def

# Output directories
BUILD_DIR = build
OUT32 = $(BUILD_DIR)/x86
OUT64 = $(BUILD_DIR)/x64

# Output files
DLL32 = $(OUT32)/nvda_sapi_bridge.dll
DLL64 = $(OUT64)/nvda_sapi_bridge.dll

.PHONY: all clean dirs x86 x64

all: dirs x86 x64

dirs:
	mkdir -p $(OUT32) $(OUT64)

x86: dirs
	$(CXX32) $(CXXFLAGS) $(SRC) $(DEF) $(LDFLAGS) $(LIBS) -o $(DLL32)

x64: dirs
	$(CXX64) $(CXXFLAGS) $(SRC) $(DEF) $(LDFLAGS) $(LIBS) -o $(DLL64)

clean:
	rm -rf $(BUILD_DIR)

install: all
	@echo "Run install.bat or install.ps1 to register the SAPI voice"

.DEFAULT_GOAL := all
