#ifndef LAB2_H
#define LAB2_H

#include <vector>


enum ins { program_name = 0, scheduler = 1, wait_strategy = 2, rounds = 3, iterations = 4, number_times=5, };
enum scheduler_nameSelection { SCHED_RR, SCHED_FIFO, SCHED_NORMAL,};
enum wait_strategies{spin,sleep,};
enum results_code {
	success, scheduler_notcorrect, waitstrategy_notcorrect, rounds_type = 5, argnum=6, num_type, toofewargc,unknown_exception = -1, iteration_type = -3,
};
int start(int argc, char* argv[], std::vector<int>& num);
int usage(char * program_name, int error_value);
bool checkinput(char* a);
#endif