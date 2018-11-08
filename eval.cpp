/* Implement of evaluator's functions */

#include "eval.h"

/* Declartion of static environments */
/* envs[0]: global environment, envs[n]: static local environment, for example:
 * in the global environment: 
 * 1. "(define a 3)" --> envs[0]["a"] = 3, "a" is defined in the
 *	  global environment;
 * 2. "(define (func a b)
 *	     (define c 4)
 *       (+ a b c))"6
 *    --> envs[0]["func"] = <compound procedure: func>, envs[1]["c"] = 4,
 *    "c" is defined in the static local environment.
 * 3. when apply_proc() return, remove static local environment from envs.
 */
static Environment envs;

/* Initialize/reset the global environment. */
void initialize_environment()
{
	envs.clear();
	envs.push_back(SubEnv());

	envs[0]["number?"] = Object(Procedure(Primitive::is_number, "number?"));
	envs[0]["integer?"] = Object(Procedure(Primitive::is_integer, "integer?"));
	envs[0]["boolean?"] = Object(Procedure(Primitive::is_boolean, "boolean?"));
	envs[0]["real?"] = Object(Procedure(Primitive::is_real, "real?"));
	envs[0]["even?"] = Object(Procedure(Primitive::is_even, "even?"));
	envs[0]["odd?"] = Object(Procedure(Primitive::is_odd, "odd?"));
	envs[0]["pair?"] = Object(Procedure(Primitive::is_pair, "pair?"));
	envs[0]["null?"] = Object(Procedure(Primitive::is_null, "null?"));
	envs[0]["list?"] = Object(Procedure(Primitive::is_list, "list?"));
	envs[0]["+"] = Object(Procedure(Primitive::add, "+"));
	envs[0]["-"] = Object(Procedure(Primitive::sub, "-"));
	envs[0]["*"] = Object(Procedure(Primitive::mul, "*"));
	envs[0]["/"] = Object(Procedure(Primitive::div, "/"));
	envs[0]["remainder"] = Object(Procedure(Primitive::remainder, "remainder"));
	envs[0]["quotient"] = Object(Procedure(Primitive::quotient, "quotient"));
	envs[0]["<"] = Object(Procedure(Primitive::less, "<"));
	envs[0]["<="] = Object(Procedure(Primitive::lessEqual, "<="));
	envs[0][">"] = Object(Procedure(Primitive::greater, ">"));
	envs[0][">="] = Object(Procedure(Primitive::greaterEqual, ">="));
	envs[0]["abs"] = Object(Procedure(Primitive::abs, "abs"));
	envs[0]["square"] = Object(Procedure(Primitive::square, "square"));
	envs[0]["sqrt"] = Object(Procedure(Primitive::sqrt, "sqrt"));
	/* Note: = can take multiple arguments, "(= 1.0 1 1 1.0)" --> true */
	/* eq? and equal? only takes two arguments, "(eq? 1.0 1)" --> false */
	envs[0]["="] = Object(Procedure(Primitive::op_equal, "="));
	envs[0]["eq?"] = Object(Procedure(Primitive::equal, "eq?"));
	envs[0]["equal?"] = Object(Procedure(Primitive::equal, "eq?"));
	envs[0]["not"] = Object(Procedure(Primitive::not, "not?"));
	envs[0]["quit"] = Object(Procedure(Primitive::quit, "quit"));
	envs[0]["exit"] = Object(Procedure(Primitive::quit, "quit"));
	envs[0]["reset"] = Object(Procedure(Primitive::reset, "reset"));
	envs[0]["cons"] = Object(Procedure(Primitive::make_cons, "cons"));
	envs[0]["list"] = Object(Procedure(Primitive::make_list, "list"));
	envs[0]["display"] = Object(Procedure(Primitive::display, "display"));
	envs[0]["newline"] = Object(Procedure(Primitive::newline, "newline"));
	envs[0]["load"] = Object(Procedure(Primitive::load, "load"));
}

/* Reset environment, restart evaluator then */
void reset_evaluator()
{
	initialize_environment();
	run_evaluator(cin);
}

/* Handler error */
void error_handler(const string& msg)
{
	cerr << msg << endl;
	cout << "Input [Enter] or [Y/y] to continue and input others to quit: ";
	char input = getchar();
	/* Delete extra characters */
	if (input != '\n')
		while (getchar() != '\n') continue;
	if (input == '\n' || input == 'y' || input == 'Y')
		run_evaluator(cin);
	else {
		cout << "Bye! Press any key to quit." << endl;
		input = getchar();
		exit(0);
	}
}

