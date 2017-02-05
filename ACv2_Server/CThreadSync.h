#pragma once

#include <boost/thread.hpp>
#include <queue>
#include <string>
#include <vector>

typedef void(*ThreadSyncFunc_t)(void *param);
typedef std::pair<ThreadSyncFunc_t, void*> ThreadSyncFuncCall_t;

class CThreadSync
{
public:
	CThreadSync(unsigned int waitTimeForLock = 10, unsigned int waitTimeTotal = 20);
	~CThreadSync();

	// NOTE: param must be a pointer to a single element allocated on heap, or NULL (not array!!!)
	// NOTE: allocation must be done by the calling function 
	// NOTE: deallocation is done internally by CThreadSync::Process()
	void AddCallbackToQueue(ThreadSyncFunc_t func, void *param);

	void Process();

// ======================================================================================
// Callbacks:

	struct OnCheaterCheckResponse__parameters {
		int playerid;
		bool ischeater;
	};
	static void OnCheaterCheckResponse(void *param);

	struct OnCheatsUpdate__parameters {
		std::vector<std::string> ProcessMD5s;
		std::vector<std::string> MD5s;
		std::vector<std::string> FileNames;
	};
	static void OnCheatsUpdate(void *param);

private:
	boost::timed_mutex _mutex;
	std::queue<ThreadSyncFuncCall_t> _queue;
	unsigned int _waitTimeForLock;
	unsigned int _waitTimeTotal;
};