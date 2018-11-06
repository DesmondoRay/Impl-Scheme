/* Main */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

#include "object.h"
#include "io_function.h"
#include "eval.h"
#include "primitive_procedures.h"

void init_env(Environment& env);

int main()
{
	Environment env;
	init_env(env);

	run_evaluator(env);

	return 0;
}


void init_env(Environment& env)
{
	env["+"] = Object(Procedure(Primitive::add));
	env["quit"] = Object(Procedure(Primitive::quit));
	env["exit"] = Object(Procedure(Primitive::quit));
	env["cons"] = Object(Procedure(Primitive::make_cons));
	env["list"] = Object(Procedure(Primitive::make_list));
	env["display"] = Object(Procedure(Primitive::display));
}