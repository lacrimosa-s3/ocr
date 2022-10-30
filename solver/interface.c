#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <errno.h>
#include "solver.h"

void cb_quit (GtkWidget*, gpointer);
void cb_print(GtkWidget*, gpointer);
void cb_sudoku(GtkWidget*, gpointer);
void cb_solve(GtkWidget*, gpointer);
void cb_open(GtkWidget*,gpointer);
struct cb_entry
{
	GtkBuilder* builder;
	char* filename;
};

int main()
{	
	gtk_init(NULL,NULL);
	// Constructs a GtkBuilder instance.
    	GtkBuilder* builder = gtk_builder_new();

    	// Loads the UI description.
    	// (Exits if an error occurs.)
    	if (gtk_builder_add_from_file(builder, "sudoku.glade", NULL) == 0)
    	{
        	//g_printerr("Error loading file: %s\n", perror->message);
        	//g_clear_error(&perror);
        	return 1;
    	}

    	// Gets the widgets.
    	GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
	GtkButton* open_button = GTK_BUTTON(gtk_builder_get_object(builder, "OPEN"));
    	GtkButton* solve_button = GTK_BUTTON(gtk_builder_get_object(builder, "SOLVE"));
	gtk_builder_connect_signals(builder,NULL);

	/*Window*/
	struct cb_entry first ={builder,"grid_00"};
	gtk_window_set_title( GTK_WINDOW(window), "Xdoku by CodeX");
	gtk_window_maximize(GTK_WINDOW (window));
	g_signal_connect(G_OBJECT (window), "destroy", G_CALLBACK(cb_quit), NULL);
	g_signal_connect(G_OBJECT(open_button),"clicked",G_CALLBACK(cb_open),&first);
	g_signal_connect(G_OBJECT(solve_button),"clicked",G_CALLBACK(cb_solve),&first);

	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}

void cb_solve(GtkWidget* p_widget, gpointer data)
{
	FILE *fileRead;
	fileRead= fopen("name_out_txt","r");
	if(fileRead == NULL)
    	{
		printf("Open a image before!");
    	}
	else
	{
		fseek(fileRead, 0, SEEK_END);
		int length = ftell(fileRead);
		fseek(fileRead,0,SEEK_SET);
		char* buffer = malloc(length);
		if(buffer)
		{
			fread(buffer,1,length,fileRead);
		}
		fclose(fileRead);
		remove("name_out_txt");
		int a = solvMain(buffer);
		a++;
		struct cb_entry *info= data;
		struct cb_entry end = {info->builder,name_file_out(buffer)};
		cb_sudoku(p_widget,&end);
	}
}

void cb_open(GtkWidget* p_widget,gpointer data)
{
        GtkWidget *dialog = NULL;
	struct cb_entry* start= data;
        gint res;
        dialog = gtk_file_chooser_dialog_new ("Open File",
                                      NULL,
                                      GTK_FILE_CHOOSER_ACTION_OPEN,
                                      ("Cancel"),
                                      GTK_RESPONSE_CANCEL,
                                      ("Open"),
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);

        res = gtk_dialog_run (GTK_DIALOG (dialog));
        if (res == GTK_RESPONSE_ACCEPT)
        {
                char *filename;
                GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
                filename = gtk_file_chooser_get_filename (chooser);
		start->filename = filename;
		FILE *fileWrite;
		fileWrite = fopen("name_out_txt","w");
		fprintf(fileWrite,"%s",filename);
		fclose(fileWrite);
		cb_sudoku(p_widget,data);
	
		g_free(filename);
        	gtk_widget_destroy (dialog);
	}
}
void cb_sudoku(GtkWidget *p_widget, gpointer data)
{
	struct cb_entry *info = data;
	GtkBuilder* builder = info->builder;
	FILE *fileRead;
        //Open your_file in read-only mode
        fileRead = fopen(info->filename, "r");
	if(fileRead != NULL)
    	{
                printf("Your file must be filled");
	        //Create a buffer with the space needed
	        char buffer[110];
	        //Seek to the beginning of the file
	        fseek(fileRead, 0, SEEK_SET);
	
        	//Read Data and put it on the buffer
        	size_t useless = fread(buffer, 110, 1, fileRead);
        	useless ++;
        	char* buffer_pt = buffer;
        	fclose(fileRead);
		size_t index_grid = 0;
        	char ch;
		GtkWidget* cell = NULL;

        	for(size_t index_buffer = 0; index_buffer<110; index_buffer++)
        	{
                	ch = *(buffer_pt + index_buffer);
                	if(ch == 46)
                	{
                        	index_grid++;
                	}
                	else
                	{
                        	if(ch>48 && ch<58)
                        	{
					char text[3];
					snprintf(text, sizeof text, "%zu", index_grid);
					char* txt = text;
					cell = GTK_WIDGET(gtk_builder_get_object(builder,txt));
					GtkTextBuffer *buffer = NULL;
					buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(cell));
					gtk_text_buffer_set_text(buffer,(gchar*)(buffer_pt + index_buffer), sizeof(char));
                                	index_grid++;
                        	}
                	}
		}
	}
	(void)p_widget;

}

void cb_quit (GtkWidget *p_widget, gpointer user_data)
{
	gtk_main_quit();
	(void)p_widget;
	(void)user_data;
}

void cb_print(GtkWidget *p_widget, gpointer data)
{
	g_print("%s", (char*) data);
	(void)p_widget;

}

