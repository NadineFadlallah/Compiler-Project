#include <bits/stdc++.h>

using namespace std;

#include "Lexer.h"
#include "Par.h"

void h_line(int n = 11) {
	for (int i = 0; i < n; i++) {
		cout << "--";
	}
}

map<int, vector<pair<int, string>>> tree;

void dfs(int u) {

	if (tree[u].empty())
		return;

	// the tree is displayed by printing grammar symbol of the the children of every node,
	// as well as the ids of those children to keep track of further printing
	// the sequence of symbols used describes the production rule used on the current node

	cout << "Node " << u << " has children:\n";

	for (auto &pr : tree[u])
		cout << pr.second << setw(12);

	cout << '\n';

	for (auto &pr : tree[u])
		cout << pr.first << setw(12);

	cout << '\n';
	h_line(100);
	cout << '\n';

	for (auto &pr : tree[u])
		dfs(pr.first);
}

int main() {
	ios::sync_with_stdio(false);

	Lexer L;

	vector<pair<string, string>> tokens = L.Tokenize("test.c");

	h_line();
	cout << "\nToken";
	cout << setw(18) << "Category\n";
	h_line();
	cout << "\n";


	for (auto pr : tokens)
		cout << pr.first << setw(18 - pr.first.length()) << pr.second << '\n';

	vector<string> lex_output;

	for (auto pr : tokens)
		if (pr.second == "id")
			lex_output.push_back("id");
		else if (pr.second == "int")
			lex_output.push_back("int");
		else if (pr.first == "int" || pr.first == "float"
				|| pr.first == "double")
			lex_output.push_back("num_datatype");
		else
			lex_output.push_back(pr.first + "_" + pr.second);

	h_line();
	cout << '\n';


	Parse P(lex_output, tokens);


	if (P.prog(0))
		cout << "Correct Syntax\n";
	else
		cout << "Syntax Error\n";

	// The parse tree consists of nodes, each node is given a unique id
	// the edge are stored using adjacency lists
	// also, an edge from node u to node v holds information about which symbol of
	// the grammar represents node v
	// node 0 is the root

	// how the tree is constructed: we basically create a new node for every call
	// to a grammar smybol (terminal or non-terminal) like expr() or E()
	// and we keep track of the parent node of the current node,
	// and add an edge from parent to current node
	// but since this is a backtracking algorithm, we might find that the first 2 production rules
	// don't match our current input but the 3rd does, so we need to get rid of the effect
	// of those failed trials, by clearing all the children going out from the current node
	// when trying a new rule

	cout << "\nParse tree...\n";
	h_line(100);
	cout << '\n';

	tree = P.tree;

	dfs(0);

	return 0;
}
