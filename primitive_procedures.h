/* Primitive procedures of Scheme */

#ifndef PRIMITIVE_PROCEDURES_H_
#define PRIMITIVE_PROCEDURES_H_

#include "object.h"

namespace Primitive {
	/* Quit */
	/* Note: return an empty Object, obs is empty too */
	Object quit(vector<Object>& obs);

	/* Return the sum of obs as an Object. */
	/* Note: Result's type could be INTEGER or REAL */
	Object add(vector<Object>& obs);
	
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