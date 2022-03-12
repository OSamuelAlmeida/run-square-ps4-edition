# Run Square: PS4 Edition

A PS4 implementation of [Run Square](https://github.com/OSamuelAlmeida/RunSquare) using [OpenOrbis Toolchain](https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain).

# Building

## Dependencies

This project requires a complete setup of the [OpenOrbis Toolchain](https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain), with the ```$OO_PS4_TOOLCHAIN``` environment variable pointing to its directory.

## Compiling

To build the `.pkg` file to install the application on your PS4, you just need to execute the following:

```shell
make
```

A `.pkg` file will be compiled and can be installed on a PS4.
