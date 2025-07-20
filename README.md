# MCP4725 driver for PYNQ Z-2
## Requirements:
- [libpynq](https://pynq.tue.nl/general/libpynq/)
## Usage
Place files `mcp4725.h` and `mcp4725.c` in `library` directory
Recompile the library and then just include `mcp4725.h`
Copy both functions form `iic_expanded.c` to `library/iic.c` and add their signatures to `library/iic.h`

This library is implemented following this [datasheet] (https://www.tinytronics.nl/product_files/000416_MCP4725%20datasheet.pdf)
Only things missing are general calls and I2C HS-mode as both of them are not suppoerted by libpynq

## TODO
- Bitbang general calls
- Add streaming option, i.e. not send START and STOP after each packet

