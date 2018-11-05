/* Implement of evaluator's functions */

#include "eval.h"

/* Evaluator start. */
void eval_run(Environment& env)
{
	while (true) {
		prompt();
		string input = get_input();
		if (input.empty()) continue;
		vector<string> split = split_input(input);
		vector<Object> obs = transform(split);
#ifndef NDEBUG
		cout << "DEBUG eval_run(): ";
		for (auto &ob : obs)
			cout << ob.get_type() << ", ";
		cout << endl;
#endif
		Object result = eval(obs, env);
		print_result(result);
	}
}

/* Transform vector of strings to vector of Objects. */
vector<Object> transform(vector<string>& split)
{
	vector<Object> result;
	for (auto &s : split) {
		if (s == "(" || s == ")")
			continue;
		else
			if (isdigit(s[0]))
				result.push_back(Object(stoi(s)));
			else
				result.push_back(Object(s));
	}
	return result;
}

/* Evaluating a expression. */
Object eval(vector<Object>& obs, Environment& env)
{
	if (obs.empty())
		return Object();
	else if (obs.size() == 1) {
		return eval(obs[0], env);
	}
	else {
		Object first = obs[0];
#ifndef NDEBUG
		cout << "DEBUG eval(): " << first.get_string() << endl;
#endif
		Object proc = eval(first, env);
		vector<Object> args;
		for (int i = 1; i < obs.size(); i++) {
			args.push_back(obs[i]);
		}
		return apply_proc(*(proc.get_proc()), args, env);
	}
}

/* Evaluating a single object. */
Object eval(Object& ob, Environment& env)
{
	if (ob.get_type() == INTEGER || ob.get_type() == REAL)
		return ob;
	if (ob.get_type() == STRING)
		return env[ob.get_string()];
}

/* Call proc with obs. */
Object apply_proc(Procedure &proc, vector<Object>& obs, Environment& env)
{
	if (proc.get_type() == PRIMITIVE)
		return proc.get_primitive()(obs);
	if (proc.get_type() == COMPOUND)
		;/* to do */
}