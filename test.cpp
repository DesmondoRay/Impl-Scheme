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

void load_code(const string& code)
{
	string copy(code);
	copy.push_back('\n');
	istringstream iss(copy);
	string input = get_input(iss);
	vector<string> split = split_input(input);
	Object result = eval(split);
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
		test_cnts++;\
		string copy(code);\
		istringstream iss(copy + "\n");\
		Object result = eval(split_input(get_input(iss)));\
		if (expect_result == result) test_pass++;\
		else REPORT_ERROR(result, expect_result);\
	} while(0)

/* Test primitive procedures 1 */
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

	TEST("(abs 0)", Object(0));
	TEST("(abs 1.2)", Object(1.2));
	TEST("(abs -7)", Object(7));
	TEST("(abs -0)", Object(0));
	TEST("(abs (- 1 2))", Object(1));

	TEST("(square 2)", Object(4));
	TEST("(square 1.245)", Object(1.245 * 1.245));
	TEST("(square 0)", Object(0));
	TEST("(square -8)", Object(64));

	TEST("(sqrt 4)", Object(2.0));
	TEST("(sqrt 5)", Object(sqrt(5.0)));
	TEST("(sqrt 0)", Object(0.0));

	TEST("(quotient 10 3)", Object(3));
	TEST("(quotient 1 3)", Object(0));
	TEST("(quotient 5 2)", Object(2));
}

/* Test primitive procedures 2 */
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

/* Test primitive procedures 3 */
static void test_primitive_3()
{
	TEST("(number? 0)", Object(true));
	TEST("(number? 1.234)", Object(true));
	TEST("(number? -786)", Object(true));
	TEST("(number? \"abc\")", Object(false));
	TEST("(number? #t)", Object(false));

	TEST("(boolean? #t)", Object(true));
	TEST("(boolean? #f)", Object(true));
	TEST("(boolean? (< 1 3))", Object(true));
	TEST("(boolean? (eq? 2 3))", Object(true));
	TEST("(boolean? 1)", Object(false));
	TEST("(boolean? \"#f\")", Object(false));

	TEST("(integer? 1345)", Object(true));
	TEST("(integer? -1345)", Object(true));
	TEST("(integer? 34.2)", Object(false));
	TEST("(integer? \"abc\")", Object(false));
	TEST("(integer? #t)", Object(false));

	TEST("(real? 1345)", Object(false));
	TEST("(real? -1345.0)", Object(true));
	TEST("(real? 34.2)", Object(true));
	TEST("(real? \"abc\")", Object(false));
	TEST("(real? #t)", Object(false));

	TEST("(even? 0)", Object(true));
	TEST("(even? 13)", Object(false));
	TEST("(even? 266)", Object(true));
	TEST("(even? -266)", Object(true));

	TEST("(odd? 0)", Object(false));
	TEST("(odd? 13)", Object(true));
	TEST("(odd? 266)", Object(false));
	TEST("(odd? -267)", Object(true));

	TEST("(pair? (cons 1 2))", Object(true));
	TEST("(pair? (list 1 2 3))", Object(true));
	TEST("(pair? '(4 5 6))", Object(true));
	TEST("(pair? 12)", Object(false));

	TEST("(null? (list 1 2))", Object(false));
	TEST("(null? (list))", Object(true));
	TEST("(null? '())", Object(true)); /* '(): empty list */
	TEST("(null? (cons 1 2))", Object(false)); 
	TEST("(null? 12)", Object(false));

	TEST("(list? (list 1 2))", Object(true));
	TEST("(null? (list))", Object(true));
	TEST("(null? (cons 1 2))", Object(false));
	TEST("(null? 12)", Object(false));

	TEST("(not #t)", Object(false));
	TEST("(not #f)", Object(true));
	TEST("(not 1)", Object(false));
	TEST("(not 0)", Object(false));
}

/* Test define expression */
static void test_define()
{
	TEST("(define n 5)", Object("n"));
	TEST("(+ n 6)", Object(11));
	TEST("(* 4.0 n)", Object(4.0 * 5));
	/* update n */
	TEST("(define n 100)", Object("n"));
	TEST("(+ n 6)", Object(106));
	TEST("(* 4.0 n)", Object(4.0 * 100));

	TEST("(define (square a) (* a a))", Object("square"));
	TEST("(square 5)", Object(25));
	TEST("(square 6.24)", Object(6.24 * 6.24));

	TEST("(define sq (lambda (x) (* x x)))", Object("sq"));
	TEST("(sq 5)", Object(25));
	TEST("(sq 6.24)", Object(6.24 * 6.24));

	TEST("(define (fact n) (if (< n 2) 1 (* n (fact (- n 1)))))", 
		Object("fact"));
	TEST("(fact 0)", Object(1));
	TEST("(fact 1)", Object(1));
	TEST("(fact 2)", Object(2));
	TEST("(fact 3)", Object(6));
	TEST("(fact 4)", Object(24));
	TEST("(fact 10)", Object(3628800));

	TEST("(define (fib n) (if (< n 3) 1 (+ (fib (- n 1)) (fib (- n 2)))))", 
		Object("fib"));
	TEST("(fib 1)", Object(1));
	TEST("(fib 2)", Object(1));
	TEST("(fib 3)", Object(2));
	TEST("(fib 4)", Object(3));
	TEST("(fib 10)", Object(55));
}

/* Test begin expression */
static void test_begin()
{
	string code("\
(define (f1 a b)\
  (define c 5)\
  (+ a b c))"); /* "c" is a local variable */
	load_code(code);
	TEST("(f1 1 2)", Object(1 + 2 + 5));
	TEST("(f1 2.3 4.5)", Object(2.3 + 4.5 + 5));

	code = "\
(define (f2 a b)\
  (+ a b)\
  (define c 5))"; /* Return the last subexpression as the result */
	load_code(code);
	TEST("(f2 2 3)", Object("c")); 

	TEST("\
(begin (define c1 (cons 1 2))\n\
  (define c2 (cons 1 2))\n\
  (eq? c1 c2))",Object(false));

	TEST("\
(begin (define c3 (cons 1 2))\n\
  (define c4 c3)\n\
  (eq? c3 c4))", Object(true));
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
	load_code("(define x 3)");
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

/* Test set! expression */
static void test_set()
{
	string code1("\
(define (make-withdraw balance)\
  (lambda (amount)\
    (if (>= balance amount)\
		(begin (set! balance (- balance amount))\
			   balance)\
		\"Insufficient funds\")))\
\n");
	string code2("(define w1 (make-withdraw 100))\n");
	string code3("(define w2 (make-withdraw 200))\n");

	load_code(code1);
	load_code(code2);
	load_code(code3);
	TEST("(w1 50)", Object(50));
	TEST("(w1 22)", Object(28));
	TEST("(w2 50)", Object(150));
	TEST("(w2 22)", Object(128));
	TEST("(w1 22)", Object(6));
	TEST("(w2 22)", Object(106));
	TEST("(w1 200)", Object("\"Insufficient funds\""));
	TEST("(w2 200)", Object("\"Insufficient funds\""));
}

/* Test evaluate code from file */
static void test_load_file(const string& filename)
{
	string code("(load \"");
	code += filename + "\")\n";
	istringstream iss(code);
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
	test_primitive_3();
	test_begin();
	test_lambda();
	test_let();
	test_cond();
	test_set();

	test_load_file("F:/Git/Learning/impl_scheme/test_file/test1.scm");

	cout << "test counts: " << test_cnts << " test pass: " << test_pass << endl;

	return;
}