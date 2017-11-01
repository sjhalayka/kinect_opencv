#pragma once



// Get the Kinect SDK 2.0
// https://www.microsoft.com/en-ca/download/details.aspx?id=44561
#include <windows.h>
#include <Kinect.h>
#pragma comment (lib, "kinect20.lib")

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

	IColorFrameReader *m_pFrameReader = NULL;
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

	IFrameDescription *pDescription = NULL;
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

	UINT nBufferSize = 0; // Number of bytes in pBuffer
	BYTE *pBuffer = NULL;
	hr = pFrame->AccessRawUnderlyingBuffer(&nBufferSize, &pBuffer);

	if (FAILED(hr))
	{
		cout << "AccessRawUnderlyingBuffer error" << endl;
		return hr;
	}

	// check out convertTo member function
	//cvtColor(in, out, CV_BGRA2BGR);
	frame_content = Mat(frame_width, frame_height, CV_8UC4, pBuffer).clone();

	cvtColor(frame_content, frame_content, CV_RGBA2BGR);

	SafeRelease(pFrame);
	SafeRelease(m_pFrameReader);

	return S_OK;
}

HRESULT get_depth_frame(Mat &frame_content, INT &frame_width, INT &frame_height, IKinectSensor *m_pKinectSensor)
{
	HRESULT hr;

	IDepthFrameReader *m_pFrameReader = NULL;
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

	IFrameDescription *pDescription = NULL;
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

	UINT nBufferSize = 0; // Number of bytes in pBuffer
	UINT16 *pBuffer = NULL;
	hr = pFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);

	if (FAILED(hr))
	{
		cout << "AccessUnderlyingBuffer error" << endl;
		return hr;
	}

	frame_content = Mat(frame_width, frame_height, CV_16UC1, pBuffer).clone();

	Mat float_frame_content(frame_width, frame_height, CV_32FC1);

	for (int j = 0; j < frame_content.rows; j++)
		for (int i = 0; i < frame_content.cols; i++)
			float_frame_content.at<float>(j, i) = frame_content.at<UINT16>(j, i) / 65535.0f;

	frame_content = float_frame_content.clone();

	SafeRelease(pFrame);
	SafeRelease(m_pFrameReader);

	return S_OK;
}

HRESULT get_infrared_frame(Mat &frame_content, INT &frame_width, INT &frame_height, IKinectSensor *m_pKinectSensor)
{
	HRESULT hr;

	IInfraredFrameReader *m_pFrameReader = NULL;
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

	IFrameDescription *pDescription = NULL;
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

	UINT nBufferSize = 0; // Number of bytes in pBuffer
	UINT16 *pBuffer = NULL;
	hr = pFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);

	if (FAILED(hr))
	{
		cout << "AccessUnderlyingBuffer error" << endl;
		return hr;
	}

	frame_content = Mat(frame_width, frame_height, CV_16UC1, pBuffer).clone();

	Mat float_frame_content(frame_width, frame_height, CV_32FC1);

	for (int j = 0; j < frame_content.rows; j++)
		for (int i = 0; i < frame_content.cols; i++)
			float_frame_content.at<float>(j, i) = frame_content.at<UINT16>(j, i) / 65535.0f;

	frame_content = float_frame_content.clone();

	SafeRelease(pFrame);
	SafeRelease(m_pFrameReader);

	return S_OK;
}
