/* Clock Algorithm with dirty bit
 * clock is a simplified implement of LRU
 * dirty bit may reduce the time of disk write.
 */
class rp_clock : public page_rp {

	vector<size_t> clocks;
	vector<char> pres;
	
	int pt;
	size_t current;
	
public:

	rp_clock()
	{
		memset(name, 0, sizeof name);
		strcpy(name, "clock with d bit");
	}
	virtual void reset_hook(int n)
	{
		clocks.resize(n);
		pres.resize(n);
		for (int i = 0; i < n; i++)
			pres[i] = 0;
		pt = 0;
		current = 0;
	}

	// find a empty frame to swap in
	void put_inside(int pos)
	{
		assert(mem.size() < n);
		size_t cur = pos;
		while (pres[pt])
			pt = (pt + 1) % n;
		pres[pt] = 1;
		clocks[pt] = pos; 
	}

	virtual void read_hook(size_t pos)
	{
		current = pos;
		if (!inside(pos) && mem.size() < n)
			put_inside(pos);
	}

	virtual void write_hook(size_t pos)
	{
		current = pos;
		if (!inside(pos) && mem.size() < n)
			put_inside(pos);
	}

	virtual size_t find_swap()
	{
		assert(mem.size() == n);
		while (1) {
			assert(inside(clocks[pt]));
			int cur = clocks[pt], mask = get_mask(cur);
			if (mask & PTE_A) {
				set_mask(cur, mask);
			} else if (mask & PTE_D) {
				write_back(cur);
			} else {
				clocks[pt] = current;
				pt = (pt + 1) % n;
				return cur;
			}
			pt = (pt + 1) % n;
		}
		assert(false);
		return 0;
	}
} rp_clock_algo;
