#include "fileManager.h"

FileManager::FileManager()
{
}

void FileManager::jsonParse(string n) {
	// Open and read File
	string rawJson;
	ifstream myfile;
	char helper[100];

	myfile.open("Files/lingua0.json");
	while (myfile.getline(helper, 100))	{
		rawJson = string(helper);
		cout << rawJson;
	}
	myfile.close();
	cout << "\n";

	string text = "{\"1\":{\"name\":\"MIKE\",\"surname\":\"TAYLOR\"},\"2\":{\"name\":\"TOM\",\"surname\":\"JERRY\"}}";

	// Parse JSON
	JSONCPP_STRING err;
	Value root;
	CharReaderBuilder builder;
	const unique_ptr<CharReader> reader(builder.newCharReader());

	if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJson.length(), &root, &err))
		cout << "Error parsing the string" << endl;

	const string name = root["2"][0]["name"].asString();
	cout << name << endl;
};

void receiveInput();