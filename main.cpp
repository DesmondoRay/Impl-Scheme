/* Main */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

#include "object.h"
#include "io_function.h"
#include "eval.h"

/* test.cpp */
void run_test();
void load_code(const string& code);

int main(int argc, char **argv)
{
	initialize_environment();

#if 1
	run_test();
#endif

	/* Preheating evaluator */
	load_code(string("(define (f) (+ 1 2))\n"));
	load_code(string("(f)\n"));

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

/*
(define (f) (* 1 2))
(f)

(define (make-withdraw balance)
(lambda (amount)
(if (>= balance amount)
(begin (set! balance (- balance amount))
balance)
\"Insufficient funds\")))

(define w1 (make-withdraw 100))
(w1 34)
(w1 34)
*/