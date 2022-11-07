/*THIS HEADER DEFINES EVERYTHING RELATED TO THE DOWNLOADER*/
#ifndef NODOWNLOAD
#include <cstddef>
#include <filesystem>
#include <gtkmm.h>
#include <webkit2/webkit2.h>
#include <vte-2.91/vte/vte.h>
#include "global_variables.hpp"
#include "externalFunctions.h"
#include "dialogs.hpp"

inline void runCommandOnTerminal(GtkWidget * terminal, Gtk::Entry * e)
{
  std::string toRun{"mangodl -D "};
  int mangadex_downloader_exists = system("mangadex-downloader -v");
  if(position == -2 && mangadex_downloader_exists != 127)
  {
    std::string mng{e->get_text()};

    toRun = "mangadex-downloader --save-as cbz --folder " + (std::string)std::filesystem::current_path() + "/downloads ";

    toRun += mng;
    
  }
  else
  {
    toRun += e->get_text();
  }
  toRun += " && exit\n";
  vte_terminal_feed_child(VTE_TERMINAL(terminal), toRun.c_str(), -1);
}

static void execute_order_66(VteTerminal *terminal, GPid pid, GError *error, gpointer user_data)
{
    if (!terminal) return;
    if(position == -2)
    {
      std::string toRun = "mangadex-downloader --save-as cbz --folder " + (std::string)std::filesystem::current_path() + "/downloads ";
      toRun += (char *) user_data;
      toRun  += " && exit\n";
      vte_terminal_feed_child(VTE_TERMINAL(terminal), toRun.c_str(), -1);
    }
    
    if (pid == -1) gtk_main_quit();
}

class NotifyDialog : public custom_dialog
{
  public:
    NotifyDialog()
    {
      set_title("Finished downloading");
      dialog_icon.set_from_icon_name("dialog-information", Gtk::ICON_SIZE_DIALOG);

      label.set_text("Finished downloading.\nPlease close the downloader window.");

      Gtk::Box * c_box = get_content_area();

      box.pack_start(dialog_icon);
      box.pack_start(label);
      c_box->pack_start(box);

      add_button("OK", GTK_RESPONSE_OK);
      show_all();
    }
  private:
    Gtk::VBox box;
    Gtk::Image dialog_icon;
    Gtk::Label label;
};

static void notify(gpointer user_data)
{
  NotifyDialog * notify = new NotifyDialog();
  if(notify->run() == GTK_RESPONSE_OK)
  {
    notify->hide();
  }
  //Note: I'm unable to close the window using this callback function. 
  //If someone manages to do so will receive my thanks 
}

class DownloadDialog : public Gtk::Window
{
  public:
    DownloadDialog(std::string title, WebKitWebView * webView){
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
            (gpointer)webkit_web_view_get_uri(webView));        /* user_data */
      
      g_signal_connect(vteTerminal, "child-exited", G_CALLBACK(notify), NULL);
      
      g_strfreev(envp);

      e.set_placeholder_text("Manga title");

      add(box);
      if(position != -2)
      {
        box.pack_start(e);
        box.pack_start(b_ok);
      }
      set_titlebar(t);
      t.set_show_close_button();
      t.set_title(title);

      e.signal_activate().connect(sigc::bind(sigc::ptr_fun(runCommandOnTerminal), vteTerminal, &e));
      b_ok.signal_clicked().connect(sigc::bind(sigc::ptr_fun(runCommandOnTerminal), vteTerminal, &e)); 

      box.pack_start(*terminal_widget); 
    }
    void hide_all()
    {
      this->hide();
    }
    private:
      Gtk::VBox box;
      Gtk::Entry e;
      Gtk::Button b_ok {"OK"};
      Gtk::HeaderBar t;
};



inline void download_manga(WebKitWebView * webView)
{
  DownloadDialog *d = new DownloadDialog("Download", webView);
  d->show_all();
}
#endif