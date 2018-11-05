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


int main()
{
	Environment env;
	env["+"] = Object(Procedure(add));

	eval_run(env);

	return 0;
}