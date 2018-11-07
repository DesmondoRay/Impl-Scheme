/* Primitive procedures of Scheme */

#ifndef PRIMITIVE_PROCEDURES_H_
#define PRIMITIVE_PROCEDURES_H_

#include <fstream>
#include "object.h"

#define NDEBUG

namespace Primitive {
	/* Quit */
	/* Note: obs should/could be empty */
	Object quit(vector<Object>& obs);

	/* Reset Evaluator, initialize environment */
	/* Note: obs should/could be empty */
	Object reset(vector<Object>& obs);

/* Primitive operation, note: result's type could be INTEGER or REAL */
	/* Return the sum of obs */
	Object add(vector<Object>& obs);
	/* Return the difference of obs */
	Object sub(vector<Object>& obs);
	/* Return the product of obs */
	Object mul(vector<Object>& obs);
	/* Return the quotient of obs */
	/* Note: always return real(double) */
	Object div(vector<Object>& obs);
	/* Return remainder, it takes two arguments */
	Object remainder(vector<Object>& obs);
	/* Return absolute value */
	Object abs(vector<Object>& obs);
	/*  */

/* Return true or false as an Object, all obs must be number */
	/* Return true if obs[0] < obs[1] < obs[2] < ... < obs[n] */
	Object less(vector<Object>& obs);
	/* Return true if obs[0] > obs[1] > obs[2] > ... > obs[n] */
	Object greater(vector<Object>& obs);
	/* Return true if obs[0] == obs[1] == obs[2] == ... == obs[n] */
	/* arguments must be numbers */
	Object op_equal(vector<Object>& obs);
	/* Return true if obs[0] <= obs[1] <= obs[2] <= ... <= obs[n] */
	Object lessEqual(vector<Object>& obs);
	/* Return true if obs[0] >= obs[1] >= obs[2] >= ... >= obs[n] */
	Object greaterEqual(vector<Object>& obs);

/* Return true if obs[0] equal obs[1] equal obs[2] equal .. equal obs[n]*/
	/* arguments could be all types */
	Object equal(vector<Object>& obs);

	/* Return the pair of obs as an Object */
	/* Note: obs.size() must be 2 */
	Object make_cons(vector<Object>& obs);

	/* Return the list of obs as an Object */
	Object make_list(vector<Object>& obs);

	/* Print obs */
	Object display(vector<Object>& obs);

	/* New line */
	Object newline(vector<Object>& obs);

	/* Load code from input file and evaluate */
	/* Usage: (load "path/name.scm") */
	Object load(vector<Object>& obs);
};

#endif