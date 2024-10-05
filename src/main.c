#include <gtk/gtk.h>

static void print_hello(GtkButton *button, gpointer data);
static void print_goodbye(GtkButton *button, gpointer data);
static void activate (GtkApplication *app, gpointer user_data);

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}

void print_hello(GtkButton *button, gpointer data) {
  GtkLabel *label = GTK_LABEL(data);

  gtk_label_set_text(label, "Hello World");
}

void print_goodbye(GtkButton *button, gpointer data) {
  GtkLabel *label = GTK_LABEL(data);

  gtk_label_set_text(label, "Goodbye World");
}

void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *label;
  GtkWidget *button1;
  GtkWidget *button2;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Hello");
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

  grid = gtk_grid_new();
  gtk_grid_set_column_homogeneous(GTK_GRID(grid), FALSE);
  gtk_grid_set_row_homogeneous(GTK_GRID(grid), FALSE);

  label = gtk_label_new("");
  gtk_widget_set_size_request(label, 500, 300);
  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 500, 300);

  button1 = gtk_button_new_with_label("Hello World");
  gtk_widget_set_size_request(button1, 250, 200);
  g_signal_connect(button1, "clicked", G_CALLBACK(print_hello), label);
  // gtk_window_set_child(GTK_WINDOW(window), button);
  gtk_grid_attach(GTK_GRID(grid), button1, 0, 300, 250, 200);

  button2 = gtk_button_new_with_label("Goodbye World");
  gtk_widget_set_size_request(button2, 250, 200);
  g_signal_connect(button2, "clicked", G_CALLBACK(print_goodbye), label);
  gtk_grid_attach(GTK_GRID(grid), button2, 250, 300, 250, 200);
  
  gtk_window_set_child(GTK_WINDOW(window), grid);
  gtk_window_present(GTK_WINDOW(window));
}
