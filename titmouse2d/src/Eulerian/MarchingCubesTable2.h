#ifndef MARCHINGCUBE2TABLE_H
#define MARCHINGCUBE2TABLE_H


#include <iostream>
#include <array>

int triangleTable[16][4] = {

		{-1, -1, -1, -1},
		{2, 3, -1, -1},
		{1, 2, -1, -1},
		{1, 3, -1, -1},
		{0, 1, -1, -1},
		{0, 3, 1, 2},
		{0, 2, -1, -1},
		{0, 3, -1, -1},
		{0, 3, -1, -1},
		{0, 2, -1, -1},
		{0, 1, 2, 3},
		{0, 1, -1, -1},
		{1, 3, -1, -1},
		{1, 2, -1, -1},
		{2, 3, -1, -1},
		{-1, -1, -1, -1}

};

#endif