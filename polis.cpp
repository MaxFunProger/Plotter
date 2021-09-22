#include "polis.h"

const int INF = 1e9;

using vs = std::vector<std::string>;
using qs = std::queue<std::string>;
using ss = std::stack<std::string>;

bool err = false;
vs opers = {"+", "-", "*", "/", "^", "_"},
right_assoc = {"_",   "abs", "sin", "cos", "tan", "cot", "log", "ln",  "^",   "lg",  "sqrt", "exp"},
funcs = {"abs", "sin", "cos", "tan", "cot", "log", "ln", "lg", "sqrt", "exp"};
std::vector<vs> prior = {
  {"+", "-"},
  {"*", "/"},
  {"_"},
  {"^"},
  {"abs", "sin", "cos", "tan", "cot", "log", "ln", "lg", "sqrt", "exp"}};

void error(std::string row = "") {
  err = true;
  throw std::runtime_error("Undefined function. Crush row: " + row);
}

bool is_in(vs& where, std::string& what) {
  return std::find(where.begin(), where.end(), what) != where.end();
}


int priority(std::string& first, std::string& second) {
  int f_p, s_p;
  for (int i = 0; i < prior.size(); ++i) {
      if (is_in(prior[i], first)) {
          f_p = i;
          break;
        }
    }
  for (int i = 0; i < prior.size(); ++i) {
      if (is_in(prior[i], second)) {
          s_p = i;
          break;
        }
    }

  return (f_p > s_p ? 2 : f_p == s_p);
}


vs to_vector(std::string input) {
  vs std_state;
  for (int i = 0; i < input.size(); ++i) {
      if (std::isdigit(input[i])) {
          std::string num = "";
          while (std::isdigit(input[i])) num += input[i++];
          if (input[i] == '.') {
              num += '.';
              ++i;
              while (std::isdigit(input[i])) num += input[i++];
            }
          std_state.push_back(num);
          --i;
          continue;
        }
      if (input[i] == 'x') {
          std_state.push_back("x");
          continue;
        }

      if (input[i] == '(') {
          std_state.push_back("(");
          continue;
        }

      if (input[i] == ')') {
          std_state.push_back(")");
          continue;
        }

      if (input[i] == ',') {
          std_state.push_back(",");
          continue;
        }

      if (std::isalpha(input[i])) {
          std::string oper = "";
          while (std::isalpha(input[i])) oper += input[i++];
          if (!is_in(funcs, oper)) {
              error("85 polis");
              return std_state;
            }
          std_state.push_back(oper);
          --i;
          continue;
        }
      std::string s = "";
      s += input[i];
      std_state.push_back(s);
    }
  return std_state;
}


bool check(vs& std_state) {
  for (int i = 1; i < std_state.size(); ++i) {
      if (is_in(opers, std_state[i])) {
          if (std_state[i] == "-" &&
              (std_state[i - 1] == "(" || is_in(opers, std_state[i - 1]))) {
              std_state[i] = "_";
              continue;
            }
          if (std_state[i - 1] != ")" && !std::isdigit(std_state[i - 1][0]) && !std_state[i-1][0] == 'x') {
              error("107 polis");
              return 0;
            }
          continue;
        }

      if (is_in(funcs, std_state[i])) {
          if (std_state[i - 1] != "(" && std_state[i - 1] != "," &&
              !is_in(opers, std_state[i - 1])) {
              error("116 polis");
              return 0;
            }
          continue;
        }

      if (std_state[i] == "(") {
          if (!is_in(opers, std_state[i - 1]) && std_state[i - 1] != "(" &&
              std_state[i - 1] != "_" && !is_in(funcs, std_state[i - 1])) {
              error("125 polis");
              return 0;
            }
          continue;
        }

      if (std_state[i] == ")" || std_state[i] == ",") {
          if (!std::isdigit(std_state[i - 1][0]) && !std_state[i-1][0] == 'x' && std_state[i - 1] != ")") {
              error("133 polis");
              return 0;
            }
          continue;
        }

      if (std::isdigit(std_state[i][0]) || std_state[i][0] == 'x') {
          if (std_state[i - 1] != "(" && std_state[i - 1] != "," &&
              std_state[i - 1] != "_" && !is_in(opers, std_state[i - 1])) {
              error("142 polis");
              return 0;
            }
          continue;
        }
      error("147 polis");
      return 0;
    }
  return 1;
}



qs to_polis(vs& std_state) {
  ss st;
  qs ans;
  for (auto el : std_state) {
      if (el == "(") {
          st.push("(");
          continue;
        }

      if (el == ")") {
          while (!st.empty() && st.top() != "(" && st.top() != ",") {
              ans.push(st.top());
              st.pop();
            }
          if (st.empty()) {
              error("167 polis");
              return ans;
            }
          st.pop();
          continue;
        }

      if (el == ",") {
          while (!st.empty() && st.top() != "(" && st.top() != ",") {
              ans.push(st.top());
              st.pop();
            }
          if (st.empty()) {
              error("180 polis");
              return ans;
            }
          st.pop();
          st.push(",");
          continue;
        }

      if (std::isdigit(el[0]) || el == "x") {
          ans.push(el);
          continue;
        }

      while (!st.empty() && st.top() != "(" && st.top() != "," &&
             priority(st.top(), el) == 2) {
          ans.push(st.top());
          st.pop();
        }
      if (!is_in(right_assoc, el)) {
          while (!st.empty() && st.top() != "(" && st.top() != "," &&
                 priority(st.top(), el) == 1) {
              ans.push(st.top());
              st.pop();
            }
        }
      /*if (st.empty()) {
          error("206 polis");
          return ans;
        }*/
      st.push(el);
    }
  /*if (!st.empty()) error("211 polis");
    return ans;*/
  while (!st.empty()){
      ans.push(st.top());
      st.pop();


  }
}


