// ThreadPoolLib.cpp : Defines the entry point for the application.
//

#include "ThreadPoolLib.h"

ThreadPool::ThreadStatus::ThreadStatus(int id, std::ostream* logStream) {
	this->id = id;
	this->logStream = logStream;
	this->func = nullptr;
	this->funcArgs = nullptr;
	this->isWorking = false;
	this->threadHandle = 0;
}
ThreadPool::ThreadStatus::ThreadStatus() {}

void ThreadPool::ThreadStatus::writeLog( std::string message) {
	this->logStream->write(message.c_str(), message.length());
}

ThreadPool::ThreadPool(int maxTasks, std::ostream* logStream) {
	this->logStream = logStream;
	this->threadLimit = maxTasks;
	for (int i = 0; i < maxTasks; i++) {
		addThread();
	}
};

int ThreadPool::getFreeThreadId() {
	for (int i = 0; i < this->threadMap.size(); i++) {
		if (!this->threadMap[i].isWorking) {
			return i;
		}
	}
	return -1;
}

int ThreadPool::addThread() {
	int newThreadId = (int)this->threadMap.size();
	this->threadMap.emplace(newThreadId, ThreadStatus(newThreadId, this->logStream));
	if (newThreadId >= threadLimit) {
		this->threadLimit++;
		this->threadMap[newThreadId].writeLog("Warning: Thread limit is reached, increasing thread limit\n");
	}
	this->threadMap[newThreadId].writeLog("Message: New thread created, id = " + std::to_string(newThreadId) + '\n');
	return newThreadId;
}

#ifdef _WIN32
unsigned int ThreadPool::funcWrapper(void* args) 
#elif __linux__
void* ThreadPool::funcWrapper(void* args)
#endif
{

	auto params = (ThreadStatus*)(args);

	try {
		params->func(params->funcArgs);
	}
	catch (std::exception& e) {
		params->writeLog("Exception: " + std::string(e.what()) + " , thread id = " + std::to_string(params->id) + '\n');
	}

#ifdef _WIN32
	CloseHandle(params->threadHandle);
#elif __linux__
	
#endif	
	params->threadHandle = 0;
	params->isWorking = false;
	params->func = nullptr;

	return 0;
}

void ThreadPool::waitAll() {
	for (int i = 0; i < this->threadLimit; i++) {
		if (this->threadMap[i].isWorking) {
#ifdef _WIN32
			WaitForSingleObjectEx(this->threadMap[i].threadHandle, INFINITE, FALSE);
#elif __linux__
			pthread_join(this->threadMap[i].threadHandle, NULL);
#endif
		}
	}
}


void ThreadPool::run(Procedure proc, void* args)
{
	int freeThreadId = getFreeThreadId();
	if (freeThreadId == -1) {
		freeThreadId = addThread();
	}

	auto params = &this->threadMap[freeThreadId];
	
	params->func = proc;
	params->funcArgs = args;
	params->isWorking = true;
	params->writeLog("Message: Task was added (Thread id = " + std::to_string(freeThreadId) + ")\n");

#ifdef _WIN32
	params->threadHandle = (void *)_beginthreadex(0, 0, &ThreadPool::funcWrapper, params, 0, 0);
#elif __linux__
	pthread_create(&params->threadHandle, NULL, &ThreadPool::funcWrapper, params);
#endif
}



