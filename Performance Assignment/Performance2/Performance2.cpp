// Performance2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Performance2.h"

#include <vector>
#include <iostream>
#include <GdiPlusHeaders.h>
#include <GdiPlusColor.h>
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


class TImage : public CImage
{
public:
	TImage processImages() {
		TImage image;

		resize(image);
		greyscale(image);
		rotate(image);

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
		b1.processImages().Save(L"IMG_1.PNG");

		TImage b2;
		b2.Load(L"IMG_2.JPG");
		b2.processImages().Save(L"IMG_2.PNG");

		TImage b3;
		b3.Load(L"IMG_3.JPG");
		b3.processImages().Save(L"IMG_3.PNG");

		TImage b4;
		b4.Load(L"IMG_4.JPG");
		b4.processImages().Save(L"IMG_4.PNG");

		TImage b5;
		b5.Load(L"IMG_5.JPG");
		b5.processImages().Save(L"IMG_5.PNG");

		TImage b6;
		b6.Load(L"IMG_6.JPG");
		b6.processImages().Save(L"IMG_6.PNG");

		TImage b7;
		b7.Load(L"IMG_7.JPG");
		b7.processImages().Save(L"IMG_7.PNG");

		TImage b8;
		b8.Load(L"IMG_8.JPG");
		b8.processImages().Save(L"IMG_8.PNG");

		TImage b9;
		b9.Load(L"IMG_9.JPG");
		b9.processImages().Save(L"IMG_9.PNG");

		TImage b10;
		b10.Load(L"IMG_10.JPG");
		b10.processImages().Save(L"IMG_10.PNG");

		TImage b11;
		b11.Load(L"IMG_11.JPG");
		b11.processImages().Save(L"IMG_11.PNG");

		TImage b12;
		b12.Load(L"IMG_12.JPG");
		b12.processImages().Save(L"IMG_12.PNG");


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
