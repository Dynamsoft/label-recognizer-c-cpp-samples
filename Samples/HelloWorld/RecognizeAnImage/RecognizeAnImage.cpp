#include <iostream>
#include <string>

#include "../../../Include/DynamsoftCaptureVisionRouter.h"

using namespace std;
using namespace dynamsoft::cvr;
using namespace dynamsoft::dlr;
using namespace dynamsoft::license;

// The following code only applies to Windows.
#if defined(_WIN64) || defined(_WIN32)
#ifdef _WIN64
#pragma comment(lib, "../../../Distributables/Lib/Windows/x64/DynamsoftCaptureVisionRouterx64.lib")
#pragma comment(lib, "../../../Distributables/Lib/Windows/x64/DynamsoftLicensex64.lib")
#else
#pragma comment(lib, "../../../Distributables/Lib/Windows/x86/DynamsoftCaptureVisionRouterx86.lib")
#pragma comment(lib, "../../../Distributables/Lib/Windows/x86/DynamsoftLicensex86.lib")
#endif
#endif

int main()
{
	int errorcode = 0;
	char error[512];

	// 1.Initialize license.
	// You can request and extend a trial license from https://www.dynamsoft.com/customer/license/trialLicense?product=dlr&utm_source=samples&package=c_cpp
	// The string 'DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9' here is a free public trial license. Note that network connection is required for this license to work.
	errorcode = CLicenseManager::InitLicense("DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9", error, 512);
	if (errorcode != ErrorCode::EC_OK && errorcode != ErrorCode::EC_LICENSE_CACHE_USED)
	{
		cout << "License initialization failed: ErrorCode: " << errorcode << ", ErrorString: " << error << endl;
	}
	else
	{
		// 2.Create an instance of CCaptureVisionRouter.
		CCaptureVisionRouter *router = new CCaptureVisionRouter;

		// 3.Recognize text from an image
		string imageFile = "../../../Images/dlr-sample.png";
		CCapturedResult *result = router->Capture(imageFile.c_str(), CPresetTemplate::PT_RECOGNIZE_TEXT_LINES);

		cout << "File: " << imageFile << endl;

		// 4.Output the recognized text.
		if (result->GetErrorCode() != 0)
		{
			cout << "Error: " << result->GetErrorCode() << "," << result->GetErrorString() << endl;
		}
		CRecognizedTextLinesResult *textLinesResult = result->GetRecognizedTextLinesResult();
		if (textLinesResult == nullptr || textLinesResult->GetItemsCount() == 0)
		{
			cout << "No text line found." << endl;
		}
		else
		{
			int count = textLinesResult->GetItemsCount();
			cout << "Recognized " << count << " text lines" << endl;
			for (int i = 0; i < count; i++)
			{
				const CTextLineResultItem *textLine = textLinesResult->GetItem(i);
				cout << ">>Line result " << i << ": " << textLine->GetText() << endl;
			}
		}

		// 5. Release the allocated memory.
		if (textLinesResult)
			textLinesResult->Release();
		if (result)
			result->Release();
		delete router, router = NULL;
	}
		return 0;
}