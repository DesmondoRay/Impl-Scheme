/* Implement of primitive procedures of Scheme */

#include "primitive_procedures.h"
#include "eval.h"

/* Quit */
Object Primitive::quit(vector<Object>& obs)
{
	cout << "Bye! Press any key to quit." << endl;
	char input = getchar();
	exit(0);
}

/* Reset Evaluator, initialize environment */
Object Primitive::reset(vector<Object>& obs)
{
	reset_evaluator();
	return Object();
}

/* Return #t(true) if object is a number */
Object Primitive::is_number(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(schem): requires exactly 1 argument -- number?");

	bool ret = obs[0].get_type() == INTEGER || obs[0].get_type() == REAL;
	return Object(ret);
}

/* Return #t(true) if object is a boolean */
Object Primitive::is_boolean(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(schem): requires exactly 1 argument -- boolean?");

	bool ret = obs[0].get_type() == BOOLEAN;
	return Object(ret);
}

/* Return #t(true) if object is a integer */
Object Primitive::is_integer(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(schem): requires exactly 1 argument -- integer?");

	bool ret = obs[0].get_type() == INTEGER;
	return Object(ret);
}

/* Return #t(true) if object is a real */
Object Primitive::is_real(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(schem): requires exactly 1 argument -- real?");

	bool ret = obs[0].get_type() == REAL;
	return Object(ret);
}

/* Return #t(true) if object is a even integer */
Object Primitive::is_even(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(schem): requires exactly 1 argument -- even?");
	if (!is_true(is_integer(vector<Object>{obs[0]})))
		error_handler("ERROR(scheme): passed a incorrect type to even?");

	bool ret = obs[0].get_integer() % 2 == 0;
	return Object(ret);
}

/* Return #t(true) if object is a odd integer */
Object Primitive::is_odd(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(schem): requires exactly 1 argument -- odd?");
	if (!is_true(is_integer(vector<Object>{obs[0]})))
		error_handler("ERROR(scheme): passed a incorrect type to odd?");

	bool ret = (obs[0].get_integer() % 2 == 1 || 
		obs[0].get_integer() % 2 == -1);
	return Object(ret);
}

/* Return #t(true) if object is a pair(or list) */
Object Primitive::is_pair(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(schem): requires exactly 1 argument -- pair?");
	
	bool ret = (obs[0].get_type() == CONS || obs[0].get_type() == LIST);
	return Object(ret);
}

/* Return #t(true) if object is a empty list */
Object Primitive::is_null(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(schem): requires exactly 1 argument -- null?");

	bool ret = (obs[0].get_type() == LIST && obs[0].get_list()->empty());
	return Object(ret);
}

/* Return #t(true) if object is a list */
Object Primitive::is_list(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(schem): requires exactly 1 argument -- list?");

	bool ret = (obs[0].get_type() == LIST);
	return Object(ret);
}

/* Return the sum of obs. */
Object Primitive::add(vector<Object>& obs)
{
	int64_t sum_i = 0;
	double sum_f = 0.0;
	for (auto &ob : obs) {
		if (ob.get_type() == INTEGER)
			sum_i += ob.get_integer();
		else if (ob.get_type() == REAL)
			sum_f += ob.get_real();
		else {
			string error_msg("ERROR(scheme): can't apply '+' to the type -- ");
			error_msg += ob.get_type_str();
			error_handler(error_msg);
		}
	}

	if (sum_f != 0.0)
		return Object(sum_i + sum_f);	// Result's type is REAL
	if (sum_i > INT_MAX || sum_i < INT_MIN)
		cerr << "WARING: overflow -- +, " << sum_i
		<< " --> " << static_cast<int>(sum_i) << endl;
	return Object(static_cast<int>(sum_i));	// Result's type is INTEGER
}

