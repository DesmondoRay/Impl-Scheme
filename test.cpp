/* Test */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

string get_input(istream &in = cin);
vector<string> split_input(string& input);

template<typename T>
void printVec(vector<T>& v)
{
	for (auto &i : v)
		cout << i << ", ";
	cout << endl;
}

int test()
{
	string input("(define a \"abc aa\nbc\")\n");
	istringstream iss(input);
	split_input(get_input(iss));

	iss.str("(define square (lambda (n)\n(* n n)))\n");
	split_input(get_input(iss));

	return 0;
}