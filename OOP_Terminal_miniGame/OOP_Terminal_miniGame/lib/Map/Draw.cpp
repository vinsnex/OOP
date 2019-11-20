#include "../../include/Map/Draw.h"
using namespace Draw;

void SplitString(const string input_string, vector<string>& v, const string c) {
	string::size_type pos1, pos2;
	pos2 = input_string.find(c);
	pos1 = 0;
	while (string::npos != pos2) {
		v.push_back(input_string.substr(pos1, pos2 - pos1));//2019/01/01 7 - 5
		pos1 = pos2 + c.size();
		pos2 = input_string.find(c, pos1);
	}
	if (pos1 != input_string.length())
		v.push_back(input_string.substr(pos1));
}

void Draw::setXY(int _x, int _y) {
	COORD XY;
	XY.X = _x;
	XY.Y = _y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), XY);
}

void Draw::show(int _x, int _y, string _image) {
	vector<string> list;
	SplitString(_image, list, "\n");
	for (vector<string>::iterator it = list.begin(); it != list.end(); it++) {
		setXY(_x, _y++);
		cout << *it;
	}
	list.clear();
}

void Draw::clearMap(int _x, int _y, int _width, int _heigh) {
	for (int i = _x; i < _x + _width; i++)
		for (int j = _y; j < _y + _heigh; j++) {
			setXY(i, j);
			cout << " ";
		}
}

string Draw::getImage(string path, string _image, string end) {
	string buff;
	string out = "";
	ifstream st(path);
	if (!st.is_open()) return "NULL";
	bool isfind = false;
	while (!st.eof() && !isfind) {
		getline(st, buff);
		if (buff == "[" + _image + "]") {
			getline(st, buff);
			while (buff != "[" + end + "]") {
				out += buff + "\n";
				getline(st, buff);
			}
			out = out.substr(0, out.rfind("\n"));
		}
	}
	st.close();
	return out;
}

void Draw::showObject(int _x, int _y, string path, string _image) {
	show(_x, _y, getImage(path, _image));
}