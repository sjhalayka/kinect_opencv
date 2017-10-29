#pragma once


#include <windows.h>
#include <d2d1.h>
#include <Kinect.h>

#pragma comment ( lib, "d2d1.lib" )
#pragma comment ( lib, "kinect20.lib" )

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