#include <gtk/gtk.h>
#include <string.h>
#include "parser.h"

#define BUTTON_AMOUNT 24
#define CLEAR_SCREEN "AC"
#define LEFT_BRACKET "("
#define RIGHT_BRACKET ")"
#define MOD "mod"
#define PI "pi"
#define SEVEN "7"
#define EIGHT "8"
#define NINE "9"
#define DIVIDE "/"
#define ROOT "sqrt"
#define FOUR "4"
#define FIVE "5"
#define SIX "6"
#define MULTIPLY "*"
#define POWER "^"
#define ONE "1"
#define TWO "2"
#define THREE "3"
#define MINUS "-"
#define ZERO "0"
#define PERIOD "."
#define PERCENT "%"
#define PLUS "+"
#define EQUALS "="

enum positions {
  CLEAR_SCREEN_POS,
  LEFT_BRACKET_POS,
  RIGHT_BRACKET_POS,
  MOD_POS,
  PI_POS,
  SEVEN_POS,
  EIGHT_POS,
  NINE_POS,
  DIVIDE_POS,
  ROOT_POS,
  FOUR_POS,
  FIVE_POS,
  SIX_POS,
  MULTIPLY_POS,
  POWER_POS,
  ONE_POS,
  TWO_POS,
  THREE_POS,
  MINUS_POS,
  ZERO_POS,
  PEROID_POS,
  PERCENT_POS,
  PLUS_POS,
  EQUALS_POS
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

  if (strcmp(params->operation, EQUALS) == 0 && *display_string != '\0') {
    char result[100] = {'\0'};
    operator_precedence_parser(display_string, result);
    strcpy(display_string, result);
    gtk_label_set_text(params->label, result);
    return ;
  }

  if (strcmp(params->operation, "=") != 0)
    strncat(display_string, params->operation, 100-strlen(display_string));

  if (strcmp(params->operation, CLEAR_SCREEN) == 0)
      *display_string = '\0';
  
  gtk_label_set_text(params->label, display_string);
}

