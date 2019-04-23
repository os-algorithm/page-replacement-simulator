/* Page replacement simulater
 * @ljt12138
 */
#include <cstdlib>
#include <ctime>

extern void analyze_main();
extern void hacker_main();

int main()
{
	srand(time(0));
	// analyze_main();
	hacker_main();
	return 0;
}

