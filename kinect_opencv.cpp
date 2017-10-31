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

	Mat depth_frame;
	INT depth_frame_width, depth_frame_height;

	hr = get_depth_frame(depth_frame, depth_frame_width, depth_frame_height, m_pKinectSensor);

	if (FAILED(hr))
	{
		cout << "get_depth_frame error" << endl;
		return hr;
	}

	Mat infrared_frame;
	INT infrared_frame_width, infrared_frame_height;

	hr = get_infrared_frame(infrared_frame, infrared_frame_width, infrared_frame_height, m_pKinectSensor);

	if (FAILED(hr))
	{
		cout << "get_infrared_frame error" << endl;
		return hr;
	}

//	VideoWriter colour_vid_out("colour.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, Size(colour_frame_width, colour_frame_height));
//	VideoWriter depth_vid_out("depth.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, Size(depth_frame_width, depth_frame_height));
//	VideoWriter infrared_vid_out("infrared.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, Size(infrared_frame_width, infrared_frame_height));
	
	imshow("colour", colour_frame);
	imshow("depth", depth_frame);
	imshow("infrared", infrared_frame);

//	colour_vid_out.write(colour_frame);
//	depth_vid_out.write(depth_frame);
//	infrared_vid_out.write(infrared_frame);

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

		hr = get_depth_frame(depth_frame, depth_frame_width, depth_frame_height, m_pKinectSensor);

		if (FAILED(hr))
		{
			cout << "get_depth_frame error" << endl;
			return hr;
		}

		hr = get_infrared_frame(infrared_frame, infrared_frame_width, infrared_frame_height, m_pKinectSensor);

		if (FAILED(hr))
		{
			cout << "get_infrared_frame error" << endl;
			return hr;
		}

		imshow("colour", colour_frame);
		imshow("depth", depth_frame);
		imshow("infrared", infrared_frame);

//		colour_vid_out.write(colour_frame);
//		depth_vid_out.write(depth_frame);
//		infrared_vid_out.write(infrared_frame);
	}



	m_pKinectSensor->Close();
	SafeRelease(m_pKinectSensor);



	destroyAllWindows();

	return 0;
}