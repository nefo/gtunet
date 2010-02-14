/*
 * gtunet.h
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

#include <pthread.h>

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "mytunet/ethcard.h"
#include "mytunet/userconfig.h"

GtkBuilder *builder;
GtkWindow *window;
GtkAboutDialog *about_dialog;
GtkComboBox *combobox_adapter;
GtkEntry *entry_username;
GtkEntry *entry_password;
GtkComboBox *combobox_limitation;
GtkImage *image_status;
GtkCheckButton *checkbutton_savepassword;
GtkButton *button_login;
GtkButton *button_logout;
GtkButton *button_about;
GtkButton *button_help;
GtkButton *button_quit;
GtkTextView *textview_info;
GtkLabel *label_account_money;
GtkLabel *label_used_money;
GtkStatusIcon *tray_icon;
GtkMenu *tray_menu;
GtkTextBuffer *textbuffer_info;
GtkTextMark *textmark_info;

GdkPixbuf *pixbuf_gtunet;
GdkPixbuf *pixbuf_status_busy;
GdkPixbuf *pixbuf_status_campus;
GdkPixbuf *pixbuf_status_domestic;
GdkPixbuf *pixbuf_status_dot1x;
GdkPixbuf *pixbuf_status_nolimit;
GdkPixbuf *pixbuf_status_none;

ETHCARD_INFO ethcards[16];
int ethcard_count;
USERCONFIG user_config;

pthread_t mytunet_service_tid;
pthread_t gtunet_log_process_tid;

GList *log_list;
pthread_mutex_t log_list_mutex;
pthread_cond_t log_list_cond;

gboolean save_password;
gboolean password_changed_by_user;

typedef struct _gtunet_log_item
{
  char *tag;
  char *str;
  char *data;
} GtunetLog;
