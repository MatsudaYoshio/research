#include <iostream>
#include <string>
#include <vector>

#include <Windows.h>

using namespace std;

int main() {
	const string path = "";
	
	HANDLE hFind;
	WIN32_FIND_DATAA win32fd;
	hFind = FindFirstFileA((path + "*.jpg").c_str(), &win32fd);

	int num = 1;
	string old_filename, new_filename;

	do {
		old_filename = path + win32fd.cFileName;
		new_filename = path + "background" + to_string(num++) + ".jpg";
		
		rename(old_filename.c_str(), new_filename.c_str());

		cout << "rename file : " << old_filename << " -> " << new_filename << endl;
	} while (FindNextFileA(hFind, &win32fd));
	CloseHandle(hFind);


	return 0;
}