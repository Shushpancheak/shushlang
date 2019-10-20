# shushlang
A compiler for shushasm and shush languages. Compiles to shush bytecode.

## Build
```shell
mkdir build && cd build
cmake .. # "-UBUILD_TESTS -DBUILD_TESTS=ON" to build tests
make
```

## Build documentation
```shell
doxygen
```
Documentation will be generated in the directory named `docs`.