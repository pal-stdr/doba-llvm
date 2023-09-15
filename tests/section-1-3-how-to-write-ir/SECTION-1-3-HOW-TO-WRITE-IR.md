# How to write IR using IR Builder

## Key Ideas
### Target and Procedure to build it
```
define i32 @main() {
entry:
  ret i32 42
}
```

1-> program is passing just one value '42' as expression
 basic compilation function compile() define

2-> Inside compile()
2.1.---> createFunction() => Every program needs atleast a `main` function.
2.1.1.---------> it is using `llvm::Function::Create()` to create `main` function. What it does:
2.1.1.1.------------> function needed to be defined by a name, a type, and vararg acceptance.
2.1.1.2.------------> And also function need to be binded with the `module`.
2.1.1.3.------------> and have to declare it's visibility.
2.1.2.------> Check if the function object (i.e. `fn`) with similar name (e.g. `main`) already exists.
2.1.3.------> Then verify the function using `llvm::verifyFunction()`
2.1.4.---> Then inside the function, we have to create a basic block named `entry` block using `llvm::BasicBlock::Create()`. What it does:
2.1.4.1.---------> Set the basic block name `entry` for this usecase
2.1.4.2.---------> It must be binded with the `ctx` (i.e. `std::unique_ptr<llvm::LLVMContext> ctx`)  and `fn` (i.e. `llvm::Function *fn`)
2.1.5.---> Then push the entry block inside the function through `irBuilder->SetInsertPoint()`
2.1.6.---> Now return the function object.

3-> Set a static arbitrary value of "42" (for place holder purpose), and push it in the function through `irBuilder->getInt32(42)`

4-> Cast this value to a type (for "42", signed i32 type), because every function needs a return type. And then push it to `irBuilder->SetInsertPoint()`.


### Class architecture
```
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
```