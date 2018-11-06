/* Primitive procedures of Scheme */

#ifndef PRIMITIVE_PROCEDURES_H_
#define PRIMITIVE_PROCEDURES_H_

#include "object.h"

namespace Primitive {
	/* Quit */
	/* Note: obs could/should be empty */
	Object quit(vector<Object>& obs);

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

	
	/* Return the pair of obs as an Object */
	/* Note: obs.size() must be 2 */
	Object make_cons(vector<Object>& obs);

	/* Return the list of obs as an Object */
	Object make_list(vector<Object>& obs);

	/* Print obs */
	Object display(vector<Object>& obs);

	/* New line */
	Object newline(vector<Object>& obs);
};

#endif