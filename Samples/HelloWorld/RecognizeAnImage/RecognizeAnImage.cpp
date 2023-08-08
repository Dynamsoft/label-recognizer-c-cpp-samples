#include<iostream>
#include<string>

#include "../../../Include/DynamsoftCaptureVisionRouter.h"

using namespace std;
using namespace dynamsoft::cvr;
using namespace dynamsoft::dlr;
using namespace dynamsoft::license;

// The following code only applies to Windows.
#if defined(_WIN64) || defined(_WIN32)
#ifdef _WIN64
#pragma comment(lib, "../../../Lib/Windows/x64/DynamsoftCaptureVisionRouterx64.lib")
#pragma comment(lib, "../../../Lib/Windows/x64/DynamsoftLicensex64.lib")
#else
#pragma comment(lib, "../../../Lib/Windows/x86/DynamsoftCaptureVisionRouterx86.lib")
#pragma comment(lib, "../../../Lib/Windows/x86/DynamsoftLicensex86.lib")
#endif
#endif

int main()
{
	int errorcode = 0;
	char error[512];

	// 1.Initialize license.
	// You can request and extend a trial license from https://www.dynamsoft.com/customer/license/trialLicense?product=dlr&utm_source=samples
	// The string 'DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9' here is a free public trial license. Note that network connection is required for this license to work.
	errorcode = CLicenseManager::InitLicense("DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9", error, 512);

	cout << "License initialization: " << errorcode << "," << error << endl;

	// 2.Create an instance of CCaptureVisionRouter.
	CCaptureVisionRouter *router = new CCaptureVisionRouter;

	// 3.Recognize text from an image
	string imageFile = "../../../Images/dlr-sample.png";
	CCapturedResult* result = router->Capture(imageFile.c_str(), CPresetTemplate::PT_RECOGNIZE_TEXT_LINES);

	cout << "File: " << imageFile << endl;

	// 4.Output the recognized text.
	if (result->GetErrorCode() != 0) {
		cout << "Error: " << result->GetErrorCode() << "," << result->GetErrorString() << endl;
	}

	int count = result->GetItemsCount();
	cout << "Recognized " << count << " text lines" << endl;
	for (int i = 0; i < count; i++) {
		const CCapturedResultItem* item = result->GetItem(i);

		CapturedResultItemType type = item->GetType();
		if (type == CapturedResultItemType::CRIT_TEXT_LINE) {
			const CTextLineResultItem* textLine = dynamic_cast<const CTextLineResultItem*>(item);

			cout << ">>Line result " << i << ": " << textLine->GetText() << endl;
		}
	}

	// 5. Release the allocated memory.
	delete router, router = NULL;
	delete result, result = NULL;
	
	return 0;
}