/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *      Andras Sevcsik <sevcsik@gmail.com>
 */

#include "PasterPastebinCa.h"

#include <Entry.h>
#include <Messenger.h>

#include <iostream>
#include <string.h>

PasterPastebinCa::PasterPastebinCa(BMessenger *messenger)
	: PasteOMaticPaster(messenger)
{
	cout << "PastebinCa constructor called\n";
}


PasterPastebinCa::~PasterPastebinCa()
{
}


bool PasterPastebinCa::Check(char *type, size_t size)
{
	char buffer[6];
	
	strncpy(buffer, type, 5);
	buffer[5] = '\0';
	
	if (!strcmp(buffer, "text/") && size < PASTEBIN_MAX_SIZE) return true;
	else return false;
}


char *PasterPastebinCa::_Paste(void *data, size_t size)
{
	return NULL;	
}


char *PasterPastebinCa::_Paste(entry_ref *ref)
{
	cout << "Bazinga! \n";
	return NULL;
}
