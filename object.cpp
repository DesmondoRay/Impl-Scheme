/* Object */

#include "object.h"

Object::~Object() {
	if (type == PROCEDURE)
		delete proc;
	/*
	else if (type == CONS)
		delete cons;
	else if (type == LIST)
		delete lst;
		*/
}

Object::Object(const Object& ob) : type(ob.get_type()) {
	if (type == STRING)
		str = ob.get_string();
	else if (type == INTEGER)
		integer = ob.get_integer();
	if (type == PROCEDURE)
		proc = new Procedure(*(ob.get_proc()));

}

Object& Object::operator=(const Object& ob) {
	type = ob.get_type();
	if (type == PROCEDURE)
		proc = new Procedure(*(ob.get_proc()));
	return *this;
}


Object::Object(const Procedure& p) : type(PROCEDURE), proc(new Procedure(p)) {

}
