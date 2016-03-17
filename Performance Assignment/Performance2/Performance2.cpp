// Performance2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Performance2.h"

#include <vector>
#include <iostream>

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


CImage* Copy(CImage *source)
{
	CImage *dest = new CImage;
	dest->Create(source->GetWidth(), source->GetHeight(), source->GetBPP());
	source->Draw(dest->GetDC(), 0, 0, dest->GetWidth(), dest->GetHeight(), 0, 0, source->GetWidth(), source->GetHeight());
	dest->ReleaseDC();
	return dest;
}

CImage* Brighten(CImage *i)
{
	CImage *dest = Copy(i);

	int width = dest->GetWidth();
	int height = dest->GetHeight();

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			COLORREF pixel = dest->GetPixel(x, y);

			BYTE r = GetRValue(pixel);
			BYTE g = GetGValue(pixel);
			BYTE b = GetBValue(pixel);

			if ((r + 10) > 255) r = 255; else r += 10;
			if ((g + 10) > 255) g = 255; else g += 10;
			if ((b + 10) > 255) b = 255; else b += 10;

			pixel = RGB(r, g, b);

			dest->SetPixel(x, y, pixel);
		}
	}

	return dest;
}

class TImage : public CImage
{
public:
	void greyscale() {

		int width = this->GetWidth();
		int height = this->GetHeight();

		
		long lAdrs;
		double grey;
		BYTE* pInImage = (BYTE*)this->GetBits();
		BYTE bRed, bGreen, bBlue;
		int pitch = this->GetPitch();

	
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

	void bilinearInterpolation() {
		// http://stackoverflow.com/questions/21572460/resizing-a-picture-vc
	}
};

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

		TImage b1;
		b1.Load(L"IMG_1.JPG");
		b1.greyscale();
		b1.Save(L"IMG_1.PNG");

		TImage b2;
		b2.Load(L"IMG_2.JPG");
		b2.greyscale();
		b2.Save(L"IMG_2.PNG");

		TImage b3;
		b3.Load(L"IMG_3.JPG");
		b3.greyscale();
		b3.Save(L"IMG_3.PNG");

		TImage b4;
		b4.Load(L"IMG_4.JPG");
		b4.greyscale();
		b4.Save(L"IMG_4.PNG");

		TImage b5;
		b5.Load(L"IMG_5.JPG");
		b5.greyscale();
		b5.Save(L"IMG_5.PNG");

		TImage b6;
		b6.Load(L"IMG_6.JPG");
		b6.greyscale();
		b6.Save(L"IMG_6.PNG");

		TImage b7;
		b7.Load(L"IMG_7.JPG");
		b7.greyscale();
		b7.Save(L"IMG_7.PNG");

		TImage b8;
		b8.Load(L"IMG_8.JPG");
		b8.greyscale();
		b8.Save(L"IMG_8.PNG");

		TImage b9;
		b9.Load(L"IMG_9.JPG");
		b9.greyscale();
		b9.Save(L"IMG_9.PNG");

		TImage b10;
		b10.Load(L"IMG_10.JPG");
		b10.greyscale();
		b10.Save(L"IMG_10.PNG");

		TImage b11;
		b11.Load(L"IMG_11.JPG");
		b11.greyscale();
		b11.Save(L"IMG_11.PNG");

		TImage b12;
		b12.Load(L"IMG_12.JPG");
		b12.greyscale();
		b12.Save(L"IMG_12.PNG");


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
