#include"DownloadManager.h"
#include"Download.h"

DownloadManager::DownloadManager() :
	inConnection_count(1), inThread_count(1)
{
}

DownloadManager::~DownloadManager()
{
}

int DownloadManager::set_valid(string url, int connection_count,
	int thread_count)
{
	strUrl = url;
	inConnection_count = connection_count;
	inThread_count = thread_count;
	return 0;
}

void DownloadManager::app_main()
{
	return;
}
