/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *      Andras Sevcsik <sevcsik@gmail.com>
 */
 
#include "defines.h"
#include "PasteOMaticPaster.h"

#include <Clipboard.h>
#include <Entry.h>
#include <Messenger.h>

#include <iostream>
#include <stdlib.h>

PasteOMaticPaster::PasteOMaticPaster(BHandler *handler) : BLooper()
{
	fHandler = handler;
	success = false;
}


void PasteOMaticPaster::MessageReceived(BMessage *message)
{
	void *data;
	char *returnString;
	int32 size;
	entry_ref ref;
	BMessage reply;
	BMessage *clip;
	
	switch (message->what)
	{
		case MESSAGE_PASTE_POINTER:
			cout << "Paster: got pointer\n";
			message->PrintToStream();
			message->FindPointer("data", &data);
			message->FindInt32("size", &size);
			returnString = _Paste(data, (size_t)size);
			break;
		case MESSAGE_PASTE_REF:
			cout << "Paster: got ref\n";
			message->PrintToStream();
			message->FindRef("ref", &ref);
			returnString = _Paste(&ref);
	}
	if (success)
	{
		cerr << "Paster sending SUCCESS message: ";
		reply.what = MESSAGE_PASTE_SUCCESS;
		reply.AddString("link", returnString);
		
		be_clipboard->Lock();
		be_clipboard->Clear();
		if ((clip = be_clipboard->Data()))
		{
			clip->AddData("text/plain", B_MIME_TYPE, returnString, strlen(returnString));
			be_clipboard->Commit();
		}
		be_clipboard->Unlock();
			
		
		reply.PrintToStream();
		fHandler->Looper()->Lock();
		fHandler->MessageReceived(&reply);
		fHandler->Looper()->Unlock();
	}
	else
	{
		cerr << "Paster sending FAIL message: ";
		reply.what = MESSAGE_PASTE_FAIL;
		reply.AddString("error", returnString);
		reply.PrintToStream();
		fHandler->Looper()->Lock();
		fHandler->MessageReceived(&reply);
		fHandler->Looper()->Unlock();
	}
	
	Quit();
}


PasteOMaticPaster::~PasteOMaticPaster()
{
}


bool PasteOMaticPaster::Check(char *type, size_t size)
{
	cerr << "Check(char *type, size_t size) not implemented\n";
	return false;
}


char *PasteOMaticPaster::_Paste(void *data, size_t size)
{
	cerr << "Paste(void *data, size_t size) not implemented\n";
	return NULL;
}


char *PasteOMaticPaster::_Paste(entry_ref *ref)
{
	cerr << "Paste(entry_ref *ref) not implemented\n";
	return NULL;
}


void PasteOMaticPaster::_Progress(int8 percentage)
{
	BMessage message;
	message.what = MESSAGE_PASTE_PROGRESS;
	message.AddInt8("percentage", percentage);
	fHandler->Looper()->Lock();
	fHandler->MessageReceived(&message);
	fHandler->Looper()->Unlock();
}


void PasteOMaticPaster::_Progress()
{
	BMessage message;
	message.what = MESSAGE_PASTE_PROGRESS;
	fHandler->Looper()->Lock();
	fHandler->MessageReceived(&message);
	fHandler->Looper()->Unlock();
}

