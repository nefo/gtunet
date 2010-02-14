/*
 * gtunet.c
 *
 * Author: Yang Xudong
 * 
 * Copyright (C) 2009-2010 Yang Xudong <yangxudong.cn@gmail.com>
 *
 * This file is part of gtunet.
 *
 * gtunet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gtunet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gtunet.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "mytunet/ethcard.h"
#include "mytunet/mytunetsvc.h"
#include "mytunet/setting.h"
#include "mytunet/logs.h"

#include "gtunet.h"
#include "log.h"
#include "callbacks.h"
#include "utils.h"
#include "actions.h"

/*
 * Functions used inside this file
 */
void            gtunet_load_gui ();
void            gtunet_init_gui ();
void            gtunet_load_user_config ();
void            gtunet_connect_signals ();
GtkStatusIcon  *gtunet_create_tray_icon();
GtkMenu        *gtunet_create_tray_menu();
void            gtunet_init_log ();

/*
 * The main function
 */
int
main (int argc, char *argv[])
{
  /* Load the pixbuf */
  gtunet_load_pixbuf();
  
  /* Initiallize the GTK+ enviroment */
  gtk_init(&argc, &argv);

  /* Load the GUI from xml file */
  gtunet_load_gui();

  /* Load configuration from the config file, usually is ~/.MyTunet/MyTunet ,
   * which is the same as other mytunet programs
   */
  mytunetsvc_set_config_file(NULL);
  gtunet_load_user_config();
  
  /* Initiallize GUI components */
  gtunet_init_gui();

  /* Connect all the signals on the GUI */
  gtunet_connect_signals();

  /* show the window and tray_icon */
  gtk_status_icon_set_visible(tray_icon, TRUE);
  gtk_widget_show((GtkWidget *)window);

  /* Initiallize the log components */
  gtunet_init_log ();
  
  /* Initiallize mytunet service */
  mytunetsvc_init();

  /* Start the GUI'S event loop */
  gtk_main();

  return 0;
}

/*
 * gtunet_load_gui
 *
 * Load the GUI components from the xml file "gtunet.gui"
 */
void
gtunet_load_gui ()
{
  /* Create the builder */
  builder = gtk_builder_new();
  if (!gtk_builder_add_from_file(builder,
                                 /* TODO, change the path below. */
                                 "gtunet.gui",
                                 NULL))
    {
      puts("Unable to load GUI xml file.");
      exit(-1);
    }

  /* get widgets from the GUI xml file */
  window                   = (GtkWindow *)      gtk_builder_get_object
    (builder, "window");
  about_dialog             = (GtkAboutDialog *) gtk_builder_get_object
    (builder, "aboutdialog");
  combobox_adapter         = (GtkComboBox *)    gtk_builder_get_object
    (builder, "combobox_adapter");
  entry_username           = (GtkEntry *)       gtk_builder_get_object
    (builder, "entry_username");
  entry_password           = (GtkEntry *)       gtk_builder_get_object
    (builder, "entry_password");
  combobox_limitation      = (GtkComboBox *)    gtk_builder_get_object
    (builder, "combobox_limitation");
  image_status             = (GtkImage *)       gtk_builder_get_object
    (builder, "image_status");
  checkbutton_savepassword = (GtkCheckButton *) gtk_builder_get_object
    (builder, "checkbutton_savepassword");
  button_login             = (GtkButton *)      gtk_builder_get_object
    (builder, "button_login");
  button_logout            = (GtkButton *)      gtk_builder_get_object
    (builder, "button_logout");
  button_about             = (GtkButton *)      gtk_builder_get_object
    (builder, "button_about");
  button_help              = (GtkButton *)      gtk_builder_get_object
    (builder, "button_help");
  button_quit              = (GtkButton *)      gtk_builder_get_object
    (builder, "button_quit");
  textview_info            = (GtkTextView *)    gtk_builder_get_object
    (builder, "textview_info");
  textbuffer_info          = (GtkTextBuffer *)  gtk_builder_get_object
    (builder, "textbuffer_info");
  label_account_money      = (GtkLabel *)  gtk_builder_get_object
    (builder, "label_account_money");
  label_used_money         = (GtkLabel *)  gtk_builder_get_object
    (builder, "label_used_money");
}

/*
 * gtunet_init_gui
 *
 * Add the components that are not in the xml file.
 */
