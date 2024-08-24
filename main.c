// main.cpp
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/xpm.h> // Ensure this header is included
#include <stdio.h>
#include <stdlib.h>

int main() {
    Display *display;
    Window root, desktop;
    XEvent event;
    Pixmap background;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open X display\n");
        return EXIT_FAILURE;
    }

    root = DefaultRootWindow(display);

    // Load the background image (XPM format)
    if (XpmReadFileToPixmap(display, root, "background.xpm", &background, NULL, NULL) != XpmSuccess) {
        // Fallback to navy blue if the image fails to load
        background = XCreatePixmap(display, root, 800, 600, DefaultDepth(display, 0));
        XSetWindowBackground(display, desktop, 0x000080); // Navy blue color
    }

    // Create a window for the desktop
    desktop = XCreateSimpleWindow(display, root, 0, 0, 800, 600, 0, 0, 0);
    XSetWindowBackgroundPixmap(display, desktop, background);
    XMapWindow(display, desktop);

    XSelectInput(display, root, SubstructureNotifyMask);

    while (1) {
        XNextEvent(display, &event);
        if (event.type == CreateNotify) {
            // Handle new window creation
            printf("New window created: %lu\n", event.xcreatewindow.window);
        }
    }

    XFreePixmap(display, background);
    XCloseDisplay(display);
    return EXIT_SUCCESS;
}