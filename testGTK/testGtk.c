# include <stdio.h>
# include <stdlib.h>
# include <gtk/gtk.h>

void callAda(GtkWidget *widget, gpointer data) {
    
}


int maint(int argc, char **argv) {
    
    GtkWidget* Fenetre = NULL;
    GtkWidget* Label = NULL;
    gchar* TexteConverti = NULL;

    gtk_init(&argc, &argv);
    
    Fenetre = gtk_window_new(GTK_WINDOW_TOPLEVEL); 
    gtk_window_set_title(GTK_WINDOW(Fenetre), "The face is a lie");
    gtk_window_set_default_size(GTK_WINDOW(Fenetre), 300, 100); // voir si taille aranger

    gtk_widget_show(Fenetre);
    
    g_signal_connect(G_OBJECT(Fenetre), "delete-event", G_CALLBACK(callAda), NULL);

    gtk_main();

    return 0;
}
