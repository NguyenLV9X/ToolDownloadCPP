#include<iostream>
using namespace std;

class DownloadManager
{
private:

	string strUrl;
	int inConnection_count;
	int inThread_count;

public:

	DownloadManager();
	DownloadManager(const DownloadManager&) = delete;
	DownloadManager& operator=(const DownloadManager&) = delete;
	~DownloadManager();
	int is_valid(string url, int connection_count = 1, int thread_count = 1);
	void app_main();

};
