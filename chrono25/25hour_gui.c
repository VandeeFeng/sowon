#include <gtk/gtk.h>
#include <time.h>
#include <stdio.h>

typedef struct {
    int new_hours;
    int new_minutes;
    int new_seconds;
} Chrono25Time;

void convert_to_25hour(time_t now, Chrono25Time *chrono_time) {
    struct tm midnight = *localtime(&now);
    midnight.tm_hour = 0;
    midnight.tm_min = 0;
    midnight.tm_sec = 0;

    double seconds_since_midnight = difftime(now, mktime(&midnight));
    double new_seconds = seconds_since_midnight * 25.0 / 24.0;

    int chrono_hours = (int)(new_seconds / 3600) % 25;
    int chrono_minutes = (int)(new_seconds / 60) % 60;
    int chrono_seconds = (int)new_seconds % 60;

    chrono_time->new_hours = chrono_hours;
    chrono_time->new_minutes = chrono_minutes;
    chrono_time->new_seconds = chrono_seconds;
}

static gboolean update_time_label(GtkLabel *label) {
    Chrono25Time chrono_time;
    time_t now = time(NULL);
    convert_to_25hour(now, &chrono_time);

    char time_text[16];
    snprintf(time_text, sizeof(time_text), "%02d:%02d:%02d",
             chrono_time.new_hours, chrono_time.new_minutes, chrono_time.new_seconds);

    gtk_label_set_text(label, time_text);
    return TRUE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "25 Hour Clock");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 50);

    GtkWidget *time_label = gtk_label_new("");
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // title
    GtkWidget *title_label = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(title_label), "<b>25hour time</b>");
    gtk_box_pack_start(GTK_BOX(vbox), title_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), time_label, TRUE, TRUE, 0);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_timeout_add_seconds(1, (GSourceFunc)update_time_label, time_label);
    update_time_label(GTK_LABEL(time_label));

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
