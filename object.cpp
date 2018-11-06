/* Implement of class Object */

#include "object.h"
#include "eval.h"

void Object::inner(const Object& ob)
{
	if (type == STRING || type == KEYWORD)
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

Object::Object(const Object& ob) : type(ob.get_type()) {
	inner(ob);
}

Object& Object::operator=(const Object& ob) {
	type = ob.get_type();
	inner(ob);
	return *this;
}

Object::Object(const string& s, int t) : type(t), str(s) {
	if (t != KEYWORD) {
		error_handler("ERROR(runtime): t must be KEYWORD "
			"-- Constructor: Object(const string& s, int t)");
	}
}

static vector<string> type_str{
	"unassigned", "integer", "real", "boolean",
	"string", "procedure", "cons", "list", "keyword"
};

string Object::get_type_str() const {
	return type_str[type];
}

Cons::Cons(const vector<Object> &obs) {
	if (obs.size() != 2)
		error_handler("ERROR(scheme): number of operands number must be 2 "
			"-- cons, usage: (cons 1 2)"
#ifndef NDEBUG
			"\nDEBUG: Construct: Cons(const vector<Object>&)"
#endif
		);
	else
		pir = make_pair(obs[0], obs[1]);
}