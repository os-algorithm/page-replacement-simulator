#include <map>
#include <assert.h>
#include <vector>
#include "header/trace.h"
using std::vector;
using std::map;

/* opt algorithm, minimize miss(pull) time.
 * @trace : memory access array
 * @n : size of physical memory size 
 */
size_t get_opt(const vector<ref_trace> &trace, int n)
{
	vector<size_t> nxt;
	int len = trace.size();
	nxt.resize(trace.size());
	map<int, int> T;
	for (int i = len-1; i >= 0; i--) {
		if (!T.count(trace[i].pos)) nxt[i] = len;
		else nxt[i] = T[trace[i].pos];
		T[trace[i].pos] = i;
	}
	map<int,int> st;
	int miss = 0;
	for (int i = 0; i < len; i++) {
		if (st.count(trace[i].pos)) {
			st[trace[i].pos] = nxt[i];
			// hit.
			// update next ref
			continue;
		}
		miss++;
		if (st.size() < n) {
			st[trace[i].pos] = nxt[i];
		} else {
			map<int,int>::iterator j = st.end();
			for (map<int,int>::iterator k = st.begin(); k != st.end(); k++)
				if (j == st.end() || k->second > j->second)
					j = k;
			assert(j != st.end());
			st.erase(j);
			// swap out j
			st[trace[i].pos] = nxt[i];
			// swap in this page.
		}
	}
	return miss;
}
