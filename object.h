/* Header file of class Object */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <list>
#include <memory>
using namespace std;

//#define USE_LIST

class Procedure;
class Cons;
class List;

/* Types of data */
enum { 
	UNASSIGNED = 0, INTEGER, REAL, BOOLEAN, 
	STRING, PROCEDURE, CONS, NIL, /*LIST,*/ KEYWORD
};

/* Object save several kinds of data */
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
#ifdef USE_LIST
	explicit Object(const List& l) : 
		type(LIST), lst(make_shared<List>(l)) {}
#endif
	Object(const string& s, int t) : type(t), str(s) {}

	/* Operator and destructor */
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
#ifdef USE_LIST
	shared_ptr<List> get_list() const { return lst; }
#endif
	/* Used to operator< and operator> */
	bool operator_inner(const Object& ob, const string& op);
	
private:
	/* Used to copy constructor and copy control*/
	void copy_inner(const Object& ob); 

	int						type;
	int						integer;
	double					real;
	bool					boolean;
	string					str;
	shared_ptr<Procedure>	proc;
	shared_ptr<Cons>		cons;
#ifdef USE_LIST
	shared_ptr<List>		lst;
#endif
};

/* Types of procedure */
enum { UNKNOWN = 0, PRIMITIVE, COMPOUND };

/* Procedure: 
 * save primitive procedure as a function pointer, implemented in 
 * primitive_procedures.cpp;
 * save compound procedure as a vector of parameters and a vector of body.
 */
class Procedure {
public:
	/* Constructor */
	Procedure() : type(UNKNOWN) {}

	// Procedure(const Procedure& p) = delete;
	Procedure& operator==(const Procedure& p) = delete;

	/* Primitive procedure constructor */
	Procedure(Object(*f)(vector<Object>&), const string& proc_name) :
		type(PRIMITIVE), name(proc_name), func(f), parameters({}), body({}),
		static_env(StaticEnv()) {}

	/* Compound procedure constructor */
	Procedure(const vector<string>& params, const vector<string>& bdy, 
		const string& proc_name): type(COMPOUND), name(proc_name), 
		func(nullptr), parameters(params), body(bdy), 
		static_env(StaticEnv()) {}

	/* Static environment, store static local variable. */
	/* Same type with SubEnv -- eval.h, and it could be changed/modified. */
	using StaticEnv = unordered_map<string, Object>;

	/* Anonymous procedure constructor */
	Procedure(const vector<string>& params, const vector<string>& bdy, 
		const string& proc_name, const StaticEnv& env) : 
		type(COMPOUND), name(proc_name), func(nullptr),
		parameters(params), body(bdy), static_env(env) {}

	/* Destructor */
	~Procedure() {}

	/* Others */
	int get_type() const { return type; }
	string get_proc_name() const { return name; }
	/* Return a function pointer -- primitive procedure */
	Object(*get_primitive()) (vector<Object>&)  { return func; }

	/* Return parameters and body of compound procedure */
	vector<string> get_parameters() const { return parameters; }
	vector<string> get_body() const { return body; }

	/* Return static_env, used to expand evaluator's environment. */
	StaticEnv get_env() const { return static_env; } 
	/* Define a new variable or update a existed variable in static_env. */
	void set_env(const string& variable, const Object& value) {
		static_env[variable] = value;
	}
private:
	int		type;		/* Type of procedure, PRIMITIVE or COMPOUND */
	string	name;		/* name of procedure */

	/* Primitive procedure, such as +, square */
	Object(*func)(vector<Object>&);	

	/* Compound procedure */
	struct {
		vector<string>	parameters;	/* Store parameters of "lambda" expression*/
		vector<string>	body;		/* Store body of "lambda" expression*/
		StaticEnv		static_env;	/* Store static variables */
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

	void set_car(const Object& val) { pir.first = val; }
	void set_cdr(const Object& val) { pir.second = val; }
private:
	pair<Object, Object> pir;
};

/* Scheme's list */
class List {
public:
	/* Constructor */
	List() : lst(list<Object>()) {}
	List(const vector<Object>& l) : lst(l.begin(), l.end()) {}
	~List() {}

	/* Others */
	Object car() { return lst.front(); }
#ifdef USE_LIST
	Object cdr() {
		auto it = lst.begin();
		return Object(vector<Object>(++it, lst.end())); 
	}
#endif
	bool empty() { return lst.empty(); }
	int size() { return lst.size(); }

	list<Object> lst;
};

#endif