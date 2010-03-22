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

#include <Application.h>

#include <iostream>
using namespace std;
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

PasteOMatic::PasteOMatic() : BApplication(SIGNATURE)
{   
    fWindow = new PasteOMaticWindow;
}


PasteOMatic::~PasteOMatic()
{
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

int main()
{
    PasteOMatic app;
    app.Run();
    
    return 0;   
}
