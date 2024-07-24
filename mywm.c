#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/xpm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libconfig.h>

#define CONFIG_PATH "$HOME/.config/mywm/mywm.conf"

// Function to set the default black background
void set_default_background(Display *display, Window root) {
    XSetWindowBackground(display, root, BlackPixel(display, DefaultScreen(display)));
    XClearWindow(display, root);
    XFlush(display);
}

// Function to set background image
void set_background_image(Display *display, Window root, const char *image_path) {
    Pixmap pixmap;
    XpmReadFileToPixmap(display, root, image_path, &pixmap, NULL, NULL);
    XSetWindowBackgroundPixmap(display, root, pixmap);
    XClearWindow(display, root);
    XFlush(display);
}

// Function to apply configuration
void apply_configuration(config_t *cfg, Display *display, Window root) {
    const char *background_image;
    const char *background_color = "#000000"; // Default color

    // Check if a background image is specified
    if (config_lookup_string(cfg, "Appearance.background_image", &background_image)) {
        set_background_image(display, root, background_image);
    } else {
        // Set default background color
        XParseColor(display, DefaultColormap(display, DefaultScreen(display)), background_color, &(XColor){0});
        XSetWindowBackground(display, root, BlackPixel(display, DefaultScreen(display)));
        XClearWindow(display, root);
        XFlush(display);
    }
}

// Function to reload the configuration
void reload_configuration(Display *display, Window root) {
    config_t cfg;
    config_init(&cfg);

    if (config_read_file(&cfg, CONFIG_PATH)) {
        apply_configuration(&cfg, display, root);
    } else {
        fprintf(stderr, "Error reading configuration file: %s\n", config_error_text(&cfg));
        set_default_background(display, root);
    }

    config_destroy(&cfg);
}

// Main function
int main() {
    Display *display;
    Window root;
    XEvent event;
    config_t cfg;

    // Open connection to X server
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open X display\n");
        exit(EXIT_FAILURE);
    }

    root = DefaultRootWindow(display);

    // Load and apply configuration
    config_init(&cfg);
    reload_configuration(display, root);

    // Main event loop
    while (1) {
        XNextEvent(display, &event);
        switch (event.type) {
            case ConfigureNotify:
                // Handle window resize/move
                printf("Window moved or resized\n");
                break;
            case MapRequest:
                // Handle new window requests
                printf("New window requested\n");
                break;
            // Add more event handling as needed
        }
    }

    XCloseDisplay(display);
    return 0;
}
