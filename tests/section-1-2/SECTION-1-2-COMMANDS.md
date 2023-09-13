## Necessary files `src/DobaLLVM.h` & `dobaLLVMmain.cpp`

### `src/DobaLLVM.h` contains the compilation procedure

### `dobaLLVMmain.cpp` is the main function which is the actual language binary `doba-llvm`

- A programming language (for our case, `doba` language) will be passed as `S` expression like following
```
std::string program = R"(
    42
)";
```

- Then the language VM will be instatiated like using `DobaLLVM` class from `src/DobaLLVM.h`
```
DobaLLVM vm;
```

- Execute the VM to have new program binary
```
vm.exec(program);
```

### How to compile the `dobaLLVMmain.cpp` main file to get the binary `doba-llvm`
```
clang++ dobaLLVMmain.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o doba-llvm
```

### How to run the binary `doba-llvm` and what we will get..
```
./doba-llvm
```

**It will generate `out.ll` file which contains module name**
- Right now it doesn't contain anything. But is a fully working module.
```
; ModuleID = 'DobaLLVM'
source_filename = "DobaLLVM"
```