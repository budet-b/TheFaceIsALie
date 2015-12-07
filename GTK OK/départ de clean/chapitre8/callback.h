#ifndef H_CALLBACK
#define H_CALLBACK

#include <gtk/gtk.h>

GtkWidget* cb_open ();
void cb_save (GtkWidget *, gpointer);
void cb_saveas (GtkWidget *, gpointer);
void cb_quit (GtkWidget *, gpointer);
void cb_modifie (GtkWidget *, gpointer);

#endif /* not H_CALLBACK */
