// Performance2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Performance2.h"

#include <vector>
#include <iostream>
#include <deque>
#include <GdiPlusHeaders.h>
#include <GdiPlusColor.h>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Timer - used to established precise timings for code.
class TIMER
{
	LARGE_INTEGER t_;

	__int64 current_time_;

public:
	TIMER()	// Default constructor. Initialises this timer with the current value of the hi-res CPU timer.
	{
		QueryPerformanceCounter(&t_);
		current_time_ = t_.QuadPart;
	}

	TIMER(const TIMER &ct)	// Copy constructor.
	{
		current_time_ = ct.current_time_;
	}

	TIMER& operator=(const TIMER &ct)	// Copy assignment.
	{
		current_time_ = ct.current_time_;
		return *this;
	}

	TIMER& operator=(const __int64 &n)	// Overloaded copy assignment.
	{
		current_time_ = n;
		return *this;
	}

	~TIMER() {}		// Destructor.

	static __int64 get_frequency()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return frequency.QuadPart;
	}

	__int64 get_time() const
	{
		return current_time_;
	}

	void get_current_time()
	{
		QueryPerformanceCounter(&t_);
		current_time_ = t_.QuadPart;
	}

	inline bool operator==(const TIMER &ct) const
	{
		return current_time_ == ct.current_time_;
	}

	inline bool operator!=(const TIMER &ct) const
	{
		return current_time_ != ct.current_time_;
	}

	__int64 operator-(const TIMER &ct) const		// Subtract a TIMER from this one - return the result.
	{
		return current_time_ - ct.current_time_;
	}

	inline bool operator>(const TIMER &ct) const
	{
		return current_time_ > ct.current_time_;
	}

	inline bool operator<(const TIMER &ct) const
	{
		return current_time_ < ct.current_time_;
	}

	inline bool operator<=(const TIMER &ct) const
	{
		return current_time_ <= ct.current_time_;
	}

	inline bool operator>=(const TIMER &ct) const
	{
		return current_time_ >= ct.current_time_;
	}
};

CWinApp theApp;  // The one and only application object

using namespace std;

class ThreadPool;

// our worker thread objects
class Worker {
public:
	Worker(ThreadPool &s) : pool(s) { }
	void operator()();
private:
	ThreadPool &pool;
};

// the actual thread pool
class ThreadPool {
public:
	ThreadPool(size_t);
	template<class F>
	void enqueue(F f, int x);
	~ThreadPool();
private:
	friend class Worker;

	// need to keep track of threads so we can join them
	std::vector< std::thread > workers;

	// the task queue
	std::deque< std::function<void(int)> > tasks;

	// synchronization
	std::mutex queue_mutex;
	std::condition_variable condition;
	bool stop;
};

void Worker::operator()()
{
	std::function<void(int)> task;
	while (true)
	{
		{   // acquire lock
			std::unique_lock<std::mutex>
				lock(pool.queue_mutex);

			// look for a work item
			while (!pool.stop && pool.tasks.empty())
			{ // if there are none wait for notification
				pool.condition.wait(lock);
			}

			if (pool.stop) // exit if the pool is stopped
				return;

			// get the task from the queue
			task = pool.tasks.front();
			pool.tasks.pop_front();

		}   // release lock

			// execute the task
		task(3);
	}
}

// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads)
	: stop(false)
{
	for (size_t i = 0; i<threads; ++i)
		workers.push_back(std::thread(Worker(*this)));
}

// the destructor joins all threads
ThreadPool::~ThreadPool()
{
	// stop all threads
	stop = true;
	condition.notify_all();

	// join them
	for (size_t i = 0; i<workers.size(); ++i)
		workers[i].join();
}

LPCWSTR inputFilenames[] = { L"IMG_1.JPG", L"IMG_2.JPG" , L"IMG_3.JPG" , L"IMG_4.JPG" , 
L"IMG_5.JPG" , L"IMG_6.JPG" , L"IMG_7.JPG", L"IMG_8.JPG", L"IMG_9.JPG", L"IMG_10.JPG", L"IMG_11.JPG",L"IMG_12.JPG" };

LPCWSTR outputFilenames[] = { L"IMG_1.PNG", L"IMG_2.PNG" , L"IMG_3.PNG" , L"IMG_4.PNG" ,
L"IMG_5.PNG" , L"IMG_6.PNG" , L"IMG_7.PNG", L"IMG_8.PNG", L"IMG_9.PNG", L"IMG_10.PNG", L"IMG_11.PNG",L"IMG_12.PNG" };

template<class F>
void ThreadPool::enqueue(F f, int x)
{
	{ // acquire lock
		std::unique_lock<std::mutex> lock(queue_mutex);

		// add the task

		std::function<void(int)> func = std::bind(f, x);

		tasks.push_back(func);
	} // release lock

	  // wake up one thread
	condition.notify_one();
}

