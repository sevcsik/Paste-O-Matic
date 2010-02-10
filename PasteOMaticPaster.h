#ifndef PASTEOMATICPASTER_H
#define PASTEOMATICPASTER_H

/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *      Andras Sevcsik <sevcsik@gmail.com>
 */

#include <Entry.h>
#include <Looper.h>
#include <Message.h>
#include <Messenger.h>

/*
 * Website handlers should subclass this class.
 * First, the application calls Check() with a mime string and its size.
 * If it returns true, the application starts the message loop with Run(),
 * then sends a PASTE_DATA message, which will call your according _Paste()
 * function.
 */
 

class PasteOMaticPaster : public BLooper {
	public:
		PasteOMaticPaster(BMessenger *messenger);
		virtual ~PasteOMaticPaster();
		
		void MessageReceived(BMessage *message);
		
		/* Return true, if you can handle mime type in "type" with "size" size */
		virtual bool Check(char *type, size_t size);
		
	private:
		/* Upload paste, return NULL on success, or an error message.
		 * It will be free()-d after return. */
		virtual char *_Paste(void *data, size_t size);
		virtual char *_Paste(entry_ref *ref);
		
		BMessenger *fMessenger;
};

#endif
