NAME= Paste-O-Matic
TYPE= APP
SRCS= PasteOMatic.cpp PasteOMaticIcon.cpp PasteOMaticWindow.cpp
RSRCS= PasteOMatic.rsrc
LIBS= /boot/develop/lib/x86/libroot.so /boot/develop/lib/x86/libbe.so /boot/common/lib/libcurl.so /boot/develop/lib/x86/libstdc++.r4.so /boot/develop/lib/x86/libtranslation.so /boot/develop/lib/x86/libtracker.so
LIBPATHS=
SYSTEM_INCLUDE_PATHS= /boot/develop/headers/be /boot/develop/headers/cpp /boot/develop/headers/posix /boot/home/config/include
LOCAL_INCLUDE_PATHS= /boot/home/projects/paste-o-matic/.
OPTIMIZE=NONE
#	specify any preprocessor symbols to be defined.  The symbols will not
#	have their values set automatically; you must supply the value (if any)
#	to use.  For example, setting DEFINES to "DEBUG=1" will cause the
#	compiler option "-DDEBUG=1" to be used.  Setting DEFINES to "DEBUG"
#	would pass "-DDEBUG" on the compiler's command line.
DEFINES=
#	specify special warning levels
#	if unspecified default warnings will be used
#	NONE = supress all warnings
#	ALL = enable all warnings
WARNINGS =
# Build with debugging symbols if set to TRUE
SYMBOLS=
COMPILER_FLAGS=-I/boot/common/include/curl
LINKER_FLAGS=

## include the makefile-engine
include $(BUILDHOME)/etc/makefile-engine
