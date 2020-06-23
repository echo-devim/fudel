#ifndef __FUDEL_GUI_H__
#define __FUDEL_GUI_H__

#define APP_NAME "org.fudel"

#include <libnotify/notify.h>
#include <iostream>
#include <gtkmm-3.0/gtkmm.h>

using namespace std;

bool show_notification(string &msg);

class AgeBox {
public:
    int show();
    AgeBox();
    ~AgeBox();
private:
    int days;
    Gtk::Entry *dayEntry;
    Gtk::Window *window;
    void on_btnok_click();
};

#endif