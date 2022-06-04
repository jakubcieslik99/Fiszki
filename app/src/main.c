#include <gtk/gtk.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    GtkStack *w_cardsContainer;
     GtkWidget *w_mainMenu;
     GtkWidget *w_uploadPanel;
      GtkWidget *w_start2Button;
     GtkWidget *w_testArea;
      GtkProgressBar *w_progressBar;
      GtkLabel *w_mistakesMade;
      GtkLabel *w_wordFrom;
      GtkEntry *w_wordTo;
      GtkButton *w_nextButton;
     GtkWidget *w_summaryScreen;
      GtkProgressBar *w_finishBar;
      GtkLabel *w_earnedPoints;
      GtkButton *w_mistakesButton;
     GtkWidget *w_mistakesScreen;
      GtkButton *w_mistakesBackButton;
      GtkTextBuffer* w_mistakesBuffer;
      GtkTextView *w_mistakesArea;
    GtkWidget *w_creditsWindow;
    GtkWidget *w_choosefileWindow;
} app_widgets;

int rows=0, current=0, points=0, mistakes=0, selected=0, *randomized, *mistaken;
double fraction=0.0;
char **from=NULL, **to=NULL;

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *w_mainWindow;
    app_widgets *widgets = g_slice_new(app_widgets);

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("glade/window_main.glade");
    w_mainWindow = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));

    //get pointers to widgets
    widgets->w_cardsContainer = GTK_STACK(gtk_builder_get_object(builder, "cardsContainer"));
     widgets->w_mainMenu = GTK_WIDGET(gtk_builder_get_object(builder, "mainMenu"));
     widgets->w_uploadPanel = GTK_WIDGET(gtk_builder_get_object(builder, "uploadPanel"));
      widgets->w_start2Button = GTK_WIDGET(gtk_builder_get_object(builder, "start2Button"));
     widgets->w_testArea = GTK_WIDGET(gtk_builder_get_object(builder, "testArea"));
      widgets->w_progressBar = GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "progressBar"));
      widgets->w_mistakesMade = GTK_LABEL(gtk_builder_get_object(builder, "mistakesMade"));
      widgets->w_wordFrom = GTK_LABEL(gtk_builder_get_object(builder, "wordFrom"));
      widgets->w_wordTo = GTK_ENTRY(gtk_builder_get_object(builder, "wordTo"));
      widgets->w_nextButton = GTK_BUTTON(gtk_builder_get_object(builder, "nextButton"));
     widgets->w_summaryScreen = GTK_WIDGET(gtk_builder_get_object(builder, "summaryScreen"));
      widgets->w_finishBar = GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "finishBar"));
      widgets->w_earnedPoints = GTK_LABEL(gtk_builder_get_object(builder, "earnedPoints"));
      widgets->w_mistakesButton = GTK_BUTTON(gtk_builder_get_object(builder, "mistakesButton"));
     widgets->w_mistakesScreen = GTK_WIDGET(gtk_builder_get_object(builder, "mistakesScreen"));
      widgets->w_mistakesBackButton = GTK_BUTTON(gtk_builder_get_object(builder, "mistakesBackButton"));
      widgets->w_mistakesArea = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "mistakesArea"));
    widgets->w_creditsWindow = GTK_WIDGET(gtk_builder_get_object(builder, "creditsWindow"));
    widgets->w_choosefileWindow = GTK_WIDGET(gtk_builder_get_object(builder, "choosefileWindow"));

    widgets->w_mistakesBuffer = gtk_text_buffer_new(NULL);

    gtk_builder_connect_signals(builder, widgets);

    g_object_unref(builder);

    gtk_widget_show(w_mainWindow);
    gtk_main();
    g_slice_free(app_widgets, widgets);

    srand(time(0));

    return 0;
}

/*------------------------------------------------------------------------------------------*/

int randomizeNumber() {
    srand(time(0));
    int r=0;
    bool flag=true;

    while(flag==true) {
        r = rand()%rows;
        for(int i=0; i<current; i++) {
            if(randomized[i]==r) {
                flag=true;
                break;
            } 
            else flag=false;
        }
    }
    //g_print("%d\n", r);
    return r;
}

