const int PTE_P = 1;
const int PTE_A = 2;
const int PTE_D = 4;
const int MAX_LEN = 1 << 22;
const int MAX_SIZE = 512;

/* Abstract Base Class for Page Replacement Algorithms
 * n : size of physical memory
 * mem : contains of physical memory, mem[i] is tag of page i, 
 *       recording the information about present bit(PTE_P), accesse bit(PTE_A) and dirty bit(PTE_D); 
 *       PTE_A, PTE_D, PTE_P are automatically setted.
 * miss, push, pull : counter for the number of page fault, swap-out and swap-in
 * virtual function find_swap() : find a valid page to swap out when a page fault occurs & no space remains
 * function read(pos) : implement an read opt on a page 
 * function write(pos): implement an write opt on a page 
 */ 
class page_rp {
	
	void swap_in(size_t pos)
	{
		miss++;
		if (mem.size() < n) {
			pull++;
			mem[pos] = PTE_P;
		} else {
			size_t page = find_swap();
			map<size_t, int>::iterator iter = mem.find(page);
			assert(iter != mem.end());
			if (iter->second & PTE_D)
				push++;
			pull++;
			mem.erase(iter);
			mem[pos] = PTE_P;
		}
	}
	
public:

	int n;
	map<size_t, int> mem;
	char name[20];
	int miss;
	int push;
	int pull;

	// init and set memory size to n
	void reset(int n)
	{
		reset_hook(n); 
		this->n = n;
		mem.clear();
		miss = push = pull = 0;
	}
	
	// find a page to swap out , return page number
	virtual size_t find_swap() = 0;
	virtual void write_hook(size_t pos)
	{ }
	virtual void read_hook(size_t pos)
	{ }
	virtual void reset_hook(int n)
	{ }

	// get mask of page 	
	int get_mask(size_t pos)
	{
		assert(mem.count(pos));
		return mem[pos];
	}

	// set mask for page
	void set_mask(size_t pos, int mask)
	{
		assert(mem.count(pos));
		mem[pos] = mask;
	}
	
	// write out a page pos and set PTE_D
	// for clock algorithm
	void write_back(size_t pos) {
		push++;
		mem[pos] &= ~PTE_D;
	}

	bool inside(size_t pos)
	{ return mem.count(pos); }
	
	void write(size_t pos) {
		map<size_t, int>::iterator iter = mem.find(pos);
		write_hook(pos);
		if (iter != mem.end()) {
			iter->second |= PTE_A | PTE_D;
		} else {
			swap_in(pos);
		}
	}
	
	void read(size_t pos) {
		map<size_t, int>::iterator iter = mem.find(pos);
		read_hook(pos);
		if (iter != mem.end()) {
			iter->second |= PTE_A;
		} else {
			swap_in(pos);
		}
	}
};
