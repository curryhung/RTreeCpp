#include "stdafx.h"
#include <iostream>
#include <string>
#include "RTree.h"
#include "json.h"
using namespace std;

struct Rect
{
	Rect() {}

	Rect(double a_minX, double a_minY, double a_maxX, double a_maxY)
	{
		min[0] = a_minX;
		min[1] = a_minY;

		max[0] = a_maxX;
		max[1] = a_maxY;
	}


	double min[2];
	double max[2];
};

struct Point
{
	Point() {}

	Point(double X, double Y, int num_, string name_, int total_, int usable_, bool act_)
	{
		min[0] = X;
		min[1] = Y;
		max[0] = X;
		max[1] = Y;
		num = num_;
		name = name_;
		total = total_;
		usable = usable_;
		parkable = total_ - usable_;
		act = act_;
	}

	double min[2];
	double max[2];

	string name;
	int num;
	int total;
	int usable;
	int parkable;
	bool act;
};

struct Point points[] =
{
	Point(121.518835, 25.023377,1,"��������",38,11,1),
	Point(121.530037, 25.013284,2,"�ۨӤ����",40,9,1),
	Point(121.499922, 25.041702,3,"���F����",26,1,1),
	Point(121.565658333, 25.0347361111,4,"�@�T�G�]",80,0,0),
};

int nrects = sizeof(points) / sizeof(points[0]);

Rect search_rect(121.52, 25.011, 121.58, 25.055); // search will find above rects that this one overlaps

bool MySearchCallback(int id, void* arg)
{
	cout << "�w��쯸�� �s�� " << id << "�C" << endl;
	return true; // keep going
}

int main()
{

	RTree<int, double, 2, double> tree;
	int i, nhits;
	cout << "�w��J���즳 " << nrects << " ���C" << endl;

	for (i = 0; i<nrects; i++)
	{
		tree.Insert(points[i].min, points[i].max, i); // Note, all values including zero are fine in this version
	}

	nhits = tree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);

	cout << "�j�M���G�G " << nhits << " ���C" << endl;

	// Iterator test 
	int itIndex = 0;
	RTree<int, double, 2, double>::Iterator it;
	for (tree.GetFirst(it);
		!tree.IsNull(it);
		tree.GetNext(it))
	{
		int value = tree.GetAt(it);

		double boundsMin[2] = { 0,0 };
		double boundsMax[2] = { 0,0 };
		it.GetBounds(boundsMin, boundsMax);
		printf("����s�� %d  = (%f,%f)\n", value, boundsMin[0], boundsMin[1]);
		itIndex++;
	}

	return 0;
}

