#include <gtk/gtk.h>
#include <string.h>

#define BUTTON_AMOUNT 20
#define CLEAR_SCREEN "AC"
#define LEFT_BRACKET "("
#define RIGHT_BRACKET ")"
#define MOD "mod"
#define PI "π"

enum positions {
  CLEAR_SCREEN_POS,
  LEFT_BRACKET_POS,
  RIGHT_BRACKET_POS,
  MOD_POS,
  PI_POS
};

typedef struct {
  GtkLabel *label;
  char operation[10];
} button_params;

static void print_string(GtkButton *button, gpointer data);
static void on_window_closed(GtkWindow *window, gpointer data);
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

void print_string(GtkButton *button, gpointer data) {
  button_params *params = (button_params *) data;
  static char display_string[100] = {'\0'};
  strcat(display_string, params->operation);

  if (strcmp(params->operation, CLEAR_SCREEN) == 0)
      *display_string = '\0';

  gtk_label_set_text(params->label, display_string);
}

void on_window_closed(GtkWindow *window, gpointer data) {
  g_free(data);
}

void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *label;
  GtkWidget *buttons[BUTTON_AMOUNT];
  button_params *button_properties[BUTTON_AMOUNT];

  for (int i = 0; i < BUTTON_AMOUNT; ++i)
      button_properties[i] = g_new(button_params, 1);

  strcpy(button_properties[CLEAR_SCREEN_POS]->operation, CLEAR_SCREEN);
  strcpy(button_properties[LEFT_BRACKET_POS]->operation, LEFT_BRACKET);
  strcpy(button_properties[RIGHT_BRACKET_POS]->operation, RIGHT_BRACKET);
  strcpy(button_properties[MOD_POS]->operation, MOD);
  strcpy(button_properties[PI_POS]->operation, PI);

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Calculator");
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

  grid = gtk_grid_new();
  gtk_grid_set_column_homogeneous(GTK_GRID(grid), FALSE);
  gtk_grid_set_row_homogeneous(GTK_GRID(grid), FALSE);

  label = gtk_label_new("");
  gtk_widget_set_size_request(label, 500, 300);
  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 500, 100);

  for (int i = 0; i < BUTTON_AMOUNT; ++i)
      button_properties[i]->label = GTK_LABEL(label);

  buttons[CLEAR_SCREEN_POS] = gtk_button_new_with_label(CLEAR_SCREEN);
  gtk_widget_set_size_request(buttons[CLEAR_SCREEN_POS], 100, 100);
  g_signal_connect(buttons[CLEAR_SCREEN_POS], "clicked", G_CALLBACK(print_string), button_properties[CLEAR_SCREEN_POS]);
  gtk_grid_attach(GTK_GRID(grid), buttons[CLEAR_SCREEN_POS], 0, 100, 100, 100);

  buttons[LEFT_BRACKET_POS] = gtk_button_new_with_label(LEFT_BRACKET);
  gtk_widget_set_size_request(buttons[LEFT_BRACKET_POS], 100, 100);
  g_signal_connect(buttons[LEFT_BRACKET_POS], "clicked", G_CALLBACK(print_string), button_properties[LEFT_BRACKET_POS]);
  gtk_grid_attach(GTK_GRID(grid), buttons[LEFT_BRACKET_POS], 100, 100, 100, 100);

  buttons[RIGHT_BRACKET_POS] = gtk_button_new_with_label(RIGHT_BRACKET);
  gtk_widget_set_size_request(buttons[RIGHT_BRACKET_POS], 100, 100);
  g_signal_connect(buttons[RIGHT_BRACKET_POS], "clicked", G_CALLBACK(print_string), button_properties[RIGHT_BRACKET_POS]);
  gtk_grid_attach(GTK_GRID(grid), buttons[RIGHT_BRACKET_POS], 200, 100, 100, 100);

  buttons[MOD_POS] = gtk_button_new_with_label(MOD);
  gtk_widget_set_size_request(buttons[MOD_POS], 100, 100);
  g_signal_connect(buttons[MOD_POS], "clicked", G_CALLBACK(print_string), button_properties[MOD_POS]);
  gtk_grid_attach(GTK_GRID(grid), buttons[MOD_POS], 300, 100, 100, 100);

  buttons[PI_POS] = gtk_button_new_with_label(PI);
  gtk_widget_set_size_request(buttons[PI_POS], 100, 100);
  g_signal_connect(buttons[PI_POS], "clicked", G_CALLBACK(print_string), button_properties[PI_POS]);
  gtk_grid_attach(GTK_GRID(grid), buttons[PI_POS], 400, 100, 100, 100);

  for (int i = 0; i < BUTTON_AMOUNT; ++i)
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_closed), button_properties[i]);
  
  gtk_window_set_child(GTK_WINDOW(window), grid);
  gtk_window_present(GTK_WINDOW(window));
}
