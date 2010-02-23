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

#define MESSAGE_PASTE_POINTER 'pptr'
#define MESSAGE_PASTE_REF 'pref'
#define MESSAGE_PASTE_SUCCESS 'psuc'
#define MESSAGE_PASTE_FAIL 'perr'
#define MESSAGE_PASTE_PROGRESS 'pprg'

#define PASTE_DESCRIPTION "Posted with Paste-O-Matic v" VERSION " from HaikuOS"

#define CONFIG_PATH "/boot/home/config/settings/Paste-O-Matic_settings"

/* Status codes */

#define my_app static_cast<PasteOMatic *> (be_app)

#endif  // _DEFINES_H_
