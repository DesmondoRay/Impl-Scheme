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
static void printVec(vector<T>& v)
{
	for (auto &i : v)
		cout << i << ", ";
	cout << endl;
}

static void test_io()
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

/* Report_error, for example: 
 * <TEST ERROR> line: 72, expect: { integer, 8 }, actual: { integer, 7 } 
 */
#define REPORT_ERROR(actual, expect) \
	do {\
		cerr << "<TEST ERROR> line: " << __LINE__ /*<< __FILE__ */ \
			<< ", expect: { " << expect.get_type_str() << ", ";\
		Primitive::display(vector<Object>{expect});\
		cerr << "}, actual: { " << actual.get_type_str() << ", ";\
		Primitive::display(vector<Object>{actual});\
		cerr << "}" << endl;\
	} while(0)

#define TEST(code, expect_result)\
	do {\
		string copy(code);\
		copy.push_back('\n'); test_cnts++;\
		istringstream iss(copy); \
		string input = get_input(iss);\
		vector<string> split = split_input(input);\
		Object result = eval(split);\
		if (expect_result == result)\
			test_pass++;\
		else REPORT_ERROR(result, expect_result);\
	} while(0)

/* Test +, -, *, / */
static void test_primitive_1()
{
	/* INT_MAX: 2147483647 INT_MIN : -2147483648 */
	TEST("(+ 3 4)", Object(3 + 4));
	TEST("(+ -3 4)", Object(-3 + 4));
	TEST("(+ 2.1 4 5.8)", Object(2.1 + 4 + 5.8));
	TEST("(+ 2147483647 1)", Object(INT_MAX + 1));
	TEST("(+ -2147483648 -1)", Object(INT_MIN - 1));

	TEST("(- 1 5)", Object(1 - 5));
	TEST("(- 5.23 2)", Object(5.23 - 2));
	TEST("(- 5.23 -2)", Object(5.23 - (-2)));
	
	TEST("(* 2147483647 34)", Object(INT_MAX * 34));
	TEST("(* 23 -3)", Object(23 * (-3)));

	TEST("(/ 2 3)", Object(2.0 / 3));
	TEST("(/ 20 -3)", Object(20.0 / (-3)));

	TEST("(remainder 20 3)", Object(20 % 3));
	TEST("(remainder 4 7)", Object(4 % 7));
	TEST("(remainder -14 7)", Object(-14 % 7));
}

/* Test <, >, =/eq?/equal? */
static void test_primitive_2()
{
	TEST("(< 1 2)", Object(1 < 2));
	TEST("(< -1 -2)", Object(-1 < -2));
	TEST("(< 1.2 3.2)", Object(1.2 < 3.2));
	TEST("(< 1 1.0)", Object(1 < 1.0));
	TEST("(< 1 2 3 4)", Object(true));
	TEST("(< 1.0 2 2.1 100)", Object(true));
	TEST("(< 1.0 1 2.1 100)", Object(false));

	TEST("(<= 1 2)", Object(1 <= 2));
	TEST("(<= 2 2)", Object(2 <= 2));
	TEST("(<= 2.0 2 2.0)", Object(true));
	TEST("(<= -1 -1.0)", Object(true));

	TEST("(> 1 2)", Object(1 > 2));
	TEST("(> -1 -2)", Object(-1 > -2));
	TEST("(> 1 1.0)", Object(1 > 1.0));
	TEST("(> (+ 1 1.0) (+ 0 2.0))", Object((1 + 1.0) > (0 + 2.0)));
	TEST("(> 6 4 3 1)", Object(true));
	TEST("(> 5 4.9999 4.3 2)", Object(true));
	TEST("(> 7 5 3.333 3.334)", Object(false));

	TEST("(>= 7 5 3.333 )", Object(true));
	TEST("(>= -7 -8)", Object(true));
	TEST("(>= 7 5 3.333 3.334)", Object(false));

	TEST("(= 1.3 1.2)", Object(1.3 == 1.2));
	TEST("(= (+ 1 1.0) (+ 0 2.0))", Object((1 + 1.0) == (0 + 2.0)));
	TEST("(= 1 1.0 1 1.0)", Object(true));
	TEST("(= 2 2.0 2 2.0 2.000001)", Object(false));

	TEST("(eq? 1 1.0)", Object(false));
	TEST("(eq? 1 1)", Object(1 == 1));
	TEST("(equal? 1.0 1.0)", Object(1.0 == 1.0));
	TEST("(equal? \"abc\" \"abc\")", Object(true));
	TEST("(equal? \"abc\" \"123\")", Object(false));
	TEST("(eq? #t #t)", Object(true));
	TEST("(eq? #t #f)", Object(false));
	/* Compare two primitive procedure */
	TEST("(eq? + +)", Object(true));
	TEST("(eq? + -)", Object(false));
	/* Compare two compound procedure */
	/* Note: the following tests must run after test_define(); */
	TEST("(eq? fact fact)", Object(true)); 
	TEST("(eq? fact square)", Object(false));
}

