#ifndef UPDATE_RESULTS_H
#define UPDATE_RESULTS_H

#include "../launcher/launcher.h"

void update_results(GtkEntry *entry, gpointer user_data);
void freeSearchResult(SearchResult *result);

#endif
