/* Test */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

#include "eval.h"
#include "io_function.h"
#include "object.h"
#include "primitive_procedures.h"

static int test_cnts = 0, test_pass = 0;

template<typename T>
void printVec(vector<T>& v)
{
	for (auto &i : v)
		cout << i << ", ";
	cout << endl;
}

void test(const string& code, Object& expect_result)
{
	string copy(code);
	copy.push_back('\n');
	test_cnts++;
	istringstream iss(copy);
	string input = get_input(iss);

	vector<string> split = split_input(input);
	Object result = eval(split);
	expect_result == result ? test_pass++ : 1;
}

/* Test +, -, *, / */
void test_primitive_1() 
{
	test("(+ 3 4)", Object(3 + 4));
	test("(+ 2.1 4 5.8)", Object(2.1 + 4 + 5.8));
	test("(- 1 5)", Object(1 - 5));
	test("(- 5.23 2)", Object(5.23 - 2));
	test("(+ 2147483647 1)", Object(INT_MAX + 1));
	test("(* 2147483647 34)", Object(INT_MAX * 34));
	test("(/ 2 3)", Object(2.0 / 3));
}

/* Test define expression */
void test_define() 
{
	test("(define n 5)", Object("define OK."));
	test("(+ n 6)", Object(11));
	test("(* 4.0 n)", Object(4.0 * 5));
	/* update n */
	test("(define n 100)", Object("define OK."));
	test("(+ n 6)", Object(106));
	test("(* 4.0 n)", Object(4.0 * 100));

	test("(define (square a) (* a a))", Object("define OK."));
	test("(square 5)", Object(25));
	test("(square 6.24)", Object(6.24 * 6.24));
	
	test("(define sq (lambda (x) (* x x)))", Object("define OK."));
	test("(sq 5)", Object(25));
	test("(sq 6.24)", Object(6.24 * 6.24));

	// test("(define (f a b) (define c 5) (+ a b c))", Object("define OK."));
	// test("(f 1 2)", Object(1 + 2 + 5));
}

/* Test lambda expression */
void test_lambda()
{
	test("((lambda (a) (+ a 3)) 3)", Object(6));
	test("((lambda (a b) (* a b)) 3 4", Object(3 * 4));
	test("((lambda (x y z) (* x y (+ x z))) 3 4 (+ 3 4))", 
		Object(3 * 4 * (3 * (3 + 4)))); 
}

void run_test()
{
	test_primitive_1();
	test_define();


	cout << "test counts: " << test_cnts << " test pass: " << test_pass << endl;

	return;
}

// INT_MAX: 2147483647 INT_MIN : -2147483648