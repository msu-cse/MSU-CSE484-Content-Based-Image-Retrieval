/*
 * Indexer.h
 *
 *  Created on: Dec 12, 2010
 *      Author: zach
 */

#ifndef INDEXER_H_
#define INDEXER_H_

#include <string>

namespace cbir {

using namespace std;

class Indexer {
public:
	Indexer(string filename);
	virtual ~Indexer();

	void buildIndex();
};

}

#endif /* INDEXER_H_ */
