#include <stdlib.h>
#include <gtk/gtk.h>
#include "callback.h"
#include "document.h"

docs_t docs = {NULL, NULL};
gchar *file_name = NULL;
static GtkWidget *p_imageFinish = NULL;
GtkWidget* cb_open ()
{
    GtkWidget *p_dialog = NULL;
    GtkWidget *image = NULL;
    p_dialog = gtk_file_chooser_dialog_new ("Ouvrir un fichier", NULL,
                                            GTK_FILE_CHOOSER_ACTION_OPEN,
                                            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                            GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                            NULL);
    if (gtk_dialog_run (GTK_DIALOG (p_dialog)) == GTK_RESPONSE_ACCEPT)
    {
        //gchar *file_name = NULL;
        
        file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
        //open_file (file_name, GTK_TEXT_VIEW (user_data));
        printf("File_name is : %s \n",file_name);
        image = gtk_image_new_from_file (file_name);
        p_imageFinish = gtk_image_new_from_file (file_name);
        //g_free (file_name), file_name = NULL;
    }
    gtk_widget_destroy (p_dialog);
    
    /* Parametre inutilise */
    return image;
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
  //gtk_window_maximize (GTK_WINDOW (p_window));
  gtk_window_set_title (GTK_WINDOW (p_window), "Editeur de texte en GTK+");
  g_signal_connect (G_OBJECT (p_window), "destroy", G_CALLBACK (cb_quit), NULL);

  /* Creation du conteneur principal */
  p_main_box = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (p_window), p_main_box);
    //GtkWidget p_text_view = NULL;
  /* Creation de la zone de texte
  {
    GtkTextBuffer *p_text_buffer = NULL;

    p_text_view = gtk_text_view_new ();
    p_text_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (p_text_view));
    g_signal_connect (G_OBJECT (p_text_buffer), "changed", G_CALLBACK (cb_modifie), NULL);
    gtk_box_pack_start (GTK_BOX (p_main_box), p_text_view, TRUE, TRUE, 0);
  }
   */
  /* Creation du conteneur pour les boutons */
  p_button_box = gtk_hbutton_box_new ();
  gtk_box_pack_start (GTK_BOX (p_main_box), p_button_box, FALSE, FALSE, 0);

  /* Creation du bouton "Ouvrir" */
  {
      GtkWidget *p_button = NULL;
    p_button = gtk_button_new_from_stock (GTK_STOCK_OPEN);
    g_signal_connect_object (G_OBJECT (p_button), "clicked", G_CALLBACK (cb_open), p_button,NULL);
      p_imageFinish = p_button;
    printf("File_name42 is : %s \n",file_name);
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
