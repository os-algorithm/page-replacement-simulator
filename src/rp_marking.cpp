/* Random Marking Algorithm
 * is k-Competitve
 */
class rp_marking : public page_rp {

	set<int> unmarked;
	
public:

	rp_marking()
	{
		memset(name, 0, sizeof name);
		strcpy(name, "marking");
	}

	
	
};
