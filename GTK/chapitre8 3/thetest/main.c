#include <stdlib.h>
#include <gtk/gtk.h>



static void open_file(GtkWidget* pOpen, gpointer pWindow, GtkWidget* pTable,)
{
    GtkWidget *image = NULL;
    GtkWidget *open;
    open = gtk_file_chooser_dialog_new("Select a file", GTK_WINDOW(pWindow),
                                       GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OK, GTK_RESPONSE_OK,
                                       GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_widget_show_all(open);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(open),
                                        g_get_home_dir());
    gint resp = gtk_dialog_run(GTK_DIALOG(open));
    if(resp == GTK_RESPONSE_OK)
    {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(open);
        image = gtk_image_new_from_file(gtk_file_chooser_get_filename(chooser));
        gtk_table_attach_defaults(GTK_TABLE(pTable),image,
                                  8,12,10,20);
    }
    else
        g_print("You pressed Cancel\n");
    gtk_widget_destroy(open);
}

int main(int argc,char *argv[])
{
    //Variable
    GtkWidget *pWindow = NULL;
    GtkWidget *pLabel = NULL;
    GtkWidget *pTable = NULL;
    GtkWidget *pButton = NULL;
    GtkWidget *pOpen = NULL;
    
    
    
    //Iniatialisation && attributs
    gtk_init(&argc, &argv);
    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(pWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(pWindow), 720, 480);
    pTable = gtk_table_new(40,20, TRUE);
    pButton = gtk_button_new_with_label("Open Image");
    
    
    
    
    //Position of the box
    gtk_table_attach_defaults(GTK_TABLE(pTable), pButton,
                              3,7,5,7);
    gtk_table_attach_defaults(GTK_TABLE(pTable), pLabel,
                              3,7,8,9);
    gtk_table_attach_defaults(GTK_TABLE(pTable), image,
                              3,7,1,3);
    
    //add the table to the main window
    gtk_container_add(GTK_CONTAINER(pWindow), GTK_WIDGET(pTable));
    
    //Signal
    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(pButton), "clicked", G_CALLBACK(open_file), NULL);
    gtk_widget_show_all(pWindow);
    gtk_main();