/* Print Scheme prompt for input. */
static inline void prompt()
{
	cout << ">>> Eval input: " << endl;
}

/* Evaluator start. */
void run_evaluator(istream& in, int mode)
{
	/* Keep only the global environment */
	if (envs.size() > 1)
		envs.erase(envs.begin() + 1, envs.end()); 

	while (in.good()) {
		if (mode == 0) 
			prompt();
		string input = get_input(in);
		if (input.empty()) 
			continue;

		vector<string> split = split_input(input);
		/* Convert '(<exp1> ... <expn>) too (list <exp1> ... <expn>) */
		convert_to_list(split);

		Object result = eval(split);
		print_result(result, mode);
	}

	return;
}

/* split = {"(", "+", "1", "2", ")", "3", "4"} 
 * --> call get_subexp(split) will return {"(", "+", "1", "2", ")"};
 * --> then, call get_single(split) will return "3".
 */
/* Get subexpression */
static vector<string> get_subexp(vector<string>& split)
{
	int cntParantheses = 1, i = 1;
	for (; i < split.size() && cntParantheses != 0; i++) {
		split[i] == ")" ? cntParantheses-- : 1;
		split[i] == "(" ? cntParantheses++ : 1;
	}
	vector<string> result(split.begin(), split.begin() + i);
	split.erase(split.begin(), split.begin() + i);
	return result;
}
/* Get a single variable */
static string get_single(vector<string>& split)
{
	string result = split[0];
	split.erase(split.begin());
	return result;
}

/* Evaluating a expression. */
Object eval(vector<string>& split)
{
	if (split.empty())
		return Object();

	/* Check if expression is a procedure, for example:
	 * "(define (f) (+ 1 2))" --> "f" is a variable, "(f)" is a procedure;
	 * when we input "f", evaluator will print "<compound procedure>",
	 * and input "(f)", evaluator will return "3".
	 */
	int is_proc = 0;
	if (split[0] == "(") {
		is_proc = 1;
		delete_ends_parentheses(split);
	}
	/* However, "quit" and "exit" are special cases, 
	 * enter "quit" and "(quit)" will exit both.
	 */
	if (is_proc == 0 && split.size() == 1) {
		if (split[0] == "quit" || split[0] == "exit")
			Primitive::quit(vector<Object>{});
		return eval(split[0]); /* Evaluate a variable */
	}
	else {
		/* Evaluate operator */
		Object op;
		if (split[0] == "(") {
			/* "lambda" anonymous function, for example: 
			 * "((lambda a (+ a 3)) 3)", "(lambda a (+ a 3))" is a procedure, 
			 * and it's arguments is "3".
			 */
			vector<string> subexp = get_subexp(split);
			op = eval(subexp);
		}
		else {
			op = eval(get_single(split));
		}

#ifndef NDEBUG
		cout << "DEBUG eval(): op.type: " << op.get_type() << endl;
#endif
		/* If op is a keyword, goto eval_keywords() */
		if (op.get_type() == KEYWORD) {
			return eval_keyword(op.get_string(), split);
		}

		/* Else evaluate arguments of the procedure */
		vector<Object> args;
		while (!split.empty()) {
			/* Evaluate a subexpression, example: */
			/* "(display (* 1 2) (+ 3 4))" -> "(* 1 2)" is subexpreession */
			if (split[0] == "(") {
				args.push_back(eval(get_subexp(split)));
			}
			/* Self-evaluating, such as number, string, bool */
			else {
				args.push_back(eval(get_single(split)));
			}
		}
		return apply_proc(op, args);	/* Call op with args */
	}
}

/* Delete parentheses of two ends */
void delete_ends_parentheses(vector<string>& split)
{
	if (split.empty())
		return;
	if (split[0] == "(") {
		split.pop_back();
		split.erase(split.begin());
	}
}

