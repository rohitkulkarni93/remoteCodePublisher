#pragma once

#include <iostream>

using namespace std;

struct ServerConfiguration
{
	string wwwroot;
	string url;
	int port;

	string publishedFilesDir;
	string uploadedFilesDir;

};
