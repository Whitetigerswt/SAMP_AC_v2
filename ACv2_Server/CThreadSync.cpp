#include "CThreadSync.h"
#include "CAntiCheat.h"
#include "CAntiCheatHandler.h"

CThreadSync::CThreadSync(unsigned int waitTimeForLock, unsigned int waitTimeTotal)
{
	_waitTimeForLock = waitTimeForLock;
	_waitTimeTotal = waitTimeTotal;
}

CThreadSync::~CThreadSync()
{

}

void CThreadSync::AddCallbackToQueue(ThreadSyncFunc_t func, void *param)
{
	boost::timed_mutex::scoped_lock(_mutex);
	_queue.push(std::make_pair(func, param));
}

void CThreadSync::Process()
{
	// Wait no more than "_waitTimeForLock" milliseconds to acquire lock, so we won't be blocking calling thread for too long
	boost::system_time time = boost::get_system_time();
	boost::timed_mutex::scoped_lock scoped_lock(_mutex, time + boost::posix_time::milliseconds(_waitTimeForLock));
	
	if (scoped_lock.owns_lock()) 
	{
		// Wait no more than "_waitTimeTotal" (since the beginning of this procedure) for same reason as above
		boost::system_time exit_time = time + boost::posix_time::milliseconds(_waitTimeTotal);
		ThreadSyncFuncCall_t temp;

		while (!_queue.empty() && exit_time >= boost::get_system_time())
		{
			// Retrieve callback function and a pointer to a param struct
			temp = _queue.front();

			// Call the function
			temp.first(temp.second);

			// Delete parameter pointer
			if (temp.second != NULL)
				delete temp.second;

			// Done with this entry!
			_queue.pop();
		}
	}
}

void CThreadSync::OnCheaterCheckResponse(void *param)
{
	OnCheaterCheckResponse__parameters *p = reinterpret_cast<OnCheaterCheckResponse__parameters*>(param);
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(p->playerid);
	if(ac)
	{ 
		ac->OnBanChecked(p->ischeater);
	}
}