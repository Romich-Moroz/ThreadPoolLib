// ThreadPoolLib.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <map>
#include <string>
// TODO: Reference additional headers your program requires here.

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#elif __linux__
#include <unistd.h>
#include <pthread.h>
#endif

typedef void (*Procedure)(void*);

class ThreadPool {
private:
	class ThreadStatus {
	private:
		std::ostream* logStream;

	public:
		ThreadStatus(int id, std::ostream* logStream);
		ThreadStatus();
		bool isWorking;
#ifdef _WIN32
		void* threadHandle;
#elif __linux__
		pthread_t threadHandle;
#endif		
		int id;
		Procedure func;
		void* funcArgs;
		void writeLog(std::string message);
	};
	std::map<int,ThreadStatus> threadMap;
	std::ostream* logStream;
	int threadLimit;


	int getFreeThreadId();
	int addThread();
#ifdef _WIN32
	static unsigned int _stdcall funcWrapper(void* args);
#elif __linux__
	static void* funcWrapper(void* args);
#endif

public:
	ThreadPool(int maxTasks, std::ostream* logStream);

	void run(Procedure proc, void* args);
	void waitAll();
};
