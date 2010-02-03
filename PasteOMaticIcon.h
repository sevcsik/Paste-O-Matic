#ifndef _PASTEOMATICICON_H_
#define _PASTEOMATICICON_H_

/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *      Andras Sevcsik <sevcsik@gmail.com>
 */

#include "defines.h"

#include <Archivable.h>
#include <Bitmap.h>
#include <Dragger.h>
#include <View.h>

typedef enum {SMALL_ICON, LARGE_ICON} icon_mode;

class PasteOMaticIcon : public BView {
    public:
        PasteOMaticIcon(icon_mode mode);
        PasteOMaticIcon(BMessage *archive);
        virtual ~PasteOMaticIcon();
        virtual void Draw(BRect rect);
        virtual status_t Archive(BMessage *archive, bool deep);
        virtual BArchivable* Instantiate(BMessage *archive);
        virtual void MessageReceived(BMessage *message);
        
        void SetDefault();
        void SetSuccess();
        void SetFail();
    private:
    	void _SetSmall();
    	void _SetLarge();
    	
    	bool fInWindow;
        BBitmap *fBitmapDefault;
        BBitmap *fBitmapSuccess;
        BBitmap *fBitmapFail;
        BBitmap *fBitmap;
        BDragger *fDragger;
};

#endif  // _PASTEOMATICICON_H_
