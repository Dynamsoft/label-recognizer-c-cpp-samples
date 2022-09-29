#include <iostream>
#include <fstream>
#include <string>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <conio.h>
#include <io.h>
#else
#include <dirent.h>
#include <sys/time.h>
#endif

#include "../../../Include/DynamsoftLabelRecognizer.h"
#include "MRZParser.h"

using namespace std;
using namespace dynamsoft::dlr;
using namespace dynamsoft::core;

#if defined(_WIN32) || defined(_WIN64)
#ifdef _WIN64
#pragma comment(lib, "../../../Lib/Windows/x64/DynamsoftLabelRecognizerx64.lib")
#pragma comment(lib, "../../../Lib/Windows/x64/DynamsoftCorex64.lib")
#else
#pragma comment(lib, "../../../Lib/Windows/x86/DynamsoftLabelRecognizerx86.lib")
#pragma comment(lib, "../../../Lib/Windows/x86/DynamsoftCorex86.lib")
#endif
#endif

bool GetImagePath(string& imgPath)
{
	bool bExit = false;
	string line;

	while (1)
	{
		cout << endl << ">> Step 1: Input your image file's full path:" << endl;

		cin >> line;

		if (line == "q" || line == "Q")
		{
			bExit = true;
			break;
		}

		ifstream fin;
		fin.open(line, ios::binary);
		if (fin.is_open())
		{
			imgPath = line;
			fin.close();
			break;
		}

		cout << "Please input a valid path." << endl;
	}

	return bExit;
}

void OutputResult(CLabelRecognizer& dlr, int errorcode, float time)
{
	if (errorcode != DM_OK)
		cout << endl << "Failed to recognize label : " << dlr.GetErrorString(errorcode) << endl;
	else
	{
		DLR_ResultArray* pDLRResults = NULL;
		dlr.GetAllResults(&pDLRResults);

		if (pDLRResults == NULL || pDLRResults->resultsCount == 0)
		{
			cout << endl << "No data detected." << endl;
		}
		else
		{
			vector<string> textLines;

			for (int i = 0; i < pDLRResults->resultsCount; ++i)
			{
				for (int j = 0; j < pDLRResults->results[i]->lineResultsCount; ++j)
					textLines.push_back(pDLRResults->results[i]->lineResults[j]->text);
			}

			MRZParser parser;
			parser.parseAndOutput(textLines);
		}

		dlr.FreeResults(&pDLRResults);
	}

	cout << endl << "Total time spent :" << time << " s" << endl;
}


int main()
{
	bool bExit = false;
	char szErrorMsg[512];
	string imgPath;
	int iErrorCode = 0;

#if defined(_WIN32) || defined(_WIN64)
	unsigned _int64 ullTimeBegin = 0;
	unsigned _int64 ullTimeEnd = 0;
#else
	struct timeval ullTimeBegin, ullTimeEnd;
#endif


	cout << "*************************************************" << endl;
	cout << "Welcome to Dynamsoft Label Recognizer - MRZ Sample" << endl;
	cout << "*************************************************" << endl;
	cout << "Hints: Please input 'Q' or 'q' to quit the application." << endl;

	// 1.Initialize license.
	// The string "DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9" here is a free public trial license. Note that network connection is required for this license to work.
	// If you want to use an offline license, please contact Dynamsoft Support: https://www.dynamsoft.com/company/contact/
	// You can also request a 30-day trial license in the customer portal: https://www.dynamsoft.com/customer/license/trialLicense?product=dlr&utm_source=github&package=c_cpp
	iErrorCode = CLicenseManager::InitLicense("DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9", szErrorMsg, 512);

	cout << "License initialization :" << szErrorMsg << endl;
	if (iErrorCode != 0)
		return 0;

	// 2.Create an instance of Label Recognizer.
	CLabelRecognizer dlr;

	// 3. Initialize MRZ a template json file.
	iErrorCode = dlr.InitRuntimeSettingsFromFile("MRZ.json");
	cout << "MRZ template initialization :" << CLabelRecognizer::GetErrorString(iErrorCode) << endl;
	if (iErrorCode != 0)
		return 0;

	while (1)
	{
		bExit = GetImagePath(imgPath);

		if (bExit)
			break;

		float costTime = 0.0;
		int errorCode = 0;

		int iNum = 0;
		do 
		{
			//cout << "Invalid number." << endl;
			cout << endl << ">> Step 2: Choose template(1-4)" << endl;
			cout << "   1. All MRZs" << endl;
			cout << "   2. Passport MRZs" << endl;
			cout << "   3. Visa MRZs" << endl;
			cout << "   4. ID MRZs" << endl;
			cin >> iNum;
		} while (iNum < 1 || iNum > 4);

#if defined(_WIN32) || defined(_WIN64)
		ullTimeBegin = GetTickCount();
#else 
		gettimeofday(&ullTimeBegin, NULL);
#endif
		if (iNum == 1)
			errorCode = dlr.RecognizeFile(imgPath.c_str(), "default");
		else if (iNum == 2)
			errorCode = dlr.RecognizeFile(imgPath.c_str(), "passport-default");
		else if (iNum == 3)
			errorCode = dlr.RecognizeFile(imgPath.c_str(), "visa-default");
		else if (iNum == 4)
			errorCode = dlr.RecognizeFile(imgPath.c_str(), "id-default");
#if defined(_WIN32) || defined(_WIN64)
		ullTimeEnd = GetTickCount();
		costTime = ((float)(ullTimeEnd - ullTimeBegin)) / 1000;
#else
		gettimeofday(&ullTimeEnd, NULL);
		costTime = (float)((ullTimeEnd.tv_sec * 1000 * 1000 + ullTimeEnd.tv_usec) - (ullTimeBegin.tv_sec * 1000 * 1000 + ullTimeBegin.tv_usec)) / (1000 * 1000);
#endif

		OutputResult(dlr, errorCode, costTime);

	}

	return 0;
}
