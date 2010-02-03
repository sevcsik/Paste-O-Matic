#ifndef _DEFINES_H_
#define _DEFINES_H_

/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *      Andras Sevcsik <sevcsik@gmail.com>
 */

#define VERSION "0.1"
#define WINDOW_TITLE "Paste-O-Matic"
#define SIGNATURE "application/x-vnd.paste-o-matic"

#if 0
#ifndef PATH_DATA_PREFIX
#   define PATH_DATA_PREFIX "data/"
#endif

#define PATH_DEFAULT_ICON PATH_DATA_PREFIX "icon.png"
#define PATH_SUCCESS_ICON PATH_DATA_PREFIX "icon_success.png"
#define PATH_FAIL_ICON PATH_DATA_PREFIX "icon_fail.png"

#define PATH_DEFAULT_SMALL_ICON PATH_DATA_PREFIX "icon_small.png"
#define PATH_SUCCESS_SMALL_ICON PATH_DATA_PREFIX "icon_small_success.png"
#define PATH_FAIL_SMALL_ICON PATH_DATA_PREFIX "icon_small_fail.png"
#endif

/* Messages */
#define MESSAGE_OPEN 'pmo '
#define MESSAGE_SETTINGS 'pms '
#define MESSAGE_FROM_TRACKER_DND 'DATA'
#define MESSAGE_FROM_OPEN_PANEL 'pmop'
#define MESSAGE_PASTE_STATUS 'pmps'

#define PASTEBIN_MAX_SIZE 150000
#define PASTEBIN_API_KEY "g1D7PIG86Idci9blKc/itMQdhTc6X/dN"
#define PASTEBIN_URL "http://pastebin.ca/quiet-paste.php?api=" PASTEBIN_API_KEY
#define PASTEBIN_ROOT "http://pastebin.ca/"

#define PASTE_DESCRIPTION "Posted with Paste-O-Matic v" VERSION " from HaikuOS"

/* Status codes */

#define PASTE_STATUS_WORKING            0
#define PASTE_STATUS_TYPE_ERROR         1
#define PASTE_STATUS_TOO_LARGE_ERROR    2
#define PASTE_STATUS_READ_ERROR         3
#define PASTE_STATUS_SUCCESS            4
#define PASTE_STATUS_CURL_ERROR         5

#define my_app static_cast<PasteOMatic *> (be_app)

#endif  // _DEFINES_H_