void setBuffer(app_widgets *a_w) {
    GtkTextIter end;
    for(int i=0; i<rows; i++) {
        if(mistaken[i]==1) {
            gtk_text_buffer_get_end_iter(a_w->w_mistakesBuffer, &end);
            gtk_text_buffer_insert(a_w->w_mistakesBuffer, &end, from[i], -1);
            gtk_text_buffer_insert(a_w->w_mistakesBuffer, &end, " - ", -1);
            gtk_text_buffer_insert(a_w->w_mistakesBuffer, &end, to[i], -1);
            //gtk_text_buffer_insert(a_w->w_mistakesBuffer, &end, "\n", -1);
        }
    }
    gtk_text_view_set_buffer(a_w->w_mistakesArea, a_w->w_mistakesBuffer);
}

void setFinals(app_widgets *a_w) {
    char finishBarText[40] = "Poprawnie wypełnionych słówek: ", pointsFinal[4], rowsFinal[4];
    sprintf(pointsFinal, "%d", points);
    sprintf(rowsFinal, "%d", rows);
    strcat(finishBarText, pointsFinal);
    strcat(finishBarText, "/");
    strcat(finishBarText, rowsFinal);
    gtk_progress_bar_set_text(a_w->w_finishBar, finishBarText);
    gtk_label_set_text(a_w->w_earnedPoints, pointsFinal);
}

void updateStatus(app_widgets *a_w) {
    const char *grabbedWord = to[selected], *writtenWord = gtk_entry_get_text(a_w->w_wordTo);
    int checker = strcmp(grabbedWord, writtenWord);
    
    //check if last is ASCII backspace or carriage return
    if(checker==10 || checker==13) points++;
    else {
        mistakes++;
        mistaken[selected] = 1;
        gchar *displayMistakes = g_strdup_printf("%d", mistakes);
        gtk_label_set_text(a_w->w_mistakesMade, displayMistakes);
        g_free(displayMistakes);
    }
    fraction = fraction+(1.0/rows);
    gtk_progress_bar_set_fraction(a_w->w_progressBar, fraction);

    current++;
    if(current<rows-1) {
        selected = randomizeNumber();
        randomized[current] = selected;

        gtk_label_set_text(a_w->w_wordFrom, from[selected]);
        gtk_entry_set_text(a_w->w_wordTo, "");
    }
    else if(current==rows-1) {
        selected = randomizeNumber();

        gtk_button_set_label(a_w->w_nextButton, "Zakończ");
        gtk_label_set_text(a_w->w_wordFrom, from[selected]);
        gtk_entry_set_text(a_w->w_wordTo, "");
    }
    else {
        setFinals(a_w);
        double fractionFinal = points*(1.0/rows);
        gtk_progress_bar_set_fraction(a_w->w_finishBar, fractionFinal);

        if(points<rows) {
            setBuffer(a_w);
            gtk_widget_set_sensitive(a_w->w_mistakesButton, true);
        }
        else gtk_widget_set_sensitive(a_w->w_mistakesButton, false);

        gtk_stack_set_visible_child(a_w->w_cardsContainer, a_w->w_summaryScreen);

        gtk_label_set_text(a_w->w_wordFrom, "null");
    }
}

void readFromFile(gchar *filename, app_widgets *a_w) {
    //count rows from file
    FILE *stream_count = fopen(filename, "r");
    int c=0;
    while(!feof(stream_count)) {
        c = fgetc(stream_count);
        if(c=='\n') rows++;
    }
    fclose(stream_count);

    //allocate arrays
    from = malloc(rows*sizeof(char*)); 
    to = malloc(rows*sizeof(char*));
    for(int i=0; i<rows; i++) {
        from[i] = malloc(64);
        to[i] = malloc(64);
    }

    //save all words to arrays
    FILE *stream_save = fopen(filename, "r");
    int field_count=0, i=0;
    char buf[130];
    while(fgets(buf, 130, stream_save)) {
        char *field = strtok(buf, ";");
        field_count=0;

        while(field) {
            if(field_count==0) {
                strcpy(from[i], field);
            }
            if(field_count==1) {
                strcpy(to[i], field);
                i++;
            }
            field = strtok(NULL, ",");
            field_count++;
        }
    }
    fclose(stream_save);
    gtk_widget_set_sensitive(a_w->w_start2Button, true);
}

