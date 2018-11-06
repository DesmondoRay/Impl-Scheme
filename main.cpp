/* Main */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

#include "object.h"
#include "io_function.h"
#include "eval.h"

//#define NDEBUG

void run_test();

int main(int argc, char **argv)
{
	initialize_environment();

#ifndef NDEBUG
	run_test();
#else
	if (argc == 1)
		run_evaluator(std::cin);
	else {
		/* to do */
	}
#endif
	
	return 0;
}