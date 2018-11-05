/* Implement of evaluator's functions */

#include "eval.h"

static vector<string> keywords{ 
	"define", "if", "set", "lambda", "begin", "let", "cond", "else" };

/* Evaluator start. */
void run_evaluator(Environment& env)
{
	while (true) {
		prompt();
		string input = get_input();
		if (input.empty()) 
			continue;

		vector<string> split = split_input(input);
		Object result = eval(split, env);
		print_result(result);
	}
}

/* Evaluating a expression. */
Object eval(vector<string>& split, Environment& env)
{
	delete_ends_parentheses(split);
	if (split.empty())
		return Object();
	else if (split.size() == 1)
		return eval(split[0], env);
	else {
		/* Eval operator -- proc */
		Object proc = eval(split[0], env);
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
					cerr << "ERROR: illegal expression" << endl;
					exit(0); /* !!!!!!!!!!!!!!!!!!!!!!!!!! */
				}
				vector<string> sub_exp(split.begin() + i, it + 1);
				args.push_back(eval(sub_exp, env));
				i = it - split.begin() + 1;
			}
			/* Eval a single operand -- self-evaluating */
			else 
				args.push_back(eval(split[i], env));
		}
		return apply_proc(proc, args, env);
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
Object eval(string& str, Environment& env)
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
		else if (env.find(str) != env.end())
			return env[str];
		else {
			cerr << "ERROR: unknown symbol -- " << str << endl;
			Primitive::quit(vector<Object>{});
		}
	}
}

/* Call proc with obs. */
Object apply_proc(Object &proc, vector<Object>& obs, Environment& env)
{
	if (proc.get_type() != PROCEDURE) {
		cerr << "ERROR: -- apply_proc()" << endl;
		exit(0);
	}
	shared_ptr<Procedure> op = proc.get_proc();
	if (op->get_type() == PRIMITIVE)
		return op->get_primitive()(obs);
	/* Compound procedure -- lambda procedure */
	else if (op->get_type() == COMPOUND)
		;/* to do */
}