#include "ThreadPool.h"


ThreadPool::~ThreadPool()
{
	stop();
}

void ThreadPool::enqueue(Task _task)
{
	//Scope added here to unlock mutex asap.
	{
		std::unique_lock<std::mutex> lock{ eventMutex };
		taskQueue.emplace(std::move(_task));
	}

	eventVariable.notify_one();
}

void ThreadPool::start(std::size_t _numberOfThreads)
{
	for(auto i = 0; i < _numberOfThreads; i++)
	{
		threads.emplace_back([=]
		{
			while(true)
			{
				Task* currentTask = nullptr;

				//Scope added here to unlock mutex asap. Keeping critical section small.
				{
					std::unique_lock<std::mutex> lock{ eventMutex };

					eventVariable.wait(lock, [=] { return stopping || !taskQueue.empty(); });

					if(stopping)
					{
						break;
					}

					if(taskQueue.empty())
					{
						stop();
					}

					currentTask = &taskQueue.front();
					taskQueue.pop();
				}

				currentTask->foo(
					currentTask->_samples, currentTask->_yStart, currentTask->_yEnd,
					currentTask->_width, currentTask->_height, currentTask->_pixelColour,
					*currentTask->_cam, currentTask->_counter, currentTask->_string, currentTask->_bvh);
			}
		});
	}
}

void ThreadPool::stop()
{
	//Scope added here to unlock mutex asap.
	{
		std::unique_lock<std::mutex> lock{ eventMutex };
		stopping = true;
	}

	eventVariable.notify_all();

	for(auto& thread : threads)
	{
		thread.join();
	}
	complete = true;
}
