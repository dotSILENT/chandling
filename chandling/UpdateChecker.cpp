#include "UpdateChecker.h"
#include "main.h"

#include "curl/curl.h"
#pragma comment(lib, "curl/lib/libcurl.lib")

#include <string>

#define CHANDLING_GITHUB_API_URL "https://api.github.com/repos/" CHANDLING_GITHUB_REPO "/releases/latest"

static size_t CurlWriteCallback(void *content, size_t size, size_t nmemb, void* ptr)
{
	((std::string*)ptr)->append((char*)content, size * nmemb);
	return size * nmemb;
}

bool UpdateChecker::CheckForUpdate()
{
	std::string buffer;

	CURL *curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

	if (!curl)
	{
		curl_easy_cleanup(curl);
		return false;
	}

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "User-Agent: CHandlingUpdateChk");
	headers = curl_slist_append(headers, "Accept: application/vnd.github.v3+json");

	curl_easy_setopt(curl, CURLOPT_URL, CHANDLING_GITHUB_API_URL);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1000);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 1000);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	if (res == CURLE_OK)
	{
		/*
			Parse tag_name from the returned json
			I know this is a silly approach, but I couldn't find any good and very lightweight JSON lib
			most of them were actually bigger than this entire project, and one single-header wasn't compatible with my MSVC version
		*/

#define JS_TAG_NAME "\"tag_name\""
		int pos = buffer.find(JS_TAG_NAME);

		if (pos == std::string::npos)
			return false;

		pos += strlen(JS_TAG_NAME);
		pos = 1 + buffer.find("\"", pos); // find the starting quote
		int endPos = buffer.find("\"", pos); // find the ending quote

		std::string newver = buffer.substr(pos, endPos - pos);

		// Just a simple string comparison, might change that some time but I think it's sufficient
		if (newver != CHANDLING_VERSION)
		{
			DebugPrint("There seems to be a new release (%s != " CHANDLING_VERSION ") available", newver.c_str());

			int result = MessageBoxA(NULL, "New version of CHandling plugin is available\nDo you want to go to the download page?\n\nUpdating to the latest version might be required to use the plugin", "CHandling Update", MB_YESNO | MB_ICONINFORMATION);

			if (result == IDYES)
			{
				ShellExecuteA(0, 0, "https://github.com/" CHANDLING_GITHUB_REPO "/releases/latest", 0, 0, SW_SHOW);
			}

			return true;
		}
#undef JS_TAG_NAME
	}
	return false;
}