/*------------------------------------------------------------------------------------------*/

void on_menuButton_clicked(GtkButton *button, app_widgets *a_w) {
    gtk_stack_set_visible_child(a_w->w_cardsContainer, a_w->w_mainMenu);
    rows=0;
    current=0;
    points=0;
    mistakes=0;
    selected=0;
    free(randomized);
    free(mistaken);
    fraction=0.0;
    free(from);
    free(to);
    gtk_text_buffer_set_text(a_w->w_mistakesBuffer, "", -1);
    gtk_text_view_set_buffer(a_w->w_mistakesArea, a_w->w_mistakesBuffer);
}

void on_mistakesBackButton_clicked(GtkButton *button, app_widgets *a_w) {
    gtk_stack_set_visible_child(a_w->w_cardsContainer, a_w->w_summaryScreen);
}

void on_mistakesButton_clicked(GtkButton *button, app_widgets *a_w) {
    gtk_stack_set_visible_child(a_w->w_cardsContainer, a_w->w_mistakesScreen);
}

void on_nextButton_clicked(GtkButton *button, app_widgets *a_w) {
    updateStatus(a_w);
}

void on_wordTo_activate(GtkEntry *entry, app_widgets *a_w) {
    updateStatus(a_w);
}

void on_finishButton_clicked(GtkButton *button, app_widgets *a_w) {
    setFinals(a_w);
    double fractionFinal = points*(1.0/rows);
    gtk_progress_bar_set_fraction(a_w->w_finishBar, fractionFinal);

    gtk_stack_set_visible_child(a_w->w_cardsContainer, a_w->w_summaryScreen);

    gtk_label_set_text(a_w->w_wordFrom, "null");
}

void on_start2Button_clicked(GtkButton *button, app_widgets *a_w) {
    randomized = malloc(rows*sizeof(int*));
    srand(time(0));
    selected = rand()%rows;
    randomized[0] = selected;
    //g_print("%d\n", selected);

    mistaken = malloc(rows*sizeof(int*));
    for(int j=0; j<rows; j++) mistaken[j] = 0;
    
    gtk_progress_bar_set_fraction(a_w->w_progressBar, 0.0);
    gtk_label_set_text(a_w->w_mistakesMade, "0");
    gtk_label_set_text(a_w->w_wordFrom, from[selected]);
    gtk_entry_set_text(a_w->w_wordTo, "");
    gtk_button_set_label(a_w->w_nextButton, "Dalej");
    gtk_stack_set_visible_child(a_w->w_cardsContainer, a_w->w_testArea);
}

void on_choosefileButton_clicked(GtkButton *button, app_widgets *a_w) {
    gchar *file_name = NULL;
    gtk_widget_show(a_w->w_choosefileWindow);
    if(gtk_dialog_run(GTK_DIALOG(a_w->w_choosefileWindow))==GTK_RESPONSE_OK) {
        file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(a_w->w_choosefileWindow));
        if(file_name!=NULL) readFromFile(file_name, a_w);
        g_free(file_name);
    }
    gtk_widget_hide(a_w->w_choosefileWindow);
}

void on_back1Button_clicked(GtkButton *button, app_widgets *a_w) {
    gtk_stack_set_visible_child(a_w->w_cardsContainer, a_w->w_mainMenu);
    rows=0;
    free(from);
    free(to);
}

void on_startButton_clicked(GtkButton *button, app_widgets *a_w) {
    gtk_widget_set_sensitive(a_w->w_start2Button, false);
    gtk_stack_set_visible_child(a_w->w_cardsContainer, a_w->w_uploadPanel);
}

void on_creditsButton_clicked(GtkButton *button, app_widgets *a_w) {
    gtk_widget_show(a_w->w_creditsWindow);
}

void on_exitButton_clicked() {
    gtk_main_quit();
}

void on_mainWindow_destroy() {
    gtk_main_quit();
}
