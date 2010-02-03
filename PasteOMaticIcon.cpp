/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *      Andras Sevcsik <sevcsik@gmail.com>
 */

#include "defines.h"
#include "PasteOMatic.h"
#include "PasteOMaticIcon.h"

#include <Application.h>
#include <Bitmap.h>
#include "IconUtils.h"
#include <Message.h>
#include <Resources.h>
#include <TranslationUtils.h>
#include <View.h>

#include <iostream>
#include <stdio.h>

PasteOMaticIcon::PasteOMaticIcon(icon_mode mode) 
    : BView(BRect(0, 0, 48, 48), "fIcon", B_FOLLOW_LEFT | B_FOLLOW_V_CENTER, 
            B_WILL_DRAW)
{   
    fBitmapDefault = NULL;
    fBitmapSuccess = NULL;
    fBitmapFail = NULL;
    
    if (mode) _SetLarge();
    else _SetSmall();   
    
    fBitmap = fBitmapDefault;
    
    SetDrawingMode(B_OP_OVER);
    SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    
    //fDragger = new BDragger(BRect(0, 0, 48, 48), this);
    //AddChild(fDragger);
}

PasteOMaticIcon::PasteOMaticIcon(BMessage *archive) : BView(archive)
{
    archive->FindPointer("fBitmapDefault", &(void *)fBitmapDefault);
    archive->FindPointer("fBitmapSuccess", &(void *)fBitmapSuccess);
    archive->FindPointer("fBitmapFail", &(void *)fBitmapFail);
    
    fBitmap = fBitmapDefault;
}


PasteOMaticIcon::~PasteOMaticIcon()
{
    delete fBitmapDefault;
    delete fBitmapSuccess;
    delete fBitmapFail;
}


status_t PasteOMaticIcon::Archive(BMessage *archive, bool deep)
{
    cout << "Archiving icon... \n";
    BView::Archive(archive, deep);
    archive->AddString("add_on", SIGNATURE);
    archive->AddPointer("fBitmapDefault", (void *)fBitmapDefault);
    archive->AddPointer("fBitmapSuccess", (void *)fBitmapSuccess);
    archive->AddPointer("fBitmapFail", (void *)fBitmapFail);
    
    return B_OK;
}


BArchivable* PasteOMaticIcon::Instantiate(BMessage *archive)
{
    if (!validate_instantiation(archive, "PasteOMaticIcon"))
    {
        return NULL;
    }
    return new PasteOMaticIcon(archive);    
}


void PasteOMaticIcon::SetDefault()
{
    fBitmap = fBitmapDefault;
    Invalidate();
}


void PasteOMaticIcon::SetSuccess()
{
    fBitmap = fBitmapSuccess;
    Invalidate();
}


void PasteOMaticIcon::SetFail()
{
    fBitmap = fBitmapFail;
    Invalidate();
}


void PasteOMaticIcon::Draw(BRect rect)
{
	SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	FillRect(BRect(0, 0, 48, 48));
    DrawBitmap(fBitmap, BPoint(0, 0));
}


void PasteOMaticIcon::MessageReceived(BMessage *message)
{
    switch(message->what)
    {
        case MESSAGE_FROM_TRACKER_DND:
        case MESSAGE_FROM_OPEN_PANEL:
            my_app->PostMessage(message);
            break;  
    }
}


void PasteOMaticIcon::_SetSmall()
{
    if (fBitmapDefault) delete fBitmapDefault;
    if (fBitmapSuccess) delete fBitmapSuccess;
    if (fBitmapFail) delete fBitmapFail;
    
    fBitmapDefault = BTranslationUtils::GetBitmap(B_RAW_TYPE, "icon_small");
    fBitmapSuccess = BTranslationUtils::GetBitmap(B_RAW_TYPE, "icon_small_success");
    fBitmapFail = BTranslationUtils::GetBitmap(B_RAW_TYPE, "icon_small_fail");  
}

void PasteOMaticIcon::_SetLarge()
{
	uint8 *hvif;
	size_t size;
	BResources *resources = BApplication::AppResources();
	
    if (fBitmapDefault) delete fBitmapDefault;
    if (fBitmapSuccess) delete fBitmapSuccess;
    if (fBitmapFail) delete fBitmapFail;
    
    fBitmapDefault = new BBitmap(BRect(0, 0, 48, 48), B_RGBA32, false, true);
    fBitmapSuccess = new BBitmap(BRect(0, 0, 48, 48), B_RGBA32);
    fBitmapFail = new BBitmap(BRect(0, 0, 48, 48), B_RGBA32);
    
    hvif = (uint8 *)resources->LoadResource(B_VECTOR_ICON_TYPE, "BEOS:ICON", &size);
    if (hvif) BIconUtils::GetVectorIcon(hvif, size, fBitmapDefault);
    
    hvif = (uint8 *)resources->LoadResource(B_VECTOR_ICON_TYPE, "icon_success", &size);
    if (hvif) BIconUtils::GetVectorIcon(hvif, size, fBitmapSuccess);
    
    hvif = (uint8 *)resources->LoadResource(B_VECTOR_ICON_TYPE, "icon_fail", &size);
    if (hvif) BIconUtils::GetVectorIcon(hvif, size, fBitmapFail);
}


