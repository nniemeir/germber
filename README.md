# GERMBER

A fork of Low Level Devel's [Gameboy emulator](https://github.com/rockytriton/LLD_gbemu) made for educational purposes.

## Changes 
* Detailed annotations (WIP)
* Codebase refactoring
* Improved error handling
* Improved memory management
* More robust argument parsing via getopt
* Debug messages can now be toggled via -d flag
* Alternative color palettes
* Alternative control scheme

## Dependencies
* GCC
* GNU make
* SDL2 Development Libraries

## Installation
Compile the project
```
make
```
Install the compiled binary
```
sudo make install
```

### Make Targets 
- `make` - Compile the binary
- `make install` – Copy binary and manpage to system directories
- `make clean` – Remove build objects
- `make fclean` - Remove build objects and binary

## Usage
```
germber [OPTIONS]
```

### Options
```
-d  Enable debug mode
-h  Display program usage
-p  Specify color palette
-r  Specify ROM path
```
