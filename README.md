# Run Square: PS4 Edition

A PS4 implementation of [Run Square](https://github.com/OSamuelAlmeida/RunSquare) using [OpenOrbis Toolchain](https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain). The source code is based on the [PS4 SDL2 Sample](https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain/tree/master/samples/SDL2).

# Building

## Dependencies

This project requires a complete setup of the [OpenOrbis Toolchain](https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain), with the ```$OO_PS4_TOOLCHAIN``` environment variable pointing to its directory.

You will also need to populate the folders ```sce_modules/``` and ```sce_sys/``` with the following files which can be obtained from the Toolchain binaries:

- ```sce_modules/libc.prx```
- ```sce_modules/libSceFios2.prx```
- ```sce_sys/about/right.sprx```

## Compiling

To build the `.pkg` file to install the application on your PS4, you just need to execute the following command:

```shell
make
```

A `.pkg` file will be compiled and can be installed on a PS4.

# The Game

Use the left joystick to control the player, collect as many coins as possible while avoiding the evil orange squares.
