#pragma once


#include <windows.h>
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

HRESULT get_colour_frame(Mat &frame_content, INT &frame_width, INT &frame_height, IKinectSensor *m_pKinectSensor)
{
	HRESULT hr;

	IColorFrameReader *m_pFrameReader;
	IColorFrameSource *pFrameSource = NULL;

	hr = m_pKinectSensor->get_ColorFrameSource(&pFrameSource);

	if (FAILED(hr))
	{
		cout << "get_ColorFrameSource error" << endl;
		return hr;
	}

	hr = pFrameSource->OpenReader(&m_pFrameReader);

	if (FAILED(hr))
	{
		cout << "OpenReader error" << endl;
		return hr;
	}

	SafeRelease(pFrameSource);

	IColorFrame* pFrame = NULL;
	hr = m_pFrameReader->AcquireLatestFrame(&pFrame);

	if (FAILED(hr))
	{
		cout << "AcquireLatestFrame error" << endl;
		return hr;
	}

	IFrameDescription *pDescription;
	hr = pFrame->get_FrameDescription(&pDescription);

	if (FAILED(hr))
	{
		cout << "get_FrameDescription error" << endl;
		return hr;
	}

	hr = pDescription->get_Width(&frame_width);

	if (FAILED(hr))
	{
		cout << "get_Width error" << endl;
		return hr;
	}

	hr = pDescription->get_Height(&frame_height);

	if (FAILED(hr))
	{
		cout << "get_Height error" << endl;
		return hr;
	}

	SafeRelease(pDescription);

	UINT nBufferSize = 0;
	BYTE pBuffer = 0;
	hr = pFrame->AccessRawUnderlyingBuffer(&nBufferSize, reinterpret_cast<BYTE**>(&pBuffer));

	if (FAILED(hr))
	{
		cout << "AccessRawUnderlyingBuffer error" << endl;
		return hr;
	}

	Mat data_m(frame_width, frame_height, CV_8UC4, pBuffer);
	cvtColor(data_m, frame_content, CV_BGRA2BGR);

	SafeRelease(pFrame);
	SafeRelease(m_pFrameReader);

	return S_OK;
}

HRESULT get_depth_frame(Mat &frame_content, INT &frame_width, INT &frame_height, IKinectSensor *m_pKinectSensor)
{
	HRESULT hr;

	IDepthFrameReader *m_pFrameReader;
	IDepthFrameSource *pFrameSource = NULL;

	hr = m_pKinectSensor->get_DepthFrameSource(&pFrameSource);

	if (FAILED(hr))
	{
		cout << "get_DepthFrameSource error" << endl;
		return hr;
	}

	hr = pFrameSource->OpenReader(&m_pFrameReader);

	if (FAILED(hr))
	{
		cout << "OpenReader error" << endl;
		return hr;
	}

	SafeRelease(pFrameSource);

	IDepthFrame* pFrame = NULL;
	hr = m_pFrameReader->AcquireLatestFrame(&pFrame);

	if (FAILED(hr))
	{
		cout << "AcquireLatestFrame error" << endl;
		return hr;
	}

	IFrameDescription *pDescription;
	hr = pFrame->get_FrameDescription(&pDescription);

	if (FAILED(hr))
	{
		cout << "get_FrameDescription error" << endl;
		return hr;
	}

	hr = pDescription->get_Width(&frame_width);

	if (FAILED(hr))
	{
		cout << "get_Width error" << endl;
		return hr;
	}

	hr = pDescription->get_Height(&frame_height);

	if (FAILED(hr))
	{
		cout << "get_Height error" << endl;
		return hr;
	}

	SafeRelease(pDescription);

	UINT nBufferSize = 0;
	UINT16 pBuffer = 0;
	hr = pFrame->AccessUnderlyingBuffer(&nBufferSize, reinterpret_cast<UINT16**>(&pBuffer));

	if (FAILED(hr))
	{
		cout << "AccessUnderlyingBuffer error" << endl;
		return hr;
	}

	Mat data_m(frame_width, frame_height, CV_16UC1, pBuffer);
	cvtColor(data_m, frame_content, CV_GRAY2BGR);

	SafeRelease(pFrame);
	SafeRelease(m_pFrameReader);

	return S_OK;
}

HRESULT get_infrared_frame(Mat &frame_content, INT &frame_width, INT &frame_height, IKinectSensor *m_pKinectSensor)
{
	HRESULT hr;

	IInfraredFrameReader *m_pFrameReader;
	IInfraredFrameSource *pFrameSource = NULL;

	hr = m_pKinectSensor->get_InfraredFrameSource(&pFrameSource);

	if (FAILED(hr))
	{
		cout << "get_InfraredFrameSource error" << endl;
		return hr;
	}

	hr = pFrameSource->OpenReader(&m_pFrameReader);

	if (FAILED(hr))
	{
		cout << "OpenReader error" << endl;
		return hr;
	}

	SafeRelease(pFrameSource);

	IInfraredFrame* pFrame = NULL;
	hr = m_pFrameReader->AcquireLatestFrame(&pFrame);

	if (FAILED(hr))
	{
		cout << "AcquireLatestFrame error" << endl;
		return hr;
	}

	IFrameDescription *pDescription;
	hr = pFrame->get_FrameDescription(&pDescription);

	if (FAILED(hr))
	{
		cout << "get_FrameDescription error" << endl;
		return hr;
	}

	hr = pDescription->get_Width(&frame_width);

	if (FAILED(hr))
	{
		cout << "get_Width error" << endl;
		return hr;
	}

	hr = pDescription->get_Height(&frame_height);

	if (FAILED(hr))
	{
		cout << "get_Height error" << endl;
		return hr;
	}

	SafeRelease(pDescription);

	UINT nBufferSize = 0;
	UINT16 pBuffer = 0;
	hr = pFrame->AccessUnderlyingBuffer(&nBufferSize, reinterpret_cast<UINT16**>(&pBuffer));

	if (FAILED(hr))
	{
		cout << "AccessUnderlyingBuffer error" << endl;
		return hr;
	}

	Mat data_m(frame_width, frame_height, CV_16UC1, pBuffer);
	cvtColor(data_m, frame_content, CV_GRAY2BGR);

	SafeRelease(pFrame);
	SafeRelease(m_pFrameReader);

	return S_OK;
}