/* Return the difference of obs */
Object Primitive::sub(vector<Object>& obs)
{
	if (obs.empty()) return Object(0);

	int diff_i = (obs[0].get_type() == INTEGER ? obs[0].get_integer() : 0);
	double diff_f = (obs[0].get_type() == REAL ? obs[0].get_real() : 0.0);
	for (int i = 1; i < obs.size(); i++) {
		Object &ob = obs[i];
		if (ob.get_type() == INTEGER)
			diff_i -= ob.get_integer();
		else if (ob.get_type() == REAL)
			diff_f -= ob.get_real();
		else {
			string error_msg("ERROR(scheme): can't apply '-' to the type -- ");
			error_msg += ob.get_type_str();
			error_handler(error_msg);
		}
	}

	if (diff_f != 0.0)
		return Object(diff_f + diff_i);	// Result's type is REAL
	return Object(diff_i);				// Result's type is INTEGER
}

/* Return the product of obs */
Object Primitive::mul(vector<Object>& obs)
{
	int64_t pro_i = 1;
	double pro_f = 1.0;
	for (auto &ob : obs) {
		if (ob.get_type() == INTEGER)
			pro_i *= ob.get_integer();
		else if (ob.get_type() == REAL)
			pro_f *= ob.get_real();
		else {
			string error_msg("ERROR(scheme): can't apply '*' to the type -- ");
			error_msg += ob.get_type_str();
			error_handler(error_msg);
		}
	}

	if (pro_f != 1.0)
		return Object(pro_f * pro_i);	// Result's type is REAL
	if (pro_i > INT_MAX || pro_i < INT_MIN)
		cerr << "WARING: overflow -- *, " << pro_i 
			 << " --> " << static_cast<int>(pro_i) << endl;
	return Object(static_cast<int>(pro_i));	// Result's type is INTEGER
}

/* Return the quotient of obs */
Object Primitive::div(vector<Object>& obs)
{
	if (obs.empty()) return Object(0);

	double quotient = (obs[0].get_type() == REAL ? obs[0].get_real() :
		static_cast<double>(obs[0].get_integer()));

	for (int i = 1; i < obs.size(); i++) {
		Object &ob = obs[i];
		if (ob.get_type() == INTEGER) {
			if (ob.get_integer() == 0)
				error_handler("ERROR(scheme): division by zero");
			quotient /= ob.get_integer();
		}
		else if (ob.get_type() == REAL) {
			if (ob.get_real() == 0.0)
				error_handler("ERROR(scheme): floating-point divide by zero");
			quotient /= ob.get_real();
		}
		else {
			string error_msg("ERROR(scheme): can't apply '/' to the type -- ");
			error_msg += ob.get_type_str();
			error_handler(error_msg);
		}
	}

	return Object(quotient);	// Result's type is REAL
}

/* Return remainder */
Object Primitive::remainder(vector<Object>& obs) 
{
	if (obs.size() != 2) {
		error_handler("ERROR(scheme): requires exactly 2 arguments -- remainder");
	}

	if (obs[0].get_type() != INTEGER || obs[1].get_type() != INTEGER) {
		string wrong_type = (obs[0].get_type() == INTEGER ?
			obs[1].get_type_str() : obs[0].get_type_str());
		string error_msg("ERROR(scheme): passed a ");
		error_msg += wrong_type;
		error_msg += " to remainder, it only takes integer.";
		error_handler(error_msg);
	}

	return Object(obs[0].get_integer() % obs[1].get_integer());
}

/* Return quotient */
Object Primitive::quotient(vector<Object>& obs)
{
	if (obs.size() != 2) {
		error_handler("ERROR(scheme): requires exactly 2 arguments -- quotient");
	}
	if (!is_true(is_integer(vector<Object>{obs[0]})) ||
		!is_true(is_integer(vector<Object>{obs[1]})))
		error_handler("ERROR(scheme): passed a incorrect type to quotient");

	int ret = obs[0].get_integer() / obs[1].get_integer();
	return Object(ret);
}

