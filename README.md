# Mint
Mint is an interpreted programming language written in modern C++(>=17). The Mint interpreter uses a _recursive descent_ parser(i.e., it starts with grammar expressions that have an higher precedence level over the others).  Mint shares a lot of functionalities with other high level programming languages, such as:
- Dynamic typing;  
- Basic control structures(such as _if-then-else_, _while_ and _for_);
- Function declaration;
- Support for the Object Oriented paradigm(class/object and inheritance);  
- Function closures.

The Mint interpreter is built following [Robert Nystrom's book: _"Crafting Interpreters"_](https://craftinginterpreters.com/), therefore the Mint programming language is heavily inspired by Lox.

## Benchmarks
Mint is quite fast for a three-walking interpreter, below you can find a table with some benchmarks:
TODO: add table

## Building
Mint is written in modern C++ from scratch without using any additional library. In order to build it, you will need a modern C++ compiler(GCC, clang, MSVC, etc.) installed in your machine and a recent version of cmake(>= 3.21). Once installed, type the following commands(note: these instructions might not work under non-POSIX environments):
```sh
$> mkdir build && cd build
$> cmake ..
$> make
```

## Usage
TODO: add cli helper
You can either use Mint by the interactive REPL or by providing a source file. Do note that the REPL does not supports statements splitted into multiple lines. You can find a complete list of supported programs into the `examples/` directory. Here some of them:
TODO:
