/* Input and Output function */

#include "io_function.h"
#include "primitive_procedures.h"
#include "eval.h"

/* Print evaluation result. */
void print_result(const Object& ob, int mode)
{
	if (mode == 0)
		cout << ">>> Eval value: ";
	if (mode == 0 || mode == 2) {
		Primitive::display(vector<Object>{ob});
		cout << endl;
	}
}

/* Read user input. */
string get_input(istream &in)
{
	string result;
	/* Used to check if the user's input is correct.  */
	int cntParantheses = 0, cntQuotations = 0;
	string stmp;
	char ctmp;
	int in_quotes = 0;
	while (in.get(ctmp)) {
		if (ctmp == ';') {/* Ignore comment line. */
			getline(in, stmp); 
			continue;
		}
		if (in_quotes == 0 && ctmp == '|' && !result.empty() &&
			result.back() == '#')
		{
			/* Ignore "#| ... |#" comment */
			result.pop_back();
			char prev = '\0';
			while (in.get(ctmp)) {
				if (ctmp != '#') continue;
				else if (prev == '|') break;
				prev = ctmp;
			}
			continue;
		}

		if (ctmp != '\n') {
			/* Add a space at both sides of parentheses. */
			if (ctmp == '(' || ctmp == ')') {
				result.push_back(' ');
				result.push_back(ctmp);
				result.push_back(' ');
			}
			else
				result.push_back(ctmp);
		}

		if (ctmp == '(') cntParantheses++;
		else if (ctmp == ')') cntParantheses--;
		else if (ctmp == '"') {
			cntQuotations++;
			in_quotes = cntQuotations % 2;
		}

		/* At end of line, check if user's input is correct or complete. */
		if (ctmp == '\n') {
			/* User input completed. */
			if (cntParantheses == 0 && (cntQuotations & 1) == 0) break;
			/* User enters a [Enter] in quotes */
			if ((cntQuotations & 1) == 1)
				result += "\\n";
			/* User input has not been completed. */
			else 
				result.push_back(' ');
		}
	}
#ifndef NDEBUG
	cout << "DEBUG get_input(): " << result << endl;
#endif
	return result;
}

/* Convert '(<exp1> ... <expn>) too (list <exp1> ... <expn>) */
static void convert_to_list(vector<string>& split)
{
	for (int i = 0; i < split.size(); i++) {
		if (split[i] == "'") {
			split[i] = "list";
			swap(split[i], split[i + 1]);
		}
	}
}

/* Split input string and store them in a vector<string>. */
vector<string> split_input(string& input)
{
	vector<string> split;
	string tmp;
	for (int i = 0; i < input.size(); ++i) {
		if (input[i] == ' ' || input[i] == '\t') continue;

		int start = i;
		/* Don't split "string", such as "abc def ghi". */
		char mark = (input[i] == '"' ? '"' : ' ');

		while (++i < input.size() && input[i] != mark);
		if (mark == '"') i++;
		split.push_back(input.substr(start, i - start));
	}
#ifndef NDEBUG
	cout << "DEBUG split_input(): ";
	for (auto s : split)
		cout << s << ", ";
	cout << endl;
#endif
	convert_to_list(split);
	return split;
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

/* Evaluate expression from a string */
void load_code(const string& code)
{
	string copy(code);
	copy.push_back('\n');
	istringstream iss(copy);
	string input = get_input(iss);
	vector<string> split = split_input(input);
	Object result = eval(split);
}