void
gtunet_init_gui ()
{
  int i;

  GtkListStore *liststore_adapter;
  GtkListStore *liststore_limitation;
  GtkTreeIter treeiter;

  GtkCellRenderer *renderer;
  renderer = gtk_cell_renderer_text_new();
  
  /*
   * 1. Init combobox_adapter
   */
  /* Get number of ethercards using mytunet API */
  ethcard_count = (int)get_ethcards(ethcards, sizeof(ethcards));

  liststore_adapter = (GtkListStore *)gtk_builder_get_object
    (builder, "liststore_adapter");
  
  /* Add the ethercards to the combobox */
  for (i = 0; i < ethcard_count; i++)
    {
      gtk_list_store_append(liststore_adapter, &treeiter);
      gtk_list_store_set(liststore_adapter, &treeiter, 0, ethcards[i].desc, -1);
    }

  /* Render */
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox_adapter),
                             renderer,
                             TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combobox_adapter),
                                 renderer,
                                 "text",
                                 0,
                                 NULL);
  
  /* Set the first adapter to be active, usually is lo */
  if (ethcard_count != 0)
    {
      gtk_combo_box_set_active(combobox_adapter, 0);
    }

  /*
   * 2. Init entry_username and entry_password
   */
  gtk_entry_set_text(entry_username, user_config.szUsername);
  if (save_password)
    {
      gtk_entry_set_text(entry_password, user_config.szMD5Password);
    }
  
  /*
   * 3. Init combobox_limitation
   */
  liststore_limitation = (GtkListStore *)gtk_builder_get_object
    (builder, "liststore_limitation");
  gtk_list_store_append(liststore_limitation, &treeiter);
  gtk_list_store_set(liststore_limitation, &treeiter, 0, "Campus", -1);
  gtk_list_store_append(liststore_limitation, &treeiter);
  gtk_list_store_set(liststore_limitation, &treeiter, 0, "Domestic", -1);
  gtk_list_store_append(liststore_limitation, &treeiter);
  gtk_list_store_set(liststore_limitation, &treeiter, 0, "None", -1);

  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox_limitation),
                             renderer,
                             TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combobox_limitation),
                                 renderer,
                                 "text",
                                 0,
                                 NULL);
  
  gtk_combo_box_set_active(combobox_limitation, 1);
  
  /*
   * 4. Init checkbutton_savepassword
   */
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_savepassword),
                               save_password);

  /*
   * 5. Init tray_icon and tray_menu
   */
  tray_icon = gtunet_create_tray_icon();
  tray_menu = gtunet_create_tray_menu();

  /*
   * 6. Init image_status
   */
  gtunet_set_status_from_pixbuf(pixbuf_status_none);

  /*
   * 7. Init textview_info
   */
  textmark_info = gtk_text_buffer_get_insert(textbuffer_info);
  
  /*
   * 8. Set window default icon
   */
  gtk_window_set_default_icon(pixbuf_gtunet);
  gtk_about_dialog_set_logo(about_dialog, pixbuf_gtunet);
}

/*
 * gtunet_connect_signals
 *
 * Connect all the signals with callback functions needed for the components of
 * the GUI. (with some events included)
 */
void
gtunet_connect_signals ()
{
  /*
   * 1. Connect the signals with gtk builder. (This includes all the signals on
   *    buttons and entries.)
   */
  gtk_builder_connect_signals(builder, NULL);

  /*
   * 2. Connect the events on window and about_dialog
   */
  g_signal_connect(G_OBJECT(window),
                   "delete_event",
                   G_CALLBACK(window_delete_event),
                   NULL);
  g_signal_connect(G_OBJECT(about_dialog),
                   "delete_event",
                   G_CALLBACK(about_dialog_delete_event),
                   NULL);

  /*
   * 3. Connect the signals on tray_icon
   */
  g_signal_connect(G_OBJECT(tray_icon),
                   "activate",
                   G_CALLBACK(on_tray_icon_clicked),
                   window);
  g_signal_connect(G_OBJECT(tray_icon),
                   "popup-menu",
                   G_CALLBACK(on_tray_icon_menu),
                   tray_menu);
}

/*
 * gtunet_create_tray_icon
 *
 * Create a tray icon on the system tray to represent current status
 */
GtkStatusIcon *
gtunet_create_tray_icon ()
{
  GtkStatusIcon *tray_icon;

  tray_icon = gtk_status_icon_new();
  gtk_status_icon_set_tooltip(tray_icon, "gTunet");

  return tray_icon;
}

/*
 * gtunet_create_tray_menu
 *
 * Create the menu on the tray icon
 */
GtkMenu *
gtunet_create_tray_menu ()
{
  GtkWidget *tray_menu;
  GtkWidget *tray_menu_quit;

  tray_menu_quit = gtk_menu_item_new_with_label("Quit");

  g_signal_connect(G_OBJECT(tray_menu_quit),
                   "activate",
                   G_CALLBACK (on_button_quit_clicked),
                   NULL);
  
  tray_menu = gtk_menu_new();
  
  gtk_menu_shell_append((GtkMenuShell *)tray_menu, tray_menu_quit);
  gtk_widget_show(tray_menu_quit);

  return (GtkMenu *)tray_menu;
}

/*
 * gtunet_load_user_config
 */
void
gtunet_load_user_config ()
{
  mytunetsvc_get_user_config(&user_config);
  save_password = setting_read_int(NULL, "savepassword", 0);
}

/*
 * gtunet_init_log
 *
 * Initiallize the log components of the program, which is used to update
 * the status of TUNET connection
 */
void
gtunet_init_log ()
{
  /*
   * 1. Set mytunet transmit log function
   */
  mytunetsvc_set_transmit_log_func(MYTUNETSVC_TRANSMIT_LOG_QT);

  /*
   * 2. Initiallize log_list and the mutex and cond
   */
  log_list = NULL;
  if (pthread_mutex_init(&log_list_mutex, NULL))
    {
      puts("Initial mutex failed.");
      return;
    }
  if (pthread_cond_init(&log_list_cond, NULL))
    {
      puts("Initial cond failed.");
      return;
    }

  /*
   * 3. Start log process thread
   */
  puts("Start log process thread ...");
  if (pthread_create(&gtunet_log_process_tid,
                     NULL,
                     gtunet_log_process_thread,
                     NULL))
    {
      puts("Log process thread creation error.");
    }
}
