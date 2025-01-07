
// This file contains the process behind creating the GUI
// Here we defined our user interface for interactive adn easy usage
// there will be comments guiding through the process of creating the GUI
// ### coment ### this is for the global idea ###
// this is for specific explanation

#include <gtk/gtk.h> // GTK4 library for creating the GUI
#include "functions.h"  // including the file that conatins the logic of our app




GtkWidget *result_buffer; // a global pointer (UI element) that all functions define here can access and modify


/* ###
 these functions will take a button abd a complimentary data (mostly users inputs)
 to handle UI interaction based on the functions defined on functions.c file
 ech of these function are static void type will be triggered on an event and accept an input
 and show their results on the global variable result_buffer
 ###
 */

// function for handling clicking on search button (it takes data from id text entry
static void on_search_clicked(GtkButton *button, GtkEntryBuffer *buffer) {
    const char *id = gtk_entry_buffer_get_text(buffer); // casting the entry to a string
    char result[200] = "\0"; // this string will hold students data in case we found the student

    // Performing search and displaying searching results
    if (search(id, result, sizeof(result))) { // searching and putting search result in result[200]
        gtk_label_set_label(GTK_LABEL(result_buffer), result);
    } else { //if student was not found then we result[200] will hold "not found"
        g_print("Not found!\n");
        gtk_label_set_label(GTK_LABEL(result_buffer), "Student was not found");
    }
}

//function for the handling insert button click (it takes all the data)
static void on_insert_clicked(GtkButton *button, Buffers *buffers) {
    const char *id = gtk_entry_buffer_get_text(buffers->idBuffer); // ID
    const char *name = gtk_entry_buffer_get_text(buffers->nameBuffer); // name
    const char *birthYear = gtk_entry_buffer_get_text(buffers->birthYearBuffer); // birthyear
    const char *group = gtk_entry_buffer_get_text(buffers->groupBuffer); // group
    const char *math = gtk_entry_buffer_get_text(buffers->mathBuffer);  // math mark
    const char *algebra = gtk_entry_buffer_get_text(buffers->algebraBuffer);  // algebra mark
    const char *oop = gtk_entry_buffer_get_text(buffers->oopBuffer);  // oop mark
    const char *sfsd = gtk_entry_buffer_get_text(buffers->sfsdBuffer);  // sfsd mark
    const char *flag1 = gtk_entry_buffer_get_text(buffers->flagBuffer);  // deleted flag
    char flag[2];
    if (strcmp(flag1, "yes") == 0) { // if the user marked student as deleted by writing yess in lower case
        strcpy(flag, "1"); // then flag buffer will hold 1
    } else{ // other wise (inluding "Yes" or "YES" will not mark the student as deleted
        strcpy(flag, "0");
    }
    // Prepare student structure
    if (atoi(birthYear) > 2020 || atoi(birthYear) < 1990) { // birthyear should be between 2020 and 1990
        gtk_label_set_label(GTK_LABEL(result_buffer), "Please enter a valid birthyear");
        return; // other wise student won't be inserted and we will have message saying that birthday not valid
    }
    if (atof(math) > 20 || atof(math) < 0 || atof(algebra) < 0 ||atof(algebra) > 20 || atof(oop) > 20 || atof(sfsd) > 20 || atof(sfsd) < 0 || atof(oop) < 0) {
        gtk_label_set_label(GTK_LABEL(result_buffer), "Please enter valid notes");
        return; // makrs should be between 20 and 0
    }
    student stu = prepareInput(id, name, birthYear, group, sfsd, oop, math, algebra, flag);
    int status = insert(&stu);
    // Insert student into the file
    if (status == 1) {
        gtk_label_set_label(GTK_LABEL(result_buffer),"Student was inserted successfully");
    } else if (status == 0) {
        gtk_label_set_label(GTK_LABEL(result_buffer),"Student already exists");
    }
    else {
        gtk_label_set_label(GTK_LABEL(result_buffer),"Invalid name or ID");
    }
}
 // a function to save changes after finishing
