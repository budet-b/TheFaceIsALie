//
//  main.c
//  
//
//  Created by Benjamin Budet on 05/12/2015.
//
//

#include <stdlib.h>
#include <gtk/gtk.h>

static char* file_name;
GtkWidget *p_window = NULL;
GtkWidget *p_main_box = NULL;
GtkWidget *p_button_box = NULL;
GtkWidget *p_text_view = NULL;
GtkWidget *p_button = NULL;

void cb_quit (GtkWidget *p_widget, gpointer user_data)
{
    gtk_main_quit();
    
    /* Parametres inutilises */
    (void)p_widget;
    (void)user_data;
}

void cb_open (GtkWidget *p_widget, gpointer user_data){
    GtkWidget *p_dialog = NULL;
    GtkWidget *foto = NULL;
    p_dialog = gtk_file_chooser_dialog_new ("Ouvrir un fichier", NULL,
                                            GTK_FILE_CHOOSER_ACTION_OPEN,
                                            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                            GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                            NULL);
    if (gtk_dialog_run (GTK_DIALOG (p_dialog)) == GTK_RESPONSE_ACCEPT)
    {
        printf("Nous allons ouvrir");
        file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
        printf(" CHEMIN DANS LE CALLBACK : %s ",file_name);
        //open_file (file_name, GTK_TEXT_VIEW (user_data));
        // gtk_image_set_from_file(GTK_IMAGE(pImage), file_name);
        foto = gtk_image_new_from_file(file_name);
        //g_free (file_name), file_name = NULL;
    }
    gtk_widget_destroy (p_dialog);
    
    /* Parametre inutilise */
    (void)p_widget;
}

int main(int argc, char **argv){
    gtk_init (&argc,&argv);
    p_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(p_window), "TheFaceIsALie");
    g_signal_connect (G_OBJECT (p_window), "destroy", G_CALLBACK (cb_quit),NULL);
    p_main_box = gtk_vbox_new (TRUE, 0);
    p_button_box = gtk_hbutton_box_new ();
    gtk_box_pack_start (GTK_BOX (p_main_box), p_button_box, FALSE, FALSE, 0);
    gtk_container_add (GTK_CONTAINER (p_window), p_main_box);

    //Button ouvrir
    {
    GtkWidget *p_button = NULL;
    p_button = gtk_button_new_from_stock (GTK_STOCK_OPEN);
    
    gtk_box_pack_start (GTK_BOX (p_main_box), p_button_box, FALSE, FALSE, 0);
    }
   /* GtkWidget *p_dialog = gtk_file_chooser_dialog_new ("Ouvrir un fichier", NULL,
                                                       GTK_FILE_CHOOSER_ACTION_OPEN,
                                                       GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                       GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                                       NULL);
    file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
    printf("filename is %s \n",file_name);
    */
    gtk_widget_show_all (p_window);
    /* Lancement de la boucle principale */
    gtk_main ();
    return EXIT_SUCCESS;
}
