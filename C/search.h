#ifndef SEARCH_H
#define SEARCH_H

#include <gio/gio.h>

GList *search_applications(GList *applications, const char *query);

#endif