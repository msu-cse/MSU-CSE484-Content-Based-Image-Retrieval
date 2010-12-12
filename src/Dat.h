/*
 * Dat.h
 *
 *  Created on: Dec 12, 2010
 *      Author: zach
 */

#ifndef DAT_H_
#define DAT_H_

#include <flann/flann.hpp>
#include <string>

namespace cbir {

typedef unsigned char feat;
typedef flann::Matrix<feat> dataset;

using namespace std;

class Dat {
public:
	Dat(string filename);
	virtual ~Dat();

	dataset *load();
	int save();

	dataset *getDataset();
	void setDataset(dataset *d);

private:

	int countLines();

	dataset *pDataset;
	string filename;
};

}

#endif /* DAT_H_ */
