#include "gui.h"

AgeBox::AgeBox() {
    days = 0;
    this->window = nullptr;
    this->dayEntry = nullptr;
}

AgeBox::~AgeBox() {
    if (this->window) {
        delete this->window;
        this->window = nullptr;
    }
    if (this->dayEntry) {
        delete this->dayEntry;
        this->dayEntry = nullptr;
    }
}

void AgeBox::on_btnok_click() {
    days = std::stoi(dayEntry->get_text());
    window->close();
}

int AgeBox::show() {
    auto app = Gtk::Application::create(APP_NAME);
    window = new Gtk::Window();
    dayEntry = new Gtk::Entry();
    window->set_title("Fudel");
    window->set_default_size(400, 200);
    window->set_border_width(20);
    Gtk::Label label;
    label.set_text("Select the remaining timelife for the file:");
    dayEntry->set_text("30");
    dayEntry->set_max_length(3);
    Gtk::Button btnOk;
    btnOk.set_label("OK");
    btnOk.signal_clicked().connect(sigc::mem_fun(this, &AgeBox::on_btnok_click));
    Gtk::VBox box;
    box.set_spacing(10);
    box.pack_start(label);
    box.pack_start(*dayEntry);
    box.pack_start(btnOk);
    window->add(box);
    box.show();
    label.show();
    dayEntry->show();
    btnOk.show();
    app->run(*window);
    return days;
}

bool show_notification(string &msg) {
    notify_init("Fudel");
    NotifyNotification* n = notify_notification_new ("Fudel", msg.c_str(), 0);
    int seconds = 7;
    notify_notification_set_timeout(n, seconds * 1000);
    return notify_notification_show(n, 0);
}