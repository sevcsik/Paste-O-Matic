/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *      Andras Sevcsik <sevcsik@gmail.com>
 */

#include "defines.h"
#include "PasterPastebinCa.h"

#include <Entry.h>
#include <File.h>
#include <Messenger.h>
#include <String.h>

#include <curl/curl.h>

#include <iostream>
#include <stdlib.h>

PasterPastebinCa::PasterPastebinCa(BHandler *handler)
	: PasteOMaticPaster(handler)
{
}


PasterPastebinCa::~PasterPastebinCa()
{
	if (fErrorString) delete fErrorString;
}


bool PasterPastebinCa::Check(char *type, size_t size)
{
	char buffer[6];
	
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
	BString response("");
	BString link;
	BFile file(ref, B_READ_ONLY);
	off_t size;
	char *content;
	char *encoded;
	
	fErrorString = NULL;
	
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
	
	if (!(content = (char *)malloc(size + 1)))
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
	
	encoded = curl_easy_escape(curl, ref->name, strlen(ref->name));
	
	postData << "name=" << encoded << "&type=1&expiry=";
	free(encoded);
	encoded = curl_easy_escape(curl, PASTE_DESCRIPTION, strlen(PASTE_DESCRIPTION));
	postData << "&description=" << encoded;
	free(encoded);
	encoded = curl_easy_escape(curl, content, strlen(content));
	postData << "&content=" << encoded;
	free(encoded);
	free(content);
	
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
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
	
	response.CopyInto(link, response.FindFirst(":") + 1, 100);
	response.Prepend(PASTEBIN_ROOT);
	
	cout << "Server response: " << response << endl;
	
end:	
	curl_easy_cleanup(curl);
	if (!success) return (char *)fErrorString->String();
	else return (char *)link.String();
}
