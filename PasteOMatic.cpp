/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *      Andras Sevcsik <sevcsik@gmail.com>
 */

#include "defines.h" 
#include "PasteOMatic.h"
#include "PasteOMaticWindow.h"

#include <Clipboard.h>
#include <Deskbar.h>
#include <Entry.h>
#include <File.h>
#include <fs_attr.h>
#include <Message.h>
#include <Node.h>

#include <curl/curl.h>

#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

PasteOMatic::PasteOMatic() : BApplication(SIGNATURE)
{   
    BDeskbar deskbar;

    fWindow = new PasteOMaticWindow;
    
    fResponse = new string;
    if (!(fCurl = curl_easy_init())) cerr << "cURL failed to init...\n";
}


PasteOMatic::~PasteOMatic()
{
    curl_easy_cleanup(fCurl);
}


string *PasteOMatic::_GetFormFieldString(char *key, char *value)
{
    string *returnString = new string("");
    char *encoded;
    encoded = curl_easy_escape(fCurl, key, strlen(key));
    returnString->append(encoded);
    free(encoded);
    returnString->append("=");
    encoded = curl_easy_escape(fCurl, value, strlen(value));
    returnString->append(encoded);
    free(encoded);
    returnString->append("&");
    return returnString;
}


void PasteOMatic::_Paste2Pastebin(entry_ref *ref)
{
    BFile file(ref, B_READ_ONLY);
    BMessage message(MESSAGE_PASTE_STATUS);
    BMessage *clip = NULL;
    off_t size;
    char *fileContent;
    string link;
    string postString;
    string *formField;
    
    message.AddInt8("status", PASTE_STATUS_WORKING);
    fWindow->PostMessage(&message);
    
    if (file.GetSize(&size) != B_OK)
    {
        message.ReplaceInt8("status", PASTE_STATUS_READ_ERROR);
        fWindow->PostMessage(&message);
        return;
    }
    
    cout << "File size: " << size << endl;
    
    if (size > PASTEBIN_MAX_SIZE)
    {
        message.ReplaceInt8("status", PASTE_STATUS_TOO_LARGE_ERROR);
        fWindow->PostMessage(&message);
        return;
    }
    
    fileContent = (char *) malloc(size + 1);
    fileContent[size] = '\0';
    if (fileContent)
    {
        if (file.Read((void *)fileContent, size) < 0)
        {
            message.ReplaceInt8("status", PASTE_STATUS_READ_ERROR);
            fWindow->PostMessage(&message); 
        }
    }
    else
    {
        cerr << "Out of memory\n";
        return;
    }
    
    formField = _GetFormFieldString("name", ref->name);
    postString.append(*formField);
    delete formField;
    
    formField = _GetFormFieldString("type", "1"); /* would be cool to guess from file type */
    postString.append(*formField);
    delete formField;
    
    formField = _GetFormFieldString("expiry", "");
    postString.append(*formField);
    delete formField;
    
    formField = _GetFormFieldString("description", 
                                    PASTE_DESCRIPTION);
    postString.append(*formField);
    delete formField;
    
    formField = _GetFormFieldString("content", fileContent);
    postString.append(*formField);
    delete formField;
    free(fileContent);
    
    fResponse->assign("");
    cout << "fResponse initial contents:  " << *fResponse;
    
    curl_easy_setopt(fCurl, CURLOPT_VERBOSE, 0);
    curl_easy_setopt(fCurl, CURLOPT_NOPROGRESS, 1);
    curl_easy_setopt(fCurl, CURLOPT_WRITEFUNCTION, _CurlWriteFunction);
    curl_easy_setopt(fCurl, CURLOPT_WRITEDATA, fResponse);
    curl_easy_setopt(fCurl, CURLOPT_POST, 1);
    curl_easy_setopt(fCurl, CURLOPT_POSTFIELDS, postString.c_str());
    curl_easy_setopt(fCurl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
    curl_easy_setopt(fCurl, CURLOPT_URL, PASTEBIN_URL);
    
    if (curl_easy_perform(fCurl) != 0)
    {
        message.ReplaceInt8("status", PASTE_STATUS_CURL_ERROR);
        fWindow->PostMessage(&message);
    }
    
    curl_easy_reset(fCurl);
    
    
    if (fResponse->substr(0, 7) != "SUCCESS")
    {
        cerr << "Server rejected paste. Response: " << fResponse << endl;
        message.ReplaceInt8("status", PASTE_STATUS_CURL_ERROR);
        fWindow->PostMessage(&message);
        return; 
    }
    
    link = PASTEBIN_ROOT;
    link.append(fResponse->substr(8, 100));
    
    message.ReplaceInt8("status", PASTE_STATUS_SUCCESS);
    message.AddString("link", link.c_str());
    fWindow->PostMessage(&message);
    
    be_clipboard->Lock();
    be_clipboard->Clear();
    if ((clip = be_clipboard->Data()))
    {
        clip->AddData("text/plain", B_MIME_TYPE, link.data(), link.size());
        be_clipboard->Commit();
    }
    be_clipboard->Unlock();
}

void PasteOMatic::_Paste2Imagebin(entry_ref *ref)
{
    cerr << "Not implemented :(\n";
}

void PasteOMatic::_Paste2Filebin(entry_ref *ref)
{
    cerr << "Not implemented :(\n";
}


void PasteOMatic::_StartPaste(entry_ref *ref)
{
    BMessage message(MESSAGE_PASTE_STATUS);
    BNode node(ref);
    attr_info nodeInfo;
    char *type;

    cout << "Starting upload of " << ref->name << endl;
    if (node.GetAttrInfo("BEOS:TYPE", &nodeInfo) != B_OK)
    {
        cerr << "Can't read file type of " << ref->name << "\n";
        message.AddInt8("status", PASTE_STATUS_TYPE_ERROR);
        fWindow->PostMessage(&message);
    }
    type = (char *) malloc(nodeInfo.size);
    node.ReadAttr("BEOS:TYPE", B_STRING_TYPE, 0, type, nodeInfo.size);
    
    strtok(type, "/");
    cout << "File type: " << type << endl;
    if (!strcmp(type, "text")) _Paste2Pastebin(ref);
    else if (!strcmp(type, "image")) _Paste2Imagebin(ref);
    else _Paste2Filebin(ref);
    
    free(type);  
}


void PasteOMatic::MessageReceived(BMessage *message)
{
    entry_ref *fileRef = new entry_ref;
    
    cout << "App caught message:\n";
    message->PrintToStream();
    
    switch (message->what)
    {
        case MESSAGE_FROM_TRACKER_DND:
        case MESSAGE_FROM_OPEN_PANEL:
            cout << "Got file: ";
            if (message->FindRef("refs", fileRef) == B_OK)
            {               
                cout << fileRef->name << endl;
                _StartPaste(fileRef);
            }
            break;
        default:
            BApplication::MessageReceived(message);
    }
}


void PasteOMatic::ArgvReceived(int32 argc, char **argv)
{
    for (int i = 0; i < argc; i++)
        cout << "Arg" << i << ": " << argv[i] << endl;
}


void PasteOMatic::ReadyToRun()
{
    cout << "Paste-O-Matic v" << VERSION << " started" << endl;
    fWindow->Show();
}


size_t _CurlWriteFunction(void *ptr, size_t size, size_t nmemb, void *stream)
{
    (static_cast<string *>(stream))->append((char *) ptr, size*nmemb);
    return size*nmemb;
}


int main()
{
    PasteOMatic app;
    app.Run();
    
    return 0;   
}
