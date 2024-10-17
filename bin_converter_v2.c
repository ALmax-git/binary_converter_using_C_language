#include <gtk/gtk.h>
#include <ctype.h>

// Function to convert binary to decimal
static void convert_binary_to_decimal(GtkWidget *widget, gpointer data) {
    const gchar *binary_input = gtk_entry_get_text(GTK_ENTRY(data));
    char *end;
    long decimal_value = strtol(binary_input, &end, 2);

    GtkWidget *output_label = g_object_get_data(G_OBJECT(widget), "output_label");
    GtkWidget *console_label = g_object_get_data(G_OBJECT(widget), "console_label");

    if (*end != '\0') {
        gtk_label_set_text(GTK_LABEL(output_label), "Invalid");
        gtk_label_set_text(GTK_LABEL(console_label), "Error: Non-binary input detected.");
    } else {
        gchar *decimal_output = g_strdup_printf("%ld", decimal_value);
        gtk_label_set_text(GTK_LABEL(output_label), decimal_output);
        g_free(decimal_output);

        gchar *console_output = g_strdup_printf("Converted %s to %ld", binary_input, decimal_value);
        gtk_label_set_text(GTK_LABEL(console_label), console_output);
        g_free(console_output);
    }
}

// Function to validate that only binary digits (0 or 1) are entered
static gboolean on_input_validate(GtkEntry *entry, const gchar *text, gint length, gint *position, gpointer data) {
    for (int i = 0; i < length; i++) {
        if (text[i] != '0' && text[i] != '1') {
            return TRUE; // Prevent non-binary input
        }
    }
    return FALSE;
}

// Function to handle the conversion on input change
static void on_input_changed(GtkEntry *entry, gpointer data) {
    convert_binary_to_decimal(GTK_WIDGET(entry), data);
}

// Function that sets up the window and the widgets
static void on_activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *binary_entry;
    GtkWidget *convert_button;
    GtkWidget *output_label;
    GtkWidget *console_label;

    // Create a window with a dark theme
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Advanced Binary Converter");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 300);

    // Apply dark theme
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "window { background-color: #2E3440; }"
        "entry { background-color: #3B4252; color: #ECEFF4; }"
        "button { background-color: #4C566A; color: #ECEFF4; }"
        "label { color: #ECEFF4; }",
        -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(window));
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);

    // Create a grid layout
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Create label to display the decimal output in a square (at the top)
    output_label = gtk_label_new("Decimal Output");
    gtk_widget_set_size_request(output_label, 200, 100); // Square area
    gtk_grid_attach(GTK_GRID(grid), output_label, 0, 0, 1, 1);

    // Create entry for binary input (below the output)
    binary_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(binary_entry), "Enter binary number");
    gtk_entry_set_max_length(GTK_ENTRY(binary_entry), 32);
    gtk_grid_attach(GTK_GRID(grid), binary_entry, 0, 1, 1, 1);

    // Set input validation for binary numbers (only allow 0 or 1)
    g_signal_connect(binary_entry, "insert-text", G_CALLBACK(on_input_validate), NULL);

    // Create a button for manual conversion (below the input field)
    convert_button = gtk_button_new_with_label("Convert");
    gtk_grid_attach(GTK_GRID(grid), convert_button, 0, 2, 1, 1);

    // Create a label for console output at the bottom
    console_label = gtk_label_new("Console Output");
    gtk_grid_attach(GTK_GRID(grid), console_label, 0, 3, 1, 1);

    // Store output and console labels for later use
    g_object_set_data(G_OBJECT(convert_button), "output_label", output_label);
    g_object_set_data(G_OBJECT(convert_button), "console_label", console_label);

    // Connect the button click event to conversion logic
    g_signal_connect(convert_button, "clicked", G_CALLBACK(convert_binary_to_decimal), binary_entry);

    // Trigger conversion on input change
    g_signal_connect(binary_entry, "changed", G_CALLBACK(on_input_changed), binary_entry);

    // Show all widgets
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Create a new GTK application
    app = gtk_application_new("com.almax.advancedbinconverter", G_APPLICATION_DEFAULT_FLAGS);

    // Connect the "activate" signal to the on_activate function
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    // Run the application
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Clean up
    g_object_unref(app);
    return status;
}
