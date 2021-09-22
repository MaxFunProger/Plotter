#ifndef POLIS_H
#define POLIS_H

#include <vector>
#include <algorithm>
#include <stack>
#include <string>
#include <stdexcept>
#include <cmath>
#include <queue>

using vs = std::vector<std::string>;
using qs = std::queue<std::string>;
using ss = std::stack<std::string>;
bool check(vs&);
qs to_polis(vs&);
double calc(qs, double);
void error(std::string);
bool is_in(vs&, std::string&);
int priority(std::string&, std::string&);
vs to_vector(std::string);




#endif // POLIS_H