void initialize_button(GtkWidget **buttons, button_params **button_properties, int pos, int width, int height, int x, int y, GtkWidget *grid) {
  buttons[pos] = gtk_button_new_with_label(button_properties[pos]->operation);
  gtk_widget_set_size_request(buttons[pos], width, height);
  g_signal_connect(buttons[pos], "clicked", G_CALLBACK(print_string), button_properties[pos]);
  gtk_grid_attach(GTK_GRID(grid), buttons[pos], x, y, width, height);
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

  GtkCssProvider *custom_css = gtk_css_provider_new();
  gtk_css_provider_load_from_path(custom_css, "src/style.css");
  gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(custom_css), GTK_STYLE_PROVIDER_PRIORITY_USER);

  for (int i = 0; i < BUTTON_AMOUNT; ++i)
      button_properties[i] = g_new(button_params, 1);

  strcpy(button_properties[CLEAR_SCREEN_POS]->operation, CLEAR_SCREEN);
  strcpy(button_properties[LEFT_BRACKET_POS]->operation, LEFT_BRACKET);
  strcpy(button_properties[RIGHT_BRACKET_POS]->operation, RIGHT_BRACKET);
  strcpy(button_properties[MOD_POS]->operation, MOD);
  strcpy(button_properties[PI_POS]->operation, PI);
  strcpy(button_properties[SEVEN_POS]->operation, SEVEN);
  strcpy(button_properties[EIGHT_POS]->operation, EIGHT);
  strcpy(button_properties[NINE_POS]->operation, NINE);
  strcpy(button_properties[DIVIDE_POS]->operation, DIVIDE);
  strcpy(button_properties[ROOT_POS]->operation, ROOT);
  strcpy(button_properties[FOUR_POS]->operation, FOUR);
  strcpy(button_properties[FIVE_POS]->operation, FIVE);
  strcpy(button_properties[SIX_POS]->operation, SIX);
  strcpy(button_properties[MULTIPLY_POS]->operation, MULTIPLY);
  strcpy(button_properties[POWER_POS]->operation, POWER);
  strcpy(button_properties[ONE_POS]->operation, ONE);
  strcpy(button_properties[TWO_POS]->operation, TWO);
  strcpy(button_properties[THREE_POS]->operation, THREE);
  strcpy(button_properties[MINUS_POS]->operation, MINUS);
  strcpy(button_properties[ZERO_POS]->operation, ZERO);
  strcpy(button_properties[PEROID_POS]->operation, PERIOD);
  strcpy(button_properties[PERCENT_POS]->operation, PERCENT);
  strcpy(button_properties[PLUS_POS]->operation, PLUS);
  strcpy(button_properties[EQUALS_POS]->operation, EQUALS);

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Calculator");
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 700);

  grid = gtk_grid_new();
  gtk_grid_set_column_homogeneous(GTK_GRID(grid), FALSE);
  gtk_grid_set_row_homogeneous(GTK_GRID(grid), FALSE);

  label = gtk_label_new("");
  gtk_widget_set_size_request(label, 500, 200);
  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 500, 200);
  gtk_widget_add_css_class(label, "screen");

  for (int i = 0; i < BUTTON_AMOUNT; ++i)
      button_properties[i]->label = GTK_LABEL(label);
  
  initialize_button(buttons, button_properties, CLEAR_SCREEN_POS, 100, 100, 0, 200, grid);
  initialize_button(buttons, button_properties, LEFT_BRACKET_POS, 100, 100, 100, 200, grid);
  initialize_button(buttons, button_properties, RIGHT_BRACKET_POS, 100, 100, 200, 200 ,grid);
  initialize_button(buttons, button_properties, MOD_POS, 100, 100, 300, 200, grid);
  initialize_button(buttons, button_properties, PI_POS, 100, 100, 400, 200, grid);
  initialize_button(buttons, button_properties, SEVEN_POS, 100, 100, 0, 300, grid);
  initialize_button(buttons, button_properties, EIGHT_POS, 100, 100, 100, 300, grid);
  initialize_button(buttons, button_properties, NINE_POS, 100, 100, 200, 300, grid);
  initialize_button(buttons, button_properties, DIVIDE_POS, 100, 100, 300, 300, grid);
  initialize_button(buttons, button_properties, ROOT_POS, 100, 100, 400, 300, grid);
  initialize_button(buttons, button_properties, FOUR_POS, 100, 100, 0, 400, grid);
  initialize_button(buttons, button_properties, FIVE_POS, 100, 100, 100, 400, grid);
  initialize_button(buttons, button_properties, SIX_POS, 100, 100, 200, 400, grid);
  initialize_button(buttons, button_properties, MULTIPLY_POS, 100, 100, 300, 400, grid);
  initialize_button(buttons, button_properties, POWER_POS, 100, 100, 400, 400, grid);
  initialize_button(buttons, button_properties, ONE_POS, 100, 100, 0, 500, grid);
  initialize_button(buttons, button_properties, TWO_POS, 100, 100, 100, 500, grid);
  initialize_button(buttons, button_properties, THREE_POS, 100, 100, 200, 500, grid);
  initialize_button(buttons, button_properties, MINUS_POS, 100, 100, 300, 500, grid);
  initialize_button(buttons, button_properties, ZERO_POS, 100, 100, 0, 600, grid);
  initialize_button(buttons, button_properties, PEROID_POS, 100, 100, 100, 600, grid);
  initialize_button(buttons, button_properties, PERCENT_POS, 100, 100, 200, 600, grid);
  initialize_button(buttons, button_properties, PLUS_POS, 100, 100, 300, 600, grid);
  initialize_button(buttons, button_properties, EQUALS_POS, 100, 200, 400, 500, grid);

  for (int i = 0; i < BUTTON_AMOUNT; ++i) {
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_closed), button_properties[i]);
  }

  gtk_window_set_child(GTK_WINDOW(window), grid);
  gtk_window_present(GTK_WINDOW(window));
}
