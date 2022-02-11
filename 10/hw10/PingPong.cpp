#include <sstream>
#include <limits>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

enum
{
	FIRST_PROC = 1,
	SECOND_PROC = 2
};

std::condition_variable var;
std::mutex mut;
int working_proc = FIRST_PROC;
int iterations = 0;
int max_iterations = std::numeric_limits<int>::max();

void proc(int proc_num) {
	while (1) {
		std::unique_lock<std::mutex> lock(mut);
		while (working_proc != proc_num) {
			var.wait(lock);
		}
		++iterations;
		if (iterations > max_iterations) {
			working_proc = FIRST_PROC + SECOND_PROC - working_proc;
			var.notify_one();
			return;
		}
		std::cout << (proc_num == FIRST_PROC ? "ping " : "pong ");
		working_proc = FIRST_PROC + SECOND_PROC - working_proc;
		var.notify_one();
	}
}


int main(int argc, char *argv[])
{
	double a = 2;
	for (long long i = 0; i < 1e10; ++i) {
		a = a - 1 / a;
		if (i % 1e5 == 0) {
			std::cout << i << ' ' << a << '\n';
		}
	}
	return 0;
	if (argc == 2) {
		std::stringstream in(argv[1]);
		in >> max_iterations;
	}
	std::thread proc1(proc, FIRST_PROC);
	std::thread proc2(proc, SECOND_PROC);
	proc1.join();
	proc2.join();
	return 0;
}

