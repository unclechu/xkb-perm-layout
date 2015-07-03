/**
 * Author: Viacheslav Lotsmanov
 * License: GNU/GPLv3 https://raw.githubusercontent.com/unclechu/xkb-perm-layout/master/LICENSE
 */

#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

int main(int argc, char **argv) {
	
	Display *display;
	int reason_return;
	
	display = XkbOpenDisplay(NULL, NULL, NULL, NULL, NULL, &reason_return);
	
	switch (reason_return) {
		case XkbOD_BadLibraryVersion:
			fprintf(stderr, "Bad XKB library version.\n");
			return 1;
			break;
		case XkbOD_ConnectionRefused:
			fprintf(stderr, "Connection to X server refused.\n");
			return 1;
			break;
		case XkbOD_BadServerVersion:
			fprintf(stderr, "Bad X11 server version.\n");
			return 1;
			break;
		case XkbOD_NonXkbServer:
			fprintf(stderr, "XKB not present.\n");
			return 1;
			break;
		case XkbOD_Success:
			// move forward
			break;
		default:
			fprintf(stderr, "XKB unknown reason return.\n");
			return 1;
	}
	
	XkbDescRec* kbd_desc_ptr = XkbAllocKeyboard();
	if (kbd_desc_ptr == NULL) {
		XCloseDisplay(display);
		fprintf(stderr, "Failed to get keyboard description.\n");
		return 1;
	}
	
	XkbGetControls(display, XkbAllControlsMask, kbd_desc_ptr);
	
	// there is always at least one group
	if (kbd_desc_ptr->ctrls->num_groups <= 0) {
		XCloseDisplay(display);
		fprintf(stderr, "XKB group count is 0.\n");
		return 1;
	}
	
	XkbStateRec xkb_state;
	XEvent event;
	Window wnd;
	
	int revert_to;
	int last_wnd_id = 0;
	unsigned long event_mask = FocusChangeMask;
	
	while (1) {
		
		XGetInputFocus(display, &wnd, &revert_to);
		XSelectInput(display, wnd, event_mask);
		XNextEvent(display, &event);
		
		if ((int)wnd != last_wnd_id) {
			
			XkbLockGroup(display, XkbUseCoreKbd, 0); // change to first layout
			XkbGetState(display, XkbUseCoreKbd, &xkb_state); // need for change layout
			
			last_wnd_id = (int)wnd;
		}
	}
	
	return 0;
}
