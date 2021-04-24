#include <gtk/gtk.h>
#include <string.h>
#include "md5.h"

static GtkWidget *input_edit;
static GtkWidget *output_edit;

static void calculate_md5(){
    const gchar* data = gtk_entry_get_text(GTK_ENTRY(input_edit));
    printf("Data for hashing: %s\n", data);
    char* out_data = md5(data, strlen(data));
    char buffer[33] = {0};
    int i;
    for ( i = 0; i< 16; ++i ){
        sprintf(buffer+2*i, "%02x", out_data[i]&0xFF);
    }
    gtk_entry_set_text(GTK_ENTRY(output_edit), buffer);
    free(out_data);
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
    GtkWidget *window;
    GtkWidget *list_layout;
    GtkWidget *solve_button;

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Simple test MD5");
    gtk_window_set_default_size (GTK_WINDOW (window), 600, 100);
    gtk_window_set_resizable(GTK_WINDOW(window), 0);

    list_layout = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(window), list_layout);

    input_edit = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(list_layout), input_edit);

    output_edit = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(output_edit), 0);
    gtk_container_add(GTK_CONTAINER(list_layout), output_edit);

    solve_button = gtk_button_new_with_label("Calculate MD5");
    gtk_container_add(GTK_CONTAINER(list_layout), solve_button);

    g_signal_connect(solve_button, "clicked", G_CALLBACK(calculate_md5), NULL);

    gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}