#pragma once



// Uses the Kinect SDK v1.8
#include <windows.h>
#include <NuiApi.h>
#pragma comment (lib, "kinect10.lib") // Use the AMD64 library for 64-bit computers

#include <opencv2/opencv.hpp>
using namespace cv;
#pragma comment(lib, "opencv_world331.lib")

#include <iostream>
using namespace std;


// Safe release for interfaces
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}
