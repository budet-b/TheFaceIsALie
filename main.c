#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>
#include <string.h>
#include <time.h>

#include "callback.h"
#include "document.h"
#include "image.h"
#include "haar.h"
#include "adaboost.h"
#include "adabase.h"
#include "process.h"
#define MAXLINES 5000

char* file_name = NULL;
char lines[MAXLINES][MAXLINES];
docs_t docs = {NULL, NULL};

void wait_for_keypressed(void) {
  SDL_Event             event;
  // Infinite loop, waiting for event
  for (;;) {
    // Take an event
    SDL_PollEvent( &event );
    // Switch on event type
    switch (event.type) {
    // Someone pressed a key -> leave the function
    case SDL_KEYDOWN: return;
    default: break;
    }
  // Loop until we got the expected event
  }
}

SDL_Surface* display_image(SDL_Surface *img) {
  SDL_Surface          *screen;
  // Set the window to the same size as the image
  screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
  if ( screen == NULL ) {
    // error management
    errx(1, "Couldn't set %dx%d video mode: %s\n",
         img->w, img->h, SDL_GetError());
  }

  /* Blit onto the screen surface */
  if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
    warnx("BlitSurface error: %s\n", SDL_GetError());

  // Update the screen
  SDL_UpdateRect(screen, 0, 0, img->w, img->h);

  // wait for a key
  wait_for_keypressed();

  // return the screen for further uses
  return screen;
}


void display_haar(haarRecord* tab, int nbFeature) {
    for(int i = 0; i < nbFeature; i++) {
        printf("| %d |\n", tab[i].value);
    }
}


int* faces(int valid, int invalid) {
    static int visage[MAXLINES];
    for(int i = 0; i < valid; i++)
        visage[i] = 1;
    for(int i = valid; i < invalid; i++)
        visage[i] = -1;
    return visage;
}


void files(int *visage, char* path[], char *paths) {

    int i = 0;
    char filepath[MAXLINES];
    strcpy(filepath,paths);
    strcat(filepath,"DB.txt");
    FILE *fp = fopen(filepath, "r");

    if (fp == 0) {
        printf("%s\n",filepath);
        fprintf(stderr, "failed to open DBtexte\n");
        exit(1);
    }

    while (i < MAXLINES && fgets(lines[i], sizeof(lines[0]), fp)) {
        lines[i][strlen(lines[i])-1] = '\0';
        i = i + 1;
    }

    i = 0;
    while(strcmp(lines[i],"\0"))
        i++;

    char *folder = paths;

    for (size_t j = 0; j<(size_t)i; j++) {
        char *temp = lines[j];
        
        char *temp2 = (char *) malloc(1 + strlen(folder)+ strlen(temp));
        strcat(temp2,folder);
        strcat(temp2,temp);
        path[j] = temp2;
    }
    fclose(fp);
    visage = faces(i-1,i);
}

void randFace(int visage[], char* pathFace[], char* pathNotFace[],int size,char* finalpath[]) {
    srand(time(NULL));
    int face = 0, notface = 0, i = 0;
    while(i<size) {
        int r = rand() % 2;
        if(r==1 && pathFace[face] != '\0' && face<(size/2)) {
            visage[i] = 1;
            finalpath[i] = pathFace[face];
            face++;
            i++;
        }
        else if (r==0 && pathNotFace[notface] != '\0' && notface<(size/2)) {
            visage[i] = -1;
            finalpath[i] = pathNotFace[notface];
            notface++;
            i++;
        }
    }
}

void transformImage(haarRecord* haarTab, char* path) {
    //char* imagecopy = copy_filename(path);
    //printf("New file = %s\n",path);
    //copy_file(path,imagecopy);
    SDL_Rect positionFond;
    positionFond.x = 0;
    positionFond.y = 0;
    SDL_Surface *img = load_image(path);
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *ecran = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
    drawRect(haarTab,img);
    SDL_BlitSurface(img, NULL, ecran, &positionFond);
    SDL_Flip(ecran);
    wait_for_keypressed();
    SDL_Quit();
}

void trainingbutton(){
    int rounds = 10;
    int nombrevisages = 10;
    int visage[MAXLINES];
    char* pathface[MAXLINES];
    files(visage,pathface,"./Images/BigDB/Face/");
    char* pathnotface[MAXLINES];
    files(visage,pathnotface,"./Images/NewDB/");
    char* finalpath[MAXLINES];
    randFace(visage, pathface, pathnotface, nombrevisages, finalpath);
    printf("Starting Training\n");
    strongClassifier* result;
    result = adaboost(finalpath, visage, nombrevisages/2, nombrevisages/2, rounds);
    printf("Training Finished, Writing Classifier\n");
    writeClassifier(result);
    printf("Classifier writed\n");

}

void process_interface(){
    printf("Sending Image\n");
    printf("IMAGE PATH IS : %s \n",file_name);
    haarRecord* haartab = process(file_name);
    transformImage(haartab, file_name);
    printf("Processed Image\n");
}

