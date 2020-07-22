#include"Download.h"
#include"curl/curl.h"
#include<fstream>

Download::Download():
	douFilesizeSV(0.0), douFilesizeLC(0.0)
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

bool Download::check_size_file_lc()
{
	streampos begin, end;
	ifstream filelc(strFilename, ios::binary);
	begin = filelc.tellg();
	filelc.seekg(0, ios::end);
	end = filelc.tellg();
	filelc.close();
	douFilesizeLC = end - begin;
	cout << "size file local:" << douFilesizeLC;
	return true;
}

bool Download::set_name_file(string url)
{
	size_t found = url.find_last_of("/");
	strFilename = url.substr(found + 1);
	return true;
}
