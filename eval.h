/* Header file of evaluator's functions */

#ifndef EVAL_H_
#define EVAL_H_

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <cassert>
using namespace std;

#include "object.h"
#include "io_function.h"
#include "primitive_procedures.h"

/* Keywords of Scheme */
static vector<string> keywords{
	"define", "if", "set!", "lambda", "begin", "let", "cond"
};

using SubEnv = unordered_map<string, Object>;
using Environment = vector<SubEnv>;

/* Reset the global environment. */
void initialize_environment();

/* Reset evaluator, reset environment */
void reset_evaluator();

/* Evaluator start. */
/* mode == 0: print prompt and value, such as ">>> Eval value: a";
 * mode == 1: don't print prompt and value, used to evaluate code from file.
 * mode == 2: don't print prompt, but print value.
 */
void run_evaluator(istream &in, int mode = 0);

/* Evaluating a expression. */
Object eval(vector<string>& split);

/* Evaluating a single string. */
Object eval(string& str);

/* Call proc with obs. */
Object apply_proc(Object &op, vector<Object>& obs);

/* Handle with keyword */
Object eval_keyword(const string& keyword, vector<string>& exp);

/* Handle with "define" expression */
Object eval_define(vector<string>& args);

/* Handle with "define" expression */
Object eval_lambda(vector<string>& exp, const string& proc_name = "*anonymous*");

/* Return true if object is some kinds of "true" */
bool is_true(Object& ob);

/* Handler with "if" expression */
Object eval_if(vector<string>& exp);

/* Handler with "begin" expression */
Object eval_begin(vector<string>& exp);

/* Handler with "begin" expression */
Object eval_let(vector<string>& exp);

/* Handler with "cond" expression */
Object eval_cond(vector<string>& exp);

/* Handler with "set" expression */
Object eval_set(vector<string>& exp);

#endif