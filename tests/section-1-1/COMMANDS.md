## How to Play with IR


### Playing with LLVM Interpreter `lli` & `clang++`

**Run from project root (i.e. /path/to/doba-llvm)**


**To generate the `.ll` file (IR or also known as assembly)**
- `-S` flag only run preprocess and compilation steps. No bin gen.
- `-emit-llvm` flag use the LLVM representation for assembler and object files. It generates the `filename.ll` file from source code.

```
clang++ -S -emit-llvm tests/section-1-1/test.cpp -o tests/section-1-1/test.ll
```


**To generate the bin from `.ll` file (IR or also known as assembly)**

```
clang++ tests/section-1-1/test.ll -o tests/section-1-1/test-bin

# Execute the bin
./tests/section-1-1/test-bin

# $? is a special shell variable that holds the exit status of the last executed command. The exit status is a numeric value that typically indicates whether the preceding command was successful (exit status 0) or encountered an error (non-zero exit status).

echo $?
# Returns 42, which is the return value from the main() function
```

**Using `lli` or llvm interpreter to execute the IR**
- `lli` cannot accept `.cpp` type source code. It can only execute `IR` or `.ll` file.
```
lli tests/section-1-1/test.ll
```



### Playing with LLVM Assembler `llvm-as`

**OVERVIEW: llvm `.ll -> .bc` assembler**

**LLVM IR is the readable representation of LLVM Bit Code (usuassly found as `filename.bc`)**

```
llvm-as tests/section-1-1/test.ll -o tests/section-1-1/test.bc
```


### Playing with LLVM Disassembler `llvm-dis`

It is completely opposite with `llvm-as`

**OVERVIEW: llvm .bc -> .ll disassembler**


```
llvm-dis tests/section-1-1/test.bc -o tests/section-1-1/test-ll-from-bc.ll
```


### How to generate machine specific assembly code with `clang++` from `.ll`

**By using the `-S` option with `clang++` command. And the assembly file extension is `filename_with_native_assembly_code.s`**
```
clang++ -S tests/section-1-1/test.ll -o tests/section-1-1/test.s

# or,

clang++ -S tests/section-1-1/test.ll -o tests/section-1-1/test.asm
```



### How to generate binary with `clang++` from machine specific assembly code (i.e. `.s` or `.asm`)

**From the assembly code (i.e. `.s` or `.asm`), we can generate file final binary using `clang++`**

```
clang++ tests/section-1-1/test.s -o tests/section-1-1/test-from-native-assembly-bin

# or,

clang++ tests/section-1-1/test.asm -o tests/section-1-1/test-from-native-assembly-bin

# And command including running the binary

clang++ tests/section-1-1/test.s -o tests/section-1-1/test-from-native-assembly-bin; ./tests/section-1-1/test-from-native-assembly-bin; echo $?
```