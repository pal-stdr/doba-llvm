# Compile and build the doba compiler "main" function:
clang++ dobaLLVMmain.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o doba-llvm

# Run doba main function:
./doba-llvm

# Execute the generated IR
lli ./out.ll