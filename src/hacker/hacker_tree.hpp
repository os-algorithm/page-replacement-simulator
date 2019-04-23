/* Simple Test Case
 * Access graph is a tree
 */

#pragma once

#include <string>
#include <iostream>
#include <vector>

using std::string;
using std::vector;

class tree_hacker : public hacker {

public:

	const static int node = 1 << 17;
	const static int length = 1 << 20;
	
	vector<int> v[node];
	vector<size_t> ref;
	
	tree_hacker()
	{ name = "tree hacker", first_run = true; }
	
	void gen_tree();
	void gen_seqence();
	void reset();
	bool first_run;
	
	virtual void main(Simulater *sim)
	{
		if (first_run) {
			reset();
			gen_tree();
			gen_seqence();
			first_run = false;
		}
		int cur = 0;
		Array<char> mem = sim->new_array<char>(node * pgsize);
		for (size_t i : ref)
			mem[i] = 1;
	}
};

void tree_hacker::gen_tree()
{
	for (size_t i = 1; i < node; i++) {
		int p = rand() % i;
		v[p].push_back(i);
	}
}

void tree_hacker::gen_seqence()
{
	int cur = 0;
	vector<size_t> id;
	for (size_t i = 0; i < node; i++)
		id.push_back(0);
	for (size_t i = 0; i < length; i++) {
		ref.push_back(cur * pgsize);
		if (!v[cur].empty()) {
			size_t nxt = v[cur][id[cur]++];
			if (id[cur] == v[cur].size())
				id[cur] = 0;
			cur = nxt;
		} else {
			cur = 0;
		}
	}
}

void tree_hacker::reset()
{
	for (int i = 0; i < node; i++)
		v[i].clear();
}
