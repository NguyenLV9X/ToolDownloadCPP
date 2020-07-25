#include"DownloadManager.h"

int main()
{
	string url;
	int connect_count;
	int thread_count;
	string out;
	DownloadManager tool;
	cout << "getfile ";
	cin >> url;
	cout << "--connection-count=";
	cin >> connect_count;
	cout << "--thread-count=";
	cin >> thread_count;
	cout << "--output=";
	cin >> out;
	tool.set_valid(url, out, connect_count, thread_count);
	tool.app_main();

	return 1;
}