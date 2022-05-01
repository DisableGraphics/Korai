#ifndef NODOWNLOAD

#include <gtkmm.h>
#include <webkit2/webkit2.h>
#include <vte-2.91/vte/vte.h>



static void execute_order_66(VteTerminal *terminal, GPid pid, GError *error, gpointer user_data)
{
    if (!terminal) return;
    if (pid == -1) gtk_main_quit();
}

inline void runCommandOnTerminal(GtkWidget * terminal, Gtk::Entry * e)
{
  std::string toRun{"mangodl -D "};
  toRun += e->get_text();
  toRun += "\n";
  vte_terminal_feed_child(VTE_TERMINAL(terminal), toRun.c_str(), -1);
}

inline void download_manga()
{
  Gtk::Dialog d{"Download"};
  Gtk::Entry e;
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

  Gtk::Box * box = d.get_content_area();
  box->pack_start(e);

  e.signal_activate().connect(sigc::bind(sigc::ptr_fun(runCommandOnTerminal), vteTerminal, &e));

  Gtk::Button b_ok {"OK"};
  box->pack_start(b_ok);
  b_ok.signal_clicked().connect(sigc::bind(sigc::ptr_fun(runCommandOnTerminal), vteTerminal, &e)); 
  Gtk::Button * b_cancel = d.add_button("Cancel", GTK_RESPONSE_CANCEL);

  box->pack_start(*terminal_widget); 

  d.show_all();
  
  switch(d.run())
  {
    case GTK_RESPONSE_CANCEL:
      d.hide();
      break;
  }
}
#endif