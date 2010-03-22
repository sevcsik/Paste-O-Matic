/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *      Andras Sevcsik <sevcsik@gmail.com>
 */

#include "PasteOMaticSettings.h"

#include <CheckBox.h>
#include <Button.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <ListView.h>
#include <Slider.h>
#include <String.h>
#include <TabView.h>

#include <iostream>
using namespace std;

PasteOMaticSettingsWindow::PasteOMaticSettingsWindow(BMessenger *messenger) 
	: BWindow(BRect(10, 30, 330, 280), "Settings", B_TITLED_WINDOW, 
			  B_NOT_ZOOMABLE | B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE)
{	
	fViewMessenger = messenger;

	BTabView *tabView = new BTabView(Bounds(), "tabView");
	tabView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	BRect rect = tabView->Bounds();
	rect.InsetBy(5, 5);
	rect.bottom -= tabView->TabHeight();
	
	tabView->AddTab(fGeneralView = new PasteOMaticSettingsGeneral(rect));
	tabView->AddTab(fPastersView = new PasteOMaticSettingsPasters(rect));
	
	tabView->TabAt(0)->SetLabel("General");
	tabView->TabAt(1)->SetLabel("Pasters");
	
	AddChild(tabView);
}


PasteOMaticSettingsWindow::~PasteOMaticSettingsWindow()
{
	
}


void PasteOMaticSettingsWindow::MessageReceived(BMessage *message)
{
	cout << "Settings window received message:\n";
	message->PrintToStream();
	fViewMessenger->SendMessage(message);
	
	switch (message->what)
	{
		case MESSAGE_SETTINGS_ICONSIZE_CHANGED:
			fGeneralView->MessageReceived(message);
			break;
		default:
			BWindow::MessageReceived(message);
	}
}


PasteOMaticSettingsGeneral::PasteOMaticSettingsGeneral(BRect rect) 
	: BView(rect, "tabGeneral", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW)
{
	BCheckBox *deskbarCheckBox;
	BCheckBox *autoClipCheckBox;
	
	deskbarCheckBox = new BCheckBox(BRect(0, 0, 100, 100), 
 					  "deskbarCheckBox", 
 					  "Live in Deskbar", 
					  new BMessage(MESSAGE_SETTINGS_DESKBAR_CHECK));	
					
	autoClipCheckBox = new BCheckBox(BRect(0, 0, 100, 100), 
					   "autoClipCheckBox", 
					   "Copy link automatically to clipboard", 
		               new BMessage(MESSAGE_SETTINGS_AUTOCLIP_CHECK));
	
	fIconSizeSlider = new BSlider(BRect(0, 0, 100, 100),
					  "fIconSizeSlider",
					  "Icon size: ",
					  new BMessage(MESSAGE_SETTINGS_ICONSIZE_CHANGED),
					  16, 128);
	fIconSizeSlider->SetModificationMessage(new BMessage(MESSAGE_SETTINGS_ICONSIZE_CHANGED));
	fIconSizeSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
	fIconSizeSlider->SetHashMarkCount(8);
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	BGroupLayout *layout = new BGroupLayout(B_VERTICAL, 0);				              
	SetLayout(layout);
	
	layout->AddView(deskbarCheckBox);
	layout->AddView(autoClipCheckBox);
	layout->AddView(fIconSizeSlider);
}


PasteOMaticSettingsGeneral::~PasteOMaticSettingsGeneral()
{
	
}


void PasteOMaticSettingsGeneral::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case MESSAGE_SETTINGS_ICONSIZE_CHANGED:
		{
			int32 value;
			if (message->FindInt32("be:value", &value) == B_OK)
			{
				BString label("Icon size (");
				label << value << "px):";
				fIconSizeSlider->SetLabel(label);
			}
		} break;
		default:
			BView::MessageReceived(message);
	}	
}


 PasteOMaticSettingsPasters::PasteOMaticSettingsPasters(BRect rect) 
	: BView(rect, "tabPasters", B_FOLLOW_LEFT | B_FOLLOW_TOP, 0)
{
	BGroupLayout *rootLayout = new BGroupLayout(B_VERTICAL, 5);
	fListView = new BListView(BRect(0, 0, 0, 0), "fListView");	
	BButton *buttonUp = new BButton(BRect(0, 0, 0, 0), "buttonUp", "Move up",
	                                new BMessage(MESSAGE_SETTINGS_MOVE_UP));
	BButton *buttonDown = new BButton(BRect(0, 0, 0, 0), "buttonDown", "Move down", 
					           	      new BMessage(MESSAGE_SETTINGS_MOVE_DOWN));
	SetLayout(rootLayout);
	AddChild(BGroupLayoutBuilder(B_VERTICAL, 0)
		.Add(fListView)
		.AddGroup(B_HORIZONTAL)
			.Add(buttonUp)
			.Add(buttonDown)
	);
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}


PasteOMaticSettingsPasters::~PasteOMaticSettingsPasters()
{
	
}