/* Return absolute value */
Object Primitive::abs(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(scheme): requires exactly 1 argument -- abs");
	if (!is_true(is_number(vector<Object>{obs[0]})))
		error_handler("ERROR(scheme): passed a incorrect type to abs");

	if (is_true(less(vector<Object>{ obs[0], Object(0) })))
		return sub(vector<Object>{ Object(0), obs[0] });
	else
		return obs[0];
}

/* Return the square of object */
Object Primitive::square(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(scheme): requires exactly 1 argument -- square");
	if (!is_true(is_number(vector<Object>{obs[0]})))
		error_handler("ERROR(scheme): passed a incorrect type to square");

	return mul(vector<Object>{obs[0], obs[0]});
}

/* Return the sqrt of object */
Object Primitive::sqrt(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(scheme): requires exactly 1 argument -- sqrt");
	if (!is_true(is_number(vector<Object>{obs[0]})))
		error_handler("ERROR(scheme): passed a incorrect type to sqrt");

	double tmp = (obs[0].get_type() == INTEGER ? 
		static_cast<double>(obs[0].get_integer()) : obs[0].get_real());

	if (tmp < 0)
		error_handler("ERROR(scheme): the evaluator does not support calling "
			"sqrt on negative numbers.");

	return Object(std::sqrt(tmp));
}


/* Return the true if obs[0] < obs[1] < obs[2] < ... < obs[n] */
Object Primitive::less(vector<Object>& obs)
{
	bool result = true;
	for (int i = 0; i < obs.size() - 1; i++) {
		result &= (obs[i] < obs[i + 1]);
	}
	return Object(result);
}

/* Return the true if obs[0] > obs[1] > obs[2] > ... > obs[n] */
Object Primitive::greater(vector<Object>& obs)
{
	bool result = true;
	for (int i = 0; i < obs.size() - 1; i++) {
		result &= (obs[i] > obs[i + 1]);
	}
	return Object(result);
}

/* Return true if obs[0] == obs[1] == obs[2] == ... == obs[n] */
/* arguments must be numbers */
Object Primitive::op_equal(vector<Object>& obs) {
	if (obs.size() < 2) {
		error_handler("ERROR(scheme): = takes at least two arguments");
	}

	bool result = true;
	for (int i = 0; i < obs.size() - 1; i++) {
		result &= (obs[i].operator_inner(obs[i + 1], "=="));
	}
	return Object(result);
}

/* Return true if obs[0] >= obs[1] >= obs[2] >= ... >= obs[n] */
Object Primitive::greaterEqual(vector<Object>& obs)
{
	if (obs.size() < 2) {
		error_handler("ERROR(scheme): >= takes at least two arguments");
	}

	bool result = true;
	for (int i = 0; i < obs.size() - 1; i++) {
		result &= (obs[i].operator>(obs[i + 1]) ||
			obs[i].operator_inner(obs[i + 1], "=="));
	}
	return Object(result);
}

/* Return true if obs[0] <= obs[1] <= obs[2] <= ... <= obs[n] */
Object Primitive::lessEqual(vector<Object>& obs)
{
	if (obs.size() < 2) {
		error_handler("ERROR(scheme): <= takes at least two arguments");
	}

	bool result = true;
	for (int i = 0; i < obs.size() - 1; i++) {
		result &= (obs[i].operator<(obs[i + 1]) ||
			obs[i].operator_inner(obs[i + 1], "=="));
	}
	return Object(result);
}


/* Return the minimum object of obs */
Object Primitive::min(vector<Object>& obs)
{
	if (obs.empty())
		error_handler("ERROR(scheme): min requires at least 1 argument");

	Object ret = obs[0];
	for (int i = 0; i < obs.size(); i++) {
		if (is_true(Primitive::greater(vector<Object>{ret, obs[i]})))
			ret = obs[i];
	}

	return ret;
}

/* Return the maximum object of obs */
Object Primitive::max(vector<Object>& obs)
{
	if (obs.empty())
		error_handler("ERROR(scheme): min requires at least 1 argument");

	Object ret = obs[0];
	for (int i = 0; i < obs.size(); i++) {
		if (is_true(Primitive::less(vector<Object>{ret, obs[i]})))
			ret = obs[i];
	}

	return ret;
}

