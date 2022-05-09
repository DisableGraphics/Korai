/*THIS HEADER DEFINES EVERYTHING RELATED TO THE DOWNLOADER*/
#include "gtkmm/headerbar.h"
#ifndef NODOWNLOAD

#include <gtkmm.h>
#include <webkit2/webkit2.h>
#include <vte-2.91/vte/vte.h>

inline void runCommandOnTerminal(GtkWidget * terminal, Gtk::Entry * e)
{
  std::string toRun{"mangodl -D "};
  toRun += e->get_text();
  toRun += "\n";
  vte_terminal_feed_child(VTE_TERMINAL(terminal), toRun.c_str(), -1);
}

static void execute_order_66(VteTerminal *terminal, GPid pid, GError *error, gpointer user_data)
{
    if (!terminal) return;
    if (pid == -1) gtk_main_quit();
}

class DownloadDialog : public Gtk::Dialog
{
  public:
    DownloadDialog(std::string title){
      GtkWidget * vteTerminal = vte_terminal_new();
      Gtk::Widget * terminal_widget = Glib::wrap( GTK_WIDGET( vteTerminal ) );
      
      gchar **envp = g_get_environ();
      gchar *command[2] = {getenv("SHELL")};

      vte_terminal_spawn_async(VTE_TERMINAL(vteTerminal),
            VTE_PTY_DEFAULT,
            NULL,         /* working directory  */
            command,      /* command */
            NULL,         /* environment */
            G_SPAWN_DEFAULT,            /* spawn flags */
            NULL, NULL,   /* child setup */
            NULL,         /* child pid */
            -1,           /* timeout */
            NULL,         /* cancellable */
            execute_order_66,  /* callback */
            NULL);        /* user_data */
      

      g_strfreev(envp);

      e.set_placeholder_text("Manga title");

      Gtk::Box * box = get_content_area();
      box->pack_start(e);
      set_titlebar(t);
      t.set_show_close_button();
      t.set_title(title);
      t.set_border_width(1);

      e.signal_activate().connect(sigc::bind(sigc::ptr_fun(runCommandOnTerminal), vteTerminal, &e));

      box->pack_start(b_ok);
      b_ok.signal_clicked().connect(sigc::bind(sigc::ptr_fun(runCommandOnTerminal), vteTerminal, &e)); 
      Gtk::Button * b_cancel = add_button("Cancel", GTK_RESPONSE_CANCEL);

      box->pack_start(*terminal_widget); 
    }
    private:
      Gtk::Entry e;
      Gtk::Button b_ok {"OK"};
      Gtk::HeaderBar t;
};



inline void download_manga()
{
  DownloadDialog d{"Download"};
  d.show_all();
  
  switch(d.run())
  {
    case GTK_RESPONSE_CANCEL:
      d.hide();
      break;
  }
}
#endif