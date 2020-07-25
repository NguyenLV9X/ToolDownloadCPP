#include<iostream>
#include<map>
#include<string>
#include<list>
#include<future>
using namespace std;

struct strData
{
	string Data;
	int numthread;
};

class Download
{
private:

	list<future<strData>> listThreads;
	map<int, string> mapData;
	double douFilesizeSV;
	double douFilesizeLC;
	string strFilename;
	string strOutput;
	int inTotalConnectDataIntoFile;
	int inTotalThreadsRunning;
	string get_range(int connection_count, int numthead);
	static size_t my_write(void* buffer, size_t size, size_t nmemb, void* param);
	strData push_one_connection(string url, int Connection_count, int numthread);
	bool add_data_in_map_into_file();
	bool set_name_file(string url);
	bool set_link_output(string link);
	bool check_threads_running(int Thread_count);
	bool check_data_finished(int Connection_count);

public:

	Download();
	Download(const Download&) = delete;
	Download& operator=(const Download&) = delete;
	~Download();

	void initialize();
	bool check_size_file_sv(string url);
	bool check_size_file_lc(string link);
	void start_download(string url, int Connection_count, int Thread_count);
	void finalize();
	
};