/* Return true if obs[0] equal obs[1] equal obs[2] equal .. equal obs[n]*/
Object Primitive::equal(vector<Object>& obs) {
	if (obs.size() != 2) {
		error_handler("ERROR(scheme): eq? and eqaul? take two aurgument");
	}

	bool result = true;
	for (int i = 0; i < obs.size() - 1; i++) {
		result &= (obs[i] == obs[i + 1]);
	}
	return Object(result);
}

/* Operator! */
Object Primitive::not(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(scheme): requires exactly 1 argument -- not");

	if (obs[0].get_type() == BOOLEAN && obs[0].get_boolean() == false)
		return Object(true);
	else
		return Object(false);
}

/* Return the pair of obs as an Object */
Object Primitive::make_cons(vector<Object>& obs) 
{
	return Object(Cons(obs));
}

/* Return the list of obs as an Object */
Object Primitive::make_list(vector<Object>& obs)
{
	return Object(List(obs));
}

/* Print obs */
Object Primitive::display(vector<Object>& obs)
{
	for (auto &ob : obs) {
		int type = ob.get_type();
#if 0
		cout << "DEBUG display(): type of ob -- "<< ob.get_type_str() << endl;
#endif
		shared_ptr<Cons> spc;	/* Used to display Cons */
		int proc_type;			/* Used to display procedure */
		string proc_name;		/* Used to display procedure */
		switch (type) {
		case UNASSIGNED:
			cerr << "*Unspecified return value*";
			break;
		case INTEGER:
			cout << ob.get_integer() << " ";
			break;
		case REAL:
			cout << ob.get_real() << " ";
			break;
		case BOOLEAN:
			cout << (ob.get_boolean() ? "true" : "false") << " ";
			break;
		case STRING:
			cout << ob.get_string() << " ";
			break;
		case PROCEDURE:
			proc_type = ob.get_proc()->get_type();
			proc_name = ob.get_proc()->get_proc_name();
			if (proc_type == PRIMITIVE)
				cout << "<primitive procedure: " << proc_name << ">";
			else if (proc_type == COMPOUND)
				cout << "<compound procedure: " << proc_name << ">";
			else
				error_handler("ERROR(scheme): unknown procedure -- display");
			break;
		case CONS:
			spc = ob.get_cons();
			cout << "(";
			Primitive::display(vector<Object>{spc->car()});
			cout << ". ";
			Primitive::display(vector<Object>{spc->cdr()});
			/* Backspace, (cons 1 2) print (1 . 2) then, instead of (1 . 2 )*/
			cout << "\b) ";
			break;
		case LIST:
			cout << "(";
			for (auto &ob : ob.get_list()->lst)
				Primitive::display(vector<Object>{ob});
			/* Backspace, (list 1 2 3) print (1 2 3), instead of (1 2 3 ) */
			cout << "\b) ";
			break;
		default: 
			error_handler("ERROR(scheme): unknown type -- display");
		}
	}
	return Object();
}

/* New line */
Object Primitive::newline(vector<Object>& obs) {
	cout << endl;
	return Object();
}

/* Load code from file and evaluate */
Object Primitive::load(vector<Object>& obs)
{
	if (obs.empty())
		error_handler(string("ERROR(scheme): need a file name -- load\n") +
			"Usage: load \"path/name\" ");
	if (obs[0].get_type() != STRING) {
		error_handler(string("ERROR(scheme): unknown file type/name -- load\n")
			+ "Usage: load \"path/name\" ");
	}

	string filename = obs[0].get_string();
	ifstream ifile(filename.substr(1, filename.size() - 2), ifstream::in);
	if (!ifile) {
		error_handler(string("ERROR(scheme): can't open this file -- ") +
			obs[0].get_string());
	}

	static int tab = 0; /* Used to print loading information. */

	if (tab == 0) cout << ">>> ";
	cout << string((tab++) * 4, ' ') << "Loading " 
		 << filename.substr(1, filename.size() - 2) << endl;

#ifdef NDEBUG
	run_evaluator(ifile, 1);
#else
	run_evaluator(ifile, 2);
#endif

	if (--tab == 0)
		cout << ">>> Loading completed!" << endl << endl;
	return Object();
}


