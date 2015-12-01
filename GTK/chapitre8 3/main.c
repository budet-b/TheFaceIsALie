#include <stdlib.h>
#include <gtk/gtk.h>
#include "callback.h"
#include "document.h"

docs_t docs = {NULL, NULL};

int main (int argc, char **argv)
{
    GtkWidget *pWindow;
    GtkWidget *pVBox;
    GtkWidget *pQuitImage;
    GtkWidget *pQuitBtn;
    //GtkWidget *p_dialog;
    
    
  GtkWidget *p_window = NULL;
  GtkWidget *p_main_box = NULL;
  GtkWidget *p_button_box = NULL;
  GtkWidget *p_text_view = NULL;
    GtkWidget *pImage;

  /* Initialisation de GTK+ */
  gtk_init (&argc, &argv);

  /* Creation de la fenetre principale de notre application */
  p_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  //gtk_window_maximize (GTK_WINDOW (p_window));
  gtk_window_set_title (GTK_WINDOW (p_window), "TheFaceIsALie");
  g_signal_connect (G_OBJECT (p_window), "destroy", G_CALLBACK (cb_quit), NULL);

  /* Creation du conteneur principal */
  p_main_box = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (p_window), p_main_box);



  /* Creation du conteneur pour les boutons */
  p_button_box = gtk_hbutton_box_new ();
  gtk_box_pack_start (GTK_BOX (p_main_box), p_button_box, FALSE, FALSE, 0);

  /* Creation du bouton "Ouvrir" */
  {
      GtkWidget *p_dialog = NULL;
      p_dialog = gtk_file_chooser_dialog_new ("Ouvrir un fichier", NULL,
                                              GTK_FILE_CHOOSER_ACTION_OPEN,
                                              GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                              GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                              NULL);
      printf("ouvrir opened \n");
      if (gtk_dialog_run (GTK_DIALOG (p_dialog)) == GTK_RESPONSE_ACCEPT)
      {
          gchar *file_name = NULL;
          
          file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
        printf("chemin : %s \n",file_name);
          printf("ici \n");
          //p_main_box = gtk_image_new_from_file(file_name);
           //         printf("ici22 \n");
          //open_file (file_name);
              //p_window = gtk_image_new_from_file(file_name);
              p_text_view = gtk_image_new_from_file(file_name);
                              printf("ici33333 \n");
          g_free (file_name), file_name = NULL;
          
      }
      gtk_widget_destroy (p_dialog);
      
      pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL); gtk_window_set_default_size(GTK_WINDOW(pWindow), 320, 200); gtk_window_set_title(GTK_WINDOW(pWindow), "GtkImage"); g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);
      pVBox = gtk_vbox_new(FALSE, 0); gtk_container_add(GTK_CONTAINER(pWindow), pVBox);
      /* Chargement d'une image a partir d'un fichier */
      gchar *file_name = NULL;
      
      file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
      pWindow = gtk_image_new_from_file(file_name);
      pImage = gtk_image_new_from_file(file_name);
      gtk_box_pack_start(GTK_BOX(pVBox), pImage, FALSE, FALSE, 5);
      pQuitBtn = gtk_button_new();
      gtk_box_pack_start(GTK_BOX(pVBox), pQuitBtn, TRUE, FALSE, 5); g_signal_connect(G_OBJECT(pQuitBtn), "clicked", G_CALLBACK(gtk_main_quit), NULL);
      /* Chargement d'une image a partir d'un GtkStockItem */
      pQuitImage = gtk_image_new_from_stock(GTK_STOCK_QUIT, GTK_ICON_SIZE_LARGE_TOOLBAR);
      gtk_container_add(GTK_CONTAINER(pQuitBtn), pQuitImage); gtk_widget_show_all(pWindow);


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
    
    
    //TEST
    
    GtkWidget *p_dialog = NULL;
    p_dialog = gtk_file_chooser_dialog_new ("Ouvrir un fichier", NULL,
                                            GTK_FILE_CHOOSER_ACTION_OPEN,
                                            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                            GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                            NULL);
    printf("ouvrir opened \n");
    if (gtk_dialog_run (GTK_DIALOG (p_dialog)) == GTK_RESPONSE_ACCEPT)
    {
       const gchar *file_name = NULL;
        
        file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
        printf("chemin : %s \n",file_name);
        printf("ici \n");
        //p_main_box = gtk_image_new_from_file(file_name);
        //         printf("ici22 \n");
        //open_file (file_name);
        //p_window = gtk_image_new_from_file(file_name);
        pWindow = gtk_image_new_from_file(file_name);
         //gtk_image_set_from_file(*pImage, const *file_name);
        printf("ici33333 \n");
    
    //TEST
        gtk_widget_show_all (p_window);
    
    pWindow = gtk_image_new_from_file(file_name);
  /* Lancement de la boucle principale */
        //gtk_image_set_from_file(*pImage,*file_name);
  gtk_main ();
    }
  return EXIT_SUCCESS;
}
