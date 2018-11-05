/* Primitive procedures */

#include "primitive_procedures.h"

Object add(vector<Object>& vob) {
	int sum = 0;
	for (auto &ob : vob) {
		if (ob.get_type() == INTEGER)
			sum += ob.get_integer();
		else if (ob.get_type() == REAL)
			sum += ob.get_real();
	}

	return Object(sum);
}