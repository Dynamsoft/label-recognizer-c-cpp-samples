#include <stdio.h>

#include <string>
#include <vector>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <conio.h>
#include <io.h>
#else
#include <dirent.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "../../include/DynamsoftLabelRecognizer.h"

using namespace std;
using namespace dynamsoft::dlr;

#if defined(_WIN32) || defined(_WIN64)
#ifdef _WIN64
#pragma comment(lib, "../../lib/windows/x64/DynamsoftLabelRecognizerx64.lib")
#else
#pragma comment(lib, "../../lib/windows/x86/DynamsoftLabelRecognizerx86.lib")
#endif
#endif

vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if ("" == str) return res;

	char * strs = new char[str.length() + 1]; 
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while (p) {
		string s = p;
		res.push_back(s);
		p = strtok(NULL, d);
	}

	return res;
}

bool GetImagePath(char* pImagePath)
{
	char pszBuffer[512] = { 0 };
	bool bExit = false;
	size_t iLen = 0;
	FILE* fp = NULL;
	while (1)
	{
		printf("\n>> Step 1: Input your image file's full path:\n");
#if defined(_WIN32) || defined(_WIN64)
		gets_s(pszBuffer, 512);
#else
		fgets(pszBuffer, 512, stdin);
		strtok(pszBuffer, "\n");
#endif
		iLen = strlen(pszBuffer);
		if (iLen > 0)
		{
			if (strlen(pszBuffer) == 1 && (pszBuffer[0] == 'q' || pszBuffer[0] == 'Q'))
			{
				bExit = true;
				break;
			}

			memset(pImagePath, 0, 512);
			if (pszBuffer[0] == '\'' || pszBuffer[0] == '\"')
			{
				if (pszBuffer[iLen - 3] == '\'')
					memcpy(pImagePath, pszBuffer + 1, iLen - 4);
				else if (pszBuffer[iLen - 2] == '\'')
					memcpy(pImagePath, pszBuffer + 1, iLen - 3);
				else if (pszBuffer[iLen - 1] == '\"')
					memcpy(pImagePath, pszBuffer + 1, iLen - 2);
			}
			else
				memcpy(pImagePath, pszBuffer, iLen);

#if defined(_WIN32) || defined(_WIN64)
			int err = fopen_s(&fp, pImagePath, "rb");
			if (err == 0)
			{
				fclose(fp);
				break;
			}
#else
			fp = fopen(pImagePath, "rb");
			if (fp != NULL)
			{
				break;
			}
#endif
		}
		printf("Please input a valid path.\n");
	}
	return bExit;
}

void OutputResult(CLabelRecognizer& dlr, int errorcode, float time)
{
	if (errorcode != DM_OK)
		printf("\nFailed to recognize label : %s\n", dlr.GetErrorString(errorcode));
	else
	{
		DLR_ResultArray* pDLRResults = NULL;
		dlr.GetAllResults(&pDLRResults);
		if (pDLRResults != NULL)
		{
			// 5. Output the raw text of MRZ.
			int rCount = pDLRResults->resultsCount;
			printf("\nRecognized %d results\n", rCount);
			for (int ri = 0; ri < rCount; ++ri)
			{
				printf("\nResult %d :\n", ri);
				DLR_Result* result = pDLRResults->results[ri];
				int lCount = result->lineResultsCount;
				for (int li = 0; li < lCount; ++li)
				{
					printf("Line result %d: %s\n", li, result->lineResults[li]->text);
				}
			}

			// 6. Parse the raw text of MRZ and output passport info.
			printf("\nPassport Info \n");
			bool bParse = false;
			for (int ri = 0; ri < rCount; ++ri)
			{
				DLR_Result* result = pDLRResults->results[ri];
				int lCount = result->lineResultsCount;
				if (lCount < 2)
				{
					continue;
				}
				string line1 = result->lineResults[0]->text;
				string line2 = result->lineResults[1]->text;
				if (line1.length() != 44 || line2.length() != 44)
				{
					continue;
				}
				if (line1[0] != 'P')
					continue;
				string tmpString = line1.substr(5);			
				int pos = tmpString.find("<<");
				if (pos > 0)
				{
					string givenName = tmpString.substr(0, pos);
					vector<string> givenNames = split(givenName, "<");
					givenName = "";
					for (int i = 0; i < givenNames.size(); ++i)
					{
						if (i != givenNames.size() - 1)
							givenName = givenName + givenNames[i] + " ";
						else
							givenName = givenName + givenNames[i];
					}
					printf("\tSurname : %s\n", givenName.c_str());

					string tmp2 = tmpString.substr(pos+2);
					pos = tmp2.find_last_not_of('<');
					if (pos > 0)
					{
						string surname = tmp2.substr(0, pos+1);
						vector<string> surnames = split(surname, "<");
						surname = "";
						for (int i = 0; i < surnames.size(); ++i)
						{
							if (i != surnames.size() - 1)
								surname = surname + surnames[i] + " ";
							else
								surname = surname + surnames[i];
						}
						printf("\tGiven Names : ");

						printf("%s\n", surname.c_str());
					}
				}
				string na = line1.substr(2, 3);
				if (na.back() == '<')
				{
					na = na.substr(0, 2);
				}
				printf("\tNationality : %s\n", na.c_str());

				tmpString = line2.substr(0, 9);
				pos = tmpString.find_first_of('<');
				if (pos > 0)
				{
					tmpString = tmpString.substr(0, pos);
				}
				printf("\tPassport Number : %s\n", tmpString.c_str());

				tmpString = line2.substr(10, 3);
				if (tmpString.back() == '<')
				{
					tmpString = tmpString.substr(0, 2);
				}
				printf("\tIssuing Country or Organization : %s\n", tmpString.c_str());

				tmpString = line2.substr(13,6);
				tmpString.insert(2,"-");
				tmpString.insert(5,"-");
				printf("\tDate of Birth(YY-MM-DD) : %s\n", tmpString.c_str());

				/*if(line2[20] == 'F' )
					tmpString = "Female";
				if (line2[20] == 'M')
					tmpString = "Male";*/
				printf("\tSex/Gender : %c\n", line2[20]);

				tmpString = line2.substr(21, 6);
				tmpString.insert(2, "-");
				tmpString.insert(5, "-");
				printf("\tPassport Expiration Date(YY-MM-DD) : %s\n", tmpString.c_str());
			}


		}
		else
		{
			printf("\nNo data detected.\n");
		}
		dlr.FreeResults(&pDLRResults);
	}

	printf("\nTotal time spent: %.3f s\n", time);
}

