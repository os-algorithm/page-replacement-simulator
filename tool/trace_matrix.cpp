#include <bits/stdc++.h>
using namespace std;

int main()
{
	int n = 1024;
	cout << n << endl;
	int pos = 0, cur = 0, B = 16;
	for (int i = 0; i < 256; i++) {
		cout << "R " << i << endl;
		cout << "W " << pos << endl;
		pos += B;
		if (pos > 256) 
			pos = ++cur;
	}
	return 0;
}
