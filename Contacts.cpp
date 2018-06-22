// Contacts.cpp : Defines the entry point for the console application.
//

#include "tinyxml2.h"
#include <cstring>
#include <iostream>
#include "Contact.h"
#include "History.h"
#include <map>
using namespace tinyxml2;
using namespace std;
vector<Contact> GetContacts(XMLElement * parent);
char filename[100] = "YaliContacts.txt";
int main(int argc, const char ** argv)
{
	cout << "Contacts version 1.00" << endl << endl;

	XMLDocument* doc = new XMLDocument();          //in tinyxml2 namespace
	if (argc == 1) {
		cout << "Loading default file: " << filename << endl;		
	}
	else {
		strcpy_s(filename, argv[1]);

	}
	doc->LoadFile(filename);
	int errorID = doc->ErrorID();
	if (errorID) {
		printf("XML file loaded and parsed failed: \n"
			"                 ErrorID: %d\n"
			"               Error Msg: %s\n"
			"              Error Line: %d", errorID, doc->ErrorName() + 4, doc->ErrorLineNum());
		cin.get();
		exit(0);
	}
	else {
		printf("File '%s' loaded and parsed succesfully\n\n", filename);
	}
	//---------------begin working on it------------------------------
	vector<Contact> contacts;
	const char * name = "Yalicontacts";
	XMLElement* root = doc->FirstChildElement(name);
	if (root) {
		contacts = GetContacts(root);
	}
	cout << "Number of contacts loaded: " << contacts.size() << endl << endl;

	cout << "   Please input the first 3 digits of phone number: ";
	char k[32] = { 0 };
	cin.getline(k, 30);

	cout << "Your have inputed :" << k << endl;

	cin.get();
    return 0;
}
vector<History> GetHistory(XMLElement * brother) {
	if (!brother) {
		throw NoDataException();
	}
	vector<History> h;
	XMLElement * QA = brother;
	while (QA) {
		const char * datetime = nullptr;
		const char * title = nullptr;
		QA->QueryStringAttribute("datetime", &datetime);
		QA->QueryStringAttribute("title", &title);
		if ((datetime) && (title)) {
			History hist(datetime, title);
			const string desc = QA->GetText();
			hist.description = desc;
			h.push_back(hist);
		}
		XMLElement * QB = (XMLElement *)QA->NextSibling();
		QA = QB;
	}
	return h;
}
vector<Contact> GetContacts(XMLElement * parent) {
	if (!parent) {
		throw NoDataException();
	}
	vector<Contact> a;
	XMLElement* QA = parent->FirstChildElement("contact");
	while (QA) {
		int id=0;
		const char *name = nullptr;
		const char *phone = nullptr;
		const char *email = nullptr;
		const char *country = nullptr;
		const char *type = nullptr;
		const char *workNo = nullptr;
		
		QA->QueryIntAttribute("ID", &id);
		QA->QueryStringAttribute("name", &name);
		QA->QueryStringAttribute("phone", &phone);		
		QA->QueryStringAttribute("country", &country);
		QA->QueryStringAttribute("email", &email);		
		QA->QueryStringAttribute("type", &type);
		QA->QueryStringAttribute("work", &workNo);
		
		
		const string company = QA->FirstChildElement("company")->GetText();
		const string address = QA->FirstChildElement("address")->GetText();
		Contact q(id, name, phone,email);		
		q.address = address;
		q.company = company;
		q.country = country;
		q.type = type;
		q.work = workNo;

		XMLElement * QAHistory = QA->FirstChildElement("history");
		if (QAHistory) {
			vector<History> hist = GetHistory(QAHistory);
			if (hist.size())
				q.histories = hist;
		}

		a.push_back(q);
		//cout << "Contact: " << q.id << " " << q.name << " " << q.phone << " successfully inserted" << endl;
		XMLElement * QB = (XMLElement *)QA->NextSibling();
		QA = QB;
	}
	return a;
}