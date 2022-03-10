//
// Created by marco on 3/10/22.
//

#ifndef MINT_MINTCALLABLE_H
#define MINT_MINTCALLABLE_H

#include <any>
#include <vector>
#include <string>

class Interpreter;
class MintCallable {
public:
    virtual unsigned short arity() = 0;
    virtual std::any call(Interpreter& interpreter, std::vector<std::any> arguments) = 0;
    virtual std::string to_string() = 0;
    virtual ~MintCallable() = default;
};


#endif //MINT_MINTCALLABLE_H
