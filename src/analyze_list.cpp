void Analyzer::analyze()
{
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
