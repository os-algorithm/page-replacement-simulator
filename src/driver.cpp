inline int get_opt(int len, int M, int n, int a[], int opt[])
{
	static int nxt[MAX_LEN];
	assert(len <= MAX_LEN);
	map<int, int> T;
	for (int i = len-1; i >= 0; i--) {
		if (!T.count(a[i])) nxt[i] = len;
		else nxt[i] = T[a[i]];
	}
}

// logical page number, physical page number, algorithm
// return opt pull number
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

void analyze(page_rp & algo)
{
	int len = 512;
	int M = 512;
	int Nl = 8, Nr = 512, Nstep = 4;
	const int T = 500;
	cout << "Algorithm : " << algo.name << ", Len = " << len << ", M = " << M << ", N = " << Nl << " .. " << Nr << " by " << Nstep << endl;
	static int miss[T], push[T], pull[T];
	for (int n = Nl, i = 0; n <= Nr; n += Nstep, i++) {
		miss[i] = push[i] = pull[i] = 0;
		for (int j = 1; j <= 20; j++) {
			analyze_random(len, M, n, algo);
			miss[i] += algo.miss;
			push[i] += algo.push;
			pull[i] += algo.pull;
		}
		miss[i] /= 20;
		push[i] /= 20;
		pull[i] /= 20;
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
}

void analyze()
{
	analyze(rp_ran_algo);
}

void init()
{
	srand(time(0));
}
