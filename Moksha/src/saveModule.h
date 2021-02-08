#pragma once
#include "../libs/pugixml/src/pugixml.hpp"
#include <stdexcept>

using namespace std;
using namespace pugi;


class SaveModule {
public:
	SaveModule();

	static void save() {

	}

	static xml_document load(string file_name) {
		xml_document doc;
		// load the XML file
		if (!doc.load_file(file_name.c_str()))
			throw invalid_argument("There's no file with that name :(");

		return doc;


	}

};