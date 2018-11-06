/* Input and Output function */

#include "io_function.h"
#include "primitive_procedures.h"

/* Print evaluation result. */
void print_result(const Object& ob, int mode)
{
	if (mode == 0)
		cout << ">>> Eval value: " << endl;
	Primitive::display(vector<Object>{ob});
	cout << endl;
}

/* Read user input. */
string get_input(istream &in)
{
	string result;
	/* Used to check if the user's input is correct.  */
	int cntParantheses = 0, cntQuotations = 0;
	string stmp;
	char ctmp;
	while (true) {
		in.get(ctmp);
		if (ctmp == ';') /* Ignore comment line. */
			getline(in, stmp);
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
		else if (ctmp == '"') cntQuotations++;

		/* At end of line, check if user's input is correct or complete. */
		if (ctmp == '\n') {
			/* User input completed. */
			if (cntParantheses == 0 && (cntQuotations & 1) == 0) break;
			/* User enters a Enter in quotes */
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

/* Split input string and store them in a vector<string>. */
vector<string> split_input(string& input)
{
	vector<string> split;
	string tmp;
	for (int i = 0; i < input.size(); ++i) {
		if (input[i] == ' ') continue;

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
	return split;
}