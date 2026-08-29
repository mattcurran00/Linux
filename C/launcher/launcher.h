#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <gtk/gtk.h>
#include <gio/gio.h>
#include "../globals.h"

typedef struct {
    GtkWidget *window;
    GtkWidget *entry;
    GtkWidget *list_box;
    GList *applications;
} Launcher;

void launcher_activate(GtkApplication *app, gpointer userData);

#endif