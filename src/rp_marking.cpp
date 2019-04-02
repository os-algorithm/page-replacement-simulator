/* Random Marking Algorithm
 * is k-Competitve
 */
class rp_marking : public page_rp {

	set<int> unmarked;
	set<int> marked;
	
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
	{
	}

	virtual void read_hook(size_t pos)
	{
		
	}

	virtual size_t find_swap()
	{
		
	}
	
} rp_marking_algo;
