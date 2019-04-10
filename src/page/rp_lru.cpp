/* Least Recently Used Algorithm 
 * the recently used pages may be used soon.  
 * swap out the least recently used page. 
 */
class rp_lru : public page_rp {

	size_t time;
	map<size_t, size_t> rec;
	priority_queue<pair<size_t,size_t>,
	        vector<pair<size_t,size_t> >,
	        greater<pair<size_t,size_t> > > que;
	
public:

	virtual void reset_hook(int n)
	{
		time = 0;
		rec.clear();
		while (!que.empty())
			que.pop();
	}

	rp_lru()
	{
		memset(name, 0, sizeof name);
		strcpy(name, "LRU");
	}

	virtual void write_hook(size_t pos)
	{ rec[pos] = ++time, que.push(make_pair(time, pos)); }

	virtual void read_hook(size_t pos)
	{ rec[pos] = ++time, que.push(make_pair(time, pos)); }

	virtual size_t find_swap()
	{
		// delete wrone info
		while (!que.empty() && (!rec.count(que.top().second)
		        || rec[que.top().second] != que.top().first))
			que.pop();
		assert(!que.empty());
		size_t sp = que.top().second, t = que.top().first;
		assert(rec[sp] == t);
		rec.erase(rec.find(sp));
		return sp;
	}
} rp_lru_algo;