/* Evaluating a single object. */
Object eval(string& str)
{
	/* NUMBER or REAL */
	if (isdigit(str[0]) || (str.size() > 1 && str[0] == '-' && isdigit(str[1]))) 
	{
		if (str.find('.') != string::npos) {
#ifndef NDEBUG
			cout << "DEBUG eval() NUMBER or REAL: " << str.find('.') << endl;
#endif
			return Object(stod(str));
		}
		else
			return Object(stoi(str));
	}
	/* BOOLEAN, true: '#t', false: '#f' */
	else if (str[0] == '#') {
		bool val = (str[1] == 't' ? true : false);
		return Object(val);
	}
	/* STRING */
	else if (str[0] == '"') {
		return Object(str);
	}
	/* KEYWORD or PROCEDURE(or variable) */
	/* example: "(define a 3)" --> variable: a, add "a" to envs, envs["a"] = 3 */
	else {
		if (find(keywords.begin(), keywords.end(), str) != keywords.end()) {
			return Object(str, KEYWORD);
		}
		else {
			/* Look for variables in each environment from the 
			 * last environment(local) to the first one(global) 
			 */
#ifndef NDEBUG
			cout << "DEBUG eval(): " << str << " " << envs.size() << endl;
#endif
			for (auto it = envs.rbegin(); it != envs.rend(); ++it) {
				if (it->find(str) != it->end())
					return (*it)[str];
			}
			string error_msg("ERROR(scheme): unknown symbol -- ");
			error_msg += str;
#ifndef NDEBUG
			error_msg += "\nDEBUG: Object eval(string& str)";
#endif
			error_handler(error_msg);
		}
	}
}

/* Add new_env(static environment of proc) to envs */
static inline void expand_env(unordered_map<string, Object>& new_env) {
	envs.push_back(new_env);
}

/* Remove a environment from envs */
static inline void remove_env(void) {
	if (envs.size() > 1)
		envs.pop_back();
}

/* Call proc with obs. */
Object apply_proc(Object &op, vector<Object>& obs)
{
	if (op.get_type() != PROCEDURE) {
#ifndef NDEBUG
		cout << "DEBUG: Object apply_proc(Object &proc, vector<Object>& obs)\n";
#endif
		error_handler("ERROR: unknown procedure -- apply_proc()");
	}
	/* Handler with procedures */
	shared_ptr<Procedure> proc = op.get_proc();
	/* Primitive procedure */
	if (proc->get_type() == PRIMITIVE)
		return proc->get_primitive()(obs);
	/* Compound procedure -- lambda procedure */
	else if (proc->get_type() == COMPOUND) {
		vector<string> parameters(proc->get_parameters());
		/* The number of parameters is not equal the number of arguments */
		if (parameters.size() != obs.size()) {
			string error_msg("ERROR(scheme): the procedure has been "
				"called with ");
			error_msg.push_back(obs.size() + '0');
			error_msg += " arguments, it requires exactly ";
			error_msg.push_back(parameters.size() + '0');
			error_msg += " arguments.";
			error_handler(error_msg);
		}

		/* Get static environment of proc */
		SubEnv proc_env = proc->get_env();
		for (int i = 0; i < parameters.size(); i++) {
			proc_env[parameters[i]] = obs[i]; /* Bind arguments to parameters */
		}

		expand_env(proc_env);
		/* Evaluating in a expanded environment */
		Object result = eval(proc->get_body());	
#if 1
		/* Update static environment of proc */
		proc_env = proc->get_env();
		for (auto &pair : proc_env) {
			proc->set_env(pair.first, envs.back()[pair.first]);
		}
#endif
		/* Remove static environment of proc from envs */
		remove_env();

		return result;
	}
}

/* Handle with keyword */
Object eval_keyword(const string& keyword, vector<string>& exp)
{
	/* Handler with keywords */
	auto it = find(keywords.begin(), keywords.end(), keyword);
	int keywords_index = it - keywords.begin();
	switch (keywords_index) {
	case (0): /* define expression */
		return eval_define(exp);
	case (1): /* if expression */
		return eval_if(exp);
	case (2): /* set expression */
		return eval_set(exp);
	case (3): /* lambda expression */
		return eval_lambda(exp);
	case (4): /* begin expression */
		return eval_begin(exp);
	case (5): /* let expression */
		return eval_let(exp);
	case (6): /* cond expression */
		return eval_cond(exp);
	default:
#ifndef NDEBUG
		cout << "DEBUG eval_keyword(): " << keyword << endl;
#endif
		error_handler(string("ERROR(runtime): unknown keyword -- ") + keyword);
	}
	// return Object(); /* Return null */
}

/* Handle with "define" expression, if define a compound procedure ,
 * convert to "lambda" expression, for example:
 * "(define (square x) (* x x))" --> exp: {"(square x)", "(* x x)"},
 * procedure name: "square", parameter: {"x"}, body: "(* x x)".
 * Convert to: "(define square (lambda (x) (* x x)))"
 */
