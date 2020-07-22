#include<iostream>
using namespace std;

class Download
{
private:
	
	double douFilesizeSV;
	double douFilesizeLC;
	string strFilename;

public:

	Download();
	Download(const Download&) = delete;
	Download& operator=(const Download&) = delete;
	~Download();

	void initialize();
	bool check_size_file_sv(string url);
	bool check_size_file_lc();
	bool set_name_file(string url);
};