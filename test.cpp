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

void test_io()
{
	string code = "(cons 1 2)\n";
	istringstream iss(code);
	test_cnts++;
	split_input(get_input(iss)) == vector<string>{"(", "cons", "1", "2", ")"} ?
		test_pass++ : 1;

	code = "((lambda(a)(+ a 3)) 3)\n";
	iss.str(code);
	test_cnts++;
	split_input(get_input(iss)) ==
		vector<string>{
		"(", "(", "lambda", "(", "a", ")", "(",
			"+", "a", "3", ")", ")", "3", ")"
	} ? test_pass++ : 1;
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

/* Test <, >, =/eq?/equal? */
void test_primitive_2()
{
	test("(< 1 2)", Object(1 < 2));
	//test("(< -1 -2)", Object(-1 < -2));
	test("(< 1.2 3.2)", Object(1.2 < 3.2));
	test("(< 1 1.0)", Object(1 < 1.0));
	test("(< 1 2 3 4)", Object(true));
	test("(< 1.0 2 2.1 100)", Object(true));
	test("(< 1.0 1 2.1 100)", Object(false));

	test("(> 1 2)", Object(1 > 2));
	//test("(> -1 -2)", Object(-1 > -2));
	test("(> 1 1.0)", Object(1 > 1.0));
	test("(> (+ 1 1.0) (+ 0 2.0))", Object((1 + 1.0) > (0 + 2.0)));
	test("(> 6 4 3 1)", Object(true));
	test("(> 5 4.9999 4.3 2)", Object(true));
	test("(> 7 5 3.333 3.334)", Object(false));

	test("(= 1.3 1.2)", Object(1.3 == 1.2));
	test("(= (+ 1 1.0) (+ 0 2.0))", Object((1 + 1.0) == (0 + 2.0)));
	test("(= 1 1.0 1 1.0)", Object(true));
	test("(= 2 2.0 2 2.0 2.000001)", Object(false));

	test("(eq? 1 1.0)", Object(false));
	test("(eq? 1 1)", Object(1 == 1));
	test("(equal? 1.0 1.0)", Object(1.0 == 1.0));
	test("(equal? \"abc\" \"abc\")", Object(true));
	test("(equal? \"abc\" \"123\")", Object(false));
	test("(eq? #t #t)", Object(true));
	test("(eq? #t #f)", Object(false));
	test("(eq? + +)", Object(true));
	test("(eq? + -)", Object(false));
	/* Compare two compound procedure */
	/* Note: the following tests must run after test_define(); */
	test("(eq? fact fact)", Object(true)); 
	test("(eq? fact square)", Object(false));
}

/* Test define expression */
void test_define() 
{
	/* INT_MAX: 2147483647 INT_MIN : -2147483648 */

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

	test("(define (fact n) (if (< n 2) 1 (* n (fact (- n 1)))))", 
		Object("define OK."));
	test("(fact 0)", Object(1));
	test("(fact 1)", Object(1));
	test("(fact 2)", Object(2));
	test("(fact 3)", Object(6));
	test("(fact 4)", Object(24));
	test("(fact 10)", Object(3628800));

	test("(define (fib n) (if (< n 3) 1 (+ (fib (- n 1)) (fib (- n 2)))))", 
		Object("define OK."));
	test("(fib 1)", Object(1));
	test("(fib 2)", Object(1));
	test("(fib 3)", Object(2));
	test("(fib 4)", Object(3));
	test("(fib 10)", Object(55));
	
	// test("(define (f a b) (define c 5) (+ a b c))", Object("define OK."));
	// test("(f 1 2)", Object(1 + 2 + 5));
}


/* Test lambda expression */
void test_lambda()
{
	test("((lambda (a) (+ a 3)) 3)", Object(6));
	test("((lambda (a b) (* a b)) 3 4)", Object(3 * 4));
	test("((lambda (x y z) (* x y (+ x z))) 3 4 (+ 3 4))", 
		Object(3 * 4 * (3 + (3 + 4)))); 
}

void test_file()
{
	istringstream iss("(load \"F:/Git/Learning/impl_scheme/test/test1.scm\")\n");
	string input = get_input(iss);
	vector<string> split = split_input(input);
	Object result = eval(split);
}

void run_test()
{
	test_io();
	test_primitive_1();
	test_define();
	test_primitive_2();
	test_lambda();

	cout << "test counts: " << test_cnts << " test pass: " << test_pass << endl;

	// test_file();

	return;
}