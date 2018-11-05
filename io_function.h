/* io_function */

#ifndef IO_FUNCTION_
#define IO_FUNCTION_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
#include "object.h"

void prompt();
void print_result(const Object& ob);
string get_input(istream &in = cin);
vector<string> split_input(string& input);

#endif