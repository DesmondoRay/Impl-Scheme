/* Header file of evaluator's functions */

#ifndef EVAL_H_
#define EVAL_H_

#include <iostream>
#include <unordered_map>
#include <algorithm>
using namespace std;
#include "object.h"
#include "io_function.h"
#include "primitive_procedures.h"

using Environment = unordered_map<string, Object>;

/* Evaluator start. */
void run_evaluator(Environment& env);

/* Evaluating a expression. */
Object eval(vector<string>& split, Environment& env);

/* Delete parentheses of two ends */
void delete_ends_parentheses(vector<string>& split);

/* Evaluating a single string. */
Object eval(string& str, Environment& env);

/* Call proc with obs. */
Object apply_proc(Object &proc, vector<Object>& obs, Environment& env);

#endif