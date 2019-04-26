#pragma once
#include <condition_variable>
#include <thread>
#include <vector>
#include <queue>
#include <functional>

#include <string>
#include "Camera.h"
#include "Vector3.h"
#include <atomic>
#include "BoundingVolumeHierarchy.h"

struct ThreadTask
{
	ThreadTask()
	{}

	ThreadTask(int samples, int yStart, int yEnd, int width, int height, std::vector<Vector3>* pixelColour, Camera* cam, std::atomic<int>* counter, std::string* string, BoundingVolumeHierarchy* bvh)
	{
		_samples = samples;
		_yStart = yStart;
		_yEnd = yEnd;
		_width = width;
		_height = height;
		_pixelColour = pixelColour;
		_cam = cam;
		_counter = counter;
		_string = string;
		_bvh = bvh;
	}

	ThreadTask(int samples, int yStart, int yEnd, int xStart, int xEnd, int width, int height, std::vector<Vector3>* pixelColour, Camera* cam, std::atomic<int>* counter, std::string* string, BoundingVolumeHierarchy* bvh)
	{
		_samples = samples;
		_yStart = yStart;
		_yEnd = yEnd;
		_xStart = xStart;
		_xEnd = xEnd;
		_width = width;
		_height = height;
		_pixelColour = pixelColour;
		_cam = cam;
		_counter = counter;
		_string = string;
		_bvh = bvh;
	}

	int _samples;
	int _yStart;
	int _yEnd;
	int _xStart;
	int _xEnd;
	int _width;
	int _height;
	std::vector<Vector3>* _pixelColour = nullptr;
	Camera* _cam;
	std::atomic<int>* _counter = nullptr;
	std::string* _string = nullptr;
	BoundingVolumeHierarchy* _bvh;

	void(*taskFunction) (int samples, int yStart, int yEnd, int width, int height, std::vector<Vector3>* pixelColour, Camera cam, std::atomic<int>* counter, std::string* string, BoundingVolumeHierarchy* bvh);

	void(*taskFunction2) (int samples, int yStart, int yEnd, int xStart, int xEnd, int width, int height, std::vector<Vector3>* pixelColour, Camera cam, std::atomic<int>* counter, std::string* string, BoundingVolumeHierarchy* bvh);
};

//Thread pool is based on this implementation, adapted to my purpose https://youtu.be/eWTGtp3HXiw

class ThreadPool
{
public:
	//using Task = std::function<void(int samples, int yStart, int yEnd, int width, int height, std::vector<Vector3>* pixelColour, Camera _cam, std::atomic<int>* _counter, std::string* _string, BoundingVolumeHierarchy* _bvh)>;
	using Task = ThreadTask;

	explicit ThreadPool(std::size_t numberOfThreads)
	{
		start(numberOfThreads);
	}
	~ThreadPool();

	void enqueue(Task _task);

	void CallForStop() { stop(); }

	bool GetCompleted() { return complete; }

	bool IsQueueEmpty() { return taskQueue.empty(); }

private:
	void start(std::size_t _numberOfThreads);
	void stop();

	std::vector<std::thread> threads;
	std::condition_variable eventVariable;
	std::mutex eventMutex;

	bool stopping = false;
	bool complete = false;
	std::queue<Task> taskQueue;

};

