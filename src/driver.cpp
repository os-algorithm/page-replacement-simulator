// opt algorithm, minimize miss(pull) time. 
// further-in-future
inline int get_opt(int len, int M, int n, int a[], int opt[])
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
		if (opt[i]) algo.read(a[i] = i%M);
		else algo.write(a[i] = i%M);
		// cout << opt[i] << "," << a[i] << " ";
	}
	// cout << endl;
	return get_opt(len, M, n, a, opt);
}

void analyze(page_rp & algo)
{
	int len = 20000; 
	int M = 512;   
	int Nl = 8, Nr = 512, Nstep = 4;
	cout << "Algorithm : " << algo.name << ", Len = " << len
	     << ", M = " << M << ", N = " << Nl << " .. " << Nr
	     << " by " << Nstep << endl;
	const int T = 500, Times = 5;
	static int miss[T], push[T], pull[T];
	static double comp[T];
	
	for (int n = Nl, i = 0; n <= Nr; n += Nstep, i++) {
		comp[i] = miss[i] = push[i] = pull[i] = 0;
		for (int j = 1; j <= Times; j++) {
			int t = analyze_random(len, M, n, algo);
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
	for (int i = 0; i < n; i++)
		cout << miss[i] << "\t";
	cout << endl;
	for (int i = 0; i < n; i++)
		cout << push[i] << "\t";
	cout << endl;
	for (int i = 0; i < n; i++)
		cout << pull[i] << "\t";
	cout << endl;
	for (int i = 0; i < n; i++)
		cout << fixed << setprecision(3) << comp[i] << "\t";
	cout << endl;
}

void analyze()
{
	analyze(rp_ran_algo);
	analyze(rp_fifo_algo);
}

void init()
{
	srand(time(0));
}
