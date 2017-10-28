// 422lab2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "lab2.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
int main(int argc,char* argv[])
{

	try {
		vector<int> numbers;
		start(argc, argv,numbers);
		cout<<"vector numbers:"<<endl;
		for (auto i = numbers.begin(); i != numbers.end(); ++i){
			std::cout << *i << ' ';
		}
		//if none of the game could be start, it means the command is wrong 
	}
	// print out the bad alloc with usage message or any other results code and its corresponding error message
	catch (results_code a) {
		return usage(argv[program_name], a);
	}
}
int start(int argc, char* argv[], vector<int>& num){
	int roundnum;
	int iterationnum;
	if (argc < argnum) {
			throw toofewargc;
		}
		else if ((string(argv[scheduler]) != "SCHED_RR") &&(string(argv[scheduler]) != "SCHED_FIFO") && (string(argv[scheduler]) != "SCHED_NORMAL")) {

			throw scheduler_notcorrect;
		}
		else if ((string(argv[wait_strategy]) != "spin") &&(string(argv[wait_strategy]) != "sleep")) {
			throw waitstrategy_notcorrect;
		}
		else if (!checkinput(argv[rounds])){
			throw rounds_type;
		}
		else if (!checkinput(argv[iterations])){
			throw iteration_type;
		}
		else{ 
			for (int i=number_times;i<argc;++i){
				if(!checkinput(argv[i])) throw num_type;
				else {
					istringstream ss(argv[i]);
					int x;
					ss>>x;
					num.push_back(x);
				}
			}
			return 0;
		}
}
bool checkinput(char* a){
	istringstream ss(a);
	int x;
	if(!(ss>>x)) return false;
	ss>>x;
	if(x<=0) return false; 
	else return true;
}

int usage(char * program_name, int error_value)// print out the program's usage when argument number is false or other error happens
{
	if (error_value == scheduler_notcorrect) {
		cout << "Scheduler not correct, type /SCHED_RR/ OR /SCHED_FIFO/ OR /SCHED_NORMAL/ please."<<endl;
		return error_value;
	}
	else if (error_value == waitstrategy_notcorrect) {
		cout << "Wait Strategy not correct, type /spin/ or /sleep/ please."<<endl;
		return error_value;
	}
	else if (error_value== rounds_type){
		cout<< "Round type not correct, type an int please."<<endl;
		return error_value;
	}
	else if (error_value== iteration_type){
		cout<< "Iteration type not correct, type an int please."<<endl;
		return error_value;
	}
	else if (error_value == toofewargc) {
		cout << "Too few arguments <program_name> <scheduler> <wait-strategy> <rounds> <iterations> <number>+" << endl;
		return error_value;
	}
	else return error_value;
}
