## target datalayout

`target datalayout` is a crucial component of a target description that specifies how data is represented in memory for a specific target architecture. It provides detailed information about the size, alignment, and other characteristics of fundamental data types like integers and floating-point numbers, as well as aggregate types like structs and arrays.

The `target datalayout` is used by the LLVM compiler to generate code that is tailored to the memory layout of the target machine. It influences various aspects of code generation and optimization, such as:

- `Data Alignment`: It specifies how data types should be aligned in memory. Different hardware architectures may have different alignment requirements, and proper alignment is essential for efficient memory access.
- `Size of Data Types`: It defines the size of fundamental data types like integers, floats, and pointers. This information is essential for generating correct and efficient code.
- `Endianness`: It indicates the byte order of the target machine, whether it's little-endian (most significant byte last) or big-endian (most significant byte first).
- `Pointer Size`: It specifies the size of pointers on the target machine, which can vary depending on the architecture.
- `Aggregate Data Types`: It defines the alignment and size of complex data structures like structs and arrays.

### Example `datalyout`
```
e-m:e-i64:64-f80:128-n8:16:32:64-S128
```
- `e-m:e` indicates little-endian byte order.
- `i64:64` specifies that i64 (64-bit integer) has a size of 64 bits.
- `f80:128` indicates that f80 (80-bit floating-point) has a size of 128 bits.
- `n8:16:32:64` defines the alignment requirements for i8, i16, i32, and i64.
- `S128` specifies that the natural alignment (the most strict alignment) is 128 bits.

The specific format of the "target data layout" string may vary depending on the target architecture and LLVM version, but it provides a standardized way to communicate important data layout information to the LLVM compiler, ensuring that generated code is both correct and optimized for the target platform.


### More Example `datalyout` from `test.ll` IR file
```
# For Intel skylake CPU, linux x86_64 config 
e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128
```

- `e-m:e`: This part indicates that the target architecture uses little-endian byte order (e) and is a generic (`m:e`) machine. Little-endian means that the least significant byte is stored at the lowest memory address.

- `p270:32:32-p271:32:32-p272:64:64`: These components define the pointer sizes and alignments for various address spaces. They are in the format of `pAS:Size:Align`, where `AS` is the address space, `Size` is the size in bits, and `Align` is the alignment in bits. For example, `p270:32:32` means that pointers in address space 270 are 32 bits in size and have a 32-bit alignment.

- `i64:64`: This part specifies that the `i64` data type (64-bit integers) is 64 bits in size. This is crucial information for generating code that correctly handles integer operations.

- `f80:128`: This part defines that the `f80` data type (80-bit floating-point numbers) is 128 bits in size. Again, this information is necessary for generating code for floating-point operations.

- `n8:16:32:64`: These components define the default alignment requirements for various data types. For example, `n8:16` means that an `i8` (8-bit integer) should be naturally aligned to 16 bits. This helps ensure efficient memory access.

- `S128`: This indicates that the **stack alignment** is 128 bits. Stack alignment ensures that when functions are called, the stack pointer is appropriately aligned to avoid performance penalties on architectures that require strict alignment.


## IR Optimization (by Removing Unnecessary code)

- How to test everytime after redacting IR code, that everything is okay?
**Ans**: By using the llvm interpreter `lli` command. Everytime we reduce something, we do the following commands. If it is returing `42`, means the process is okay.
```
lli tests/section-1-1/test.ll; echo $?
```

### From line 14 (at `test.ll`), the metadata starts

- We can remove them. After removal, it looks like this,

```
; ModuleID = 'tests/section-1-1/test.cpp'
source_filename = "tests/section-1-1/test.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  ret i32 42
}
```

### Remove unnecessary allocation from function `entry block` and other top info

- We can remove the unnecessary `alloca`, `store`, etc. from function `entry` block. Because this function is just returning an integer value of `42`
- `define`: This keyword is used to define a function in LLVM IR.
- `dso_local`: This attribute indicates that the function is local to the current dynamic shared object (DSO) or module. In other words, it's not meant to be visible or callable from other modules.
- `noundef`: This attribute indicates that the function does not produce undefined behavior when called. In LLVM, **undef** is a concept used to represent values that are undefined or unspecified. This attribute ensures that the `main` function is well-defined and doesn't invoke undefined behavior.
- `#0`: This is a metadata reference or **debug location**. It's used for debugging information and is typically associated with source code location information. The `#0` here refers to the absence of specific debug location information, indicating that no source code location information is attached to this function definition.

So, the line `define dso_local noundef i32 @main() #0` is defining a function named main that returns a 32-bit integer (`i32`). It's marked as `dso_local`, meaning it's not meant to be externally visible, and it's also marked as `noundef`, indicating that it doesn't produce undefined behavior when called. There is no specific source code location information attached to this function in this representation.

```
define dso_local noundef i32 @main() #0 {
entry:
  ret i32 42
}
```

### Make the function to it's most minimal form

```
define i32 @main() {
  ret i32 42
}
```

### We can also use `clang++` to compile and then execute the bin

- Command for that
```
clang++ tests/section-1-1/test.ll -o tests/section-1-1/test-minimal-ll-clang-bin; ./tests/section-1-1/test-minimal-ll-clang-bin; echo $?

# Returns
warning: overriding the module target triple with x86_64-unknown-linux-gnu [-Woverride-module]
1 warning generated.
42
```

- Means, we have to add metadata to IR for `clang++`. So let's add the metadata to `test.ll` IR
```
; ModuleID = 'tests/section-1-1/test.cpp'
source_filename = "tests/section-1-1/test.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
  ret i32 42
}
```

Run the command again
```
clang++ tests/section-1-1/test.ll -o tests/section-1-1/test-minimal-ll-clang-bin; ./tests/section-1-1/test-minimal-ll-clang-bin; echo $?
# Returns
42
```
No error 😁