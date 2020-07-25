#include"Download.h"
#include<fstream>
#include<iterator>
#include<filesystem>
#include<regex>

#define CURL_STATICLIB
#include"curl/curl.h"

#ifdef _DEBUG
#pragma comment
#   pragma comment (lib, "curl/libcurl_a_debug.lib")
#else
#   pragma comment (lib, "curl/libcurl_a.lib")
#endif

strData Download::push_one_connection(string url, int connection_count, int numthread)
{
	strData datathread;
	datathread.numthread = numthread;
	struct curl_slist* list_range = NULL;
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	string strRange = get_range(connection_count, numthread);
	list_range = curl_slist_append(list_range, strRange.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list_range);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &datathread.Data);
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //show info
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return datathread;
}

bool Download::add_data_in_map_into_file()
{
	ofstream filedata;
	filedata.open(strOutput, ios::app | ios::binary | ios::out);
	map<int, string>::iterator itr;
	for (itr = mapData.begin(); itr != mapData.end(); ++itr) 
	{
		if (itr->first == inTotalConnectDataIntoFile)
		{
			filedata << itr->second;
			mapData.erase(itr);
			inTotalConnectDataIntoFile++;
		}
		else if (itr->first > inTotalConnectDataIntoFile)
			break;
	}
	filedata.close();
	
	return true;
}

string Download::get_range(int connection_count, int numthead)
{
	int inStart = (((int)douFilesizeSV - (int)douFilesizeLC) / connection_count * numthead) + numthead;
	if (douFilesizeLC >= 0)
		inStart += (int)douFilesizeLC;
	int inEnd = inStart + ((int)douFilesizeSV - (int)douFilesizeLC) / connection_count;
	string strStart = to_string(inStart);
	string strEnd = to_string(inEnd);
	if (connection_count == numthead + 1)
		strEnd = to_string((int)douFilesizeSV - 1);
	string strRange = "Range: bytes=" + strStart + "-" + strEnd;
	return strRange;
}

size_t Download::my_write(void* buffer, size_t size, size_t nmemb, void* param)
{
	string& text = *static_cast<std::string*>(param);
	size_t totalsize = size * nmemb;
	text.append(static_cast<char*>(buffer), totalsize);
	return totalsize;
}

Download::Download():
	douFilesizeSV(0.0), douFilesizeLC(0.0), inTotalConnectDataIntoFile(0), inTotalThreadsRunning(0)
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
	set_name_file(url);
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

bool Download::check_size_file_lc(string link)
{
	set_link_output(link);
	streampos begin, end;
	ifstream filelc(strOutput, ios::binary);
	begin = filelc.tellg();
	filelc.seekg(0, ios::end);
	end = filelc.tellg();
	filelc.close();
	douFilesizeLC = end - begin;
	if (douFilesizeLC >= douFilesizeSV)
	{
		remove(strOutput.c_str());
		douFilesizeLC = 0;
	}
	return true;
}

void Download::start_download(string url, int Connection_count, int Thread_count)
{
	for (int i = 0; i < Connection_count; i++)
	{
		while (1)
		{
			if (!check_threads_running(Thread_count))
			{
				check_data_finished(Connection_count);
				break;
			}
		}

		listThreads.push_back(async(launch::async, &Download::push_one_connection, this, url, Connection_count, i));
	}
	while (1)
	{
		if (!check_data_finished(Connection_count))
			break;
	}
	return;
}

void Download::finalize()
{
	curl_global_cleanup();
	return;
}

bool Download::set_name_file(string url)
{
	size_t found = url.find_last_of("/");
	strFilename = url.substr(found + 1);
	return true;
}

bool Download::set_link_output(string link)
{
	strOutput = link;
	return true;
}

bool Download::check_threads_running(int Thread_count)
{
	if (Thread_count > inTotalThreadsRunning)
	{
		inTotalThreadsRunning++;
		return false;
	}
	else
	{
		list<future<strData>>::iterator itr;
		chrono::milliseconds span(0);
		for (itr = listThreads.begin(); itr != listThreads.end(); ++itr)
		{
			if (itr->wait_for(span) == future_status::ready)
				return false;
		}
	}
	return true;
}

bool Download::check_data_finished(int Connection_count)
{
	list<future<strData>>::iterator itr;
	chrono::milliseconds span(0);
	for (itr = listThreads.begin(); itr != listThreads.end(); ++itr)
	{
		if (itr->wait_for(span) == future_status::ready)
		{
			strData datathread = itr->get();
			mapData.insert(pair<int, string>(datathread.numthread, datathread.Data));
			listThreads.erase(itr);
			add_data_in_map_into_file();
		}
	}
	if (inTotalConnectDataIntoFile == Connection_count)
		return false;
	return true;
}
