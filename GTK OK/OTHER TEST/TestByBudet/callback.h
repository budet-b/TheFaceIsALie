#ifndef H_CALLBACK
#define H_CALLBACK

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

struct GError {
    GQuark       domain;
    gint         code;
    gchar       *message;
};

gchar* cb_open (GtkWidget *, gpointer);
void cb_save (GtkWidget *, gpointer);
void cb_saveas (GtkWidget *, gpointer);
void cb_quit (GtkWidget *, gpointer);
void cb_modifie (GtkWidget *, gpointer);

#endif /* not H_CALLBACK */
