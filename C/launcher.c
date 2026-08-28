#include "launcher.h"
#include "glib.h"
#include "gtk/gtk.h"
#include <string.h>

static void clear_results(Launcher *launcher){
    GList *children = gtk_container_get_children(GTK_CONTAINER(launcher->list_box));

    for(GList *item = children; item != NULL; item = item->next){
        gtk_widget_destroy(GTK_WIDGET(item->data));
    }
    g_list_free(children);
}

static void launch_application(GtkListBox *box, GtkListBoxRow *row, gpointer user_data){
    Launcher *launcher = user_data;
    GAppInfo *app = g_object_get_data(G_OBJECT(row), "application");

    if(app == NULL){
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

static void update_results(GtkEntry *entry, gpointer user_data){
    Launcher *launcher = user_data;

    const char *query = gtk_entry_get_text(entry);
    clear_results(launcher);
    
    if (query == NULL || *query == '\0') {
        return;
    }

    int result_count = 0;

    for (GList *item = launcher->applications;
         item != NULL && result_count < 10;
         item = item->next) {

        GAppInfo *app = G_APP_INFO(item->data);

        if (!g_app_info_should_show(app)) {
            continue;
        }

        const char *name = g_app_info_get_display_name(app);

        if (name == NULL) {
            continue;
        }

        char *lower_name = g_utf8_strdown(name, -1);
        char *lower_query = g_utf8_strdown(query, -1);

        gboolean matches =
            strstr(lower_name, lower_query) != NULL;

        g_free(lower_name);
        g_free(lower_query);

        if (!matches) {
            continue;
        }

        GtkWidget *label = gtk_label_new(name);

        gtk_widget_set_halign(
            label,
            GTK_ALIGN_START
        );

        gtk_widget_set_margin_start(label, 12);
        gtk_widget_set_margin_end(label, 12);
        gtk_widget_set_margin_top(label, 8);
        gtk_widget_set_margin_bottom(label, 8);

        GtkWidget *row = gtk_list_box_row_new();

        gtk_container_add(
            GTK_CONTAINER(row),
            label
        );

        g_object_set_data_full(
            G_OBJECT(row),
            "application",
            g_object_ref(app),
            g_object_unref
        );

        gtk_container_add(
            GTK_CONTAINER(launcher->list_box),
            row
        );

        result_count++;
    }

    gtk_widget_show_all(launcher->list_box);
}

static gboolean handle_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    (void)widget;

    Launcher *launcher = user_data;

    if (event->keyval == GDK_KEY_Escape) {
        gtk_widget_hide(launcher->window);
        return TRUE;
    }

    if (event->keyval == GDK_KEY_Return) {

        GtkListBoxRow *row =
            gtk_list_box_get_selected_row(
                GTK_LIST_BOX(launcher->list_box)
            );

        if (row == NULL) {
            row = gtk_list_box_get_row_at_index(
                GTK_LIST_BOX(launcher->list_box),
                0
            );
        }

        if (row != NULL) {
            launch_application(
                GTK_LIST_BOX(launcher->list_box),
                row,
                launcher
            );
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

    launcher->window =
        gtk_application_window_new(app);

    gtk_window_set_title(
        GTK_WINDOW(launcher->window),
        "Search"
    );

    gtk_window_set_default_size(
        GTK_WINDOW(launcher->window),
        600,
        350
    );

    gtk_window_set_position(
        GTK_WINDOW(launcher->window),
        GTK_WIN_POS_CENTER
    );

    gtk_window_set_decorated(
        GTK_WINDOW(launcher->window),
        FALSE
    );

    gtk_window_set_keep_above(
        GTK_WINDOW(launcher->window),
        TRUE
    );

    GtkWidget *container =
        gtk_box_new(
            GTK_ORIENTATION_VERTICAL,
            8
        );

    gtk_container_set_border_width(
        GTK_CONTAINER(container),
        12
    );

    launcher->entry =
        gtk_search_entry_new();

    launcher->list_box =
        gtk_list_box_new();

    gtk_list_box_set_selection_mode(
        GTK_LIST_BOX(launcher->list_box),
        GTK_SELECTION_SINGLE
    );

    gtk_box_pack_start(
        GTK_BOX(container),
        launcher->entry,
        FALSE,
        FALSE,
        0
    );

    gtk_box_pack_start(
        GTK_BOX(container),
        launcher->list_box,
        TRUE,
        TRUE,
        0
    );

    gtk_container_add(
        GTK_CONTAINER(launcher->window),
        container
    );

    g_signal_connect(
        launcher->entry,
        "changed",
        G_CALLBACK(update_results),
        launcher
    );

    g_signal_connect(
        launcher->list_box,
        "row-activated",
        G_CALLBACK(launch_application),
        launcher
    );

    g_signal_connect(
        launcher->window,
        "key-press-event",
        G_CALLBACK(handle_key_press),
        launcher
    );

    gtk_widget_show_all(launcher->window);

    gtk_widget_grab_focus(launcher->entry);
}