#include<iostream>
using namespace std;

class Download
{
private:
	
	double douFilesizeSV;
	double douFilesizeLC;
	string strFilename;
	string get_range(int connection_count, int numthead);
	static size_t my_write(void* buffer, size_t size, size_t nmemb, void* param);
	void push_one_connection(string url, int Connection_count, int numthead);

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