double calc(qs polis, double x) {
  std::stack<double> st;
  while (!polis.empty()) {
      if (std::isdigit((polis.front()[0]))) {
          st.push(stod(polis.front()));
          polis.pop();
          continue;
        }
      if (polis.front() == "x") {
          st.push(x);
          polis.pop();
          continue;
        }

      if (polis.front() == "+") {
          if (st.empty()) {
              error("230 polis");
              return 0;
            }
          polis.pop();
          double a, b;
          b = st.top();
          st.pop();
          if (st.empty()) {
              error("238 polis");
              return 0;
            }
          a = st.top();
          st.pop();
          st.push(a + b);
          continue;
        }

      if (polis.front() == "-") {
          if (st.empty()) {
              error("249 polis");
              return 0;
            }
          polis.pop();
          double a, b;
          b = st.top();
          st.pop();
          if (st.empty()) {
              error("257 polis");
              return 0;
            }
          a = st.top();
          st.pop();
          st.push(a - b);
          continue;
        }

      if (polis.front() == "*") {
          if (st.empty()) {
              error("268 polis");
              return 0;
            }
          polis.pop();
          double a, b;
          b = st.top();
          st.pop();
          if (st.empty()) {
              error("276 polis");
              return 0;
            }
          a = st.top();
          st.pop();
          st.push(a * b);
          continue;
        }

      if (polis.front() == "/") {
          if (st.empty()) {
              error("287 polis");
              return 0;
            }
          polis.pop();
          double a, b;
          b = st.top();
          st.pop();
          if (st.empty()) {
              error("295 polis");
              return 0;
            }
          a = st.top();
          st.pop();
          st.push(a / b);
          continue;
        }

      if (polis.front() == "_") {
          if (st.empty()) {
              error("306 polis");
              return 0;
            }
          polis.pop();
          double a = st.top();
          st.pop();
          st.push(-a);
          continue;
        }

      if (polis.front() == "abs") {
          if (st.empty()) {
              error("318 polis");
              return 0;
            }
          polis.pop();
          double a = std::fabs(st.top());
          st.pop();
          st.push(a);
          continue;
        }

      if (polis.front() == "sin") {
          if (st.empty()) {
              error("320 polis");
              return 0;
            }
          polis.pop();
          double a = std::sin(st.top());
          st.pop();
          st.push(a);
          continue;
        }

      if (polis.front() == "cos") {
          if (st.empty()) {
              error("342 polis");
              return 0;
            }
          polis.pop();
          double a = std::cos(st.top());
          st.pop();
          st.push(a);
          continue;
        }

      if (polis.front() == "tan") {
          if (st.empty()) {
              error("354 polis");
              return 0;
            }
          polis.pop();
          double a = std::tan(st.top());
          st.pop();
          st.push(a);
          continue;
        }

      if (polis.front() == "cot") {
          if (st.empty()) {
              error("366 polis");
              return 0;
            }
          polis.pop();
          double a = 1.0 / std::tan(st.top());
          st.pop();
          st.push(a);
          continue;
        }

      if (polis.front() == "log") {
          if (st.empty()) {
              error("378 polis");
              return 0;
            }
          polis.pop();
          double a = st.top();
          st.pop();
          if (st.empty()) {
              error("385 polis");
              return 0;
            }
          double b = st.top();
          st.pop();
          if (a < 0 || b < 0) {
              //error("391 polis");
              return INF;
            }
          st.push(std::log(b) / std::log(a));
          continue;
        }

      if (polis.front() == "^") {
          polis.pop();
          if (st.empty()) {
              error("401 polis");
              return 0;
            }
          double a = st.top();
          st.pop();
          if (st.empty()) {
              error("407 polis");
              return 0;
            }
          double b = st.top();
          if (b < 0 && a - int(a) != 0) {
              //error("412 polis");
              return INF;
            }
          st.pop();
          st.push(std::pow(b, a));
          continue;
        }

      if (polis.front() == "ln") {
          polis.pop();
          if (st.empty()) {
              error("423 polis");
              return 0;
            }
          double a = st.top();
          st.pop();
          if (a < 0) {
              //error("429 polis");
              return INF;
            }
          st.push(std::log(a));
          continue;
        }

      if (polis.front() == "lg") {
          polis.pop();
          if (st.empty()) {
              error("439 polis");
              return 0;
            }
          double a = st.top();
          st.pop();
          if (a < 0) {
              //error("445 polis");
              return INF;
            }
          st.push(std::log(a) / std::log(10));
          continue;
        }

      if (polis.front() == "sqrt") {
          polis.pop();
          if (st.empty()) {
              error("455 polis");
              return 0;
            }
          double a = st.top();
          st.pop();
          if (a < 0) {
              //error("461 polis");
              return INF;
            }
          st.push(std::sqrt(a));
          continue;
        }
      if (polis.front() == "exp") {
          polis.pop();
          if (st.empty()) {
              error("472 polis");
              return 0;
            }
          double a = st.top();
          st.pop();
          st.push(std::exp(a));
          continue;
        }
    }
  if (st.size() != 1) {
      error("469 polis");
      return 0;
    }
  return st.top();
}
