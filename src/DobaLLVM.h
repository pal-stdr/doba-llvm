#ifndef DobaLLVM_h
#define DobaLLVM_h



#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"


/**
 * @brief Doba to LLVM IR Compiler
 */

class DobaLLVM
{
public:
    DobaLLVM() {
        moduleInit();
    }

    /**
     * @brief Executes a program
     * This is main API Method
     */
    void exec(const std::string &program);

    ~DobaLLVM(){ };

private:

    /**
     * @brief Initialize the module. 
     * Will be called from the contructor of this class (i.e. DobaLLVM())
     */
    void moduleInit() {

        // Open a new context
        ctx = std::make_unique<llvm::LLVMContext>();

        // Open a new module
        module = std::make_unique<llvm::Module>("DobaLLVM", *ctx);

        // Create a new builder for the module.
        irBuilder = std::make_unique<llvm::IRBuilder<>>(*ctx);

    }




    /**
     * @brief Global LLVM Context
     * It owns and manages the core "global" data of LLVM's core infrastructure,
     * including the type and constant unique value
     */
    std::unique_ptr<llvm::LLVMContext> ctx;



    /**
     * @brief Module 
     * A module instance is used to store all the information related to an LLVM module.
     * Modules are the top level container of all other LLVM IR objects. Each module directly contains
     * a list of global variables, list of functions, a list of libs (or other modules) on which this modules depends on, 
     * a symbol table, and various data about the target's characteristics.
     * 
     * A module maintains a GlobalList object that is used to hold all constant references to global variables in the module.
     * When a global variable is destroyed, it should have no entries in the GlobalList.
     * The main container class is for the LLVM IR.
     */
    std::unique_ptr<llvm::Module> module;



    /**
     * @brief IRBuilder
     * This provides uniform API for creating instructions and inserting them into BasicBlock:
     * either at the end of the BasicBlock, or at the specific iterator location in a block.
     */
    std::unique_ptr<llvm::IRBuilder<>> irBuilder;


    /**
     * @brief LLVM function pointer
     * Currently compiling function
     */
    llvm::Function *fn;


    /**
     * @brief Compiles an expression
     */
    void compile(/* TODO: ast */);


    /**
     * @brief Creates a function
     * Callsite is compile()
     * Returns ptr to a object of type llvm::Function. Simply speaking, returns a function
     */
    llvm::Function *createFunction(
        const std::string &fnName,
        llvm::FunctionType *fnType
    );


    /**
     * @brief Main compile loop
     * Callsite compile()
     * gen() will accept the AST expression in future.
     * gen() should return a ptr to the value of llvm::Value type.
     * This value/llvm::Value could be anything (e.g. Scalars (number, string), Reference type (instructions, objects))
     */
    // llvm::Value *gen(/* exp */);



    /**
     * @brief Saves IR to .ll file
     */
    void saveModuleToFile(const std::string &fileName);

};



/**
 * @brief Executes a program (Private)
 * This is main API Method
 */
void DobaLLVM::exec(const std::string &program) {
    // Things to do

    // // 1. Parse the program (generate the AST using Parser)
    // auto ast = parser->parser(program);

    // // 2. Compile the AST down to LLVM IR
    // compile(ast);
    compile();

    // // 3. Print generated IR in the terminal
    module->print(llvm::outs(), nullptr);

    // // 4. Save module IR to file
    saveModuleToFile("./out.ll");
}


/**
 * @brief Saves IR to .ll file
 */
void DobaLLVM::saveModuleToFile(const std::string &fileName) {
    std::error_code errorCode;
    llvm::raw_fd_ostream outLL(fileName, errorCode);

    if (!errorCode) {
        module->print(outLL, nullptr);
    } else {
        llvm::errs() << "Error opening file for writing.\n";
    }
}


/**
 * @brief Creates a function
 * Callsite is compile
 * Returns ptr to a object of type llvm::Function. Simply speaking, returns a function
 */
llvm::Function *DobaLLVM::createFunction(
    const std::string &fnName,
    llvm::FunctionType *fnType
) {
    // Check, if the function prototype might already be exist
    fn = module->getFunction(fnName);

    // If not, allocate the function
    // module is a smart unique_ptr. But the last argument accept a reference. That's why we needed to dereference it first
    // llvm::Function::ExternalLinkage means, function is visible outside
    if (fn == nullptr) {
        fn = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, fnName, *module);
    }

    // Now we have to verify the function
    llvm::verifyFunction(*fn, nullptr);



    // ========= Create Basic Blocks starts ==========
    /**
     * @brief Creates a basic block
     * If the "fn" is passed, the block is automatically appended
     * to the parent function. Otherwise,
     * the block should later be appended manually via,
     * fn->getBasicBlockList().push_back(block); (e.g. for "if/else" statements)
     */

    // Now we have to create function basic block (here, "entry block")
    // It is more like attaching the basic block to the function
    llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(*ctx, "entry", fn);
    
    // Now pass it to the irBuilder
    irBuilder->SetInsertPoint(entryBlock);

    // ========= Create Basic Blocks ends ==========

    return fn;
}




/**
 * @brief Compiles an expression
 */
void DobaLLVM::compile(/* TODO: ast */) {
    
    // Before passing the AST, our first target is to static compile the following IR, without taking any program/AST inputs
    // define i32 @main() {
    //     ret i32 42
    // }


    // // 1. Create main function (Every program needs an entry point, which is a main function). For the above IR case, we need to create "@main"
    fn = createFunction(
        "main",
        llvm::FunctionType::get(
            irBuilder->getInt32Ty(),    // return type
            false                       // vararg (means variable argument)
        )    
    );


    // // 2. Compile the body of the main function. This behaves like an interpreter (i.e. going through AST nodes recursively), but this things happens in compilation time. Also known as codegen. Finally this result will be returned in the following step.
    // auto result = gen(/* ast */);

    // For now, we will return just one "42" value. That's why we are writing this outside he scope of gen()
    llvm::Value *result = irBuilder->getInt32(42);


    // 3. After having the result, we have to return (i.e. "ret") the result. For that, first we need to cast the result to i32
    auto i32Result = irBuilder->CreateIntCast(result, irBuilder->getInt32Ty(), true);


    // 4. Now we have to return it
    irBuilder->CreateRet(i32Result);

}

#endif