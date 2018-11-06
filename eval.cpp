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
	envs[0]["quit"] = Object(Procedure(Primitive::quit));
	envs[0]["exit"] = Object(Procedure(Primitive::quit));
	envs[0]["cons"] = Object(Procedure(Primitive::make_cons));
	envs[0]["list"] = Object(Procedure(Primitive::make_list));
	envs[0]["display"] = Object(Procedure(Primitive::display));
}

/* Reset environment, restart evaluator then */
void reset_evaluator()
{
	initialize_environment();
	run_evaluator();
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
		run_evaluator();
	else {
		cout << "Bye! Press any key to quit." << endl;
		input = getchar();
		exit(0);
	}
}

/* Evaluator start. */
void run_evaluator()
{
	/* Keep only the global environment */
	if (envs.size() > 1)
		envs.erase(envs.begin() + 1, envs.end()); 

	while (true) {
		prompt();
		string input = get_input();
		if (input.empty()) 
			continue;

		vector<string> split = split_input(input);
		Object result = eval(split);
		print_result(result);
	}
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
		/* Eval operator -- proc */
		Object proc = eval(split[0]);
#ifndef NDEBUG
		cout << "DEBUG eval(): " << proc.get_type() << endl;
#endif
		/* Eval arguments -- args */
		vector<Object> args;
		for (int i = 1; i < split.size(); i++) {
			/* Eval a subexpression, example: */
			/* expression: "(display (cons 1 2))", sub_exp: "(cons 1 2)" */
			if (split[i] == "(") {
				auto it = find(split.begin() + i, split.end(), ")");
				if (it == split.end()) {
					string error_msg ("ERROR(scheme): illegal expression --");
					error_msg += " (";
					for (auto &s : split) error_msg += s + " ";
					error_msg += ")";
#ifndef NDEBUG
					error_msg += "\nDEBUG: Object eval(vector<string>& split)";
#endif
					error_handler(error_msg);
				}
				vector<string> sub_exp(split.begin() + i, it + 1);
				args.push_back(eval(sub_exp));
				i = it - split.begin() + 1;
			}

			/* Eval a single operand -- self-evaluating */
			else 
				args.push_back(eval(split[i]));
		}
		return apply_proc(proc, args);
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
		if (str.find('.') != str.size())
			return Object(stod(str));
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
		if (find(keywords.begin(), keywords.end(), str) != keywords.end())
			return Object(str, KEYWORD);
		else {
			/* Look for variables in each environment from the 
			 * last environment(local) to the first one(global) 
			 */
#ifndef NDEBUG
			cout << "DEBUG eval(): " << str << " " << envs.size() << endl;
#endif
#if 0
			for (auto &e : envs) {
				if (e.find(str) != e.end())
					return e[str];
			}
#endif
#if 1
			for (auto it = envs.rbegin(); it != envs.rend(); ++it) {
				if (it->find(str) != it->end())
					return (*it)[str];
			}
#endif
			string error_msg("ERROR(scheme): unknown symbol -- ");
			error_msg += str;
#ifndef NDEBUG
			error_msg += "\nDEBUG: Object eval(string& str)";
#endif
			error_handler(error_msg);
		}
	}
}

/* Call proc with obs. */
Object apply_proc(Object &proc, vector<Object>& obs)
{
	if (proc.get_type() != PROCEDURE) {
#ifndef NDEBUG
		cout << "DEBUG: Object apply_proc(Object &proc, vector<Object>& obs)\n";
#endif
		error_handler("ERROR: unknown procedure -- apply_proc()");
	}
	shared_ptr<Procedure> op = proc.get_proc();
	if (op->get_type() == PRIMITIVE)
		return op->get_primitive()(obs);
	/* Compound procedure -- lambda procedure */
	else if (op->get_type() == COMPOUND)
		;/* to do */
}