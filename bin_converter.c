#include <gtk/gtk.h>
#include <stdlib.h>

// Function to convert binary to decimal
static void on_convert_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *binary_input = gtk_entry_get_text(GTK_ENTRY(data));
    char *end;
    long decimal_value = strtol(binary_input, &end, 2);

    if (*end != '\0') {
        gtk_entry_set_text(GTK_ENTRY(data), "Invalid Binary");
    } else {
        gchar *decimal_output = g_strdup_printf("%ld", decimal_value);
        gtk_entry_set_text(GTK_ENTRY(data), decimal_output);
        g_free(decimal_output);
    }
}

// Function that sets up the window and the widgets
static void on_activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *binary_entry;
    GtkWidget *convert_button;

    // Create a window with a dark theme
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Binary Converter");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    
    // Apply dark theme
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "window { background-color: #2E3440; }"
        "entry { background-color: #3B4252; color: #ECEFF4; }"
        "button { background-color: #4C566A; color: #ECEFF4; }",
        -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(window));
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);

    // Create a grid layout
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Create entry for binary input
    binary_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(binary_entry), "Enter binary number");
    gtk_grid_attach(GTK_GRID(grid), binary_entry, 0, 0, 1, 1);

    // Create a button for conversion
    convert_button = gtk_button_new_with_label("Convert to Decimal");
    gtk_grid_attach(GTK_GRID(grid), convert_button, 0, 1, 1, 1);

    // Connect the button click event to conversion logic
    g_signal_connect(convert_button, "clicked", G_CALLBACK(on_convert_button_clicked), binary_entry);

    // Show all widgets
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Create a new GTK application
    app = gtk_application_new("com.almax.binconverter", G_APPLICATION_DEFAULT_FLAGS);
    
    // Connect the "activate" signal to the on_activate function
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    
    // Run the application
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Clean up
    g_object_unref(app);
    return status;
}
