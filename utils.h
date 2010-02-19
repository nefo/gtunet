/*
 * utils.h
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

void gtunet_load_pixbuf ();
void gtunet_set_status_from_pixbuf (GdkPixbuf *pixbuf);
gboolean gtunet_user_config_is_valid ();
void gtunet_textview_info_append (char *tag, char *prefix, char *info);
void gtunet_clear_money_label ();
GtunetLog *gtunet_create_log_item (LOG *log);
void gtunet_add_to_log_list (GtunetLog *gtunet_log);
void gtunet_process_log ();
void gtunet_destroy_log_list ();
void gtunet_update_state_icon (int state);