/* Test define expression */
static void test_define()
{
	TEST("(define n 5)", Object("define OK."));
	TEST("(+ n 6)", Object(11));
	TEST("(* 4.0 n)", Object(4.0 * 5));
	/* update n */
	TEST("(define n 100)", Object("define OK."));
	TEST("(+ n 6)", Object(106));
	TEST("(* 4.0 n)", Object(4.0 * 100));

	TEST("(define (square a) (* a a))", Object("define OK."));
	TEST("(square 5)", Object(25));
	TEST("(square 6.24)", Object(6.24 * 6.24));

	TEST("(define sq (lambda (x) (* x x)))", Object("define OK."));
	TEST("(sq 5)", Object(25));
	TEST("(sq 6.24)", Object(6.24 * 6.24));

	TEST("(define (fact n) (if (< n 2) 1 (* n (fact (- n 1)))))", 
		Object("define OK."));
	TEST("(fact 0)", Object(1));
	TEST("(fact 1)", Object(1));
	TEST("(fact 2)", Object(2));
	TEST("(fact 3)", Object(6));
	TEST("(fact 4)", Object(24));
	TEST("(fact 10)", Object(3628800));

	TEST("(define (fib n) (if (< n 3) 1 (+ (fib (- n 1)) (fib (- n 2)))))", 
		Object("define OK."));
	TEST("(fib 1)", Object(1));
	TEST("(fib 2)", Object(1));
	TEST("(fib 3)", Object(2));
	TEST("(fib 4)", Object(3));
	TEST("(fib 10)", Object(55));
}

/* Test define expression */
static void test_begin()
{
	string code("\
(define (f1 a b)\
  (define c 5)\
  (+ a b c))"); /* "c" is a local variable */

	TEST(code, Object("define OK."));
	TEST("(f1 1 2)", Object(1 + 2 + 5));
	TEST("(f1 2.3 4.5)", Object(2.3 + 4.5 + 5));

	code = "\
(define (f2 a b)\
  (+ a b)\
  (define c 5))"; /* Return the last subexpression as the result */

	TEST(code, Object("define OK."));
	TEST("(f2 2 3)", Object("define OK.")); 
}

/* Test lambda expression */
static void test_lambda()
{
	TEST("((lambda (a) (+ a 3)) 3)", Object(6));
	TEST("((lambda (a b) (* a b)) 3 4)", Object(3 * 4));
	TEST("((lambda (x y z) (* x y (+ x z))) 3 4 (+ 3 4))", 
		Object(3 * 4 * (3 + (3 + 4)))); 
}

/* Test let expression */
static void test_let()
{
	TEST("(let ((a 3) (b 4)) (+ a b))", Object(3 + 4));
	TEST("(let ((add +) (mul *)) (mul (add 3 4) (add 2 3)))", Object(7 * 5));
}

/* Test cond expression */
static void test_cond()
{
	TEST("(define x 3)", Object("define OK."));
	string code1("\
(cond ((= x 2) 2)\
      ((= x 3) 3))");
	TEST(code1, Object(3));

	string code2("\
(cond ((= x 1) (+ x x))\
	  ((= x 2) (* x x))\
	  (else (* x x x)))");
	TEST(code2, Object(3 * 3 * 3));

	string code3("\
(cond ((> x 5) 5)\
	  ((< x 2) 2))");
	TEST(code3, Object());
}

/* Test evaluate code from file */
static void test_file()
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
	test_begin();
	test_lambda();
	test_let();
	test_cond();
	
	cout << "test counts: " << test_cnts << " test pass: " << test_pass << endl;

	// test_file();

	return;
}