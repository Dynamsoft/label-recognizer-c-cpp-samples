#include "MRZParser.h"

#include <iostream>
#include <regex>

bool MRZParser::parseTD3(const vector<string>& vec, string& errMsg)
{
	string strLineRegex[2] = { "([PV])[A-Z<]([A-Z]{3})([A-Z<]*[A-Z])<<([A-Z<]*[A-Z])[A-Z<]*",
	"([A-Z0-9<]{9})[0-9]([A-Z]{3})([0-9]{2})(0[0-9]|1[0-2])(0[1-9]|[1-2][0-9]|3[0-1])[0-9]([MF<])([0-9]{2})(0[0-9]|1[0-2])(0[1-9]|[1-2][0-9]|3[0-1])[0-9][A-Z0-9<]{16}" };

	regex regexPatten[2];
	smatch result[2];

	for (int i = 0; i < 2; ++i)
	{
		regex regexPatten = regex(strLineRegex[i]);
		bool bMatch = regex_match(vec[i], result[i], regexPatten);
		if (!bMatch)
		{
			errMsg = "Failed to parse the line ";
			errMsg += to_string(i + 1);
			return false;
		}
	}

	m_docCode = result[0][1];
	if (m_docCode == "P")
		m_docType = "Passport";
	else 
		m_docType = "Visa-A";

	m_issuer = result[0][2];
	m_surname = result[0][3];
	m_givenname = result[0][4];

	replace(m_surname.begin(), m_surname.end(), '<', ' ');
	replace(m_givenname.begin(), m_givenname.end(), '<', ' ');

	m_docId = result[1][1];
	size_t pos = m_docId.find_first_of('<');
	m_docId = m_docId.substr(0, pos);

	m_nationality = result[1][2];

	m_birthYear = result[1][3];
	m_birthMonth = result[1][4];
	m_birthDay = result[1][5];
	if (result[1][6] == "F")
		m_gender = "Female";
	else if (result[1][6] == "M")
		m_gender = "Male";
	else
		m_gender = "Unspecified";
	m_expireYear = result[1][7];
	m_expireMonth = result[1][8];
	m_expireDay = result[1][9];

	return true;
}

bool MRZParser::parseTD2(const vector<string>& vec, string& errMsg)
{
	string strLineRegex[2] = { "([ACIV])[A-Z<]([A-Z]{3})([A-Z<]*[A-Z])<<([A-Z<]*[A-Z])[A-Z<]*",
	 "([A-Z0-9<]{9})[0-9]([A-Z]{3})([0-9]{2})(0[0-9]|1[0-2])(0[1-9]|[1-2][0-9]|3[0-1])[0-9]([MF<])([0-9]{2})(0[0-9]|1[0-2])(0[1-9]|[1-2][0-9]|3[0-1])[0-9][A-Z0-9<]{7}[0-9]" };

	regex regexPatten[2];
	smatch result[2];

	for (int i = 0; i < 2; ++i)
	{
		regex regexPatten = regex(strLineRegex[i]);
		bool bMatch = regex_match(vec[i], result[i], regexPatten);
		if (!bMatch)
		{
			errMsg = "Failed to parse the line ";
			errMsg += to_string(i + 1);
			return false;
		}
	}

	m_docCode = result[0][1];
	if (m_docCode == "V")
		m_docType = "Visa-B";
	else
		m_docType = "ID Card";

	m_issuer = result[0][2];
	m_surname = result[0][3];
	m_givenname = result[0][4];

	replace(m_surname.begin(), m_surname.end(), '<', ' ');
	replace(m_givenname.begin(), m_givenname.end(), '<', ' ');

	m_docId = result[1][1];
	size_t pos = m_docId.find_first_of('<');
	m_docId = m_docId.substr(0, pos);

	m_nationality = result[1][2];

	m_birthYear = result[1][3];
	m_birthMonth = result[1][4];
	m_birthDay = result[1][5];
	if (result[1][6] == "F")
		m_gender = "Female";
	else if (result[1][6] == "M")
		m_gender = "Male";
	else
		m_gender = "Unspecified";
	m_expireYear = result[1][7];
	m_expireMonth = result[1][8];
	m_expireDay = result[1][9];
	
	return true;
}

