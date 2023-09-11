### Run from project root (i.e. /path/to/doba-llvm)


**To generate the .ll file (IR or also known as assembly)**
- `-S` flag only run preprocess and compilation steps. No bin gen.
- `-emit-llvm` flag use the LLVM representation for assembler and object files. It generates the `filename.ll` file from source code.

```
clang++ -S -emit-llvm tests/section-1-1/test.cpp -o tests/section-1-1/test.ll
```


**To generate the bin from .ll file (IR or also known as assembly)**

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
