# mul2leftshift

A demo LLVM pass transforms mul to left shift. 

# How to use

1. Modify `CMakeLists.txt`:

```cmake
#set your LLVM installation directory
set(LLVM_DIR YOUR_LLVM_INSTALLATION_DIR)
```

2. Building:

```shell
mkdir build&&cd build
cmake ..
make
```

3. Run:

```sh
opt -load-pass-plugin=libMul2LeftShiftPass.so -passes="mul2leftshift" ../examples/before.ll
```

