/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *      Andras Sevcsik <sevcsik@gmail.com>
 */

#include "defines.h"
#include "PasteOMatic.h"
#include "PasteOMaticView.h"
#include "PasteOMaticPaster.h"

#include "PasterPastebinCa.h"

#include <Application.h>
#include <Bitmap.h>
#include <File.h>
#include <fs_attr.h>
#include "IconUtils.h"
#include <Message.h>
#include <Messenger.h>
#include <Node.h>
#include <Resources.h>
#include <String.h>
#include <TranslationUtils.h>
#include <View.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

PasteOMaticView::PasteOMaticView(uint8 size = 48) 
    : BView(BRect(0, 0, size, size), "fIcon", B_FOLLOW_LEFT | B_FOLLOW_V_CENTER, 
            B_WILL_DRAW)
{   
    fBitmapDefault = NULL;
    fBitmapSuccess = NULL;
    fBitmapFail = NULL;
    
    _SetSize(size);
    fSize = size;
    
    fBitmap = fBitmapDefault;
    
    fMessenger = new BMessenger(this, NULL);
    
    SetDrawingMode(B_OP_OVER);
    SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    
    //fDragger = new BDragger(BRect(0, 0, 48, 48), this);
    //AddChild(fDragger);
}

PasteOMaticView::~PasteOMaticView()
{
    delete fBitmapDefault;
    delete fBitmapSuccess;
    delete fBitmapFail;
}


status_t PasteOMaticView::Archive(BMessage *archive, bool deep)
{
    cout << "Archiving icon... \n";
    BView::Archive(archive, deep);
    archive->AddString("add_on", SIGNATURE);
    
    return B_OK;
}


BArchivable* PasteOMaticView::Instantiate(BMessage *archive)
{
    if (!validate_instantiation(archive, "PasteOMaticView"))
    {
        return NULL;
    }
    return new PasteOMaticView;    
}


void PasteOMaticView::_SetDefault()
{
    fBitmap = fBitmapDefault;
    Invalidate();
}


void PasteOMaticView::_SetSuccess()
{
    fBitmap = fBitmapSuccess;
    Invalidate();
}


void PasteOMaticView::_SetFail()
{
    fBitmap = fBitmapFail;
    Invalidate();
}


void PasteOMaticView::Draw(BRect rect)
{
	SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	//FillRect(BRect(0, 0, 48, 48));
    DrawBitmap(fBitmap, BPoint(0, 0));
}


void PasteOMaticView::MessageReceived(BMessage *message)
{
	entry_ref ref;
	
	cout << "View recieved message: \n";
    message->PrintToStream();
    
    switch(message->what)
    {
    	case MESSAGE_FROM_OPEN_PANEL:
    	case MESSAGE_FROM_TRACKER_DND:
    		if (message->FindRef("refs", &ref) == B_OK)
    			_StartPaste(ref);
    		break;
    	case MESSAGE_PASTE_FAIL:
    		_SetFail();
    		if (Looper()) Looper()->PostMessage(message);
    	default:
    		BView::MessageReceived(message);
    }
}

PasteOMaticPaster *PasteOMaticView::_FindPaster(char *type, size_t size)
{
	PasteOMaticPaster *paster;
	
	for (int i = 0; i < 1; i++)
	{ 
		switch (i) /* There must be a nicer way to do this... */
		{
			case 0:
				paster = static_cast<PasteOMaticPaster *> (new PasterPastebinCa(fMessenger));
				break;
		}
		
		if (paster->Check(type, size))
		{
			cout << "Paster will be " << i << endl;
			break;
		}
		else
		{
			delete paster;
			paster = NULL;
		}
	}
	return paster;
}

void PasteOMaticView::_SetSize(uint8 size)
{
	if (size <= 16)
	{
		fSize = 16;
		_SetSmall();	
	}
	else
	{
		fSize = size;
		_SetLarge();
	}
}

void PasteOMaticView::_SetSmall()
{
    if (fBitmapDefault) delete fBitmapDefault;
    if (fBitmapSuccess) delete fBitmapSuccess;
    if (fBitmapWorking) delete fBitmapWorking;
    if (fBitmapFail) delete fBitmapFail;
    
    fBitmapDefault = BTranslationUtils::GetBitmap(B_RAW_TYPE, "icon_small");
    fBitmapSuccess = BTranslationUtils::GetBitmap(B_RAW_TYPE, "icon_small_success");
    fBitmapWorking = BTranslationUtils::GetBitmap(B_RAW_TYPE, "icon_small_working");
    fBitmapFail = BTranslationUtils::GetBitmap(B_RAW_TYPE, "icon_small_fail"); 
    
    Invalidate(); 
}

void PasteOMaticView::_SetLarge()
{
	uint8 *hvif;
	size_t size;
	BResources *resources = BApplication::AppResources();
	
    if (fBitmapDefault) delete fBitmapDefault;
    if (fBitmapSuccess) delete fBitmapSuccess;
    if (fBitmapWorking) delete fBitmapWorking;
    if (fBitmapFail) delete fBitmapFail;
    
    fBitmapDefault = new BBitmap(BRect(0, 0, fSize, fSize), B_RGBA32);
    fBitmapSuccess = new BBitmap(BRect(0, 0, fSize, fSize), B_RGBA32);
    fBitmapWorking = new BBitmap(BRect(0, 0, fSize, fSize), B_RGBA32);
    fBitmapFail = new BBitmap(BRect(0, 0, fSize, fSize), B_RGBA32);
    
    hvif = (uint8 *)resources->LoadResource(B_VECTOR_ICON_TYPE, "BEOS:ICON", &size);
    if (hvif) BIconUtils::GetVectorIcon(hvif, size, fBitmapDefault);
    
    hvif = (uint8 *)resources->LoadResource(B_VECTOR_ICON_TYPE, "icon_success", &size);
    if (hvif) BIconUtils::GetVectorIcon(hvif, size, fBitmapSuccess);
    
    hvif = (uint8 *)resources->LoadResource(B_VECTOR_ICON_TYPE, "icon_working", &size);
    if (hvif) BIconUtils::GetVectorIcon(hvif, size, fBitmapWorking);
    
    hvif = (uint8 *)resources->LoadResource(B_VECTOR_ICON_TYPE, "icon_fail", &size);
    if (hvif) BIconUtils::GetVectorIcon(hvif, size, fBitmapFail);
    
    Invalidate();
}


void PasteOMaticView::_StartPaste(void *data, size_t size)
{
	cout << "Pretending doing something useful...\n";	
}


void PasteOMaticView::_StartPaste(entry_ref ref)
{
	PasteOMaticPaster *paster;
	char *type;
	off_t size;
	BNode node(&ref);
	BFile file(&ref, B_READ_ONLY);
	attr_info attrInfo;
	BMessage message;
	BString errorString;
	
	node.GetAttrInfo("BEOS:TYPE", &attrInfo);
	type = (char *) malloc(attrInfo.size);
	node.ReadAttr("BEOS:TYPE", B_STRING_TYPE, 0, type, attrInfo.size);
	file.GetSize(&size);
	
	paster = _FindPaster(type, (size_t)size);
	if (!paster)
	{
		errorString << "Can't handle type (" << type << ") or size (" << size << " bytes)";
		
		message.what = MESSAGE_PASTE_FAIL;
		message.AddString("error", errorString.String());
		MessageReceived(&message);
		return;
	}
	
	paster->Run();
	
	message.what = MESSAGE_PASTE_REF;
	message.AddRef("ref", &ref);
	paster->PostMessage(&message);
}