int main(int argc, char* argv[]) {
    
    //TEST
    GtkWidget *p_window = NULL;
    GtkWidget *p_main_box = NULL;
    GtkWidget *p_button_box = NULL;
    GtkWidget *p_text_view = NULL;
    
    /* Initialisation de GTK+ */
    gtk_init (&argc, &argv);
    
    /* Creation de la fenetre principale de notre application */
    p_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    //gtk_window_maximize (GTK_WINDOW (p_window));
    gtk_window_set_title (GTK_WINDOW (p_window), "The Face Is A Lie");
    g_signal_connect (G_OBJECT (p_window), "destroy", G_CALLBACK (cb_quit), NULL);
    
    /* Creation du conteneur principal */
    p_main_box = gtk_vbox_new (TRUE, 0);
    gtk_container_add (GTK_CONTAINER (p_window), p_main_box);
    
    /* Creation de la zone de texte */
    {
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
            printf("Nous allons ouvrir \n");
            file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
            printf(" CHEMIN DANS LE CALLBACK : %s \n",file_name);
            //open_file (file_name, GTK_TEXT_VIEW (user_data));
            // gtk_image_set_from_file(GTK_IMAGE(pImage), file_name);
            foto = gtk_image_new_from_file(file_name);
            //g_free (file_name), file_name = NULL;
        }
        gtk_widget_destroy (p_dialog);
        
        //TEST
        GtkWidget *test = NULL;
        p_text_view = gtk_image_new_from_file(file_name);
        printf("le chemin obtenu est : %s \n", file_name);
        test = gtk_image_new_from_file(file_name);
        g_signal_connect (G_OBJECT (test), "clicked", G_CALLBACK (cb_modifie), NULL);
        gtk_box_pack_start (GTK_BOX (p_main_box), p_text_view, TRUE, TRUE, 0);
    }
    
    /* Creation du conteneur pour les boutons */
    p_button_box = gtk_hbutton_box_new ();
    gtk_box_pack_start (GTK_BOX (p_main_box), p_button_box, FALSE, FALSE, 0);
    
    /* Creation du bouton "Ouvrir" */
    {
        GtkWidget *p_button = NULL;
        //GtkWidget *p_pulco = NULL;
        p_button = gtk_button_new_from_stock (GTK_STOCK_OPEN);
        g_signal_connect (G_OBJECT (p_button), "clicked", G_CALLBACK (cb_open), p_text_view);
        gtk_box_pack_start (GTK_BOX (p_button_box), p_button, FALSE, FALSE, 0);
        
    }
    
    /* Creation du bouton "APPLY" */
    {
        GtkWidget *p_button = NULL;
        char* pathimage = file_name;
        printf("Process interface filename is : %s \n",pathimage);
        //p_button = gtk_button_new_from_stock (GTK_STOCK_APPLY);
        p_button = gtk_button_new_with_label ("identification");
        g_signal_connect (G_OBJECT (p_button), "clicked", G_CALLBACK(process_interface), NULL);
        gtk_box_pack_start (GTK_BOX (p_button_box), p_button, FALSE, FALSE, 0);
    }
    
    /* Creation du bouton "Sauvegarder sous" */
    {
        GtkWidget *p_button = NULL;
        p_button = gtk_button_new_with_label ("Learning");
        g_signal_connect (G_OBJECT (p_button), "clicked", G_CALLBACK (trainingbutton), NULL);
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

    
    //TEST

    
    
    FILE *database = NULL;
    FILE *classifier = NULL;
    
    if(argc < 2)
        errx(2, "Usage : \t\n train nbImage nbRound\t\n identify path\t\n add\t\n read\t\n search\t\n identify\t\n");

    if(strcmp(argv[1], "train") == 0) {
        int visage[MAXLINES];
        char* pathface[MAXLINES];
        files(visage,pathface,"./Images/BigDB/Face/");
        char* pathnotface[MAXLINES];
        files(visage,pathnotface,"./Images/NewDB/");
        char* finalpath[MAXLINES];
        randFace(visage, pathface, pathnotface, atoi(argv[2]), finalpath);
        printf("Starting Training\n");
        strongClassifier* result;
        result = adaboost(finalpath, visage, atoi(argv[2])/2, atoi(argv[2])/2, atoi(argv[3]));
        printf("Training Finished, Writing Classifier\n");
        writeClassifier(result);
        printf("Classifier writed\n");
        return 0;
    }

    if((database = (fopen("database", "a+"))) == NULL) {
        fprintf(stderr,"Impossible to open file in lecture");
        fclose(database);
    }

    if(strcmp(argv[1], "add") == 0) {
        database = fopen("database", "a+");
        add(argc, argv, database);
        fclose(database);
    }

    if(strcmp(argv[1], "read") == 0) {
        database = fopen("database", "r");
        readDB(database);
        fclose(database);
    }

    if(strcmp(argv[1], "search") == 0) {
        database = fopen("database","r");
        search(argv, database);
        fclose(database);
    }

    if(strcmp(argv[1], "identify") == 0) {
        printf("Sending Image\n");
        haarRecord* haartab = process(argv[2]);
        transformImage(haartab, argv[2]);
        printf("Processed Image\n");
    }

    

    return EXIT_SUCCESS;
}