int main()
{
	bool bExit = false;
	char szErrorMsg[512] = { 0 };
	char pszImageFile[512] = { 0 };
	bool autoRegion = false;
	DM_Point region[4] = { { 0,0 },{ 100,0 },{ 100,100 },{ 0,100 } };
#if defined(_WIN32) || defined(_WIN64)
	unsigned _int64 ullTimeBegin = 0;
	unsigned _int64 ullTimeEnd = 0;
#else
	struct timeval ullTimeBegin, ullTimeEnd;
#endif


	printf("*************************************************\n");
	printf("Welcome to Dynamsoft Label Recognizer - Passport MRZ Sample\n");
	printf("*************************************************\n");
	printf("Hints: Please input 'Q' or 'q' to quit the application.\n");

	// 1.Initialize license.
	// The string "DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9" here is a 7-day free license. Note that network connection is required for this license to work.
	// If you want to use an offline license, please contact Dynamsoft Support: https://www.dynamsoft.com/company/contact/
	// You can also request a 30-day trial license in the customer portal: https://www.dynamsoft.com/customer/license/trialLicense?product=dlr&utm_source=github&package=c_cpp
	CLabelRecognizer::InitLicense("DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9",szErrorMsg, 512);
	
	printf("License initialization: %s\n\n", szErrorMsg);
	
	// 2.Create an instance of Label Recognizer.
	CLabelRecognizer dlr;

	// 3. Append config by a template json file.
	int ret = dlr.AppendSettingsFromFile("wholeImgMRZTemplate.json");

	while (1)
	{	
		bExit = GetImagePath(pszImageFile);
		if (bExit)
			break;
		float costTime = 0.0;
		int errorCode = 0;

#if defined(_WIN32) || defined(_WIN64)
		ullTimeBegin = GetTickCount();

		// 4. Recognize text from the image file. The second parameter is set to "locr" which is defined in the template json file.
		errorCode = dlr.RecognizeByFile(pszImageFile, "locr");
		ullTimeEnd = GetTickCount();
		costTime = ((float)(ullTimeEnd - ullTimeBegin)) / 1000;
#else
		gettimeofday(&ullTimeBegin, NULL);
		
		// 4. Recognize text from the image file. The second parameter is set to "locr" which is defined in the template json file.
		errorCode = dlr.RecognizeByFile(pszImageFile, "locr");
		gettimeofday(&ullTimeEnd, NULL);
		costTime = (float)((ullTimeEnd.tv_sec * 1000 * 1000 + ullTimeEnd.tv_usec) - (ullTimeBegin.tv_sec * 1000 * 1000 + ullTimeBegin.tv_usec)) / (1000 * 1000);
#endif

		OutputResult(dlr, errorCode, costTime);

	}

	return 0;
}
