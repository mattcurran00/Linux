#include "search.h"
#include "globals.h"
#include "gio/gio.h"
#include "glib-object.h"
#include "glib.h"
#include <string.h>

int matchScore(const char *name, const char *query);
int compareResults(gconstpointer a, gconstpointer b);

GList *search_applications(GList *applications, const char *query)
{
    GList *results = NULL;

    if (query == NULL || *query == '\0') {
        return results;
    }

    char *lower_query = g_utf8_strdown(query, -1);

    for (GList *item = applications; item != NULL; item = item->next) {

        GAppInfo *app = G_APP_INFO(item->data);

        const char *name = g_app_info_get_display_name(app);

        if (name == NULL) {
            continue;
        }

        char *lower_name = g_utf8_strdown(name, -1);
        int matchConfidence = matchScore(lower_name, lower_query);

        SearchResult *result = g_new(SearchResult, 1);
        result->info = g_object_ref(app);
        result->score = matchConfidence;

        results = g_list_append(results, result);


        g_free(lower_name);
    }   

    g_free(lower_query);

    results = g_list_sort(results, compareResults);

    while(g_list_length(results) > 10){
        GList *last = g_list_last(results);

        SearchResult *result = last->data;
        g_object_unref(result->info);
        g_free(result);

        results = g_list_delete_link(results,last);
    }

    return results;
}

int matchScore(const char *name, const char *query)
{
    if (strcmp(name, query) == 0)
        return 100;

    if (g_str_has_prefix(name, query))
        return 80;

    const char *p = name;

    while ((p = strstr(p, query)) != NULL) {
        if (p == name || !g_ascii_isalnum(*(p - 1)))
            return 60;

        p++;
    }

    if (strstr(name, query) != NULL)
        return 20;

    return 0;
}

int compareResults(gconstpointer a, gconstpointer b){
    const SearchResult *resultA = a;
    const SearchResult *resultB = b;

    return resultB->score - resultA->score;
}
