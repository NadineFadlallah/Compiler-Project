#include <bits/stdc++.h>

using namespace std;

class Parse {
public:
	int next_id = 0;
	map<int, vector<pair<int, string>>> tree;
	Parse(vector<string> line, vector<pair<string, string>> tokens);
	bool term(int p, string tok);
	bool idOrNum(int p);
	bool E1(int p);
	bool E2(int p);
	bool E3(int p);
	bool E(int p);
	bool T1(int p);
	bool T2(int p);
	bool T3(int p);
	bool T4(int p);
	bool T(int p);
	bool identify1(int p);
	bool identify2(int p);
	bool identify(int p);
	bool expr1(int p);
	bool expr2(int p);
	bool expr3(int p);
	bool expr4(int p);
	bool expr5(int p);
	bool expr(int p);
	bool cond1(int p);
	bool cond2(int p);
	bool cond3(int p);
	bool cond4(int p);
	bool cond5(int p);
	bool cond6(int p);
	bool cond(int p);
	bool prog(int p);
	bool prog1(int p);
	bool prog2(int p);
	vector<string> line;
	vector<pair<string, string>> tokens;
	vector<string>::iterator next_tok;
};
