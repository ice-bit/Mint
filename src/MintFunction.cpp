#include "MintFunction.h"
#include "Environment.h"
#include "Interpreter.h"

std::string MintFunction::to_string() {
    return std::string("<fn " + declaration->name.lexeme + ">");
}

unsigned short MintFunction::arity() {
    return declaration->params.size();
}

std::any MintFunction::call(Interpreter &interpreter, std::vector<std::any> arguments) {
    auto env = std::make_shared<Environment>(closure);
    for(auto i = 0UL; i < declaration->params.size(); i++)
        env->define(declaration->params[i].lexeme, arguments[i]);

    try {
        interpreter.execute_block(declaration->body, env);
    } catch(MintReturn& returnValue) {
        return returnValue.value;
    }

    return nullptr;
}


