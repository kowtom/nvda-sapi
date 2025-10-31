# NVDA Controller Client Library

This directory should contain the NVDA Controller Client library files.

## How to obtain the library

You can get the `nvdaControllerClient.lib` and `nvdaControllerClient.dll` files from:

1. **Download from NVDA source**: https://github.com/nvaccess/nvda
   - The library files are typically in the `include` directory of NVDA distributions

2. **Or download from releases**: Download from NVDA releases and extract from installation

3. **Or build from source**: Clone the NVDA repository and build the controller client library

## Required files

Place these files in this directory:
- `nvdaControllerClient.lib` - Import library for linking
- `nvdaControllerClient.dll` - Runtime library (optional, usually installed with NVDA)

The header file `nvdaControllerClient.h` is already provided in this repository.
