#include<iostream>
using namespace std;

class DownloadManager
{
private:

	string strUrl;
	string strlinkoutput;
	int inConnection_count;
	int inThread_count;

public:

	DownloadManager();
	DownloadManager(const DownloadManager&) = delete;
	DownloadManager& operator=(const DownloadManager&) = delete;
	~DownloadManager();
	int set_valid(string url, string out, int connection_count = 1,
		int thread_count = 1);
	void app_main();

};
