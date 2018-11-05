/* Header file of evaluator's functions */

#ifndef EVAL_H_
#define EVAL_H_

#include <iostream>
#include <unordered_map>
using namespace std;
#include "object.h"
#include "io_function.h"

using Environment = unordered_map<string, Object>;

/* Evaluator start. */
void eval_run(Environment& env);

/* Transform vector of strings to vector of Objects. */
/* Example: {"(", "+", "3", "4", ")"} -> {Object("+"), Object(3), Object(4)} */
vector<Object> transform(vector<string>& split);

/* Evaluating a single object. */
Object eval(Object& ob, Environment& env);

/* Evaluating a expression. */
Object eval(vector<Object>& obs, Environment& env);

/* Call proc with obs. */
Object apply_proc(Procedure &proc, vector<Object>& obs, Environment& env);


#endif