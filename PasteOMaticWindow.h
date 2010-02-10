#ifndef _PASTEOMATICWINDOW_H_
#define _PASTEOMATICWINDOW_H_

/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *      Andras Sevcsik <sevcsik@gmail.com>
 */

#include "PasteOMaticView.h"
 
#include <Button.h>
#include <FilePanel.h>
#include <Message.h>
#include <TextControl.h>
#include <View.h>
#include <Window.h>


class PasteOMaticWindow : public BWindow {
    public:
        PasteOMaticWindow();
        virtual ~PasteOMaticWindow();
        
        virtual void MessageReceived(BMessage *message);
        virtual bool QuitRequested();
        
    private:
    	void _UpdateStatus(BMessage *message);
    	
        BTextControl *fTextControl;
        BButton *fOpenButton;
        BButton *fSettingsButton;
        BFilePanel *fOpenPanel;
        
        PasteOMaticView *fView;
};  

#endif  // _PASTEOMATICWINDOW_H_
