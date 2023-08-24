// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/core.hpp"

// clang-format off

#if defined(GEN_PLATFORM_WINDOWS)

#if defined(_WINDOWS_) && !defined(GEN_MINIMAL_WINDOWS_INCLUDE)
    GEN_MESSAGE ( " " )
    GEN_MESSAGE ( "You have included windows.h before MinWindows.h" )
    GEN_MESSAGE ( "All useless stuff from the windows headers won't be excluded !!!" )
    GEN_MESSAGE ( " " )
#endif // _WINDOWS_

#define GEN_MINIMAL_WINDOWS_INCLUDE

// WIN32_LEAN_AND_MEAN excludes rarely-used services from windows headers.
#define WIN32_LEAN_AND_MEAN

// The below excludes some other unused services from the windows headers -- see windows.h for details.
#define NOGDICAPMASKS            // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#define NOVIRTUALKEYCODES        // VK_*
#define NOWINMESSAGES            // WM_*, EM_*, LB_*, CB_*
#define NOWINSTYLES              // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#define NOSYSMETRICS             // SM_*
#define NOMENUS                  // MF_*
#define NOICONS                  // IDI_*
#define NOKEYSTATES              // MK_*
#define NOSYSCOMMANDS            // SC_*
#define NORASTEROPS              // Binary and Tertiary raster ops
#define NOSHOWWINDOW             // SW_*
#define OEMRESOURCE              // OEM Resource values
#define NOATOM                   // Atom Manager routines
#define NOCLIPBOARD              // Clipboard routines
#define NOCOLOR                  // Screen colors
#define NOCTLMGR                 // Control and Dialog routines
#define NODRAWTEXT               // DrawText() and DT_*
#define NOGDI                    // All GDI #defines and routines
#define NOKERNEL                 // All KERNEL #defines and routines
#define NOUSER                   // All USER #defines and routines
//#define NONLS                  // All NLS #defines and routines // required by crt.h
#define NOMB                     // MB_* and MessageBox()
#define NOMEMMGR                 // GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE               // typedef METAFILEPICT
#define NOMINMAX                 // Macros min(a,b) and max(a,b)
#define NOMSG                    // typedef MSG and associated routines
#define NOOPENFILE               // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL                 // SB_* and scrolling routines
#define NOSERVICE                // All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND                  // Sound driver routines
#define NOTEXTMETRIC             // typedef TEXTMETRIC and associated routines
#define NOWH                     // SetWindowsHook and WH_*
#define NOWINOFFSETS             // GWL_*, GCL_*, associated routines
#define NOCOMM                   // COMM driver routines
#define NOKANJI                  // Kanji support stuff.
#define NOHELP                   // Help engine interface.
#define NOPROFILER               // Profiler interface.
#define NODEFERWINDOWPOS         // DeferWindowPos routines
#define NOMCX                    // Modem Configuration Extensions
#define NOCRYPT
#define NOTAPE
#define NOIMAGE
#define NOPROXYSTUB
#define NORPC

// https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nn-unknwn-iunknown
struct IUnknown;

// Finally now we can include windows.h
#include <Windows.h>


// HACK: Define macros that some windows.h variants don't
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif
#ifndef WM_DWMCOMPOSITIONCHANGED
#define WM_DWMCOMPOSITIONCHANGED 0x031E
#endif
#ifndef WM_DWMCOLORIZATIONCOLORCHANGED
#define WM_DWMCOLORIZATIONCOLORCHANGED 0x0320
#endif
#ifndef WM_COPYGLOBALDATA
#define WM_COPYGLOBALDATA 0x0049
#endif
#ifndef WM_UNICHAR
#define WM_UNICHAR 0x0109
#endif
#ifndef UNICODE_NOCHAR
#define UNICODE_NOCHAR 0xFFFF
#endif
#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0
#endif
#ifndef GET_XBUTTON_WPARAM
#define GET_XBUTTON_WPARAM(w) (HIWORD(w))
#endif
#ifndef EDS_ROTATEDMODE
#define EDS_ROTATEDMODE 0x00000004
#endif
#ifndef DISPLAY_DEVICE_ACTIVE
#define DISPLAY_DEVICE_ACTIVE 0x00000001
#endif
#ifndef _WIN32_WINNT_WINBLUE
#define _WIN32_WINNT_WINBLUE 0x0603
#endif
#ifndef _WIN32_WINNT_WIN8
#define _WIN32_WINNT_WIN8 0x0602
#endif
#ifndef WM_GETDPISCALEDSIZE
#define WM_GETDPISCALEDSIZE 0x02e4
#endif
#ifndef USER_DEFAULT_SCREEN_DPI
#define USER_DEFAULT_SCREEN_DPI 96
#endif
#ifndef OCR_HAND
#define OCR_HAND 32649
#endif


#endif //GEN_PLATFORM_WINDOWS
