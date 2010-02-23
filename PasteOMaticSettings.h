#ifndef PASTEOMATICSETTINGS_H
#define PASTEOMATICSETTINGS_H

/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *      Andras Sevcsik <sevcsik@gmail.com>
 */

#include <ListView.h>
#include <Slider.h>
#include <Window.h>

#define MESSAGE_SETTINGS_AUTOCLIP_CHECK 'sacc'
#define MESSAGE_SETTINGS_ICONSIZE_CHANGED 'sic '
#define MESSAGE_SETTINGS_MOVE_UP 'smup'
#define MESSAGE_SETTINGS_MOVE_DOWN 'smdn'
#define MESSAGE_SETTINGS_DESKBAR_CHECK 'sdc '

class PasteOMaticSettingsGeneral : public BView {
	public:
		PasteOMaticSettingsGeneral(BRect rect);
		virtual ~PasteOMaticSettingsGeneral();
		virtual void MessageReceived(BMessage *message);
		
	private:
		BSlider *fIconSizeSlider;
};


class PasteOMaticSettingsPasters : public BView {
	public:
		PasteOMaticSettingsPasters(BRect rect);
		virtual ~PasteOMaticSettingsPasters();
		
	private:
		BListView *fListView;
};


class PasteOMaticSettingsWindow : public BWindow {
	public:
		PasteOMaticSettingsWindow(BMessenger *messenger);
		virtual ~PasteOMaticSettingsWindow();
		virtual void PasteOMaticSettingsWindow::MessageReceived(BMessage *message);
		
	private:
		PasteOMaticSettingsGeneral *fGeneralView;
		PasteOMaticSettingsPasters *fPastersView;
		BMessenger *fViewMessenger;
};

#endif
