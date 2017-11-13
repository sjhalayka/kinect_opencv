#include "kinect1.8_opencv.h"

int main(void)
{
	INuiSensor *m_pNuiSensor = NULL;
	INuiSensor *pNuiSensor = NULL;
	HANDLE m_hNextDepthFrameEvent;
	HANDLE m_pDepthStreamHandle;
	HRESULT hr;

	int iSensorCount = 0;
	hr = NuiGetSensorCount(&iSensorCount);
	if (FAILED(hr))
	{
		cout << "NuiGetSensorCount error" << endl;
		return hr;
	}

	// Look at each Kinect sensor
	for (int i = 0; i < iSensorCount; ++i)
	{
		// Create the sensor so we can check status, if we can't create it, move on to the next
		hr = NuiCreateSensorByIndex(i, &pNuiSensor);
		if (FAILED(hr))
		{
			cout << "NuiCreateSensorByIndex error" << endl;
			continue;
		}

		// Get the status of the sensor, and if connected, then we can initialize it
		hr = pNuiSensor->NuiStatus();
		if (S_OK == hr)
		{
			m_pNuiSensor = pNuiSensor;
			break;
		}

		// This sensor wasn't OK, so release it since we're not using it
		pNuiSensor->Release();
	}
	
	if (NULL == m_pNuiSensor)
	{
		cout << "No Kinect found" << endl;
		return hr;
	}
	else
	{
		// Initialize the Kinect and specify that we'll be using depth
		hr = m_pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH);
		if (SUCCEEDED(hr))
		{
			// Create an event that will be signaled when depth data is available
			m_hNextDepthFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

			// Open a depth image stream to receive depth frames
			hr = m_pNuiSensor->NuiImageStreamOpen(
				NUI_IMAGE_TYPE_DEPTH,
				NUI_IMAGE_RESOLUTION_640x480,
				0,
				2,
				m_hNextDepthFrameEvent,
				&m_pDepthStreamHandle);

			if (FAILED(hr))
			{
				cout << "NuiImageStreamOpen error" << endl;
				return hr;
			}

		}
		else
		{
			cout << "NuiInitialize error" << endl;
			return hr;
		}
	}

	NUI_IMAGE_FRAME imageFrame;

	// Attempt to get the depth frame
	hr = m_pNuiSensor->NuiImageStreamGetNextFrame(m_pDepthStreamHandle, 1000, &imageFrame);

	if (FAILED(hr))
	{
		cout << "NuiImageStreamGetNextFrame error" << endl;
		return hr;
	}

	BOOL nearMode;
	INuiFrameTexture* pTexture;

	// Get the depth image pixel texture
	hr = m_pNuiSensor->NuiImageFrameGetDepthImagePixelFrameTexture(
		m_pDepthStreamHandle, &imageFrame, &nearMode, &pTexture);

	if (FAILED(hr))
	{
		cout << "NuiImageFrameGetDepthImagePixelFrameTexture error" << endl;

		m_pNuiSensor->NuiImageStreamReleaseFrame(m_pDepthStreamHandle, &imageFrame);

		return hr;
	}

	NUI_LOCKED_RECT LockedRect;

	// Lock the frame data so the Kinect knows not to modify it while we're reading it
	pTexture->LockRect(0, &LockedRect, NULL, 0);

	// Make sure we've received valid data
	if (0 == LockedRect.Pitch)
	{
		cout << "Pitch error" << endl;
		return -1;
	}

	

	const NUI_DEPTH_IMAGE_PIXEL *pBufferRun = reinterpret_cast<const NUI_DEPTH_IMAGE_PIXEL *>(LockedRect.pBits);


	// end pixel is start + width*height - 1
	const NUI_DEPTH_IMAGE_PIXEL * pBufferEnd = pBufferRun + (640 * 480);

	vector<INT16> depths;

	while (pBufferRun < pBufferEnd)
	{
		// discard the portion of the depth that contains only the player index
		INT16 depth = pBufferRun->depth;

		depths.push_back(depth);

		pBufferRun++;
	}

	Mat frame_content = Mat(480, 640, CV_16UC1, &depths[0]).clone();

	Mat float_frame_content(480, 640, CV_32FC1);

	for (int j = 0; j < frame_content.rows; j++)
		for (int i = 0; i < frame_content.cols; i++)
			float_frame_content.at<float>(j, i) = pow(frame_content.at<UINT16>(j, i) / 65535.0f, 1/10.0f);

	imshow("content", float_frame_content);

	waitKey();

	pTexture->UnlockRect(0);
	pTexture->Release();
	m_pNuiSensor->NuiShutdown();
	CloseHandle(m_hNextDepthFrameEvent);
	SafeRelease(m_pNuiSensor);

	destroyAllWindows();

	return 0;
}
