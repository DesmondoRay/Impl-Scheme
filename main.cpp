/* Main */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

#include "object.h"
#include "io_function.h"
#include "eval.h"

int main()
{
	initialize_environment();
	run_evaluator();

	return 0;
}