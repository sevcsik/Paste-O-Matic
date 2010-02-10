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
#include "PasteOMaticWindow.h"

#include <Bitmap.h>
#include <Button.h>
#include <Entry.h>
#include <GraphicsDefs.h>
#include <GroupLayout.h>
#include <InterfaceDefs.h>
#include <LayoutItem.h>
#include <TextControl.h>
#include <Window.h>

#include <iostream>

PasteOMaticWindow::PasteOMaticWindow() : BWindow(BRect(100, 100, 550, 148), 
                                                 WINDOW_TITLE,
                                                 B_TITLED_WINDOW,
                                                 B_NOT_V_RESIZABLE |
                                                 B_NOT_ZOOMABLE)
{
    BGroupLayout *layout;
    BLayoutItem *item;
    
    fView = new PasteOMaticView();
   
    fTextControl = new BTextControl(BRect(0, 0, 10, 10), "fTextControl",
                                    "link", "<- Drop something here", NULL);
    fTextControl->SetEnabled(false);
    
    fOpenButton = new BButton(BRect(10, 0, 20, 10), "fOpenButton",
                              "Open File" B_UTF8_ELLIPSIS,
                              new BMessage(MESSAGE_OPEN));
    fOpenButton->MakeDefault(true);
    
    fSettingsButton = new BButton(BRect(10, 0, 20, 10), "fSettingsButton",
                                 "Settings" B_UTF8_ELLIPSIS,
                                 new BMessage(MESSAGE_SETTINGS));
    
    layout = new BGroupLayout(B_HORIZONTAL, 0);
    SetLayout(layout);
    layout->View()->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    item = layout->AddView(static_cast<BView *> (fView));
    item->SetExplicitMaxSize(BSize(48, 48));
    item->SetExplicitMinSize(BSize(48, 48));
    layout->AddView(static_cast<BView *> (fTextControl));
    layout->AddView(static_cast<BView *> (fOpenButton));
    layout->AddView(static_cast<BView *> (fSettingsButton));
    
    SetSizeLimits(450, 10000000, 40, 10000000);
}

PasteOMaticWindow::~PasteOMaticWindow()
{
}

void PasteOMaticWindow::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case MESSAGE_SETTINGS:
		case MESSAGE_OPEN:
			fView->MessageReceived(message);
			break;
		case MESSAGE_PASTE_FAIL:
			_UpdateStatus(message);
			break;
		default:
			BWindow::MessageReceived(message);
	}
}


bool PasteOMaticWindow::QuitRequested()
{
    cout << "Bye\n";
    my_app->PostMessage(B_QUIT_REQUESTED);
    return true;
}

void PasteOMaticWindow::_UpdateStatus(BMessage *message)
{
	const char *errorString;
	
	message->FindString("error", &errorString);
	fTextControl->SetText(errorString);	
}
