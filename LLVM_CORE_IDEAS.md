## Section 1.1
### LLVM Evergreen concepts
- Generic concepts of compilation
- Design patterns of VMs and Compilers
- LLVM Core ecosystem

### Continiously upgrading concepts
- Changing APIs of the IR builder
- Changing format of the LLVM IR
- Evolving LLVM JIT compiler


### Diff betwn a Raw ptrs from llvm-14, vs Opaque ptrs from llvm-15+ (2:39min)
Upto llvm-14, in IR, the Raw ptrs is used. That means in SSA, variables can be addressed as raw `*` ptr. Also for casting from one type to another type, explicit bitcast keyword is used.
From llvm-15, opaque ptrs are used. We donot have to explicitly use `*` to denote ptrs. Rather we can use `ptr` keyword. Also casting between different types are done automatically. (i.e. no explicit `bitcast` keyword is not necessary to use.)

**Raw ptrs (upto llvm-14)**
```
load i64* %p

define i32 @test(i32* %p) {
    store i32 0, i32* %p
    %bc = bitcast i32* %p to i64*
    %v = load i64, i64* %bc
    ret i64 %v
}
```

**Opaque ptrs (from llvm-15+)**
```
load i64* %p

define i32 @test(ptr %p) {
    store i32 0, ptr %p
    // %bc = bitcast i32* %p to i64* (No need of this expression)
    %v = load i64, ptr %p
    ret i64 %v
}
```