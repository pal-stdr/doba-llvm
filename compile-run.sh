# Compile and build the doba compiler "main" function:
clang++ dobaLLVMmain.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o doba-llvm

# Run doba main function:
./doba-llvm

# Execute the generated IR
lli ./out.ll

# A terminal is just another program which can execute other programs, and process the exit code from those programs. Usually exit code 0 means successful execution. We just leverage the exit code (the return value from the main function) to play around with the value 42. However, terminal may allocate only certain amount of exit codes and truncate everything else.
echo $?