static void on_save_clicked(GtkButton *button, Buffers *buffers) {
    overWrite(); // this function will overwrite the text file holding our data by filling it with our binary file
    // in a formated way
    gtk_label_set_label(GTK_LABEL(result_buffer), "Data wes saved"); // we will get message "data was saved"
}

// Callback function for the "Mark as deleted" button
static void on_mark_clicked(GtkButton *button, GtkEntryBuffer *buffer) {
    const char *text = gtk_entry_buffer_get_text(buffer); // taking student's ID and storing it in string text
    int status = deleteLogically(text);
    if (status == 1) { // if student exists then it will be flaged
        gtk_label_set_label(GTK_LABEL(result_buffer), "Student was marked as deleted");
    } else if (status == 2) {
        gtk_label_set_label(GTK_LABEL(result_buffer), "Student was unmarked as deleted");
    }
    else { // otherwise we will receive a message "could not mark student as deleted"
        gtk_label_set_label(GTK_LABEL(result_buffer), "could not mark Student as deleted");
    }
}

// Callback function for the "extract" button
static void on_extract_clicked(GtkButton *button, GtkEntryBuffer *buffer) {
    const char *text = gtk_entry_buffer_get_text(buffer); // getting the group and storing it into string
    char studentnames[1000] = "";  // Buffer for student names (same idea as search button)
    int status = extract(text, studentnames, sizeof(studentnames)); // status eaither 1 or 0
    char dest [1050];
    snprintf(dest, sizeof(dest), "Students from Group %s are : \n%s",text, studentnames);
    if (status == 1) { // if 1 (means extracting was succesful) then we will show students names
        gtk_label_set_label(GTK_LABEL(result_buffer), dest);
    } else { // other wise we will have a message "No students found!"
        gtk_label_set_label(GTK_LABEL(result_buffer), "No students found!\n");
    }
}

// Callback function for the "preapare" button
/* ####
preapre button gets the text file that contains our data and turn it into a structured file
for easy manipulation, it is necessary to click it in order to have the structured file otherwise
data won't be managed
####
*/
static void on_prepare_clicked(GtkButton *button, GtkEntryBuffer *buffer) {
    if(prepare()) {
        gtk_label_set_label(GTK_LABEL(result_buffer), "Data is prepared for manipulation");
    }
    else {
        gtk_label_set_label(GTK_LABEL(result_buffer), "could not prepare data");
    }
}
// Callback function for the "exit" button
static void on_exit_clicked(GtkButton *button, gpointer *buffer) {
    // clicking exit will save our new data automatically and close the window
    overWrite();
    GtkWindow *window = GTK_WINDOW(buffer);
    gtk_window_close(GTK_WINDOW(window));
}
// Callback function for the "reset data" button
static void on_reset_clicked(GtkButton *button, GtkEntryBuffer *buffer) {
    if(reset() && overWrite()) { // clicking this button will remove the flaged students and save the data
        // in our text file and our binary file also
        gtk_label_set_label(GTK_LABEL(result_buffer), "File was Reorganized");
    }
    else {
        gtk_label_set_label(GTK_LABEL(result_buffer), "could not reorganize file");
    }
}
// Callback for handling "modify" button
static void on_modify_clicked(GtkWidget *button, Buffers *buffers) {
    /*
     #####
     the function modify can modify all the data except average which will be
     calculated and ID which it will take to modify the student having that ID
     ####
     */
    const char *id = gtk_entry_buffer_get_text(buffers->idBuffer);
    const char *name = gtk_entry_buffer_get_text(buffers->nameBuffer);
    const char *birthYear = gtk_entry_buffer_get_text(buffers->birthYearBuffer);
    const char *group = gtk_entry_buffer_get_text(buffers->groupBuffer);
    const char *math = gtk_entry_buffer_get_text(buffers->mathBuffer);  // math marks
    const char *algebra = gtk_entry_buffer_get_text(buffers->algebraBuffer);  // algebra marks
    const char *oop = gtk_entry_buffer_get_text(buffers->oopBuffer);  // oop marks
    const char *sfsd = gtk_entry_buffer_get_text(buffers->sfsdBuffer);  // sfsd marks
    const char *flag = gtk_entry_buffer_get_text(buffers->flagBuffer);  // flag
    if (birthYear[0] != '\0') {
        if (atoi(birthYear) > 2020 || atoi(birthYear) < 1990) {
            gtk_label_set_label(GTK_LABEL(result_buffer), "Please enter a valid birthyer");
            return; // again age is between 1990 and 2020
        }
    }

    if (atof(math) > 20 || atof(math) < 0 || atof(algebra) < 0 ||atof(algebra) > 20 || atof(oop) > 20 || atof(sfsd) > 20 || atof(sfsd) < 0 || atof(oop) < 0) {
        gtk_label_set_label(GTK_LABEL(result_buffer), "Please enter valid notes");
        return; // notes are between 20 and 0
    }
    student stu = prepareInput(id, name, birthYear, group, sfsd, oop, math, algebra, flag);
    if (modify(&stu)) { // in case modifying we will have "Modifying was successful"
        gtk_label_set_label(GTK_LABEL(result_buffer), "Modifying was successful");
    }
    else { //otherwise "Couldn't modify student"
        gtk_label_set_label(GTK_LABEL(result_buffer), "Couldn't modify student");
    }

}

