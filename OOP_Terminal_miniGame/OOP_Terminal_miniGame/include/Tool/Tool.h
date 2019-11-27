#ifndef _TOOL_H_
#define _TOOL_H_

#include <iostream>
#include<vector>
#include<string>
#include <io.h>
#include <direct.h>
#include <Windows.h>

using std::string;
using std::vector;

class Tool {
public:
    static void findDir(string _path, vector<string>& _files);
    static void findFile(string _path, vector<string>& _files);
    static void mkdir(string _path);
    static void SplitString(const string input_string, vector<string>& v, const string c);
    static void getIpAppName(string _path, vector<string>& _data);
};

#endif // ! _TOOL_H_