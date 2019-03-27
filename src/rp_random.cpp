/* Random Replacement Algorithm
 * replace an random page in memory.
 */
class rp_ran : public page_rp {
	
public:

	rp_ran()
	{
		memset(name, 0, sizeof name);
		strcpy(name, "random");
	}

	virtual size_t find_swap()
	{
		assert(mem.size() == n);
		int ran = rand()%n+1;
		map<size_t, int>::iterator iter = mem.begin();
		for (int i = 1; i < ran; i++)
			iter++;
		return iter->first;
	}
} rp_ran_algo;
