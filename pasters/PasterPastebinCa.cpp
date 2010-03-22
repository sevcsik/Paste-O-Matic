/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *      Andras Sevcsik <sevcsik@gmail.com>
 */

#include "defines.h"
#include "PasterPastebinCa.h"

#include <curl/curl.h>

#include <Entry.h>
#include <File.h>
#include <Messenger.h>
#include <String.h>

#include <iostream>
using namespace std;
#include <stdlib.h>

const char TYPES[][2][10] = {
	{".ada",	"19"},
	{".asm", 	"21"},
	{".asp", 	"22"},
	{".css", 	"24"},
	{".cpp", 	"4"},
	{".c", 		"3"},
	{".htm", 	"26"},
	{".h", 		"4"},
	{".php",	"5"},
	{".rb", 	"10"},
	{".sh",		"23"},
	{".pl",		"6"},
	{".py",		"11"} 	
};


PasterPastebinCa::PasterPastebinCa(BHandler *handler)
	: PasteOMaticPaster(handler)
{
	fErrorString = NULL;
	fLink = NULL;
}


PasterPastebinCa::~PasterPastebinCa()
{
	if (fErrorString) delete fErrorString;
	else if (fLink) delete fLink;
}


bool PasterPastebinCa::Check(char *type, size_t size)
{
	char buffer[6];
	
	cout << sizeof(TYPES) << endl;
	
	strncpy(buffer, type, 5);
	buffer[5] = '\0';
	
	if (!strcmp(buffer, "text/") && size < PASTEBIN_MAX_SIZE) return true;
	else return false;
}


size_t PasterPastebinCa::_CurlWriteFunction(void *ptr, size_t size, 
											size_t nmemb, void *stream)
{
    (static_cast<BString *>(stream))->Append((char *) ptr, size*nmemb);
    return size*nmemb;
}


char *PasterPastebinCa::_Paste(void *data, size_t size)
{
	return NULL;	
}


char *PasterPastebinCa::_Paste(entry_ref *ref)
{
	CURL *curl = curl_easy_init();
	BString postData;
	BString response;
	BString filename;
	BFile file(ref, B_READ_ONLY);
	off_t size;
	char *content;
	char *encoded;
	char *type;
	
	fErrorString = NULL;
	
	_Progress();
	
	if (!curl)
	{
		fErrorString = new BString(E"Failed to initialize curl.");
		goto end;	
	}
	
	if (file.GetSize(&size) != B_OK)
	{
		fErrorString = new BString(E"Can't read file.");
		goto end;
	}
	
	if (!(content = new char[size + 1]))
	{
		fErrorString = new BString(E"Out of memory.");
		goto end;
	}
	
	content[file.Read((void *)content, size)] = '\0';
	
	if (strlen(content) == 0)
	{
		fErrorString = new BString(E"Can't read file.");
		goto end;
	}
	
	filename = ref->name;
	for (uint8 i = 0; i < sizeof(TYPES)/20; i++)
	{
		if (filename.IFindFirst(TYPES[i][0]) != B_ERROR)
		{
			type = (char *) TYPES[i][1];
			break;
		}
	}
	
	encoded = curl_easy_escape(curl, ref->name, strlen(ref->name));
	
	postData << "name=" << encoded << "&type=" << type << "&expiry=";
	free(encoded);
	encoded = curl_easy_escape(curl, PASTE_DESCRIPTION, strlen(PASTE_DESCRIPTION));
	postData << "&description=" << encoded;
	free(encoded);
	encoded = curl_easy_escape(curl, content, strlen(content));
	postData << "&content=" << encoded;
	free(encoded);
	free(content);
	
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _CurlWriteFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.String());
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
    curl_easy_setopt(curl, CURLOPT_URL, PASTEBIN_URL);
    
    if (curl_easy_perform(curl))
    {
		fErrorString = new BString(E"Network failure.");
		goto end;
	}
	
	if (response.FindFirst("SUCCESS:") == B_ERROR)
	{
		fErrorString = new BString(E"Paste rejected.");
		goto end;
	}
	
	success = true;
	fLink = new BString;
	
	response.CopyInto(*fLink, response.FindFirst(":") + 1, 100);
	fLink->Prepend(PASTEBIN_ROOT);
	
	cout << "Server response: " << response << endl;
	
end:	
	curl_easy_cleanup(curl);
	if (!success) return (char *)fErrorString->String();
	else return (char *)fLink->String();
}
