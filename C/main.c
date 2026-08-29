#include <gtk/gtk.h>
#include <gio/gio.h>
#include "launcher/launcher.h"

/**
 * Program to simulate the spotlight search feature on mac
 * but using C instead cos why not 
 * 
 * 31/07/2026
 */

int main(int argc, char *argv[])
{
    GtkApplication *app = gtk_application_new(
        "com.example.spotlight",
        G_APPLICATION_DEFAULT_FLAGS
    );

    g_signal_connect(app, "activate", G_CALLBACK(launcher_activate), NULL);

    int status = g_application_run(
        G_APPLICATION(app),
        argc,
        argv
    );

    g_object_unref(app);

    return status;
}