/*
 * callbacks.c
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
#include <pthread.h>
#include <gtk/gtk.h>

#include "mytunet/logs.h"

#include "gtunet.h"
#include "actions.h"
#include "utils.h"

void
on_entry_password_changed (GtkWidget *widget, gpointer data)
{
  if (password_changed_by_user)
    {
      gtunet_set_changed_password();
    }
}

void
on_button_login_clicked (GtkWidget *widget, gpointer data)
{
  pthread_mutex_lock(&log_list_mutex);
  /*
   * If the thread is running, just return
   */
  if (mytunet_service_tid)
    {
      return;
    }
  
  /*
   * 1. Set the current confiuration.
   */
  if (gtunet_user_config_is_valid())
    {
      gtunet_set_mytunet_user_config();
    }
  else
    {
      puts("User configuration invalid.");
      return;
    }
  
  /*
   * 2. Start mytunet service.
   */
  gtunet_start_mytunet_service();
  
  pthread_mutex_unlock(&log_list_mutex);
}

void
on_button_logout_clicked (GtkWidget *widget, gpointer data)
{ 
  gtunet_stop_mytunet_service();
}
  
void
on_button_about_clicked (GtkWidget *widget, gpointer data)
{
  gtk_widget_show((GtkWidget *)about_dialog);
}

void
on_button_help_clicked (GtkWidget *widget, gpointer data)
{
  /* TODO: Add help contents display function */
  ;
}

void
on_button_quit_clicked (GtkWidget *widget, gpointer data)
{
  puts("gTunet is exiting...");
  
  gtunet_save_user_config();
  gtunet_stop_mytunet_service();
  gtunet_cleanup_mytunet_service();
  gtunet_cancel_log_thread();
  
  gtk_main_quit();
}

void
on_tray_icon_clicked (GtkStatusIcon *status_icon,
                      gpointer gp_window)
{
  GtkWidget *window;
  window = (GtkWidget *)gp_window;

  if (GTK_WIDGET_VISIBLE(window))
    {
      gtk_widget_hide(window);
    }
  else
    {
      gtk_widget_show(window);
    }
}

void
on_tray_icon_menu (GtkStatusIcon *status_icon,
                   guint button,
                   guint activate_time,
                   gpointer popup_menu)
{
  gtk_menu_popup(popup_menu,
                 NULL,
                 NULL,
                 gtk_status_icon_position_menu,
                 status_icon,
                 button,
                 activate_time);
}

gboolean
window_delete_event (GtkWidget *window,
                     GdkEvent *event,
                     gpointer data)
{
  gtk_widget_hide((GtkWidget *)window);
  return TRUE;
}

gboolean
about_dialog_delete_event (GtkWidget *about_dialog,
                           GdkEvent *event,
                           gpointer data)
{
  gtk_widget_hide((GtkWidget *)about_dialog);
  return TRUE;
}

void
on_aboutdialog_close (GtkWidget *widget, gpointer data)
{
  gtk_widget_hide((GtkWidget *)about_dialog);
}

void
on_aboutdialog_response (GtkWidget *widget,
                         gint response_id,
                         gpointer data)
{
  gtk_widget_hide((GtkWidget *)about_dialog);
}

void
on_checkbutton_savepassword_toggled (GtkToggleButton *button, gpointer data)
{
  save_password = gtk_toggle_button_get_active
    (GTK_TOGGLE_BUTTON (checkbutton_savepassword));
}
