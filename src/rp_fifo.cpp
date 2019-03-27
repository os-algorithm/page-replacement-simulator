/* FIFO Replacement Algorithm
 * First-in-first-out, replace the "oldest" page.
 * May suffer from Belady's Anomaly. 
 */
class rp_fifo : public page_rp {

	size_t time;
	queue<size_t> reco;
	
public:

	virtual void reset_hook(int n)
	{
		time = 0;
		while (!reco.empty())
			reco.pop();
	}
	
	rp_fifo()
	{
		memset(name, 0, sizeof name);
		strcpy(name, "FIFO");
	}

	virtual void write_hook(size_t pos)
	{
		if (!mem.count(pos))
			reco.push(pos);
	}

	virtual void read_hook(size_t pos)
	{
		if (!mem.count(pos))
			reco.push(pos);
	}
	
	virtual size_t find_swap()
	{
		assert(!reco.empty());
		size_t t = reco.front(); reco.pop();
		return t;
	}
	
} rp_fifo_algo;
