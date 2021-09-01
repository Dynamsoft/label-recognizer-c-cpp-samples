#include<iostream>
#include "../../include/DynamsoftLabelRecognizer.h"

using namespace std;
using namespace dynamsoft::dlr;

// The following code only applies to Windows.
#if defined(_WIN64) || defined(_WIN32)
    #ifdef _WIN64
        #pragma comment(lib, "../../lib/windows/x64/DynamsoftLabelRecognizerx64.lib")
    #else
        #pragma comment(lib, "../../lib/windows/x86/DynamsoftLabelRecognizerx86.lib")
    #endif
#endif

int main()
{
    int errorcode = 0;
    DLR_ResultArray *pDLRResults = NULL;
    DLR_Result* result = NULL;
    char error[512];
	int lCount, rCount, li, ri;

	// 1.Initialize license.
	// The string "DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9" here is a 7-day free license. Note that network connection is required for this license to work.
	// If you want to use an offline license, please contact Dynamsoft Support: https://www.dynamsoft.com/company/contact/
	// You can also request a 30-day trial license in the customer portal: https://www.dynamsoft.com/customer/license/trialLicense?product=dlr&utm_source=github&package=c_cpp
	CLabelRecognizer::InitLicense("DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9", error, 512);

    cout << "License initialization: " << error << endl;

	// 2.Create an instance of Label Recognizer.
	CLabelRecognizer dlr;

	// 3.Recognize text from an image file.
    errorcode = dlr.RecognizeByFile("../../images/dlr-sample-vin.png", "");
    if(errorcode != DM_OK)
        cout << CLabelRecognizer::GetErrorString(errorcode) << endl;

    // 4. Get all recognized results.
    dlr.GetAllResults(&pDLRResults);

    if (pDLRResults != NULL && pDLRResults->resultsCount > 0)
    {
        rCount = pDLRResults->resultsCount;
        cout << "Recognized " << rCount << " results"<< endl;
        for (ri = 0; ri < rCount; ++ri)
        {
            // Get result of each text area (also called label).
            result = pDLRResults->results[ri];
            lCount = result->lineResultsCount;
            for (li = 0; li < lCount; ++li)
            {
                // Get the result of each text line in the label.
                cout << ">>Line result " << li << ": " << result->lineResults[li]->text << endl;
            }
        }
    }
    else
    {
        cout << "No data detected." << endl;
    }

    if(pDLRResults != NULL)           
        CLabelRecognizer::FreeResults(&pDLRResults);
    
    return 0;
}