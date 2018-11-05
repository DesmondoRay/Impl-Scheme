/* Implement of class Object */

#include "object.h"

Object::Object(const Object& ob) : type(ob.get_type()) {
	if (type == STRING)
		str = ob.get_string();
	else if (type == INTEGER)
		integer = ob.get_integer();
	else if (type == REAL)
		real = ob.get_real();
	else if (type == BOOLEAN)
		boolean = ob.get_boolean();
	else if (type == PROCEDURE)
		proc = ob.get_proc();
	else if (type == CONS)
		cons = ob.get_cons();
	else if (type == LIST)
		lst = ob.get_list();
}

Object& Object::operator=(const Object& ob) {
	type = ob.get_type();
	if (type == STRING)
		str = ob.get_string();
	else if (type == INTEGER)
		integer = ob.get_integer();
	else if (type == REAL)
		real = ob.get_real();
	else if (type == BOOLEAN)
		boolean = ob.get_boolean();
	else if (type == PROCEDURE)
		proc = ob.get_proc();
	else if (type == CONS)
		cons = ob.get_cons();
	else if (type == LIST)
		lst = ob.get_list();
	return *this;
}

Cons::Cons(const vector<Object> &obs) {
	if (obs.size() != 2)
		cerr << "error: Cons(const vector<Object>), vector.size() must be 2"
		<< endl;
	else
		pir = obs;
}