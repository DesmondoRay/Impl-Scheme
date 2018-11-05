/* Object */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <iostream>
#include <vector>
#include <list>
using namespace std;

class Procedure;
//class Cons;
//class List;

enum { UNASSIGNED = 0, INTEGER, REAL, BOOLEAN, STRING, PROCEDURE, CONS, LIST };

class Object {
public:
	Object() : type(UNASSIGNED) {}
	Object(const Object& ob);
	Object& operator=(const Object& ob);
	~Object();

	explicit Object(int val) : type(INTEGER), integer(val) {}
	explicit Object(double val) : type(REAL), real(val) {}
	explicit Object(bool val) : type(BOOLEAN), boolean(val) {}
	explicit Object(const string& s) : type(STRING), str(s) {}
	explicit Object(const Procedure& p);

	int get_type() const { return type; }
	int get_integer() const { return integer; }
	double get_real() const { return real; }
	bool get_boolean() const { return boolean; }
	string get_string() const { return str; }
	Procedure* get_proc() const { return proc; }
	
private:
	int type;
	int				integer;
	double			real;
	bool			boolean;
	string			str;
	Procedure*		proc;
	//Cons*			cons;
	//List*			lst;
};

enum { NOTHING = 0, PRIMITIVE, COMPOUND };

class Procedure {
public:
	Procedure() : type(NOTHING) {}
	Procedure(Object& ob) : 
		type(COMPOUND), func(nullptr), proc(ob.get_string()) {}
	Procedure(Object(*f)(vector<Object>&)) : type(PRIMITIVE), func(f), proc("") {}

	int get_type() const { return type; }
	Object(*get_primitive()) (vector<Object>&) { return func; }
	//Object (*vector<Object>&)) get_primitive() const { return func; }
private:
	int type;					/* Type of procdure, primitive or compound */
	Object(*func)(vector<Object>&);	/* Primitive procedure, such as +, square */
	string proc;		/* Compound procudere, such as (lambda (x) (+ x 1)) */
};

/*
class Cons {
public:
	Cons() = default;
	Cons(const Object& a, const Object& b) : pair(vector<Object>{ a, b }) {}
	~Cons() {};
	Object car() { return pair[0]; }
	Object cdr() { return pair[1]; }
private:
	vector<Object> pair;
};

class List {
public:
	List() = default;
	List(const vector<Object>& l) : lst(l.begin(), l.end()) {}
	Object car() { return lst.front(); }
	Object cdr() {
		auto it = lst.begin();
		return Object(vector<Object>(++it, lst.end())); 
	}
	bool empty() { return lst.empty(); }
	int size() { return lst.size(); }
private:
	list<Object> lst;
};
*/

#endif