/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *      Andras Sevcsik <sevcsik@gmail.com>
 */

#include "PasteOMaticPaster.h"

#include <Entry.h>
#include <Messenger.h>

#define PASTEBIN_MAX_SIZE 150000
#define PASTEBIN_API_KEY "g1D7PIG86Idci9blKc/itMQdhTc6X/dN"
#define PASTEBIN_URL "http://pastebin.ca/quiet-paste.php?api=" PASTEBIN_API_KEY
#define PASTEBIN_ROOT "http://pastebin.ca/"

class PasterPastebinCa : public PasteOMaticPaster {
	public:
		PasterPastebinCa(BMessenger *messenger);
		virtual ~PasterPastebinCa();
		virtual bool Check(char *type, size_t size);
	private:
		virtual char *_Paste(void *data, size_t size);
		virtual char *_Paste(entry_ref *ref);
};
