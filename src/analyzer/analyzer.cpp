#include <dirent.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <set>

using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::fixed;
using std::setprecision;
using std::set;

#include "../header/trace.h"
#include "../page/page_rp.h"
#include "../page/rp_random.hpp"
#include "../page/rp_lru.hpp"
#include "../page/rp_clock.hpp"
#include "../page/rp_fifo.hpp"
#include "../page/rp_marking.hpp"

extern size_t get_opt(const vector<ref_trace>&, int);

// maximum of logical memory pages 
const int MM = 512;
// number of memory frames
const int Nl = 8, Nr = 512, Nstep = 4;
// sequence length for random OPT test 
const int Rlen = 10000;

const char *trace_path = "/mnt/c/Users/ljt12138/Desktop/oslab/sim_ran/trace";

/* Analyzer using trace frome /trace_path/*.trace
 * display efficiency of replacement algorithm 
 * when memory size if Nl .. Nr by Nstep pages
 */
class Analyzer {

	vector<ref_trace> trace;
	
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
		trace.resize(len);
		for (int i = 0; i < len; i++) {
			char op;
			size_t pos;
			_wa_ = fscanf(tra, "%c %lu\n", &op, &pos);
			assert(op == 'R' || op == 'W');
			assert(pos < MM);
			trace[i].pos = pos, trace[i].type = (op == 'R' ? Read : Write);
		}
		fclose(tra);
		return true;
	}

	double analyze_trace(int n, page_rp & algo)
	{
		algo.reset(n);
		for (int i = 0; i < trace.size(); i++) {
			if (trace[i].type == Read) algo.read(trace[i].pos);
			else if (trace[i].type == Write) algo.write(trace[i].pos);
			else assert(false);
		}
		return 1.0*algo.pull/get_opt(trace, n);
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
		trace.resize(Rlen);
		for (int n = Nl; n <= Nr; n += Nstep) {
			for (int j = 0; j < Rlen; j++) {
				if (rand()&1) {
					trace[j].type = Read;
					trace[j].pos = rand()%MM;
				} else {
					trace[j].type = Write;
					trace[j].pos = rand()%MM;
				}
			}
			int miss = get_opt(trace, n);
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
		vector<ref_trace> trace;
		trace.resize(len);
		for (int i = 0; i < len; i++) {
			trace[i].type = ((rand()&1) ? Read : Write);
			if (trace[i].type == Read) algo.read(trace[i].pos = rand()%M);
			else algo.write(trace[i].pos = rand()%M);
		}
		return get_opt(trace, n);
	}

	void analyze_random(page_rp & algo)
	{
		cout << "Algorithm : " << algo.name << ", Len = " << Rlen
		     << ", M = " << MM << ", N = " << Nl << " .. " << Nr
		     << " by " << Nstep << endl;

		const int Times = 5;
		vector<int> miss, push, pull;
		miss.resize(500), push.resize(500), pull.resize(500);
		vector<double> comp;
		comp.resize(500);
		
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

		cout << "QWQ" << endl;
		int n = (Nr - Nl) / Nstep + 1;
		
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


void Analyzer::analyze()
{
	rp_marking rp_marking_algo;
	rp_fifo rp_fifo_algo;
	rp_lru rp_lru_algo;
	rp_clock rp_clock_algo;
	rp_ran rp_ran_algo;
	
	analyze_opt_random();
	analyze_random(rp_ran_algo);
	analyze_random(rp_marking_algo);
	analyze_random(rp_fifo_algo);

	DIR * dir = opendir(trace_path);
	dirent * ptr;

	while((ptr = readdir(dir)) != NULL) {
		if (read_trace(ptr)) {
			analyze_trace(rp_ran_algo, true);
			analyze_trace(rp_fifo_algo);
			analyze_trace(rp_lru_algo);
			analyze_trace(rp_clock_algo);
			analyze_trace(rp_marking_algo, true);
		}
	}
}

void analyze_main()
{
	init();
	analyzer.analyze();
}
