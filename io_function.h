/* io_function */

#ifndef IO_FUNCTION_
#define IO_FUNCTION_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
#include "object.h"

/* Print Scheme prompt for input. */
void prompt();

/* Print evaluation result. */
void print_result(const Object& ob);

/* Read user input. */
string get_input(istream &in = cin);

/* Split input string and store them in a vector<string>. */
vector<string> split_input(string& input);

#endif