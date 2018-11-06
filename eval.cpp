/* Implement of evaluator's functions */

#include "eval.h"

/* Declartion static env */
Environment Evaluator::env;

/* Constructor */
Evaluator::Evaluator() {
	if (env.empty())
		env.push_back(unordered_map<string, Object>());
	initialize_environment();
}

/* Evaluator start. */
void Evaluator::run_evaluator()
{
	/* Keep only the global environment */
	if (env.size() > 1)
		env.erase(env.begin() + 1, env.end()); 

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

/* Reset environment, restart evaluator then */
void Evaluator::reset_evaluator()
{
	initialize_environment();
	run_evaluator();
}

/* Evaluating a expression. */
Object Evaluator::eval(vector<string>& split)
{
	delete_ends_parentheses(split);
	if (split.empty())
		return Object();
	else if (split.size() == 1)
		return eval(split[0]);
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
					cerr << "ERROR: illegal expression -- eval()" << endl;
					run_evaluator();
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
void Evaluator::delete_ends_parentheses(vector<string>& split)
{
	if (split.empty())
		return;
	if (split[0] == "(") {
		split.pop_back();
		split.erase(split.begin());
	}
}

/* Evaluating a single object. */
Object Evaluator::eval(string& str)
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
			cout << "DEBUG eval(): " << env.size() << endl;
#endif
#if 0
			for (auto &e : env) {
				if (e.find(str) != e.end())
					return e[str];
			}
#endif
#if 1
			for (auto it = env.rbegin(); it != env.rend(); ++it) {
				if (it->find(str) != it->end())
					return (*it)[str];
			}
#endif
			cerr << "ERROR: unknown symbol: " << str << " -- eval()" << endl;
			run_evaluator();
		}
	}
}

/* Call proc with obs. */
Object Evaluator::apply_proc(Object &proc, vector<Object>& obs)
{
	if (proc.get_type() != PROCEDURE) {
		cerr << "ERROR: unknown procedure -- apply_proc()" << endl;
		run_evaluator();
	}
	shared_ptr<Procedure> op = proc.get_proc();
	if (op->get_type() == PRIMITIVE)
		return op->get_primitive()(obs);
	/* Compound procedure -- lambda procedure */
	else if (op->get_type() == COMPOUND)
		;/* to do */
}

void Evaluator::initialize_environment()
{
	env[0]["+"] = Object(Procedure(Primitive::add));
	env[0]["quit"] = Object(Procedure(Primitive::quit));
	env[0]["exit"] = Object(Procedure(Primitive::quit));
	env[0]["cons"] = Object(Procedure(Primitive::make_cons));
	env[0]["list"] = Object(Procedure(Primitive::make_list));
	env[0]["display"] = Object(Procedure(Primitive::display));
}