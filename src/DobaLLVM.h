#ifndef DobaLLVM_h
#define DobaLLVM_h



#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"


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
    void exec(const std::string &program) {
        // Things to do

        // // 1. Parse the program (generate the AST using Parser)
        // auto ast = parser->parser(program);

        // // 2. Compile the AST down to LLVM IR
        // compile(ast)

        // // 3. Print generated IR in the terminal
        module->print(llvm::outs(), nullptr);

        // // 4. Save module IR to file
        saveModuleToFile("./out.ll");
    }

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
     * @brief Saves IR to .ll file
     */
    void saveModuleToFile(const std::string &fileName) {
        std::error_code errorCode;
        llvm::raw_fd_ostream outLL(fileName, errorCode);

        if (!errorCode) {
            module->print(outLL, nullptr);
        } else {
            llvm::errs() << "Error opening file for writing.\n";
        }

        
    }

};




#endif