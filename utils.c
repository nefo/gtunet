/*
 * utils.c
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
#include <gdk-pixbuf/gdk-pixdata.h>

#include "mytunet/userconfig.h"
#include "mytunet/logs.h"
#include "mytunet/util.h"

#include "gtunet.h"
#include "pixdata.h"
#include "utils.h"
#include "log.h"

void
gtunet_load_pixbuf ()
{
  g_type_init();
  
  pixbuf_gtunet = gdk_pixbuf_from_pixdata
    (&pixdata_gtunet, FALSE, NULL);
  pixbuf_status_busy = gdk_pixbuf_from_pixdata
    (&pixdata_status_busy, FALSE, NULL);
  pixbuf_status_campus = gdk_pixbuf_from_pixdata
    (&pixdata_status_campus, FALSE, NULL);
  pixbuf_status_domestic = gdk_pixbuf_from_pixdata
    (&pixdata_status_domestic, FALSE, NULL);
  pixbuf_status_dot1x = gdk_pixbuf_from_pixdata
    (&pixdata_status_dot1x, FALSE, NULL);
  pixbuf_status_nolimit = gdk_pixbuf_from_pixdata
    (&pixdata_status_nolimit, FALSE, NULL);
  pixbuf_status_none = gdk_pixbuf_from_pixdata
    (&pixdata_status_none, FALSE, NULL);
}

void
gtunet_set_status_from_pixbuf (GdkPixbuf *pixbuf)
{
  gtk_status_icon_set_from_pixbuf(tray_icon, pixbuf);
  gtk_image_set_from_pixbuf(image_status, pixbuf);
}

gboolean
gtunet_user_config_is_valid ()
{
  int username_length;
  int password_length;
  int ethercard_name_length;
  int eth_i;
  
  username_length = gtk_entry_get_text_length(entry_username);
  password_length = gtk_entry_get_text_length(entry_password);
  eth_i = gtk_combo_box_get_active(combobox_adapter);
  ethercard_name_length = strlen(ethcards[eth_i].desc);
  
  if ((username_length > (sizeof(user_config.szUsername) - 1)) ||
      (username_length == 0) ||
      (password_length == 0) ||
      (ethercard_name_length > (sizeof(user_config.szAdaptor) - 1)))
    {
      return FALSE;
    }
  
  return TRUE;
}

void
gtunet_textview_info_append (char *tag, char *prefix, char *info)
{
  gtk_text_buffer_insert_at_cursor(textbuffer_info, tag, strlen(tag));
  gtk_text_buffer_insert_at_cursor(textbuffer_info, " ", 1);
  gtk_text_buffer_insert_at_cursor(textbuffer_info, prefix, strlen(prefix));
  gtk_text_buffer_insert_at_cursor(textbuffer_info, " ", 1);
  gtk_text_buffer_insert_at_cursor(textbuffer_info, info, strlen(info));
  gtk_text_buffer_insert_at_cursor(textbuffer_info, "\n", 1);
  gtk_text_view_scroll_to_mark(textview_info, textmark_info,
                               0, FALSE, 0, 0);
}

void
gtunet_clear_money_label ()
{
  gtk_label_set_text(label_account_money, "");
  gtk_label_set_text(label_used_money, "");
}

GtunetLog *
gtunet_create_log_item (LOG *log)
{
  GtunetLog *glog;
  char *buf;
  
  if (log->datalen == 0)
    {
      return NULL;
    }

  glog = (GtunetLog *)malloc(sizeof(GtunetLog));
  
  buf = (char *)malloc(strlen(log->tag) + 1);
  glog->tag = strcpy (buf, log->tag);
  
  if (strcmp(glog->tag, "MYTUNETSVC_LIMITATION") &&
      strcmp(glog->tag, "MYTUNETSVC_STATE"))
    {
      buf = (char *)malloc(strlen(log->str) + 1);
      glog->str = strcpy (buf, log->str);
    }
  else
    {
      glog->str = NULL;
    }
  
  buf = (char *)malloc(log->datalen * 3 + 3);
  buf2hex(log->data, log->datalen, buf);
  glog->data = buf;
  
  return glog;    
}

void
gtunet_add_to_log_list (GtunetLog *glog)
{
  pthread_mutex_lock(&log_list_mutex);
  
  log_list = g_list_append(log_list, glog);
  pthread_cond_signal(&log_list_cond);

  pthread_mutex_unlock(&log_list_mutex);
}

void
gtunet_process_log ()
{
  GList *head;
  GtunetLog *head_log;
  
  pthread_mutex_lock(&log_list_mutex);
  
  while (log_list == NULL)
    {
      pthread_cond_wait(&log_list_cond, &log_list_mutex);
    }
  
  /*
   * Process the log here
   */
  head = log_list;
  head_log = (GtunetLog *)(head->data);
  log_list = log_list->next;

  /*
   * TODO: Add code here
   *  change the status and print the info based on head
   */
  if (strcmp(head_log->tag, "MYTUNETSVC_LIMITATION") &&
      strcmp(head_log->tag, "MYTUNETSVC_STATE"))
    {
      printf("gtunet: %s %s %s\n",
             head_log->tag, head_log->data, head_log->str);
    }
  else
    {
      printf("gtunet: %s\n", head_log->tag);
    }

  free(head_log->tag);
  if (head_log->str)
    {
      free(head_log->str);
    }
  free(head_log->data);
  free(head_log);
  g_list_free_1(head);
  
  pthread_mutex_unlock(&log_list_mutex);
}

void
gtunet_destroy_log_list ()
{
  GList *current;
  GtunetLog *cur_log;

  pthread_mutex_lock(&log_list_mutex);
  
  current = log_list;
  while (current)
    {
      cur_log = (GtunetLog *)current->data;

      free(cur_log->tag);
      if (cur_log->str)
        {
          free(cur_log->str);
        }
      free(cur_log->data);
      free(cur_log);
      
      current = current->next;
    }

  g_list_free(log_list);
  log_list = NULL;

  pthread_mutex_unlock(&log_list_mutex);
}
