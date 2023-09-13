### DOBA language expression format
```
(+ 5 10)            // addition

(set x 15)          // assignment

(if (> x 10)        // if
    (print "ok")
    (print "err")
)


( def foo (bar)                         // Non typed
    (+ bar 10)
)

( def foo ((bar number)) -> number     // Typed
    (+ bar 10)
)

// In LLVM, Types are used to know which operation to apply and how much memory to allocate
```


### How to compile + execute program 
**Compile The binary (i.e doba-llvm) that will execute our "doba-llvm" language**
```
clang++ doba-llvm.cpp -o doba-llvm
```

**Execute the custom program**
```
./doba-llvm -e '(var x 10) x'
```
**or,**
```
./doba-llvm -f /path/to/test.doba
```

### LLVM core libs to build the program
```
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
```

### Most essential command
**`llc` or llvm compiler**
```
llc --version

# Returns:
LLVM version 16.0.6
  Optimized build with assertions.
  Default target: x86_64-unknown-linux-gnu
  Host CPU: skylake

  Registered Targets:
    nvptx   - NVIDIA PTX 32-bit
    nvptx64 - NVIDIA PTX 64-bit
    x86     - 32-bit X86: Pentium-Pro and above
    x86-64  - 64-bit X86: EM64T and AMD64
```

**`clang++` to emit llvm ir**

- `-S` flag only run preprocess and compilation steps. No bin gen.
- `-emit-llvm` flag use the LLVM representation for assembler and object files. It generates the `filename.ll` file from source code.