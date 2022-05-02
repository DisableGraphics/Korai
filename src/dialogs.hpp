#include "gtkmm/aboutdialog.h"
#include "gtkmm/dialog.h"
#include <gtkmm.h>
#include <webkit2/webkit2.h>
#include <vte-2.91/vte/vte.h>
#include "icon.xpm"

class MIMEDialog : public Gtk::Dialog
{
    public:
        MIMEDialog()
        {
            //Notifies the user that the database has finished reloading
            Gtk::Button * button = add_button("OK", GTK_RESPONSE_ACCEPT);
            button->set_hexpand();
            
            img.set_from_icon_name("dialog-information", (Gtk::IconSize)GTK_ICON_SIZE_DIALOG);
            Gtk::Box * box;

            box = get_content_area();
            
            box->pack_start(img);
            
            box->pack_start(okmessage);
            
            show_all();
        }
    private:
        Gtk::Image img;
        Gtk::Label okmessage{"Finished reloading the database"};
};
class DeleteDialog : public Gtk::Dialog
{
    public:
        DeleteDialog()
        {
            Gtk::Box * box = get_content_area();

            img.set_from_icon_name("dialog-warning", (Gtk::IconSize)GTK_ICON_SIZE_DIALOG);

            box->pack_start(img);

            box->pack_end(label);
            
            add_button("Cancel", GTK_RESPONSE_CANCEL);
            add_button("Delete", GTK_RESPONSE_ACCEPT);

            show_all();
        }
    private:
        Gtk::Image img;
        Gtk::Label label{"Are you sure?"};
   
};
class AboutkDialog : public Gtk::AboutDialog
{   
    public:
        AboutkDialog(){
            Gtk::Box * box = get_content_area();
            set_authors({"DisableGraphics"});
            set_website("https://github.com/DisableGraphics/Korai");
            set_program_name("Korai");
            Glib::RefPtr<Gdk::Pixbuf> pix = Gdk::Pixbuf::create_from_xpm_data(icon);
            set_logo(pix);
            set_license_type(Gtk::LICENSE_GPL_3_0);
            set_copyright("Made by DisableGraphics");
            Gtk::Label label;
            //It's funny that the longest line on this method is the link to my Patreon
            label.set_markup("<a href ='https://www.patreon.com/DisableGraphics' title =''>Donate</a>");
            box->pack_start(label);
            show_all();
            Gtk::Widget * but = get_action_area()->get_children()[1]; //The "License" button wouldn't hide automatically, so I forced it to
            but->hide();
        }
};