#include <gtk/gtk.h>



// a simple GTK4 app for quick demonstration of how the library works



static void on_button1_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *label = data;
    gtk_label_set_text(GTK_LABEL(label), "Hello");
}

static void on_button2_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *label = (GtkWidget *)data;
    gtk_label_set_text(GTK_LABEL(label), "");
}

static void on_quit_button_clicked(GtkWidget *widget, gpointer data) {
    g_application_quit(G_APPLICATION(data));  // Quit the application using the passed app instance
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *label;
    GtkWidget *box;
    GtkWidget *button3;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Simple App");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 150);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(window), box);

    label = gtk_label_new("");  // Create an empty label initially
    gtk_box_append(GTK_BOX(box), label);

    button1 = gtk_button_new_with_label("Show 'Hello'");
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), label);
    gtk_box_append(GTK_BOX(box), button1);

    button2 = gtk_button_new_with_label("Clear Text");
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button2_clicked), label);
    gtk_box_append(GTK_BOX(box), button2);

    button3 = gtk_button_new_with_label("Quit");
    g_signal_connect(button3, "clicked", G_CALLBACK(on_quit_button_clicked), app);
    gtk_box_append(GTK_BOX(box), button3);

    gtk_window_present(GTK_WINDOW(window));
}

int main2(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.simpleapp", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}
