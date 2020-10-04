
#include <iostream>
#include "ThreadPoolLib.h"

void testProc(void* args) {
	std::cout << "asd\n";
	//platform specific sleep
}



int main()
{
	std::ostream s(std::cout.rdbuf());
	ThreadPool tp(3, &s);
	for (int i = 0; i < 10; i++) {
		tp.run(testProc, nullptr);
	}
	tp.waitAll();
	return 0;
}