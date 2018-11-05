/* Header file of class Object */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <memory>
using namespace std;

class Procedure;
class Cons;
class List;

enum { UNASSIGNED = 0, INTEGER, REAL, BOOLEAN, STRING, PROCEDURE, CONS, LIST };

/* Object save several types of data */
class Object {
public:
	/* Constructor */
	Object() : type(UNASSIGNED) {}
	Object(const Object& ob);
	
	explicit Object(int val) :			type(INTEGER),	integer(val){}
	explicit Object(double val) :		type(REAL),		real(val) {}
	explicit Object(bool val) :			type(BOOLEAN),	boolean(val) {}
	explicit Object(const string& s) :	type(STRING),	str(s) {}
	explicit Object(const Procedure& p) :
		type(PROCEDURE), proc(make_shared<Procedure>(p)) {}
	explicit Object(const Cons& c) : 
		type(CONS), cons(make_shared<Cons>(c)) {}
	explicit Object(const List& l) : 
		type(LIST), lst(make_shared<List>(l)) {}

	/* Operator= and destructor */
	Object& operator=(const Object& ob);
	~Object() {}

	/* Others */
	int get_type() const { return type; }
	int get_integer() const { return integer; }
	double get_real() const { return real; }
	bool get_boolean() const { return boolean; }
	string get_string() const { return str; }
	shared_ptr<Procedure> get_proc() const { return proc; }
	shared_ptr<Cons> get_cons() const { return cons; };
	shared_ptr<List> get_list() const { return lst; }
	
private:
	int						type;
	int						integer;
	double					real;
	bool					boolean;
	string					str;
	shared_ptr<Procedure>	proc;
	shared_ptr<Cons>		cons;
	shared_ptr<List>		lst;
};

enum { UNKNOWN = 0, PRIMITIVE, COMPOUND };

/* Procedure: 
 * save primitive procedure as a function pointer, implemented in 
 *	primitive_procedures.cpp;
 * save compound procedure as a string.
 */
class Procedure {
public:
	Procedure() : type(UNKNOWN) {}
	explicit Procedure(Object& ob) :
		type(COMPOUND), func(nullptr), proc(ob.get_string()) {}
	explicit Procedure(Object(*f)(vector<Object>&)) :
		type(PRIMITIVE), func(f), proc("") {}

	int get_type() const { return type; }
	Object(*get_primitive()) (vector<Object>&) { return func; }
private:
	int type;					/* Type of procdure, primitive or compound */
	Object(*func)(vector<Object>&);	/* Primitive procedure, such as +, square */
	string proc;		/* Compound procudere, such as (lambda (x) (+ x 1)) */
};

/* Scheme's pair */
class Cons {
public:
	Cons() = delete;
	Cons(const Object& a, const Object& b) : pir(vector<Object>{ a, b }) {}
	explicit Cons(const vector<Object> &obs);
	~Cons() {}

	Object car() const { return pir[0]; }
	Object cdr() const { return pir[1]; }
private:
	vector<Object> pir;
};

/* Scheme's list */
class List {
public:
	List() = delete;
	List(const vector<Object>& l) : lst(l.begin(), l.end()) {}
	~List() {}

	Object car() { return lst.front(); }
	Object cdr() {
		auto it = lst.begin();
		return Object(vector<Object>(++it, lst.end())); 
	}
	bool empty() { return lst.empty(); }
	int size() { return lst.size(); }

	list<Object> lst;
};


#endif