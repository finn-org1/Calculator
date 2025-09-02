#include <gtk/gtk.h>
#include <string>
#include <sstream>
#include <stack>
#include <cctype>

static GtkWidget *display;
static GtkWidget *expression_label;
static std::string expression = "";

double evaluate(const std::string& expr) {
    std::stack<double> numbers;
    std::stack<char> ops;
    std::stringstream ss(expr);
    char ch;
    double num;
    while (ss >> ch) {
        if (isdigit(ch)) {
            ss.putback(ch);
            ss >> num;
            numbers.push(num);
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            while (!ops.empty() && ((ops.top() == '*' || ops.top() == '/') || (ch == '+' || ch == '-'))) {
                double b = numbers.top(); numbers.pop();
                double a = numbers.top(); numbers.pop();
                char op = ops.top(); ops.pop();
                if (op == '+') numbers.push(a + b);
                else if (op == '-') numbers.push(a - b);
                else if (op == '*') numbers.push(a * b);
                else if (op == '/') numbers.push(a / b);
            }
            ops.push(ch);
        }
    }
    while (!ops.empty()) {
        double b = numbers.top(); numbers.pop();
        double a = numbers.top(); numbers.pop();
        char op = ops.top(); ops.pop();
        if (op == '+') numbers.push(a + b);
        else if (op == '-') numbers.push(a - b);
        else if (op == '*') numbers.push(a * b);
        else if (op == '/') numbers.push(a / b);
    }
    return numbers.top();
}

void on_button_clicked(GtkWidget *widget, gpointer data) {
    const char *text = (const char *)data;
    if (strcmp(text, "=") == 0) {
        if (!expression.empty()) {
            gtk_label_set_text(GTK_LABEL(expression_label), expression.c_str());
            try {
                double result = evaluate(expression);
                std::stringstream ss;
                ss << result;
                gtk_label_set_text(GTK_LABEL(display), ss.str().c_str());
                expression = ss.str();
            } catch (...) {
                gtk_label_set_text(GTK_LABEL(display), "Error");
                expression = "";
            }
        }
    } else if (strcmp(text, "C") == 0) {
        expression = "";
        gtk_label_set_text(GTK_LABEL(expression_label), "");
        gtk_label_set_text(GTK_LABEL(display), "0");
    } else {
        expression += text;
        gtk_label_set_text(GTK_LABEL(display), expression.c_str());
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Display area
    GtkWidget *display_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(display_box, "display-box");
    gtk_box_pack_start(GTK_BOX(vbox), display_box, FALSE, FALSE, 10);

    expression_label = gtk_label_new("");
    gtk_widget_set_name(expression_label, "expression-label");
    gtk_label_set_xalign(GTK_LABEL(expression_label), 1.0);
    gtk_box_pack_start(GTK_BOX(display_box), expression_label, FALSE, FALSE, 0);

    display = gtk_label_new("0");
    gtk_widget_set_name(display, "result-label");
    gtk_label_set_xalign(GTK_LABEL(display), 1.0);
    gtk_box_pack_start(GTK_BOX(display_box), display, FALSE, FALSE, 0);

    // CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    const char *css =
        "#display-box { background-color: #f0f0f0; border-radius: 10px; padding: 10px; margin: 5px; }"
        "#expression-label { color: #888; font-size: 12px; }"
        "#result-label { color: #000; font-size: 24px; font-weight: bold; }"
        "button { transition: all 0.2s ease; }"
        "button:hover { background-color: #e0e0e0; transform: scale(1.05); }";
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

    const char *buttons[4][4] = {
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {"0", "C", "=", "+"}
    };

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            GtkWidget *button = gtk_button_new_with_label(buttons[i][j]);
            gtk_widget_set_hexpand(button, TRUE);
            gtk_widget_set_vexpand(button, TRUE);
            g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), (gpointer)buttons[i][j]);
            gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);
        }
    }

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
