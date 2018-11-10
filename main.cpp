/* Main */

#include <iostream>
using namespace std;

#include "object.h"
#include "io_function.h"
#include "eval.h"

/* test.cpp */
void run_test();

int main(int argc, char **argv)
{
	initialize_environment();

#if 1
	run_test();
#endif

	if (argc == 1)
		run_evaluator(std::cin);
	else {
		/* Load code from file to evaluate */
		ifstream input(argv[1], ifstream::in);
		if (input)
			run_evaluator(input, 1);
		else
			cerr << "ERROR(runtime): couldn't open file -- " << argv[1] << endl;
	}
	
	return 0;
}