/* Return the car of object */
Object Primitive::car(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(scheme): requires exactly 1 argument -- car");
	if (obs[0].get_type() != CONS && obs[0].get_type() != LIST)
		error_handler("ERROR(scheme): passed an incorrect type to car");
	if (is_true(is_null(vector<Object>{ obs[0] })))
		error_handler("ERROR(scheme): passed an empty list to car");

	if (obs[0].get_type() == CONS)
		return obs[0].get_cons()->car();
	else
		return obs[0].get_list()->car();
}

/* Return the cdr of object */
Object Primitive::cdr(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(scheme): requires exactly 1 argument -- cdr");
	if (obs[0].get_type() != CONS && obs[0].get_type() != LIST)
		error_handler("ERROR(scheme): passed an incorrect type to cdr");
	if (is_true(is_null(vector<Object>{ obs[0] })))
		error_handler("ERROR(scheme): passed an empty list to cdr");

	if (obs[0].get_type() == CONS)
		return obs[0].get_cons()->cdr();
	else
		return obs[0].get_list()->cdr();
}

/* Return the caar of object */
Object Primitive::caar(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(scheme): requires exactly 1 argument -- caar");
	if (obs[0].get_type() != CONS && obs[0].get_type() != LIST)
		error_handler("ERROR(scheme): passed an incorrect type to caar");
	if (is_true(is_null(vector<Object>{ obs[0] })))
		error_handler("ERROR(scheme): passed an empty list to caar");

	return car(vector<Object>{ car(obs) });
}

/* Return the cadr of object */
Object Primitive::cadr(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(scheme): requires exactly 1 argument -- cadr");
	if (obs[0].get_type() != CONS && obs[0].get_type() != LIST)
		error_handler("ERROR(scheme): passed an incorrect type to cadr");
	if (is_true(is_null(vector<Object>{ obs[0] })))
		error_handler("ERROR(scheme): passed an empty list to cadr");

	return car(vector<Object>{ cdr(obs) });
}

/* Return the cdar of object */
Object Primitive::cdar(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(scheme): requires exactly 1 argument -- cdar");
	if (obs[0].get_type() != CONS && obs[0].get_type() != LIST)
		error_handler("ERROR(scheme): passed an incorrect type to cdar");
	if (is_true(is_null(vector<Object>{ obs[0] })))
		error_handler("ERROR(scheme): passed an empty list to cdar");

	return cdr(vector<Object>{ car(obs) });
}

/* Return the cddr of object */
Object Primitive::cddr(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(scheme): requires exactly 1 argument -- cddr");
	if (obs[0].get_type() != CONS && obs[0].get_type() != LIST)
		error_handler("ERROR(scheme): passed an incorrect type to cddr");
	if (is_true(is_null(vector<Object>{ obs[0] })))
		error_handler("ERROR(scheme): passed an empty list to cddr");

	return cdr(vector<Object>{ cdr(obs) });
}

/* Append objects or lists to obs[0] */
Object Primitive::append(vector<Object>& obs)
{
	if (obs.empty())
		return Object(List());

	vector<Object> new_list;
	for (auto &ob : obs) {
		if (ob.get_type() != LIST)
			new_list.push_back(ob);
		else {
			for (auto &o : ob.get_list()->lst)
				new_list.push_back(o);
		}
	}

	return Object(List(new_list));
}


/* Return length of obs[0] */
Object Primitive::length(vector<Object>& obs)
{
	if (obs.size() != 1)
		error_handler("ERROR(scheme): requires exactly 1 argument -- length");
	if (obs[0].get_type() != LIST)
		error_handler("ERROR(scheme): passed an argument to length, "
			"is not a list");

	return Object(obs[0].get_list()->size());
}