#include"Download.h"
#include"curl/curl.h"
#include<fstream>
#include<iterator>

void Download::push_one_connection(string url, int connection_count, int numthead)
{
	struct curl_slist* list_range = NULL;
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	string strRange = get_range(connection_count, numthead);
	list_range = curl_slist_append(list_range, strRange.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list_range);

	string data;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //show info
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	mapData.insert(pair<int, string>(numthead, data));
	add_data_in_map_into_file();
}

bool Download::add_data_in_map_into_file()
{
	ofstream filedata;
	filedata.open(strFilename, ios::app | ios::binary | ios::out);
	if (!filedata)
		return false; //File opened in somewhere

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
	int inStart = ((douFilesizeSV - douFilesizeLC) / connection_count * numthead) + numthead;
	int inEnd = inStart + (douFilesizeSV - douFilesizeLC) / connection_count;
	string strStart = to_string(inStart);
	string strEnd = to_string(inEnd);
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
	douFilesizeSV(0.0), douFilesizeLC(0.0), inTotalConnectDataIntoFile(0)
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

bool Download::check_size_file_lc()
{
	streampos begin, end;
	ifstream filelc(strFilename, ios::binary);
	begin = filelc.tellg();
	filelc.seekg(0, ios::end);
	end = filelc.tellg();
	filelc.close();
	douFilesizeLC = end - begin;
	return true;
}

bool Download::set_name_file(string url)
{
	size_t found = url.find_last_of("/");
	strFilename = url.substr(found + 1);
	return true;
}
