/* Implement of class Object */

#include "object.h"
#include "eval.h"

void Object::copy_inner(const Object& ob)
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
	copy_inner(ob);
}

Object& Object::operator=(const Object& ob) {
	type = ob.get_type();
	copy_inner(ob);
	return *this;
}

bool Object::operator_inner(const Object& ob, const string& op) {
	if (op != "<" && op != ">" && op != "==") 
		error_handler(string("ERROR(runtime): Object::operator_inner() takes") + 
			" <, >, = as second argument");
	if (!is_number() && !ob.is_number()) {
		string error_msg("ERROR(scheme): passed a ");
		error_msg += ob.get_type_str() + " to ";
		error_msg += string({ op[0] }) +", it only takes integer and real.";
		error_handler(error_msg);
	}

	double lhs = (type == INTEGER ? integer : real);
	double rhs = (ob.get_type() == INTEGER ? ob.get_integer() : ob.get_real());

	return (op == "<" ? (lhs < rhs) : 
		(op == ">" ? (lhs > rhs) : (abs(lhs - rhs) <= 1e-9)));
}

bool Object::operator==(const Object& ob) {
	int type2 = ob.get_type();

	if (type != type2)
		return false;
	else if (type == INTEGER)
		return integer == ob.get_integer();
	else if (type == REAL)
		return abs(real - ob.get_real()) <= 1e-9;
	else if (type == STRING || type == KEYWORD)
		return str == ob.get_string();
	else if (type == BOOLEAN)
		return boolean == ob.get_boolean();
	else if (type == PROCEDURE)
		return proc == ob.get_proc();
	else if (type == CONS)
		return true; /* to do */
	else if (type == LIST)
		return true; /* to do */
	return true; // UNASSIGNED
}

bool Object::operator<(const Object& ob) {
	return operator_inner(ob, "<");
}

bool Object::operator>(const Object& ob) {
	return operator_inner(ob, ">");
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