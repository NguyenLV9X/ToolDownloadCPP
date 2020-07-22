#include "Download.h"
#include"curl/curl.h"

Download::Download():
	douFilesizeSV(0.0)
{
}

Download::~Download()
{
}

void Download::initialize()
{
	curl_global_init(CURL_GLOBAL_ALL);
}

bool Download::check_size_file_sv(string url)
{
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
	curl_easy_perform(curl);
	curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD,
		&douFilesizeSV);
	curl_easy_cleanup(curl);
	if( douFilesizeSV == -1 )
		return false;

	return true;
}
