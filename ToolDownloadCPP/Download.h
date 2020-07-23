#include<iostream>
#include<map>
#include<string>
using namespace std;

class Download
{
private:

	map<int, string> mapData;
	double douFilesizeSV;
	double douFilesizeLC;
	string strFilename;
	int inTotalConnectDataIntoFile;
	string get_range(int connection_count, int numthead);
	static size_t my_write(void* buffer, size_t size, size_t nmemb, void* param);
	void push_one_connection(string url, int Connection_count, int numthead);
	bool add_data_in_map_into_file();
	bool set_name_file(string url);

public:

	Download();
	Download(const Download&) = delete;
	Download& operator=(const Download&) = delete;
	~Download();

	void initialize();
	bool check_size_file_sv(string url);
	bool check_size_file_lc();
};