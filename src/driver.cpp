#include <dirent.h>

// maximum of logical memory pages 
const int MM = 512;
// number of memory frames
const int Nl = 8, Nr = 512, Nstep = 4;
// sequence length for random OPT test 
const int Rlen = 10000;

const char *trace_path = "/mnt/c/Users/ljt12138/Desktop/oslab/sim_ran/trace";

/* opt algorithm, minimize miss(pull) time.
 * @len : length of memory access trace
 * @M   : number of logical pages
 * @n   : number of physical pages
 * @a   : memory access trace array
 * @opt : memory access R/W array 
 */
template<typename A, typename B>
inline int get_opt(int len, int M, int n, A &a, B &opt)
{
	static int nxt[MAX_LEN];
	assert(len <= MAX_LEN);
	map<int, int> T;
	for (int i = len-1; i >= 0; i--) {
		if (!T.count(a[i])) nxt[i] = len;
		else nxt[i] = T[a[i]];
		T[a[i]] = i;
	}
	map<int,int> st;
	int miss = 0;
	for (int i = 0; i < len; i++) {
		if (st.count(a[i])) {
			st[a[i]] = nxt[i];
			// hit.
			// update next ref
			continue;
		}
		miss++;
		if (st.size() < n) {
			st[a[i]] = nxt[i];
		} else {
			map<int,int>::iterator j = st.end();
			for (map<int,int>::iterator k = st.begin(); k != st.end(); k++)
				if (j == st.end() || k->second > j->second)
					j = k;
			assert(j != st.end());
			st.erase(j);
			// swap out j
			st[a[i]] = nxt[i];
			// swap in this page.
		}
	}
	return miss;
}

/* Analyzer using trace frome /trace_path/*.trace
 * display efficiency of replacement algorithm 
 * when memory size if Nl .. Nr by Nstep pages
 */
class Analyzer {

	vector<size_t> a;
	vector<char> opt;
	
	// read trace files frome trace_path
	bool read_trace(dirent *ptr) {
		int _wa_;
		string str = trace_path;
		str = str + "/" + ptr->d_name;
		size_t pos = str.size()-6;
		if (str.substr(pos, 6) != ".trace")
			return false;
		cout << " -- Read Trace : " << ptr->d_name << " -- " << endl;
		FILE *tra = fopen(str.c_str(), "r");
		int len;
		assert(tra != NULL);
		assert(fscanf(tra, "%d\n", &len) == 1);
		a.resize(len);
		opt.resize(len);
		for (int i = 0; i < len; i++) {
			char op;
			size_t pos;
			_wa_ = fscanf(tra, "%c %lu\n", &op, &pos);
			assert(op == 'R' || op == 'W');
			assert(pos < MM);
			a[i] = pos, opt[i] = op;
		}
		fclose(tra);
		return true;
	}

	double analyze_trace(int n, page_rp & algo)
	{
		algo.reset(n);
		for (int i = 0; i < a.size(); i++) {
			if (opt[i] == 'R') algo.read(a[i]);
			else if (opt[i] == 'W') algo.write(a[i]);
			else assert(false);
		}
		return 1.0*algo.pull/get_opt(a.size(), MM, n, a, opt);
	}

	// analyze using current trace
	void analyze_trace(page_rp & algo, bool rand = false)
	{
		vector<double> rat;
		rat.resize((Nr-Nl)/Nstep + 1);
		const int T = 5;
		for (int j = 0; j < T; j++) {
			for (int n = Nl, i = 0; n <= Nr; n += Nstep, i++) {
				rat[i] += analyze_trace(n, algo);
			}
		}
		for (int i = 0; i < rat.size(); i++)
			rat[i] /= T;
		cout << "Algorithm name : " << algo.name << endl;
		for (int i = 0; i < rat.size(); i++)
			cout << fixed << setprecision(3) << rat[i] << "\t";
		cout << endl;
	}

	// analyze opt algorithm in random situation
	void analyze_opt_random()
	{
		cout << "OPT Miss Ratio : M = " << MM << ", len = " << Rlen << endl;
		opt.resize(Rlen);
		a.resize(Rlen);
		for (int n = Nl; n <= Nr; n += Nstep) {
			for (int j = 0; j < Rlen; j++) {
				if (rand()&1) {
					opt[j] = 'R';
					a[j] = rand()%MM;
				} else {
					opt[j] = 'L';
					a[j] = rand()%MM;
				}
			}
			int miss = get_opt(Rlen, MM, n, a, opt);
			cout << fixed << setprecision(3) << 1.0*miss/Rlen << "\t";
		}
		cout << endl;
	}

	
	/* analyze using random sequence 
	 * @len     : memory access sequence length
	 * @M       : logical page number
	 * @n       : physical memory page number
	 * @page_rp : algorithm
	 * return opt pull number
	 */
	int analyze_random(int len, int M, int n, page_rp & algo)
	{
		algo.reset(n);
		static int a[MAX_LEN], opt[MAX_LEN];
		assert(len <= MAX_LEN);
		for (int i = 0; i < len; i++) {
			opt[i] = rand()&1;
			if (opt[i]) algo.read(a[i] = rand()%M);
			else algo.write(a[i] = rand()%M);
		}
		return get_opt(len, M, n, a, opt);
	}

	void analyze_random(page_rp & algo)
	{
		cout << "Algorithm : " << algo.name << ", Len = " << Rlen
		     << ", M = " << MM << ", N = " << Nl << " .. " << Nr
		     << " by " << Nstep << endl;
		const int T = 500, Times = 5;
		static int miss[T], push[T], pull[T];
		static double comp[T];
		for (int n = Nl, i = 0; n <= Nr; n += Nstep, i++) {
			comp[i] = miss[i] = push[i] = pull[i] = 0;
			for (int j = 1; j <= Times; j++) {
				int t = analyze_random(Rlen, MM, n, algo);
				assert(1.0*algo.miss/t >= 1);
				comp[i] += 1.0*algo.miss/t;
				miss[i] += algo.miss;
				push[i] += algo.push;
				pull[i] += algo.pull;
			}
			miss[i] /= Times;
			push[i] /= Times;
			pull[i] /= Times;
			comp[i] /= Times;
		}
	
		int n = (Nr - Nl) / Nstep + 1;

		/* for (int i = 0; i < n; i++)
			cout << miss[i] << "\t";
		cout << endl;
		for (int i = 0; i < n; i++)
			cout << push[i] << "\t";
		cout << endl;
		for (int i = 0; i < n; i++)
			cout << pull[i] << "\t";
			cout << endl; */
		
		for (int i = 0; i < n; i++)
			cout << fixed << setprecision(3) << comp[i] << "\t";
		cout << endl;
	}

public:

	// analyze all trace file

	void analyze();
	
} analyzer;

void init()
{
	srand(time(0));
}

#include "analyze_list.cpp"
