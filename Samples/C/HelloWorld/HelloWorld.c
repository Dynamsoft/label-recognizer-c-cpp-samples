#include <stdio.h>
#include "../../../Include/DynamsoftLabelRecognizer.h"

// The following code only applies to Windows.
#if defined(_WIN64) || defined(_WIN32)
    #ifdef _WIN64
        #pragma comment(lib, "../../../Lib/Windows/x64/DynamsoftLabelRecognizerx64.lib")
        #pragma comment(lib, "../../../Lib/Windows/x64/DynamsoftCorex64.lib")
    #else
        #pragma comment(lib, "../../../Lib/Windows/x86/DynamsoftLabelRecognizerx86.lib")
        #pragma comment(lib, "../../../Lib/Windows/x86/DynamsoftCorex86.lib")
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
	// The string "DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9" here is a free public trial license. Note that network connection is required for this license to work.
	// If you want to use an offline license, please contact Dynamsoft Support: https://www.dynamsoft.com/company/contact/
	// You can also request a 30-day trial license in the customer portal: https://www.dynamsoft.com/customer/license/trialLicense?product=dlr&utm_source=github&package=c_cpp
	DC_InitLicense("DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9", error, 512);
    
    printf("License initialization: %s\n", error);

	// 2.Create an instance of Label Recognizer.
    void* dlr = DLR_CreateInstance();

	// 3.Recognize text from an image file.
    errorcode = DLR_RecognizeFile(dlr, "../../../Images/dlr-sample-vin.png", "");
    if(errorcode != DM_OK)
        printf("%s\n", DLR_GetErrorString(errorcode));

    // 4. Get all recognized results.
    DLR_GetAllResults(dlr, &pDLRResults);

    if (pDLRResults != NULL && pDLRResults->resultsCount > 0)
    {
        rCount = pDLRResults->resultsCount;
        printf("Recognized %d results\n", rCount);
        for (ri = 0; ri < rCount; ++ri)
        {
            // Get result of each text area (also called label).
            result = pDLRResults->results[ri];
            lCount = result->lineResultsCount;
            for (li = 0; li < lCount; ++li)
            {
                // Get the result of each text line in the label.
                printf(">>Line result %d: %s\n", li, result->lineResults[li]->text);
            }
        }
    }
    else
    {
        printf("No data detected.\n");
    }

    if(pDLRResults != NULL)           
        DLR_FreeResults(&pDLRResults);
    
    DLR_DestroyInstance(dlr);

    return 0;
}