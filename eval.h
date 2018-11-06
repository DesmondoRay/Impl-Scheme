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

static vector<string> keywords{
	"define", "if", "set", "lambda", "begin", "let", "cond", "else" };

using Environment = vector<unordered_map<string, Object>>;

class Evaluator {
public:
	/* Constructor */
	Evaluator();

	/* Evaluator start. */
	void run_evaluator();

	/* Reset evaluator, reset environment */
	void reset_evaluator();
private:
	/* Evaluating a expression. */
	Object eval(vector<string>& split);

	/* Delete parentheses of two ends */
	void delete_ends_parentheses(vector<string>& split);

	/* Evaluating a single string. */
	Object eval(string& str);

	/* Call proc with obs. */
	Object apply_proc(Object &proc, vector<Object>& obs);

	/* Reset the global environment. */
	void initialize_environment();

	/* env[0]: global environment, env[n]: local environment, for example:
	 * 1. "(define a 3)" --> env[0]["a"] = 3, "a" is defined in the 
	 *	  global environment;
	 * 2. "(define (func a b)
	 *	     (define c 4)
	 *       (+ a b c))"
	 *    --> env[0]["func"] = <...compound procedure>, env[1]["c"] = 4,
	 *    "c" is defined in the local environment.
	 */
	static Environment env;
};

#endif