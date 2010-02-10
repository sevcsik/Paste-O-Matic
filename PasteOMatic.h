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

#include <curl/curl.h>

class PasteOMatic : public BApplication {
    public:
        PasteOMatic();
        virtual ~PasteOMatic();
        
        virtual void ArgvReceived(int32 argc, char **argv);
        virtual void ReadyToRun();  
        
    private:
        PasteOMaticWindow *fWindow;
};

int main();

#endif  // _PASTEOMATIC_H_
