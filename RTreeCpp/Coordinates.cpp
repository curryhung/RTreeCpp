#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "RTree.h"
#include "json.hpp" 
using namespace std;
using json = nlohmann::json;

template <class T>
void convertFromString(T &, const string &);

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

	void show() {
		cout << "站位編號: " << num << endl;
		cout << "經度: " << min[0] << endl;
		cout << "緯度: " << min[1] << endl;
		cout << "站位名稱: " << name << endl;
		cout << "總車位: " << total << endl;
		cout << "可使用車輛: " << usable << endl;
		cout << "可停放車輛: " << parkable << endl;
		cout << "站位開啟與否: " << act << endl << endl;
	}
};

struct Point points[500];

/*

	Point(121.518835, 25.023377,1,"牯嶺公園",38,11,1),
	Point(121.530037, 25.013284,2,"自來水園區",40,9,1),
	Point(121.499922, 25.041702,3,"長沙公園",26,1,1),
	Point(121.565658333, 25.0347361111,4,"世貿二館",80,0,0),

*/



//Rect search_rect(121.52, 25.011, 121.58, 25.055); // search will find above rects that this one overlaps

bool MySearchCallback(int id, void* arg)
{
	cout << "已找到站位 編號 " << id << "。" << endl;
	return true; // keep going
}


int main()
{
	ifstream inFile;
	inFile.open("YouBikeTP.json");// Open the input file

	stringstream strStream;
	strStream << inFile.rdbuf();// Read the file
	string jsonString = strStream.str();// string holds the content of the file

	json youbikeJson = json::parse(jsonString);

	double lng, lat;
	string snaen;
	int count = 0,tot, sbi, act;

	for (int i = 1; i < 500; i++) {
		stringstream ss;
		ss << i;
		string str = ss.str();
		if (i < 10)
			str = "000" + str;
		else if (i < 100)
			str = "00" + str;
		else
			str = "0" + str;
		if(!youbikeJson["retVal"][str].empty()) {
			
			count++;
			
			convertFromString(lng, youbikeJson["retVal"][str]["lng"].get<string>());
			convertFromString(lat, youbikeJson["retVal"][str]["lat"].get<string>());
			snaen = youbikeJson["retVal"][str]["snaen"].get<string>();
			convertFromString(tot, youbikeJson["retVal"][str]["tot"].get<string>());
			convertFromString(sbi, youbikeJson["retVal"][str]["sbi"].get<string>());
			convertFromString(act, youbikeJson["retVal"][str]["act"].get<string>());
		
		
			points[i] = Point(lng,lat,i,snaen,tot,sbi,act);
		}
	}


	RTree<int, double, 2, double> tree;
	int i, nhits;
	cout << "已輸入站位有 " << count << " 站。" << endl;

	for (i = 1; i < 500; i++)
	{
		if (points[i].min[0] == 0.000000) 
			continue;

		tree.Insert(points[i].min, points[i].max, i); // Note, all values including zero are fine in this version
	}


	double xmin=0, ymin=0, xmax=0, ymax=0;


	//功能列表 1.輸入範圍 2.看特定站位資訊 3.結束

	int flag = 0;

	Rect search_rect(xmin, ymin, xmax, ymax);
	

	while (flag != EOF) {

		cout << "功能列表：1.輸入範圍搜尋站位 2.查詢特定站位資訊 3.結束  " ;
		cin >> flag;

		switch (flag) {
		case 1:
			cout << endl << "輸入您要搜尋的經度、緯度：(ex.121.54 25.02 121.56 25.05)" << endl << endl;
			cin >> search_rect.min[0];
			cin >> search_rect.min[1];
			cin >> search_rect.max[0];
			cin >> search_rect.max[1];
			cout << endl << endl;
			nhits = tree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);
			cout << endl << "搜尋結果： 共 " << nhits << " 站。" << endl << endl;
			continue;

		case 2:

			again:
			cout << endl << "輸入您要搜尋的站位編號(1～404)：" << endl << endl;
			int num;
			cin >> num;
			if (num > 404) {
				cout << "無此站位！" << endl << endl;
				goto again;
			}
			points[num].show();
			continue;

		case 3:
			flag = -1;
			break;
		}
	}
	



	/*
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
		printf("站位編號 %d  = (%f,%f)\n", value, boundsMin[0], boundsMin[1]);
		itIndex++;
	}
	*/
	return 0;
}

template <class T>
void convertFromString(T &value, const string &s) {
	stringstream ss(s);
	ss >> value;
}