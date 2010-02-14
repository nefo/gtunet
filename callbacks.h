/*
 * callbacks.h
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

void on_entry_password_changed (GtkWidget *widget, gpointer data);
void on_button_login_clicked   (GtkWidget *widget, gpointer data);
void on_button_logout_clicked  (GtkWidget *widget, gpointer data);
void on_button_about_clicked   (GtkWidget *widget, gpointer data);
void on_button_help_clicked    (GtkWidget *widget, gpointer data);
void on_button_quit_clicked    (GtkWidget *widget, gpointer data);

void on_tray_icon_clicked (GtkStatusIcon *status_icon,
                           gpointer window);
void on_tray_icon_menu (GtkStatusIcon *status_icon,
                        guint button,
                        guint activate_time,
                        gpointer popup_menu);

gboolean window_delete_event (GtkWidget *window,
                              GdkEvent *event,
                              gpointer data);

gboolean about_dialog_delete_event (GtkWidget *about_dialog,
                                    GdkEvent *event,
                                    gpointer data);
void on_aboutdialog_close (GtkWidget *widget, gpointer data);
void on_aboutdialog_response (GtkWidget *widget,
                              gint response_id,
                              gpointer data);
void on_checkbutton_savepassword_toggled (GtkToggleButton *button,
                                          gpointer data);

