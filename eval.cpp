/* Implement of evaluator's functions */

#include "eval.h"

/* Declartion static env */
/* env[0]: global environment, env[n]: local environment, for example:
* 1. "(define a 3)" --> env[0]["a"] = 3, "a" is defined in the
*	  global environment;
* 2. "(define (func a b)
*	     (define c 4)
*       (+ a b c))"
*    --> env[0]["func"] = <...compound procedure>, env[1]["c"] = 4,
*    "c" is defined in the local environment.
*/
static Environment envs;

/* Initialize/reset the global environment. */
void initialize_environment()
{
	envs.clear();
	envs.push_back(unordered_map<string, Object>());

	envs[0]["+"] = Object(Procedure(Primitive::add));
	envs[0]["-"] = Object(Procedure(Primitive::sub));
	envs[0]["*"] = Object(Procedure(Primitive::mul));
	envs[0]["/"] = Object(Procedure(Primitive::div));
	envs[0]["<"] = Object(Procedure(Primitive::less));
	envs[0][">"] = Object(Procedure(Primitive::greater));
	/* Note: = can take multiple arguments; */
	/* eq? and equal? only takes two arguments, I'm not going to re-implement it. */
	envs[0]["="] = Object(Procedure(Primitive::equal));
	envs[0]["eq?"] = Object(Procedure(Primitive::equal));
	envs[0]["equal?"] = Object(Procedure(Primitive::equal));
	envs[0]["quit"] = Object(Procedure(Primitive::quit));
	envs[0]["exit"] = Object(Procedure(Primitive::quit));
	envs[0]["cons"] = Object(Procedure(Primitive::make_cons));
	envs[0]["list"] = Object(Procedure(Primitive::make_list));
	envs[0]["display"] = Object(Procedure(Primitive::display));
	envs[0]["load"] = Object(Procedure(Primitive::load));
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
		Object result = eval(split);
		print_result(result, mode);
	}

	return;
}

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

/* Evaluating a expression. */
Object eval(vector<string>& split)
{
	delete_ends_parentheses(split);
	if (split.empty())
		return Object();
	else if (split.size() == 1) {
		if (split[0] == "quit" || split[0] == "exit")
			Primitive::quit(vector<Object>{});
		return eval(split[0]);
	}
	else {
		/* Evaluate operator */
		Object op;
		if (split[0] == "(") {
			/* "lambda" anonymous function, for example: 
			 * "((lambda a (+ a 3)) 3)", "(lambda a (+ a 3)" is a procedure, 
			 * and it's arguments is "3".
			 */
			vector<string> subexp = get_subexp(split);
			op = eval(subexp);
		}
		else {
			op = eval(split[0]);
			split.erase(split.begin());
		}

#ifndef NDEBUG
		cout << "DEBUG eval(): op.type: " << op.get_type() << endl;
#endif
		/* If op is a keyword, goto eval_keywords() */
		if (op.get_type() == KEYWORD) {
			return eval_keyword(op.get_string(), split);
		}

		/* Else evaluate procedure and it's arguments */
		vector<Object> args;
		while (!split.empty()) {
			/* Evaluate a subexpression, example: */
			/* expression: "(display (cons 1 2))", sub_exp: "(cons 1 2)" */
			if (split[0] == "(") {
				args.push_back(eval(get_subexp(split)));
			}
			/* Self-evaluating, such as number, string, bool */
			else {
				args.push_back(eval(split[0]));
				split.erase(split.begin());
			}
		}
		return apply_proc(op, args);
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
	if (isdigit(str[0])) {
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
	/* example: "(define a 3)" --> variable: a, env["a"] = 3 */
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

/* Add new_env(local environment) to envs */
static inline void expand_env(unordered_map<string, Object>& new_env) {
	envs.push_back(new_env);
}

/* Remove a local environment from envs */
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
	if (proc->get_type() == PRIMITIVE)
		return proc->get_primitive()(obs);
	/* Compound procedure -- lambda procedure */
	else if (proc->get_type() == COMPOUND) {
		vector<string> parameters(proc->get_parameters());
		if (parameters.size() != obs.size()) {
			string error_msg("ERROR(scheme): the procedure has been called with ");
			error_msg.push_back(obs.size() + '0');
			error_msg += " arguments, it requires exactly ";
			error_msg.push_back(parameters.size() + '0');
			error_msg += " arguments.";
			error_handler(error_msg);
		}

		/* Make a new environment to evaluate compound procedure */
		unordered_map<string, Object> new_env;
		for (int i = 0; i < parameters.size(); i++) {
			new_env[parameters[i]] = obs[i]; /* Bind arguments to parameters */
		}
		expand_env(new_env);
		/* Evaluating in a expanded environment */
		Object result = eval(proc->get_body());	
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
	case (3): /* lambda expression */
		return eval_lambda(exp);
	case (4): /* begin expression */
	case (5): /* let expression */
	case (6): /* cond expression */
	case (7): /* else expression */
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
 * "(define (square x) (* x x))" --> args: {"(square x)", "(* x x)"},
 * "square": variable, "x": parameter, "(* x x)": body.
 * Convert to: (define square (lambda (x) (* x x)))
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
	/* Define a procedure, convert to "lambda" expression */
	if (exp[0] == "(") {
		string variable = exp[1];
		/* delete variable, {(square x), (* x x)} --> {(x), (* x x)} */
		exp.erase(exp.begin() + 1); 
		(*curr_env)[variable] = eval_lambda(exp);
	}
	/* Define a common variable, such as (define a 3);
	 * or define a procedure, such as (define square (lambda (x) (* x x))).
	 */
	else {
		string variable = exp[0];
		(*curr_env)[variable] =
			eval(vector<string>(exp.begin() + 1, exp.end()));
	}
#ifndef NDEBUG
	cout << "DEBUG eval_define(): define OK " << endl;
#endif
	return Object("define OK.");
}

/* Handle with "define" expression, for example:
 * "(lambda (x) (+ x 3))" --> Procedure({"x"}, {"(", "+", "x", "3", ")"}),
 * construct a compound procedure, and return it as an Object.
 */
Object eval_lambda(vector<string>& exp)
{
	if (exp.empty() || exp[0] != "(")
		error_handler("ERROR(scheme): illegal lambda expression");

	/* Split exp into parameters and body */
	vector<string> parameters;
	int i = 1;
	for (; i < exp.size() && exp[i] != ")"; i++) {
		parameters.push_back(exp[i]);
	}
	vector<string> body(exp.begin() + i + 1, exp.end());

	/* Construct a compound procedure */
	Procedure proc(parameters, body);
	return Object(proc);
}

/* Return true if object is some kinds of "true",
 * Note: In Scheme, only "#f" is false, everything else is true. :) hopefully
 */
static bool is_true(Object& ob)
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
		error_handler("");
	else if (exp[0] == "(") {
		predicate = eval(get_subexp(exp));
	}
	else {
		predicate = eval(exp[0]);
		exp.erase(exp.begin());
	}

	if (exp.empty())
		error_handler("ERROR(scheme): Ill-formed special -- if");

	/* If predicate is true, return consequent */
	if (is_true(predicate)) {
		if (exp[0] == "(")
			return eval(get_subexp(exp));
		else {
			return eval(exp[0]);
			exp.erase(exp.begin());
		}
	}
	else {
		/* Skip consequent */
		if (exp[0] == "(")
			get_subexp(exp);
		else
			exp.erase(exp.begin());

		if (exp.empty())	/* Alternative could be empty */
			return Object();
		else if (exp[0] == "(")
			return eval(get_subexp(exp));
		else
			return eval(exp[0]);
	}
}