class TImage : public CImage
{
public:
	TImage processImages() {
		TImage image;

		resize(image);
		greyscale(image);
		rotate(image);
		//bilinearFilter(image);

		return image;
	}

private:
	void greyscale(TImage &image) {

		int width = image.GetWidth();
		int height = image.GetHeight();

		long lAdrs;
		double grey;
		BYTE* pInImage = (BYTE*)image.GetBits();
		BYTE bRed, bGreen, bBlue;
		int pitch = image.GetPitch();

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				lAdrs = y * pitch + x * 3;

				bRed = *(pInImage + lAdrs);
				bGreen = *(pInImage + lAdrs + 1);
				bBlue = *(pInImage + lAdrs + 2);
				
				grey =  bRed*0.587 + bGreen*0.299 + bBlue*0.114;	

				*(pInImage + lAdrs) = static_cast<BYTE>(grey);
				*(pInImage + lAdrs + 1) = static_cast<BYTE>(grey);
				*(pInImage + lAdrs + 2) = static_cast<BYTE>(grey);
			}
		}
	}

	void rotate(TImage &image) {
		Bitmap* gdiPlusBitmap = Bitmap::FromHBITMAP(image, NULL);
		gdiPlusBitmap->RotateFlip(Rotate90FlipNone);
		HBITMAP hbmp;
		gdiPlusBitmap->GetHBITMAP(Color::White, &hbmp);
		image.Attach(hbmp);
	}

	void resize(TImage &image) {
		int width = this->GetWidth() / 2;
		int height = this->GetHeight() / 2;

		image.Create(width, height, this->GetBPP());

		this->StretchBlt(image.GetDC(), 0, 0, width, height, SRCCOPY);
	}

	void bilinearFilter(TImage &image) {
		int width = image.GetWidth();
		int height = image.GetHeight();

		long lAdrs;

		long lAdrs1;
		long lAdrs2;
		long lAdrs3;
		long lAdrs4;

		BYTE* pInImage = (BYTE*)image.GetBits();
		BYTE bRed, bGreen, bBlue;
		int pitch = image.GetPitch();
		double newColour;

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				if ((x > 0) && (y > 0) && (x < width) & (y < height))
				{
					lAdrs = y * pitch + x * 3;

					lAdrs1 = (y-1) * pitch + x * 3;
					lAdrs2 = y * pitch + (x+1) * 3;
					lAdrs3 = (y+1) * pitch + x * 3;
					lAdrs4 = y * pitch + (x-1) * 3;

					int px = (int)x; // floor of x
					int py = (int)y;

					float fx = x - px;
					float fy = y - py;
					float fx1 = 1.0f - fx;
					float fy1 = 1.0f - fy;

					int w1 = fx1 * fy1 * 256.0f;
					int w2 = fx  * fy1 * 256.0f;
					int w3 = fx1 * fy  * 256.0f;
					int w4 = fx  * fy  * 256.0f;

					int outr = *(pInImage + lAdrs1) * w1 + *(pInImage + lAdrs2) * w2 + *(pInImage + lAdrs3) * w3 + *(pInImage + lAdrs4) * w4;
					int outg = *(pInImage + lAdrs1 + 1) * w1 + *(pInImage + lAdrs2 + 1) * w2 + *(pInImage + lAdrs3 + 1) * w3 + *(pInImage + lAdrs4 + 1) * w4;
					int outb = *(pInImage + lAdrs1 + 2) * w1 + *(pInImage + lAdrs2 + 2) * w2 + *(pInImage + lAdrs3 + 2) * w3 + *(pInImage + lAdrs4 + 2) * w4;

					newColour = outr + outg + outb;

					*(pInImage + lAdrs) = static_cast<BYTE>(newColour);
					*(pInImage + lAdrs + 1) = static_cast<BYTE>(newColour);
					*(pInImage + lAdrs + 2) = static_cast<BYTE>(newColour);
				}
			}
		}
	}


};

void work(int x) {
	TImage image;

	image.Load(inputFilenames[x]);
	image.processImages().Save(outputFilenames[x]);
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize Microsoft Foundation Classes, and print an error if failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// Application starts here...

		// Time the application's execution time.
		TIMER start;	// DO NOT CHANGE THIS LINE

						//--------------------------------------------------------------------------------------
						// Process the images...   // Put your code here...

		ThreadPool threadpool(4);

		for (size_t i = 0; i < 12; i++)
		{
			threadpool.enqueue(&work, i);
		}



		//-------------------------------------------------------------------------------------------------------

		// How long did it take?...   DO NOT CHANGE FROM HERE...

		TIMER end;

		TIMER elapsed;

		elapsed = end - start;

		__int64 ticks_per_second = start.get_frequency();

		// Display the resulting time...

		double elapsed_seconds = (double)elapsed.get_time() / (double)ticks_per_second;

		cout << "Elapsed time (seconds): " << elapsed_seconds;
		cout << endl;
		cout << "Press a key to continue" << endl;

		char c;
		cin >> c;
	}

	return nRetCode;
}
