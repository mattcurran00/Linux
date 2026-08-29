#include "updateResults.h"
#include "../search.h"
#include "../globals.h"
#include "glib.h"
#include "gtk/gtk.h"

static void clear_results(Launcher *launcher)
{
    GList *children = gtk_container_get_children(GTK_CONTAINER(launcher->list_box));

    for (GList *item = children; item != NULL; item = item->next) {
        gtk_widget_destroy(GTK_WIDGET(item->data));
    }

    g_list_free(children);
}

void update_results(GtkEntry *entry, gpointer user_data)
{
    Launcher *launcher = user_data;

    const char *query = gtk_entry_get_text(entry);
    clear_results(launcher);

    GList *results = search_applications(launcher->applications, query);
    if (query == NULL || *query == '\0') {
        g_list_free_full(results, (GDestroyNotify)freeSearchResult);
        return;
    }

    for (GList *item = results; item != NULL; item = item->next) {
        SearchResult *result = item->data;
        GAppInfo *app = result->info;

        const char *name = g_app_info_get_display_name(app);
        GtkWidget *label = gtk_label_new(name);

        gtk_widget_set_halign(label, GTK_ALIGN_START);
        gtk_widget_set_margin_start(label, 12);
        gtk_widget_set_margin_end(label, 12);
        gtk_widget_set_margin_top(label, 8);
        gtk_widget_set_margin_bottom(label, 8);

        GtkWidget *row = gtk_list_box_row_new();
        gtk_container_add(GTK_CONTAINER(row), label);
        g_object_set_data_full(G_OBJECT(row), "application", g_object_ref(app), g_object_unref);
        gtk_container_add(GTK_CONTAINER(launcher->list_box), row);
    }

    g_list_free_full(results, (GDestroyNotify)freeSearchResult);
    gtk_widget_show_all(launcher->list_box);
}

void freeSearchResult(SearchResult *result)
{
    g_object_unref(result->info);
    g_free(result);
}
