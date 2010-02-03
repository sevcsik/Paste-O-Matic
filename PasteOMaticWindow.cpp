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
#include "PasteOMaticWindow.h"

#include <Bitmap.h>
#include <Button.h>
#include <Entry.h>
#include <FilePanel.h>
#include <GraphicsDefs.h>
#include <GroupLayout.h>
#include <InterfaceDefs.h>
#include <LayoutItem.h>
#include <TextControl.h>
#include <TranslationUtils.h>
#include <View.h>
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
    BMessenger windowMessenger(NULL, this);
    BMessage openPanelMessage(MESSAGE_FROM_OPEN_PANEL);
    
    fIcon = new PasteOMaticIcon(LARGE_ICON);
   
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
    item = layout->AddView(static_cast<BView *> (fIcon));
    item->SetExplicitMaxSize(BSize(48, 48));
    item->SetExplicitMinSize(BSize(48, 48));
    layout->AddView(static_cast<BView *> (fTextControl));
    layout->AddView(static_cast<BView *> (fOpenButton));
    layout->AddView(static_cast<BView *> (fSettingsButton));
    
    SetSizeLimits(450, 10000000, 40, 10000000);
    
    fOpenPanel = new BFilePanel(B_OPEN_PANEL, &windowMessenger, NULL,
                                0, false, &openPanelMessage, 
                                /*fFilter*/ NULL, true, true);
}

PasteOMaticWindow::~PasteOMaticWindow()
{
}

void PasteOMaticWindow::MessageReceived(BMessage *message)
{
    entry_ref dndEntryRef;
    int8 status;
    const char *link;
    string linkText;
    
    cout << "Window caught message:\n";
    message->PrintToStream();
    switch (message->what)
    {
        /* from GUI */
        
        case MESSAGE_OPEN:
            fOpenPanel->Show();
            break;
        case MESSAGE_SETTINGS:
            break;
        case MESSAGE_FROM_TRACKER_DND:
        case MESSAGE_FROM_OPEN_PANEL:
        	my_app->PostMessage(message);
        
        /* from app */
        
        case MESSAGE_PASTE_STATUS:
            message->FindInt8("status", &status);
            
            switch (status)
            {
                case PASTE_STATUS_WORKING:
                    fTextControl->SetText("Working...");
                    fIcon->SetDefault();
                    break;
                case PASTE_STATUS_TYPE_ERROR:
                    fTextControl->SetText("Error: can't determine file type");
                    fIcon->SetFail();
                    break;
                case PASTE_STATUS_READ_ERROR:
                    fTextControl->SetText("Error: can't read file");
                    fIcon->SetFail();
                    break;
                case PASTE_STATUS_TOO_LARGE_ERROR:
                    fTextControl->SetText("Error: file is too large");
                    fIcon->SetFail();
                    break;
                case PASTE_STATUS_CURL_ERROR:
                    fTextControl->SetText("Error: upload failed");
                    fIcon->SetFail();
                    break;
                case PASTE_STATUS_SUCCESS:
                    cout << "Success message received!\n";
                    message->FindString("link", &link);
                    cout << "Link: " << link;
                    linkText = link;
                    linkText += " copied";
                    fTextControl->SetText(linkText.c_str());
                    fIcon->SetSuccess();
                    break;
                default:
                    cout << "Unknown status message: " << status << "\n";
            }
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

