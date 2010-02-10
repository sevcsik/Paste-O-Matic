/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *      Andras Sevcsik <sevcsik@gmail.com>
 */
 
#include "defines.h"
#include "PasteOMaticPaster.h"

#include <Entry.h>
#include <Messenger.h>

#include <iostream>
#include <stdlib.h>

PasteOMaticPaster::PasteOMaticPaster(BMessenger *messenger) : BLooper()
{
	fMessenger = messenger;	
}

void PasteOMaticPaster::MessageReceived(BMessage *message)
{
	void *data;
	char *status;
	int32 size;
	entry_ref ref;
	BMessage reply;
	
	switch (message->what)
	{
		case MESSAGE_PASTE_POINTER:
			cout << "Paster: got pointer\n";
			message->PrintToStream();
			message->FindPointer("data", &data);
			message->FindInt32("size", &size);
			status = _Paste(data, (size_t)size);
			break;
		case MESSAGE_PASTE_REF:
			cout << "Paster: got ref\n";
			message->PrintToStream();
			message->FindRef("ref", &ref);
			status = _Paste(&ref);
	}
	if (!status)
	{
		reply.what = MESSAGE_PASTE_SUCCESS;
		fMessenger->SendMessage(&reply);
	}
	else
	{
		reply.what = MESSAGE_PASTE_FAIL;
		reply.AddString("error", status);
		fMessenger->SendMessage(&reply);
		free(status);
	}
}

PasteOMaticPaster::~PasteOMaticPaster()
{
}

bool PasteOMaticPaster::Check(char *type, size_t size)
{
	cerr << "Check() not implemented\n";
	return false;
}

char *PasteOMaticPaster::_Paste(void *data, size_t size)
{
	cerr << "Paste() not implemented\n";
	return NULL;
}

char *PasteOMaticPaster::_Paste(entry_ref *ref)
{
	cerr << "Paste() not implemented\n";
	return NULL;
}

