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
void showcontact(Contact & ct);
void promptAtQuestion();
char filename[100] = "YaliContacts.txt";
int main(int argc, const char ** argv)
{
	cout << "Contacts version 1.10" << endl << endl;

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
	while (true) {
		cout << "   Please input the first 3 digits of phone number: ";
		char k[32] = { 0 };
		cin.getline(k, 30);

		cout << "Your have inputed :" << k << endl;

		for (vector<Contact>::iterator it = contacts.begin(); it != contacts.end(); ++it) {
			string phone = it->phone;
			string work = it->work;
			std::size_t found = phone.find(k);
			std::size_t found2 = work.find(k);
			if ((found != std::string::npos) || (found2 != std::string::npos)) {
				std::cout << it->id << " " << phone.c_str() << " " << it->name << " " << it->email << " ";
				cout << it->histories[0].title << endl;
			}
		}

		promptAtQuestion();
		while (true) {			
			cin.getline(k, 30);
			system("CLS");
			int idinput = atoi(k);
			if ((k[0] == 'x') || (k[0] == 'q'))
				exit(0);
			else if ((idinput > 0) && (idinput < contacts.size())) {
				Contact ct = contacts[idinput];
				showcontact(ct);
				promptAtQuestion();
			}
			else {
				break;
			}
		}
	}
	cin.get();
    return 0;
}
void showcontact(Contact & ct) {
	cout << "ID: " << ct.id << endl;
	cout << "name: " << ct.name << endl;
	cout << "phone: " << ct.phone << endl;
	cout << "email: " << ct.email << endl;
	cout << "company: " << ct.company << endl;
	cout << "address: " << ct.address << endl;
	cout << "History: " << endl;
	int sz = ct.histories.size();
	for (int i = 0; i< sz ; i++) {
		cout << "    Date: " << ct.histories[i].datetime << "    title: " << ct.histories[i].title << endl;
		cout << "        " << ct.histories[i].description << endl;
	}
}
void promptAtQuestion() {
	printf("\n\n --------------------------------------------\n");
	printf("        ID Number: jump to contact\n");
	printf("            Enter: Search \n");	
	printf("           q or x: Exit \n");
	printf("       Your Input: ");
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
		
		Contact q(id, name, phone, email);
		
		XMLElement * company = QA->FirstChildElement("company");
		if (company) {
			q.company = company->GetText();
		}
		
		XMLElement * addr = QA->FirstChildElement("address");
		if (addr) {
			string address = addr->GetText();
			q.address = address;
		}		
		
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