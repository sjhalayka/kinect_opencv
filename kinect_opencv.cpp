#include "kinect_opencv.h"

// Get the Kinect SDK 2.0
// https://www.microsoft.com/en-ca/download/details.aspx?id=44561

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