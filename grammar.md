# Mint Grammar Specification
Here is the grammer specification of the Mint programming language.


## Mint Syntax
The syntax of Mint is defined starting from an entire program or an entire line and going down to either classes, function or variable declaration.  

```
program ::= <declaration> <EOF>
```

### Declaration
A program is just a set of function/class/variable declarations. Each declaration can bind new statements:
```
declaration ::= <class_decl>
              | <fun_decl>
              | <var_decl>
              | <statement>

class_decl ::= "class" <identifier> { "+" <identifier> )?
               "{" <function> "}"

fun_decl ::= "fn" <function>
var_decl ::= "let" <identifier> ( "=" <expression> )? ";"
```

### Statements
```
statement ::= <expr_stmt>
            | <for_stmt>
            | <if_stmt>
            | <print_stmt>
            | <return_stmt>
            | <while_stmt>
            | <block>

expr_stmt   ::= <expression> ";"
for_stmt    ::= "for" "(" ( <var_decl> | <expr_stmt> | ";") <expression>? ";" <expression> ")" <statement>
if_stmt     ::= "if" "(" <expression> ")" <statement> ( "else" <statement> )?
print_stmt  ::= "print" <expression> ";"
return_stmt ::= "return" <expression>? ";"
while_stmt  ::= "while" "(" <expression> ")" <statement>
block       ::= "{" <declaration> "}";
```

### Expressions
```
expression  ::= <assignment>
assignment  ::= ( <call> "." )? <identifier> "=" <assignment> | <logic_or>
logic_or    ::= <logic_and> ( "or" <logic_and> )*
logic_and   ::= <equality> ( "and" <equality> )*
equality    ::= <comparison> ( ( "!=" | "==" ) <comparison> )*
comparison  ::= <term> ( ( ">" | ">=" | "<" | "<=") <term> )*
term        ::= <factor> ( ( "-" | "+" ) <factor> )*
factor      ::= <unary> ( ( "/" | "*" ) <unary> )*
unary       ::= ( "!" | "-" ) <unary> | <call>
call        ::= <primary> ( "(" <arguments>? ")" | "." <identifier> )*
primary     ::= "true" | "false" "nil" | "this"
               | <NUMBER> | <STRING> | <IDENTIFIER> | "(" <expression> ")"
               | "super" "." <IDENTIFIER>
function    ::= <IDENTIFIER> "(" <parameters>? ")" <block>
parameters  ::= <IDENTIFIER> ( "," <IDENTIFIER )*
arguments   ::= <expression> ( "," <expression> )*
```

## Mint Grammar
The grammar is used by the lexer analyzer to group up characters into tokens.
```
NUMBER      ::= DIGIT+ ( "." DIGIT+ )?
STRING      ::= "\"" <any char except "\"">* "\""
IDENTIFIER  ::= <ALPHA> ( <ALPHA> | <DIGIT> )*
ALPHA       ::= "a" ... "z" | "A" ... "Z" | "_"
DIGIT       ::= "0" ... "9"

