#pragma once

#include <string>
#include <vector>

using namespace std;

class MRZParser
{
private:
	string m_docId;
	string m_docType;
	string m_docCode;
	string m_sizeType;
	string m_nationality;
	string m_issuer;
	string m_birthYear;
	string m_birthMonth;
	string m_birthDay;
	string m_expireYear;
	string m_expireMonth;
	string m_expireDay;
	string m_gender;
	string m_surname;
	string m_givenname;

	vector<string> m_rawText;

private:
	bool parseTD1(const vector<string>& vec, string& errMsg);
	bool parseTD2(const vector<string>& vec, string& errMsg);
	bool parseTD3(const vector<string>& vec, string& errMsg);
	void filterErrorLines(vector<string>& vec);

public:
	bool parse(const vector<string>& textLines, string& errMsg);
	void parseAndOutput(const vector<string>& textLines);


	string getDocId() { return m_docId; }
	string getDocType() { return m_docType; }
	string getNationality() { return m_nationality; }
	string getIssuer() { return m_issuer; }
	string getSurname() { return m_surname; }
	string getGivenName() { return m_givenname; }
	string getGender() { return m_gender; }
	string getBirthYear() { return m_birthYear; }
	string getBirthMonth() { return m_birthMonth; }
	string getBirthDay() { return m_birthDay; }
	string getExpireYear() { return m_expireYear; }
	string getExpireMonth() { return m_expireMonth; }
	string getExpireDay() { return m_expireDay; }
};