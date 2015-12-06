#include <stdlib.h>
#include <gtk/gtk.h>
#include "callback.h"
#include "document.h"

docs_t docs = {NULL, NULL};
static char* file_name = NULL;
/*gchar filePATH (){
    GtkWidget *p_dialog = NULL;
    //GtkWidget *foto = NULL;
    p_dialog = gtk_file_chooser_dialog_new ("Ouvrir un fichier", NULL,
                                            GTK_FILE_CHOOSER_ACTION_OPEN,
                                            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                            GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                            NULL);
    if (gtk_dialog_run (GTK_DIALOG (p_dialog)) == GTK_RESPONSE_ACCEPT)
    {
        gchar *file_name = NULL;
        printf("Nous allons ouvrir \n");
        file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
        gtk_widget_destroy (p_dialog);
            return *file_name;
    }
    gchar *falsename = NULL;
    return *falsename;
}
 */

void cb_test (GtkWidget *p_widget, gpointer user_data){
    
    // TEST
    
    GtkWidget *p_dialog = NULL;
    GtkWidget *foto = NULL;
    p_dialog = gtk_file_chooser_dialog_new ("Ouvrir un fichier", NULL,
                                            GTK_FILE_CHOOSER_ACTION_OPEN,
                                            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                            GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                            NULL);
    if (gtk_dialog_run (GTK_DIALOG (p_dialog)) == GTK_RESPONSE_ACCEPT)
    {
        //gchar *file_name = NULL;
        printf("Nous allons ouvrir");
        file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
        //open_file (file_name, GTK_TEXT_VIEW (user_data));
        // gtk_image_set_from_file(GTK_IMAGE(pImage), file_name);
        foto = gtk_image_new_from_file(file_name);
        //g_free (file_name), file_name = NULL;
    }
    gtk_widget_destroy (p_dialog);
    
    //TEST
    GtkWidget *test = NULL;
    test = gtk_image_new_from_file(file_name);
    printf("le chemin obtenu est : %s \n", file_name);
    test = gtk_image_new_from_file(file_name);
}

int main (int argc, char **argv)
{
  GtkWidget *p_window = NULL;
  GtkWidget *p_main_box = NULL;
  GtkWidget *p_button_box = NULL;
  GtkWidget *p_text_view = NULL;

  /* Initialisation de GTK+ */
  gtk_init (&argc, &argv);

  /* Creation de la fenetre principale de notre application */
  p_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_maximize (GTK_WINDOW (p_window));
  gtk_window_set_title (GTK_WINDOW (p_window), "The Face Is A Lie");
  g_signal_connect (G_OBJECT (p_window), "destroy", G_CALLBACK (cb_quit), NULL);

  /* Creation du conteneur principal */
  p_main_box = gtk_vbox_new (TRUE, 0);
  gtk_container_add (GTK_CONTAINER (p_window), p_main_box);

  /* Creation de la zone de texte */
  {
    //GtkTextBuffer *p_text_buffer = NULL;
      
      
      
      
  }

  /* Creation du conteneur pour les boutons */
  p_button_box = gtk_hbutton_box_new ();
  gtk_box_pack_start (GTK_BOX (p_main_box), p_button_box, FALSE, FALSE, 0);

  /* Creation du bouton "Ouvrir" */
  {
    GtkWidget *p_button = NULL;
      //GtkWidget *p_pulco = NULL;
      printf("CHEMINNNN %s \n ",file_name);
    p_button = gtk_button_new_from_stock (GTK_STOCK_OPEN);
    g_signal_connect (G_OBJECT (p_button), "clicked", G_CALLBACK (cb_test), p_text_view);
    gtk_box_pack_start (GTK_BOX (p_button_box), p_button, FALSE, FALSE, 0);

  }

  /* Creation du bouton "Sauvegarder" */
  {
    GtkWidget *p_button = NULL;

    p_button = gtk_button_new_from_stock (GTK_STOCK_SAVE);
    g_signal_connect (G_OBJECT (p_button), "clicked", G_CALLBACK (cb_save), NULL);
    gtk_box_pack_start (GTK_BOX (p_button_box), p_button, FALSE, FALSE, 0);
  }

  /* Creation du bouton "Sauvegarder sous" */
  {
    GtkWidget *p_button = NULL;

    p_button = gtk_button_new_from_stock (GTK_STOCK_SAVE_AS);
    g_signal_connect (G_OBJECT (p_button), "clicked", G_CALLBACK (cb_saveas), NULL);
    gtk_box_pack_start (GTK_BOX (p_button_box), p_button, FALSE, FALSE, 0);
  }

  /* Creation du bouton "Quitter" */
  {
    GtkWidget *p_button = NULL;

    p_button = gtk_button_new_from_stock (GTK_STOCK_QUIT);
    g_signal_connect (G_OBJECT (p_button), "clicked", G_CALLBACK (cb_quit), NULL);
    gtk_box_pack_start (GTK_BOX (p_button_box), p_button, FALSE, FALSE, 0);
  }

  /* Affichage de la fenetre principale */
  gtk_widget_show_all (p_window);
  /* Lancement de la boucle principale */
  gtk_main ();
  return EXIT_SUCCESS;
}
