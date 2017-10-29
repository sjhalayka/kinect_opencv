#include "kinect_opencv.h"

// Get the Kinect SDK 2.0
// https://www.microsoft.com/en-ca/download/details.aspx?id=44561

int get_colour_frame(Mat &frame_content)
{
	IColorFrameSource *pFrameSource = NULL;
	hr = m_pKinectSensor->get_ColorFrameSource(&pFrameSource);

	if (FAILED(hr))
	{
		cout << "get_ColorFrameSource error" << endl;
		return hr;
	}

	hr = pFrameSource->OpenReader(&m_pColorFrameReader);

	if (FAILED(hr))
	{
		cout << "OpenReader error" << endl;
		return hr;
	}

	SafeRelease(pFrameSource);

	IColorFrame* pFrame = NULL;
	hr = m_pColorFrameReader->AcquireLatestFrame(&pFrame);

	if (FAILED(hr))
	{
		cout << "AcquireLatestFrame error" << endl;
		return hr;
	}

	INT frame_width, frame_height;
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
	hr = pColorFrame->AccessRawUnderlyingBuffer(&nBufferSize, reinterpret_cast<BYTE**>(&pBuffer));

	if (FAILED(hr))
	{
		cout << "AccessRawUnderlyingBuffer error" << endl;
		return hr;
	}

	Mat data_m(frame_width, frame_height, CV_8UC4, pBuffer);

	SafeRelease(pColorFrame);
	SafeRelease(m_pFrameReader);

	frame_content = data_m.clone();
}


int main(void)
{
	HRESULT hr;

	IKinectSensor *m_pKinectSensor = NULL;
	IColorFrameReader *m_pFrameReader;

	hr = GetDefaultKinectSensor(&m_pKinectSensor);

	if (FAILED(hr))
	{
		cout << "GetDefaultKinectSensor error" << endl;
		return hr;
	}

	hr = m_pKinectSensor->Open();

	if (FAILED(hr))
	{
		cout << "Open error" << endl;
		return hr;
	}

	Mat frame;
	get_colour_frame(frame);

	imshow("data", frame);
	imwrite("out.png", frame);

	waitKey(0);




	m_pKinectSensor->Close();
	SafeRelease(m_pKinectSensor);

	return 0;
}