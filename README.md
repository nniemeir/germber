# GERMBER

An annotated fork of Low Level Devel's [Gameboy emulator](https://github.com/rockytriton/LLD_gbemu) made for educational purposes.

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
-d			            Enable debug mode
-h                      Display program usage
-p                      Specify color palette
-r                      Specify ROM path
```
