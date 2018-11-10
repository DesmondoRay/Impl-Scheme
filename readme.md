## Implementation of Scheme in C++

- DesmondoRay
- 2018/11/5
- Windows: Visual Studio 2015

This program is basic scheme interpreter. It consists of four major parts: Object, Io_function, Primitive_procedures, Eval.

### Object 
- An Object saves the basic datas of Scheme, include integer, real, boolean, string(symbol), procedure and pair.  
- A Procedure can represent primitive-procedure and compound-procedure.  
Primitive-procedure is a function pointer;  
Compound-procedure consists of three parts: parameters, body and environment, only some special compound-procedure have their own environment, reference SICP page 150(Chinese version) or page 297(English version).k

### Io_function
Module io_function get input from string, std::cin and files, split the input into individual elements, and do some conversion, such as '(1 2 3) --> (list 1 2 3).

### Primitive-procedure
Implement part of primitive procedure of Scheme.

### Eval
The evaluators evaluates each input expression and prints out the result.

### Usage
- (quit) or (exit) to quit
- (load "path/filename") to load code from files
- (reset) to reset environment and restart evaluator

