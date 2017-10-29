#include "kinect.h"


int main(void)
{
	HRESULT hr;

	// Current Kinect
	IKinectSensor *m_pKinectSensor = NULL;
	IColorFrameReader *m_pColorFrameReader;
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

	IColorFrameSource *pColorFrameSource = NULL;
	hr = m_pKinectSensor->get_ColorFrameSource(&pColorFrameSource);
	
	if (FAILED(hr))
	{
		cout << "get_ColorFrameSource error" << endl;
		return hr;
	}
	
	hr = pColorFrameSource->OpenReader(&m_pColorFrameReader);

	if (FAILED(hr))
	{
		cout << "OpenReader error" << endl;
		return hr;
	}

	SafeRelease(pColorFrameSource);

	IColorFrame* pColorFrame = NULL;
	hr = m_pColorFrameReader->AcquireLatestFrame(&pColorFrame);

	if (FAILED(hr))
	{
		cout << "AcquireLatestFrame error" << endl;
		return hr;
	}

	UINT nBufferSize = 0;
	BYTE pBuffer = 0;
	hr = pColorFrame->AccessRawUnderlyingBuffer(&nBufferSize, reinterpret_cast<BYTE**>(&pBuffer));

	if (FAILED(hr))
	{
		cout << "AccessRawUnderlyingBuffer error" << endl;
		return hr;
	}

	Mat data_m(640, 480, CV_8UC4, reinterpret_cast<BYTE *>(pBuffer));
	imshow("data", data_m);

	waitKey(0);

	SafeRelease(m_pColorFrameReader);
	m_pKinectSensor->Close();
	SafeRelease(m_pKinectSensor);

	return 0;















	/*

	// Color reader
	IColorFrameReader*      m_pColorFrameReader = 0;

	// Initialize the Kinect and get the color reader
	IColorFrameSource* pColorFrameSource = 0;

	HRESULT hr = m_pKinectSensor->Open();

	if (SUCCEEDED(hr))
	{
		hr = m_pKinectSensor->get_ColorFrameSource(&pColorFrameSource);
	}

	if (SUCCEEDED(hr))
	{
		hr = pColorFrameSource->OpenReader(&m_pColorFrameReader);
	}

	SafeRelease(pColorFrameSource);






	// done with color frame reader
	SafeRelease(m_pColorFrameReader);

	// close the Kinect Sensor
	if (m_pKinectSensor)
	{
		m_pKinectSensor->Close();
	}

	SafeRelease(m_pKinectSensor);

	return 0;

	*/
}