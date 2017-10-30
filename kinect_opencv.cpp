#include "kinect_opencv.h"

// Get the Kinect SDK 2.0
// https://www.microsoft.com/en-ca/download/details.aspx?id=44561

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

int main(void)
{
	HRESULT hr;

	IKinectSensor *m_pKinectSensor = NULL;

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

	Mat colour_frame;
	INT colour_frame_width, colour_frame_height;

	hr = get_colour_frame(colour_frame, colour_frame_width, colour_frame_height, m_pKinectSensor);

	if (FAILED(hr))
	{
		cout << "get_colour_frame error" << endl;
		return hr;
	}

	VideoWriter colour_vid_out("colour.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, Size(colour_frame_width, colour_frame_height));
	
	imshow("colour", colour_frame);
	colour_vid_out.write(colour_frame);

	for (;;)
	{
		if (waitKey(33) >= 0)
			break;

		hr = get_colour_frame(colour_frame, colour_frame_width, colour_frame_height, m_pKinectSensor);

		if (FAILED(hr))
		{
			cout << "get_colour_frame error" << endl;
			return hr;
		}

		imshow("colour", colour_frame);
		colour_vid_out.write(colour_frame);
	}

	m_pKinectSensor->Close();
	SafeRelease(m_pKinectSensor);

	return 0;
}