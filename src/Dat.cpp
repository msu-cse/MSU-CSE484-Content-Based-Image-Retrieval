/*
 * Dat.cpp
 *
 *  Created on: Dec 12, 2010
 *      Author: zach
 */

#include "Dat.h"
#include <string>
#include <fstream>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator
#include <boost/tokenizer.hpp>

namespace cbir {

using namespace std;

typedef tokenizer<escaped_list_separator<char> > Tokenizer;

Dat::Dat(string filename) {
	// TODO Auto-generated constructor stub
	this->filename = filename;
	this->pDataset = null;
}

Dat::~Dat() {
	// TODO Auto-generated destructor stub
	if (this->pDataset)
		delete[] this->pDataset;
}

dataset * Dat::load() {
	int numLines = countLines();

	if (!numLines)
		return;

	int curLine = 0;
	ifstream fin(this->filename);

	feat *data = new feat[numLines][128];
//	dataset *tempDataset = new dataset(numLines, 128, );

	while (getline(fin, line)) {
		int curToken = 0;
		istringstream stream(line);

		while(!stream.eof()) {
			istringstream >> data[curLine][curToken++];
		}

		curLine++;
	}

	fin.close();

	return d;
}

int Dat::countLines() {
	ifstream fin(this->filename);

	if (!fin.is_open())
		return 0;
	string line;

	int numLines = 0;
	while (getline(fin, line)) {
		numLines++;
		Tokenizer tok(line);
		for (Tokenizer::iterator it(tok.begin()), end(tok.end()); it != end; ++it) {

		}

	}

	fin.close();
	return numLines;
}

}