// Main activation function
/*
 ######
    this function will call the window to appear in our screen, it contains
    all the UI elements like buttons, labels, Entry texts... there
    will be few leading comments to explain the process of creating the UI
 ######
*/
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window, *btnSearch, *btnInsert, *btnSave, *btnExtract, *btnMark, *label;
    GtkWidget *textEntryId, *grid, *box, *childBox1, *childBox2, *childBox3, *childBox4;
    GtkWidget *textEntryName, *textInputGrp, *textEntryYear, *textEntryMark;
    GtkWidget *btnPrepare, *btnExit, *btnOrganise, *btnModify;
    Buffers *buffers = g_malloc(sizeof(Buffers));
    GtkWidget *noteBox, *textEntrySfsd, *textEntryOop, *textEntryMath, *textEntryAlgebra;

    // Load CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "C:/Users/pc/CLionProjects/FlatBaseGTK4/style.css");
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Create application window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "FlatBase");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600); // Smaller window size

    // Create grid
    grid = gtk_grid_new();
    gtk_widget_set_margin_start(grid, 10); // Reduced margin
    gtk_widget_set_margin_end(grid, 10); // Reduced margin
    gtk_widget_set_margin_top(grid, 10); // Reduced margin
    gtk_widget_set_margin_bottom(grid, 10); // Reduced margin
    gtk_window_set_child(GTK_WINDOW(window), grid);

    // Create main box
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(box, 10); // Reduced margin
    gtk_widget_set_margin_end(box, 10); // Reduced margin
    gtk_widget_set_margin_top(box, 10); // Reduced margin
    gtk_widget_set_margin_bottom(box, 10); // Reduced margin
    gtk_grid_attach(GTK_GRID(grid), box, 0, 0, 1, 1);

    // Create title label
    label = gtk_label_new(NULL);
    const char *markup = "<span font='Arial 16' weight='bold'>Welcome to FlatBase</span>";
    gtk_label_set_markup(GTK_LABEL(label), markup);
    gtk_box_append(GTK_BOX(box), label);

    // Create result buffer
    result_buffer = GTK_WIDGET(gtk_label_new("Results will be shown here"));
    gtk_widget_set_size_request(result_buffer, 100, 50);
    gtk_label_set_xalign(GTK_LABEL(result_buffer), 0.5);
    gtk_label_set_yalign(GTK_LABEL(result_buffer), 0.5);
    gtk_grid_attach(GTK_GRID(grid), result_buffer, 1, 0, 1, 1);
    gtk_widget_set_hexpand(result_buffer, TRUE);
    gtk_widget_set_vexpand(result_buffer, TRUE);

    // Initialize buffers
    buffers->idBuffer = gtk_entry_buffer_new(NULL, 0);
    buffers->nameBuffer = gtk_entry_buffer_new(NULL, 0);
    buffers->birthYearBuffer = gtk_entry_buffer_new(NULL, 0);
    buffers->groupBuffer = gtk_entry_buffer_new(NULL, 0);
    buffers->sfsdBuffer = gtk_entry_buffer_new(NULL, 0);
    buffers->oopBuffer = gtk_entry_buffer_new(NULL, 0);
    buffers->mathBuffer = gtk_entry_buffer_new(NULL, 0);
    buffers->algebraBuffer = gtk_entry_buffer_new(NULL, 0);
    buffers->flagBuffer = gtk_entry_buffer_new(NULL, 0);

    // Create child boxes
    childBox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); // Reduced spacing
    childBox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); // Reduced spacing
    childBox3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // Reduced spacing
    childBox4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20); // Reduced spacing
    noteBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); // Reduced spacing

    gtk_box_append(GTK_BOX(box), childBox1);
    gtk_box_append(GTK_BOX(box), childBox2);
    gtk_box_append(GTK_BOX(box), childBox3);
    gtk_box_append(GTK_BOX(box), childBox4);

    // Create text entries
    textEntryId = gtk_entry_new_with_buffer(buffers->idBuffer);
    textEntryName = gtk_entry_new_with_buffer(buffers->nameBuffer);
    textInputGrp = gtk_entry_new_with_buffer(buffers->groupBuffer);
    textEntryYear = gtk_entry_new_with_buffer(buffers->birthYearBuffer);
    textEntryMark = gtk_entry_new_with_buffer(buffers->flagBuffer);
    textEntrySfsd = gtk_entry_new_with_buffer(buffers->sfsdBuffer);
    textEntryOop = gtk_entry_new_with_buffer(buffers->oopBuffer);
    textEntryMath = gtk_entry_new_with_buffer(buffers->mathBuffer);
    textEntryAlgebra = gtk_entry_new_with_buffer(buffers->algebraBuffer);

    // Set placeholders
    gtk_entry_set_placeholder_text(GTK_ENTRY(textEntryId), "Enter ID");
    gtk_entry_set_placeholder_text(GTK_ENTRY(textEntryName), "Enter Name");
    gtk_entry_set_placeholder_text(GTK_ENTRY(textInputGrp), "Enter Group");
    gtk_entry_set_placeholder_text(GTK_ENTRY(textEntryYear), "Enter Year");
    gtk_entry_set_placeholder_text(GTK_ENTRY(textEntryMark), "mark student as 'deleted'");
    gtk_entry_set_placeholder_text(GTK_ENTRY(textEntrySfsd), "SFSD Note");
    gtk_entry_set_placeholder_text(GTK_ENTRY(textEntryOop), "OOP Note");
    gtk_entry_set_placeholder_text(GTK_ENTRY(textEntryAlgebra), "ALG Note");
    gtk_entry_set_placeholder_text(GTK_ENTRY(textEntryMath), "MATH Note");

    // Create buttons
    btnSearch = gtk_button_new_with_label("Search");
    btnMark = gtk_button_new_with_label("mark/unmark as Deleted");
    btnInsert = gtk_button_new_with_label("Insert");
    btnModify = gtk_button_new_with_label("Modify");
    btnExtract = gtk_button_new_with_label("Extract");
    btnPrepare = gtk_button_new_with_label("Prepare Data");
    btnExit = gtk_button_new_with_label("Exit");
    btnOrganise = gtk_button_new_with_label("Reset Data");
    btnSave = gtk_button_new_with_label("Save Changes");



    // Add CSS classes
    gtk_widget_add_css_class(window, "main-window");
    gtk_widget_add_css_class(grid, "main-grid");
    gtk_widget_add_css_class(box, "main-box");
    gtk_widget_add_css_class(result_buffer, "result-label");

    gtk_widget_add_css_class(btnSearch, "action-button");
    gtk_widget_add_css_class(btnMark, "action-button");
    gtk_widget_add_css_class(btnInsert, "primary-button");
    gtk_widget_add_css_class(btnModify, "primary-button");
    gtk_widget_add_css_class(btnExtract, "action-button");
    gtk_widget_add_css_class(btnPrepare, "system-button");
    gtk_widget_add_css_class(btnExit, "system-button");
    gtk_widget_add_css_class(btnOrganise, "system-button");
    gtk_widget_add_css_class(btnSave, "system-button");

    gtk_widget_add_css_class(textEntryId, "text-entry");
    gtk_widget_add_css_class(textEntryName, "text-entry");
    gtk_widget_add_css_class(textInputGrp, "text-entry");
    gtk_widget_add_css_class(textEntryYear, "text-entry");
    gtk_widget_add_css_class(textEntryMark, "text-entry");
    gtk_widget_add_css_class(textEntrySfsd, "text-entry");
    gtk_widget_add_css_class(textEntryOop, "text-entry");
    gtk_widget_add_css_class(textEntryMath, "text-entry");
    gtk_widget_add_css_class(textEntryAlgebra, "text-entry");

    // Connect signals
    g_signal_connect(btnSearch, "clicked", G_CALLBACK(on_search_clicked), buffers->idBuffer);
    g_signal_connect(btnMark, "clicked", G_CALLBACK(on_mark_clicked), buffers->idBuffer);
    g_signal_connect(btnInsert, "clicked", G_CALLBACK(on_insert_clicked), buffers);
    g_signal_connect(btnExtract, "clicked", G_CALLBACK(on_extract_clicked), buffers->groupBuffer);
    g_signal_connect(btnPrepare, "clicked", G_CALLBACK(on_prepare_clicked), NULL);
    g_signal_connect(btnOrganise, "clicked", G_CALLBACK(on_reset_clicked), NULL);
    g_signal_connect(btnExit, "clicked", G_CALLBACK(on_exit_clicked), window);
    g_signal_connect(btnModify, "clicked", G_CALLBACK(on_modify_clicked), buffers);
    g_signal_connect(btnSave, "clicked", G_CALLBACK(on_save_clicked), NULL);

    // Arrange widgets in childBox1
    gtk_box_append(GTK_BOX(childBox1), textEntryId);
    gtk_box_append(GTK_BOX(childBox1), btnSearch);
    gtk_box_append(GTK_BOX(childBox1), btnMark);

    // Arrange widgets in childBox2
    gtk_box_append(GTK_BOX(childBox2), textInputGrp);
    gtk_box_append(GTK_BOX(childBox2), btnExtract);

    // Arrange widgets in childBox3
    gtk_box_append(GTK_BOX(childBox3), textEntryName);
    gtk_box_append(GTK_BOX(childBox3), textEntryYear);
    gtk_box_append(GTK_BOX(childBox3), textEntryMark);
    gtk_box_append(GTK_BOX(childBox3), noteBox);
    gtk_box_append(GTK_BOX(childBox3), btnInsert);
    gtk_box_append(GTK_BOX(childBox3), btnModify);

    // Arrange widgets in childBox4
    gtk_box_append(GTK_BOX(childBox4), btnPrepare);
    gtk_box_append(GTK_BOX(childBox4), btnSave);
    gtk_box_append(GTK_BOX(childBox4), btnOrganise);
    gtk_box_append(GTK_BOX(childBox4), btnExit);

    // Arrange widgets in noteBox
    gtk_box_append(GTK_BOX(noteBox), textEntrySfsd);
    gtk_box_append(GTK_BOX(noteBox), textEntryOop);
    gtk_box_append(GTK_BOX(noteBox), textEntryMath);
    gtk_box_append(GTK_BOX(noteBox), textEntryAlgebra);

    gtk_window_present(GTK_WINDOW(window));
}


// the main function will run our activate function which will eventually leads to running the app
int main(int argc, char **argv) { // almost all apps made using GTK will have a similar main function
    GtkApplication *app;
    int status;
    app = gtk_application_new("com.FlatBase", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
