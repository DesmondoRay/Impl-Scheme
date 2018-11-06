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

enum { 
	UNASSIGNED = 0, INTEGER, REAL, BOOLEAN, 
	STRING, PROCEDURE, CONS, LIST, KEYWORD
};

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
	explicit Object(const char* s) :	type(STRING),	str(s) {}
	explicit Object(const Procedure& p) :
		type(PROCEDURE), proc(make_shared<Procedure>(p)) {}
	explicit Object(const Cons& c) : 
		type(CONS), cons(make_shared<Cons>(c)) {}
	explicit Object(const List& l) : 
		type(LIST), lst(make_shared<List>(l)) {}
	Object(const string& s, int t);


	/* Operator= and destructor */
	Object& operator=(const Object& ob);
	bool operator==(const Object& ob);
	bool operator<(const Object& ob);
	bool operator>(const Object& ob);
	~Object() {}

	/* Others */
	int get_type() const { return type; }
	/* Return name of type */
	string get_type_str() const;
	int get_integer() const { return integer; }
	double get_real() const { return real; }
	bool is_number() const { return type == INTEGER || type == REAL; }
	bool get_boolean() const { return boolean; }
	string get_string() const { return str; }
	shared_ptr<Procedure> get_proc() const { return proc; }
	shared_ptr<Cons> get_cons() const { return cons; };
	shared_ptr<List> get_list() const { return lst; }

	/* Used to operator<, operator> and operator= */
	bool operator_inner(const Object& ob, const string& op);
	
private:
	/* Used to copy constructor and copy control*/
	void copy_inner(const Object& ob); 
	int						type;
	bool					boolean;
	int						integer;
	double					real;
	string					str;
	shared_ptr<Procedure>	proc;
	shared_ptr<Cons>		cons;
	shared_ptr<List>		lst;
};

enum { UNKNOWN = 0, PRIMITIVE, COMPOUND };

/* Procedure: 
 * save primitive procedure as a function pointer, implemented in 
 *	primitive_procedures.cpp;
 * save compound procedure as a vector of parameters and a vector of body.
 */
class Procedure {
public:
	/* Constructor */
	Procedure() : type(UNKNOWN) {}
	explicit Procedure(const vector<string>& params, const vector<string>& bdy):
		type(COMPOUND), func(nullptr), parameters(params), body(bdy) {}
	explicit Procedure(Object(*f)(vector<Object>&)) :
		type(PRIMITIVE), func(f), parameters({}), body({}) {}

	/* Others */
	int get_type() const { return type; }
	Object(*get_primitive()) (vector<Object>&)  { return func; }
	vector<string> get_parameters() const { return parameters; }
	vector<string> get_body() const { return body; }
private:
	int type;					/* Type of procdure, primitive or compound */
	Object(*func)(vector<Object>&);	/* Primitive procedure, such as +, square */
	struct {
		vector<string> parameters;	/* Save parameters of "lambda" expression*/
		vector<string> body;		/* Save body of "lambda" expression*/
	};

	/* Why not choose to use string to save compound procedures:
	 * Every time we apply arguments to compound procedure, the Evaluator must 
	 * split the string into parameters and boyd, it will take time to do this.
	 */
	// string proc;		/* Compound procudere, such as (lambda (x) (+ x 1)) */
};

/* Scheme's pair */
class Cons {
public:
	/* Constructor */
	Cons() = delete;
	Cons(const Object& a, const Object& b) : pir(make_pair(a, b)) {}
	explicit Cons(const vector<Object> &obs);
	~Cons() {}

	/* Others */
	Object car() const { return pir.first; }
	Object cdr() const { return pir.second; }
private:
	pair<Object, Object> pir;
};

/* Scheme's list */
class List {
public:
	/* Constructor */
	List() = delete;
	List(const vector<Object>& l) : lst(l.begin(), l.end()) {}
	~List() {}

	/* Others */
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