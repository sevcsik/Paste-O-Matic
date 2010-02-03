#ifndef _PASTEOMATIC_H_
#define _PASTEOMATIC_H_

/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *      Andras Sevcsik <sevcsik@gmail.com>
 */

#include "PasteOMaticWindow.h"

#include <Application.h>
#include <Message.h>
#include <Path.h>

#include <curl/curl.h>

#include <string>

class PasteOMatic : public BApplication {
    public:
        PasteOMatic();
        virtual ~PasteOMatic();
        
        virtual void ArgvReceived(int32 argc, char **argv);
        virtual void MessageReceived(BMessage *message);
        virtual void ReadyToRun();  
        
    private:
        PasteOMaticWindow *fWindow;
        
        CURL *fCurl;
        string *fResponse;
                
        void _StartPaste(entry_ref *ref);
        string *_GetFormFieldString(char *key, char *value);
        void _Paste2Filebin(entry_ref *ref);
        void _Paste2Imagebin(entry_ref *ref);
        void _Paste2Pastebin(entry_ref *ref);
};

size_t _CurlWriteFunction(void *ptr, size_t size, size_t nmemb, void *stream);

int main();

#endif  // _PASTEOMATIC_H_
