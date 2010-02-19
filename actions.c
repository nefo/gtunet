/* 
 * actions.c
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

#include <string.h>
#include <pthread.h>

#include "mytunet/mytunetsvc.h"
#include "mytunet/tunet.h"
#include "mytunet/dot1x.h"
#include "mytunet/ethcard.h"
#include "mytunet/setting.h"
#include "mytunet/logs.h"

#include "gtunet.h"
#include "actions.h"
#include "utils.h"
#include "log.h"

void
gtunet_start_mytunet_service ()
{
  mytunetsvc_clear_stop_flag();
  mytunetsvc_set_global_config_from(&user_config);
  
  puts("Start mytunet service thread ...");
  
  if (pthread_create(&mytunet_service_tid,
                     NULL,
                     gtunet_mytunet_service_thread,
                     NULL))
    {
      puts("Service thread creation error.");
    }
}

void
gtunet_stop_mytunet_service ()
{
  pthread_mutex_lock(&log_list_mutex);
  
  mytunetsvc_set_stop_flag();

  tunet_reset();
  dot1x_reset();
  
  if (mytunet_service_tid)
    {
      puts("Stop mytunet service thread ...");
      while (pthread_cancel(mytunet_service_tid))
        {
          puts("Failed to cancel mytunet service thread.");
        }
      mytunet_service_tid = 0;
    }
  
  if (log_list)
    {
      gtunet_destroy_log_list();
    }
  
  gtunet_clear_money_label();
  gtunet_set_status_from_pixbuf(pixbuf_status_none);
  
  pthread_mutex_unlock(&log_list_mutex);
}

void
gtunet_cleanup_mytunet_service ()
{
  mytunetsvc_cleanup();
}

void
gtunet_cancel_log_thread ()
{
  pthread_mutex_lock(&log_list_mutex);

  if (gtunet_log_process_tid)
    {
      puts("Stop log thread ...");
      while (pthread_cancel(gtunet_log_process_tid))
        {
          puts("Failed to cancel log thread.");
        }
    }
  
  pthread_mutex_unlock(&log_list_mutex);
}

void
gtunet_set_mytunet_user_config()
{
  /*
   * 1. Set ethercard
   */
  int eth_i;

  eth_i = gtk_combo_box_get_active(combobox_adapter);
  userconfig_set_adapter(&user_config, ethcards[eth_i].desc);
  gtunet_textview_info_append
    ("[CONFIG]", "Ethercard:", ethcards[eth_i].desc);
  gtunet_textview_info_append
    ("[CONFIG]", "MAC:", ethcards[eth_i].mac);
  gtunet_textview_info_append
    ("[CONFIG]", "IP:", ethcards[eth_i].ip);
  
  /*
   * 2. Set username
   */
  char *username;

  username = (char *)gtk_entry_get_text(entry_username);
  userconfig_set_username(&user_config, username);
  gtunet_textview_info_append
    ("[CONFIG]", "Username:", username);
  
  /*
   * 3. Set password
   */
  password_changed_by_user = FALSE;
  gtk_entry_set_text(entry_password, user_config.szMD5Password);
  password_changed_by_user = TRUE;
  
  /*
   * 4. Set limitation and dot1x, since dot1x is no longer supportted by
   *    TUNET, we simply set it to FALSE
   */
  int limitation_i;
  int limitation;

  limitation_i = gtk_combo_box_get_active(combobox_limitation);
  switch (limitation_i)
    {
    case 0:
      {
        limitation = LIMITATION_CAMPUS;
        gtunet_textview_info_append
          ("[CONFIG]", "Limitation:", "CAMPUS");
        break;
      }
    case 1:
      {
        limitation = LIMITATION_DOMESTIC;
        gtunet_textview_info_append
          ("[CONFIG]", "Limitation:", "DOMESTIC");
        break;
      }
    case 2:
      {
        limitation = LIMITATION_NONE;
        gtunet_textview_info_append
          ("[CONFIG]", "Limitation:", "INTERNATIONAL(NONE)");
        break;
      }
    default:
      {
        limitation = LIMITATION_DOMESTIC;
        gtunet_textview_info_append
          ("[CONFIG]", "Limitation:", "DOMESTIC");
      }
    }
  userconfig_set_limitation(&user_config, limitation);
  userconfig_set_dot1x(&user_config, FALSE, FALSE);

  /*
   * Save configuration to the config file
   */
  gtunet_save_user_config();
}

void
gtunet_save_user_config ()
{
  mytunetsvc_set_user_config_dot1x(user_config.bUseDot1x, FALSE);

  if (save_password)
    {
      mytunetsvc_set_user_config(user_config.szUsername,
                                 user_config.szMD5Password,
                                 TRUE,
                                 user_config.szAdaptor,
                                 user_config.limitation,
                                 0);
    }
  else
    {
      mytunetsvc_set_user_config(user_config.szUsername,
                                 "",
                                 TRUE,
                                 user_config.szAdaptor,
                                 user_config.limitation,
                                 0);
    }

  setting_write_int(NULL, "savepassword", save_password);
}

void
gtunet_set_changed_password ()
{
  char *password;
  
  password = (char *)gtk_entry_get_text(entry_password);
  userconfig_set_password(&user_config, password);
}

void *
gtunet_mytunet_service_thread(void *data)
{
  mytunetsvc_main(0, NULL);
  return NULL;
}

void *
gtunet_log_process_thread(void *data)
{
  while (1)
    {
      pthread_mutex_lock(&log_list_mutex);
      gtunet_process_log();
      pthread_mutex_unlock(&log_list_mutex);
    }

  return NULL;
}
