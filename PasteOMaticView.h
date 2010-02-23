#ifndef _PasteOMaticView_H_
#define _PasteOMaticView_H_

/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *      Andras Sevcsik <sevcsik@gmail.com>
 */

#include "defines.h"
#include "PasteOMaticPaster.h"
#include "PasteOMaticSettings.h"

#include <Archivable.h>
#include <Bitmap.h>
#include <Dragger.h>
#include <Entry.h>
#include <FilePanel.h>
#include <Message.h>
#include <Messenger.h>
#include <View.h>

typedef enum {SMALL_ICON, LARGE_ICON} icon_mode;

class PasteOMaticView : public BView {
    public:
        PasteOMaticView();
        PasteOMaticView(BMessage *archive);
        virtual ~PasteOMaticView();
        virtual void Draw(BRect rect);
        virtual status_t Archive(BMessage *archive, bool deep);
        static BArchivable* Instantiate(BMessage *archive);
        virtual void MessageReceived(BMessage *message);
        
    private:
    	PasteOMaticPaster *_FindPaster(char *type, size_t size);
    	void _SetSize(uint8 size);
    	void _SetSmall();
    	void _SetLarge();
    	void _SetDefault();
        void _SetSuccess();
        void _SetFail();
        void _SetWorking();
        void _SetProgress(BMessage *message);
        void _ShowPanel();
        void _ShowSettings();
        
    	void PasteOMaticView::_StartPaste(void *data, size_t size);
    	void PasteOMaticView::_StartPaste(entry_ref ref);
    	
    	bool fInWindow;
    	int16 fSize;
    	int8 fProgress;
    	
        BBitmap *fBitmapDefault;
        BBitmap *fBitmapSuccess;
        BBitmap *fBitmapFail;
        BBitmap *fBitmapWorking;
        BBitmap *fBitmap;
        BHandler *fParentHandler;
        BDragger *fDragger;
        BFilePanel *fPanel;
        BMessage *fConfig;
        BFile *fConfigFile;
        PasteOMaticSettingsWindow *fSettings;
};

#endif  // _PasteOMaticView_H_
