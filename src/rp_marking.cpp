/* Random Marking Algorithm
 * is k-Competitve
 */
class rp_marking : public page_rp {

	set<int> unmarked;
	set<int> marked;
	size_t current;

	void hook_fun(size_t pos)
	{
		if (unmarked.count(pos)) {
			unmarked.erase(pos);
			marked.insert(pos);
		} else if (!marked.count(pos) && marked.size() + unmarked.size() < n) {
			marked.insert(pos);
		}
		current = pos;
	}

public:

	virtual void reset_hook(int n)
	{
		unmarked.clear();
		marked.clear();
	}
	
	rp_marking()
	{
		memset(name, 0, sizeof name);
		strcpy(name, "Random Marking");
	}

	virtual void write_hook(size_t pos)
	{ hook_fun(pos); }

	virtual void read_hook(size_t pos)
	{ hook_fun(pos); }

	virtual size_t find_swap()
	{
		if (unmarked.size() + marked.size() != n) {
			cout << unmarked.size() + marked.size() << endl;
			cout << n << endl;
		}
		assert(unmarked.size() + marked.size() == n);
		if (unmarked.empty())
			swap(unmarked, marked);
		size_t ran = rand() % unmarked.size();
		set<int>::iterator it = unmarked.begin();
		for (size_t i = 0; i < ran; i++)
			it++;
		size_t ret = *it;
		unmarked.erase(it);
		marked.insert(current);
		return ret;
	}
	
} rp_marking_algo;
