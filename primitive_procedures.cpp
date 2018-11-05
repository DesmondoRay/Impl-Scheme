/* Implement of primitive procedures of Scheme */

#include "primitive_procedures.h"

/* Quit */ /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
Object Primitive::quit(vector<Object>& obs)
{
	exit(0);
	return Object();
}

/* Return the sum of obs as an Object. */
Object Primitive::add(vector<Object>& obs)
{
	int sum_i = 0;
	double sum_f = 0.0;
	for (auto &ob : obs) {
		if (ob.get_type() == INTEGER)
			sum_i += ob.get_integer();
		else if (ob.get_type() == REAL)
			sum_f += ob.get_real();
		else {
			cerr << "ERROR: can't apply '+' to this type!" << endl;/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
			Primitive::quit(vector<Object>{});
		}
	}

	if (sum_f != 0.0)
		return Object(sum_i + sum_f);	// Result's type is REAL
	return Object(sum_i);				// Result's type is INTEGER
}

/* Return the pair of obs as an Object */
Object Primitive::make_cons(vector<Object>& obs) {
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
		shared_ptr<Cons> spc; /* Used to display Cons */
		switch (type) {
		case INTEGER:
			cout << ob.get_integer() << " ";
			break;
		case REAL:
			cout << ob.get_real() << " ";
			break;
		case BOOLEAN:
			cout << ob.get_boolean() << " ";
			break;
		case STRING:
			cout << ob.get_string() << " ";
			break;
		case PROCEDURE:
			if (ob.get_type() == PRIMITIVE)
				cout << "<primitive procedure>" << endl; /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
			else if (ob.get_type() == COMPOUND)
				cout << "<compound procedure>" << endl;
			else {
				cerr << "ERROR: unknown procedure!" << endl;
				Primitive::quit(vector<Object>{});
			}
			break;
		case CONS:
			spc = ob.get_cons();
			cout << "(";
			Primitive::display(vector<Object>{spc->car()});
			cout << ". ";
			Primitive::display(vector<Object>{spc->cdr()});
			// Backspace, example: (cons 1 2) print (1 . 2), instead of (1 . 2 )
			cout << "\b)";
			break;
		case LIST:
			cout << "(";
			for (auto &ob : ob.get_list()->lst)
				Primitive::display(vector<Object>{ob});
			cout << "\b)";	// backspace
			break;
		default: /* UNASSIGNED */
			cerr << "null";
		}
	}
	return Object();
}

/* New line */
Object Primitive::newline(vector<Object>& obs) {
	cout << endl;
	return Object();
}