Object eval_define(vector<string>& exp)
{
	if (exp.empty())
		error_handler(string("ERROR(scheme): illegal define expression"));
#ifndef NDEBUG
	cout << "DEBUG eval_define(): ";
	for (auto &s : exp)
		cout << s << " ";
	cout << endl;
#endif
	/* Add a new definition to current environment,
	 * or update a value of definition in current environment.
	 */
	Environment::iterator curr_env = envs.end() - 1;
	string ret;
	/* Define a procedure, convert to "lambda" expression */
	if (exp[0] == "(") {
		string proc_name = exp[1];
		ret = proc_name;
		/* delete procedure name, {(square x), (* x x)} --> {(x), (* x x)} */
		exp.erase(exp.begin() + 1); 
		(*curr_env)[proc_name] = eval_lambda(exp, proc_name);
	}
	/* Define a common variable, such as (define a 3);
	 * or define a procedure, such as (define square (lambda (x) (* x x))).
	 */
	else {
		string variable = exp[0];
		ret = variable;
		(*curr_env)[variable] =
			eval(vector<string>(exp.begin() + 1, exp.end()));
	}
#ifndef NDEBUG
	cout << "DEBUG eval_define(): define OK " << endl;
#endif
	return Object(ret);
}

/* Handle with "lambda" expression, for example:
 * "(lambda (x) (+ x 3))" --> parameters: {"x"}, body: "(+ x 3)",
 * Procedure constructor:
 *		Procedure(const vector<string>& params, const vector<string>& bdy);
 * construct a compound procedure, and return it as an Object.
 */
Object eval_lambda(vector<string>& exp, const string& proc_name)
{
	if (exp.empty() || exp[0] != "(")
		error_handler("ERROR(scheme): illegal lambda expression");

	/* Split exp into parameters and body */
	vector<string> parameters;
	int i = 1;
	for (; i < exp.size() && exp[i] != ")"; i++) {
		parameters.push_back(exp[i]);
	}

	/* Make the body a begin expression:"(begin <exp1> <exp2> ... <expn> )" */
	vector<string> body{ "(", "begin", ")" };
	body.insert(body.begin() + 2, exp.begin() + i + 1, exp.end());

	/* Construct a compound procedure */
	/* Note!!!: To store the values of local variables, the evaluator will 
	 * establish an environment that holds these values, and store it at
	 * class Procedure::static_env.
	 * This implementation is not good, but it does work, 
	 * using shared_ptr might be a better choice.
	 * You can read SICP chapter 3.1 on page 152(Chinese version) or 
	 * 300(English version) if you want.
	 */
#if 1
	/* No need to save global environment */
	if (envs.size() == 1)
		return Object(Procedure(parameters, body, proc_name));
	else
		return Object(Procedure(parameters, body, proc_name, envs.back()));
#else
	return Object(Procedure(parameters, body, proc_name));
#endif
}

/* Return true if object is some kinds of "true",
 * Note: In Scheme, only "#f" is false, everything else is true. :) hopefully
 */
bool is_true(Object& ob)
{
	if (ob.get_type() == BOOLEAN)
		return ob.get_boolean();
	return true;
}

/* Handler with "if" expression, for example: 
 * "(if (> a 2) (+ a 3) (- a 1))" --> exp: "(> a 2) (+ a 3) (- a 1)",
 * predicate: "(> a 2)", consequent: "(+ a 3)", alternative: "(- a 1)".
 */
Object eval_if(vector<string>& exp) {
	Object predicate, consequent, alternative;
	/* Evaluate predicate */
	if (exp.empty())
		error_handler("ERROR(scheme): Ill-formed special -- if");
	else if (exp[0] == "(") {
		predicate = eval(get_subexp(exp));
	}
	else {
		predicate = eval(get_single(exp));
	}

	if (exp.empty())
		error_handler("ERROR(scheme): Ill-formed special -- if");

	/* If predicate is true, return consequent */
	if (is_true(predicate)) {
		if (exp[0] == "(")
			return eval(get_subexp(exp));
		else {
			return eval(get_single(exp));
		}
	}
	/* Else return alternative */
	else {
		/* Skip consequent */
		if (exp[0] == "(")
			get_subexp(exp);
		else
			get_single(exp);

		if (exp.empty())	/* Alternative could be empty */
			return Object();
		else if (exp[0] == "(")
			return eval(get_subexp(exp));
		else
			return eval(exp[0]);
	}
}