bool MRZParser::parseTD1(const vector<string>& vec, string& errMsg)
{
	string strLineRegex[3] = { "([ACI])[A-Z<]([A-Z]{3})([A-Z0-9<]{9})[0-9][A-Z0-9<]{15}",
		"([0-9]{2})(0[0-9]|1[0-2])(0[1-9]|[1-2][0-9]|3[0-1])[0-9]([MF<])([0-9]{2})(0[0-9]|1[0-2])(0[1-9]|[1-2][0-9]|3[0-1])[0-9]([A-Z]{3})[A-Z0-9<]{11}[0-9]",
		"([A-Z<]*[A-Z])<<([A-Z<]*[A-Z])<*" };

	regex regexPatten[3];
	smatch result[3];

	for (int i = 0; i < 3; ++i)
	{
		regex regexPatten = regex(strLineRegex[i]);
		bool bMatch = regex_match(vec[i], result[i], regexPatten);
		if (!bMatch)
		{
			errMsg = "Failed to parse the line ";
			errMsg += to_string(i + 1);
			return false;
		}
	}

	m_docCode = result[0][1];
	m_docType = "ID Card";

	m_issuer = result[0][2];
	m_docId = result[0][3];
	size_t pos = m_docId.find_first_of('<');
	m_docId = m_docId.substr(0, pos);

	m_birthYear = result[1][1];
	m_birthMonth = result[1][2];
	m_birthDay = result[1][3];
	if (result[1][4] == "F")
		m_gender = "Female";
	else if (result[1][4] == "M")
		m_gender = "Male";
	else
		m_gender = "Unspecified";
	m_expireYear = result[1][5];
	m_expireMonth = result[1][6];
	m_expireDay = result[1][7];
	m_nationality = result[1][8];
	m_surname = result[2][1];
	m_givenname = result[2][2];

	replace(m_surname.begin(), m_surname.end(), '<', ' ');
	replace(m_givenname.begin(), m_givenname.end(), '<', ' ');

	return true;
}

void MRZParser::filterErrorLines(vector<string>& vec)
{
	size_t lastLineLength = vec[vec.size() - 1].length();

	if (lastLineLength == 30 && vec.size() > 3)
	{
		while (vec.size() > 3)
			vec.erase(vec.begin());
	}
	else if ((lastLineLength == 44 || lastLineLength == 36) && vec.size() > 2)
	{
		while (vec.size() > 2)
			vec.erase(vec.begin());
	}
}

bool MRZParser::parse(const vector<string>& textLines, string& errMsg)
{
	m_rawText = textLines;

	filterErrorLines(m_rawText);

	cout << "MRZ Raw Text:" << endl;
	for (int i = 0; i < m_rawText.size(); ++i)
	{
		cout << "\tLine " << (i + 1) << ": " << m_rawText[i] << endl;
	}

	cout << endl;

	if (m_rawText.size() == 3 && m_rawText[0].size() == 30 && m_rawText[1].size() == 30 && m_rawText[2].size() == 30)
	{
		m_sizeType = "TD1";
		return parseTD1(m_rawText, errMsg);
	}
	else if (m_rawText.size() == 2 && m_rawText[0].size() == 36 && m_rawText[1].size() == 36)
	{
		m_sizeType = "TD2";
		return parseTD2(m_rawText, errMsg);
	}
	else if (m_rawText.size() == 2 && m_rawText[0].size() == 44 && m_rawText[1].size() == 44)
	{
		m_sizeType = "TD3";
		return parseTD3(m_rawText, errMsg);
	}

	errMsg = "Failed to recognize the MRTD image.";
	return false;
}

void MRZParser::parseAndOutput(const vector<string>& textLines)
{
	string errMsg;
	if (!parse(textLines, errMsg))
	{
		cout << "Error: " << errMsg << endl;
		return;
	}

	cout << "Parsed Information:" << endl;
	cout << "\tDocument Type: " << m_docType << endl;
	cout << "\tDocument ID: " << m_docId << endl;
	cout << "\tSurname: " << m_surname << endl;
	cout << "\tGiven Name: " << m_givenname << endl;
	cout << "\tNationality: " << m_nationality << endl;
	cout << "\tIssuing Country or Organization: " << m_issuer << endl;
	cout << "\tGender: " << m_gender << endl;
	cout << "\tDate of Birth(YY-MM-DD): " << m_birthYear << "-" << m_birthMonth << "-" << m_birthDay << endl;
	cout << "\tExpiration Date(YY-MM-DD): " << m_expireYear << "-" << m_expireMonth << "-" << m_expireDay << endl;
}