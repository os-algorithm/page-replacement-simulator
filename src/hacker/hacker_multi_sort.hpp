/* Simple Test Case
 * multi thread : sort
 */

class multi_sort_hacker : public hacker {

public:

	multi_sort_hacker()
	{ name = "multi sort hacker"; }

	void sort(Array<int> a, Array<int> tmp, int L, int R)
	{
		if (L >= R) return;
		int mid = (L+R) >> 1;
		sort(a, tmp, L, mid), sort(a, tmp, mid+1, R);
		int top = L, i = L, j = mid+1;
		while (i <= mid && j <= R) {
			if (a[i] < a[j]) tmp[top++] = a[i++];
			else tmp[top++] = a[j++];
		}
		while (i <= mid) tmp[top++] = a[i++];
		while (j <= R) tmp[top++] = a[j++];
		for (int i = L; i <= R; i++)
			a[i] = tmp[i];
	}
	
	void sort_thread(int n)
	{
		Array<int> a = sim->new_array<int>(n);
		Array<int> tmp = sim->new_array<int>(n);
		for (int i = 0; i < n; i++)
			a[i] = i;
		for (int i = 0; i < n; i++) {
			int pos = rand() % (i + 1);
			int p = a[pos], q = a[i];
			a[pos] = q, a[i] = p;
		}
		sort(a, tmp, 0, n-1);
		for (int i = 0; i < n; i++)
			assert(a[i] == i);
	}
	
	virtual void main(Simulater *sim)
	{
		this->sim = sim;
		sort_thread(1 << 18);
	}
} ;