/* Handler with "begin" expression, for example:
 * exp == "(begin (+ 1 2) (+ 3 4))";
 * First evaluate "(+ 1 2)", and then evaluate "(+ 3 4)", 
 * and return the last subexpression as the result.
 */
Object eval_begin(vector<string>& exp)
{
	Object result;
	while (!exp.empty()) { /* Exp may be empty */
		if (exp[0] == "(")
			result = eval(get_subexp(exp));
		else
			result = eval(get_single(exp));
	}

	return result;
}

/* Handler with "let" expression, convert "let" expression to "lambda" 
 * expression, for example:
 * (let ((<var1> <exp1>) ... (<var_n><exp_n>)) <body>)
 * --> ((lambda (<var1> ... <var_n>) <body>) <exp1> ... <exp_n>)
 */
Object eval_let(vector<string>& exp)
{
	if (exp.empty())
		error_handler("ERROR(scheme): ill-formed special from: let");
	
	/* Split exp into vars, exps and body */
	vector<string> pairs_of_vars_and_exps = get_subexp(exp);
	vector<string> body = exp;

	delete_ends_parentheses(pairs_of_vars_and_exps);
	vector<string> vars, exps;
	while (!pairs_of_vars_and_exps.empty()) {
		/* one_pair: (<var> <exp>) */
		vector<string> one_pair = get_subexp(pairs_of_vars_and_exps); 
		vars.push_back(one_pair[1]);
		exps.push_back(one_pair[2]);
	}

	/* Convert to "lambda" expression */
	vector<string> lambda_exp{ "(", "(", "lambda", "(" };
	lambda_exp.insert(lambda_exp.end(), vars.begin(), vars.end());
	lambda_exp.push_back(")");
	lambda_exp.insert(lambda_exp.end(), body.begin(), body.end());
	lambda_exp.push_back(")");
	lambda_exp.insert(lambda_exp.end(), exps.begin(), exps.end());
	lambda_exp.push_back(")");
	
	/* Evaluate "lambda" expression */
	return eval(lambda_exp);
}

/* Handler with "cond" expression, for example:
 * (cond ((> x 0) x)
 *		 ((= x 0) (display 'zero) 0)
 *		 (else (- x)))
 * In this example, "(> x 0)" and "(= x 0)" are predicates.
 */
Object eval_cond(vector<string>& exp)
{
	if (exp.empty())
		error_handler("ERROR(scheme): ill-formed special -- cond");

	/* If the first sub exp is "else" expression */
	while (!exp.empty()) {
		/* Get first clause */
		vector<string> first_clause = get_subexp(exp);
		delete_ends_parentheses(first_clause);

		if (first_clause[0] == "else") {
			if (exp.empty())
				/* Delete "else" */
				first_clause.erase(first_clause.begin());
			else
				/* There are exps after "else" expression */
				error_handler("ERROR(scheme): else clause isn't last -- cond");
		}
		else {
			/* Check if predicate is true */
			Object predicate;
			if (first_clause[0] == "(")
				predicate = eval(get_subexp(first_clause));
			else
				predicate = eval(get_single(first_clause));

			/* If predicate is false, evaluate next predicate */
			if (!is_true(predicate))
				continue;
			/* Else goto following step */
		}

		/* Make a "begin" expression */
		vector<string> begin_exp{ "(", "begin", ")" };
		begin_exp.insert(begin_exp.begin() + 2,
			first_clause.begin(), first_clause.end());
		return eval(begin_exp);
	}

	/* If there is no "else" expression and no predicate is true, return null */
	return Object();
}

/* Handler with "set" expression, for example: 
 * "(set! <var> <exp>)" 
 * --> add <var> to current environment, or update it's value.
 */
Object eval_set(vector<string>& exp)
{
	if (exp.empty()) {
		error_handler("ERROR(scheme): ill-formed special form -- set!");
	}
	if (exp[0] == "(") {
		error_handler("ERROR(scheme): variable required, usage: " 
			"(set! var value) -- set!");
	}
	string variable = get_single(exp);

	Object ret = eval(exp);
#if 1
	Environment::iterator curr_env = envs.end() - 1;
	(*curr_env)[variable] = ret;
#endif

	return ret;
}