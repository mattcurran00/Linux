#include "launcheractivate.h"
#include "../utils/updateResults.h"
#include "glib.h"
#include "gtk/gtk.h"

static void launch_application(GtkListBox *box, GtkListBoxRow *row, gpointer user_data)
{
    (void)box;
    Launcher *launcher = user_data;
    GAppInfo *app = g_object_get_data(G_OBJECT(row), "application");

    if (app == NULL) {
        return;
    }

    GError *error = NULL;
    if (!g_app_info_launch(app, NULL, NULL, &error)) {
        g_printerr("Could not launch application: %s\n", error->message);
        g_error_free(error);
        return;
    }

    gtk_widget_destroy(launcher->window);
}

static gboolean handle_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    (void)widget;

    Launcher *launcher = user_data;

    if (event->keyval == GDK_KEY_Escape) {
        gtk_widget_hide(launcher->window);
        return TRUE;
    }

    if (event->keyval == GDK_KEY_Return) {
        GtkListBoxRow *row = gtk_list_box_get_selected_row(GTK_LIST_BOX(launcher->list_box));
        if (row == NULL) {
            row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(launcher->list_box), 0);
        }
        if (row != NULL) {
            launch_application(GTK_LIST_BOX(launcher->list_box), row, launcher);
        }
        return TRUE;
    }

    return FALSE;
}

void launcher_activate(GtkApplication *app, gpointer user_data)
{
    (void)user_data;
    Launcher *launcher = g_new0(Launcher, 1);
    launcher->applications = g_app_info_get_all();
    launcher->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(launcher->window), "Search");
    gtk_window_set_default_size(GTK_WINDOW(launcher->window), 600, 350);
    gtk_window_set_position(GTK_WINDOW(launcher->window), GTK_WIN_POS_CENTER);
    gtk_window_set_decorated(GTK_WINDOW(launcher->window), FALSE);
    gtk_window_set_keep_above(GTK_WINDOW(launcher->window), TRUE);

    GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(container), 12);

    launcher->entry = gtk_search_entry_new();
    launcher->list_box = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(launcher->list_box), GTK_SELECTION_SINGLE);

    gtk_box_pack_start(GTK_BOX(container), launcher->entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(container), launcher->list_box, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(launcher->window), container);

    g_signal_connect(launcher->entry, "changed", G_CALLBACK(update_results), launcher);
    g_signal_connect(launcher->list_box, "row-activated", G_CALLBACK(launch_application), launcher);
    g_signal_connect(launcher->window, "key-press-event", G_CALLBACK(handle_key_press), launcher);

    gtk_widget_show_all(launcher->window);
    gtk_widget_grab_focus(launcher->entry);
}
