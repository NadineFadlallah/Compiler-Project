#include "Par.h"

Parse::Parse(vector<string> _line, vector<pair<string, string>> _tokens) : line(_line), tokens(_tokens) {
	next_tok = line.begin();
	next_id = 1;
	for (vector<string>::iterator i = line.begin(); i != line.end(); i++)
		cout << *i << endl;
}

bool Parse::term(int p, string tok) {
	//If we reached the end of the line vector and the expression we are trying to match is not over yet 
	//we will reach the end iterator and we using dereference operator * on it will cause an error
	//prevents errors in the program when the parser is checking an incomplete (wrong) syntax like x = 2 (no ;)
	int id = next_id++;
	if (next_tok != line.end()) {
		tree[p].push_back(make_pair(id, tokens[next_tok - line.begin()].first));
		return *next_tok++ == tok;
	} else
		return false;
}

//numbers and identifiers both come next interchangeably after the equal (=) notation so this function checks weather if one of them exist
bool Parse::idOrNum(int p) {
	std::vector<string>::iterator save = next_tok;
	int id = next_id++;
	tree[p].push_back(make_pair(id, "ID_OR_NUM"));
	if (term(id, "id"))
		return true;
	else {
		next_tok = save;
		tree[id].clear();
		return term(id, "int");
	}
}

//E --> T+E | T-E | T 
bool Parse::E1(int p) {
	return T(p) && term(p, "+_op") && E(p);
}
bool Parse::E2(int p) {
	return T(p) && term(p, "-_op") && E(p);
}
bool Parse::E3(int p) {
	return T(p);
}

bool Parse::E(int p) {
	std::vector<string>::iterator save = next_tok;
	int id = next_id++;
	tree[p].push_back(make_pair(id, "E"));
	if (E1(id))
		return true;
	else {
		next_tok = save;
		tree[id].clear();
		if (E2(id))
			return true;
		else {
			next_tok = save;
			tree[id].clear();
			return E3(id);
		}
	}
}

//T --> int*T | int/T | int | id*T | id/T | id | (E)
bool Parse::T1(int p) {
	return idOrNum(p) && term(p, "*_op") && T(p);
}
bool Parse::T2(int p) {
	return idOrNum(p) && term(p, "/_op") && T(p);
}
bool Parse::T3(int p) {
	return idOrNum(p);
}
bool Parse::T4(int p) {
	return term(p, "(_del") && E(p) && term(p, ")_del");
}
bool Parse::T(int p) {
	vector<string>::iterator save = next_tok;
	int id = next_id++;
	tree[p].push_back(make_pair(id, "T"));
	if (T1(id))
		return true;
	else {
		next_tok = save;
		tree[id].clear();
		if (T2(id))
			return true;
		else {
			next_tok = save;
			tree[id].clear();
			if (T3(id))
				return true;
			else {
				next_tok = save;
				tree[id].clear();
				return T4(id);
			}
		}
	}
}

//cond --> E == E | E != E | E >= E | E <= E | E > E | E < E
bool Parse::cond1(int p) {
	return E(p) && term(p, "==_op") && E(p);
}
bool Parse::cond2(int p) {
	return E(p) && term(p, ">=_op") && E(p);
}
bool Parse::cond3(int p) {
	return E(p) && term(p, "<=_op") && E(p);
}
bool Parse::cond4(int p) {
	return E(p) && term(p, "!=_op") && E(p);
}
bool Parse::cond5(int p) {
	return E(p) && term(p, ">_op") && E(p);
}
bool Parse::cond6(int p) {
	return E(p) && term(p, "<_op") && E(p);
}
bool Parse::cond(int p) {
	vector<string>::iterator save = next_tok;
	int id = next_id++;
	tree[p].push_back(make_pair(id, "COND"));
	if (cond1(id))
		return true;
	else {
		next_tok = save;
		tree[id].clear();
		if (cond2(id))
			return true;
		else {
			next_tok = save;
			tree[id].clear();
			if (cond3(id))
				return true;
			else {
				next_tok = save;
				tree[id].clear();
				if (cond4(id))
					return true;
				else {
					next_tok = save;
					tree[id].clear();
					if (cond5(id))
						return true;
					else {
						next_tok = save;
						tree[id].clear();
						return cond6(id);
					}
				}
			}
		}
	}
}

//used for identifiers like int x; | int x = 2 * 3;
bool Parse::identify1(int p) {
	return term(p, "num_datatype") && term(p, "id") && term(p, "=_op") && E(p)
			&& term(p, ";_del");
}
bool Parse::identify2(int p) {
	return term(p, "num_datatype") && term(p, "id") && term(p, ";_del");
}
bool Parse::identify(int p) {
	vector<string>::iterator save = next_tok;
	int id = next_id++;
	tree[p].push_back(make_pair(id, "IDENTIFY"));
	if (identify1(id))
		return true;
	else {
		next_tok = save;
		tree[id].clear();
		return (identify2(id));
	}
}

//expr --> if (COND) EXPR else EXPR | while (COND) EXPR | id = EXPR
bool Parse::expr1(int p) {
	return identify(p);
}
;
bool Parse::expr2(int p) {
	return term(p, "id") && term(p, "=_op") && E(p) && term(p, ";_del");
}
bool Parse::expr3(int p) {
	return term(p, "if_key") && term(p, "(_del") && cond(p) && term(p, ")_del") && expr(p)
			&& term(p, "else_key") && expr(p);
}
bool Parse::expr4(int p) {
	return term(p, "while_key") && term(p, "(_del") && cond(p) && term(p, ")_del")
			&& expr(p);
}
bool Parse::expr5(int p) {
	return term(p, "for_key") && term(p, "(_del") && identify(p) && cond(p)
			&& term(p, ";_del") && term(p, "id") && term(p, "=_op") && E(p)
			&& term(p, ")_del") && expr(p);
}
bool Parse::expr(int p) {
	vector<string>::iterator save = next_tok;
	int id = next_id++;
	tree[p].push_back(make_pair(id, "EXPR"));
	if (expr1(id))
		return true;
	else {
		next_tok = save;
		tree[id].clear();
		if (expr2(id))
			return true;
		else {
			next_tok = save;
			tree[id].clear();
			if (expr3(id))
				return true;
			else {
				next_tok = save;
				tree[id].clear();
				if (expr4(id))
					return true;
				else {
					next_tok = save;
					tree[id].clear();
					return expr5(id);
				}
			}
		}
	}
}

//This is the starting grammar so after it's finished if must check if it reached the end of the tokens
//IT makes the compiler support multiple statements
// prog --> prog expr | expr
bool Parse::prog1(int p) {
	return expr(p) && prog(p);
}
bool Parse::prog2(int p) {
	return expr(p);
}
bool Parse::prog(int p = 0) {
	vector<string>::iterator save = next_tok;
	int id = next_id++;
	tree[p].push_back(make_pair(id, "PROG"));
	if (prog1(id))
		return true;
	else {
		next_tok = save;
		tree[id].clear();
		return (prog2(id) && next_tok == line.end()); //after making the last derivation, it checks if it reached the end of the token
													//if not then there are other statments that haven't been checked yet
	}
}
