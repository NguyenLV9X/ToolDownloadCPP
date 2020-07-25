#include"DownloadManager.h"
#include"Download.h"

DownloadManager::DownloadManager() :
	inConnection_count(1), inThread_count(1)
{
}

DownloadManager::~DownloadManager()
{
}

int DownloadManager::set_valid(string url, string out , int connection_count,
	int thread_count)
{
	strUrl = url;
	strlinkoutput = out;
	inConnection_count = connection_count;
	inThread_count = thread_count;
	return 0;
}

void DownloadManager::app_main()
{
	Download manager;
	manager.initialize();
	manager.check_size_file_sv(strUrl);
	manager.check_size_file_lc(strlinkoutput);
	manager.start_download(strUrl, inConnection_count, inThread_count);
	manager.finalize();

	return;
}
