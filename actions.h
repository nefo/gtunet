/*
 * actions.h
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

void gtunet_start_mytunet_service ();
void gtunet_stop_mytunet_service ();
void gtunet_cleanup_mytunet_service ();
void gtunet_cancel_log_thread ();
void gtunet_set_mytunet_user_config ();
void gtunet_save_user_config ();
void gtunet_set_changed_password ();
void *gtunet_mytunet_service_thread (void *data);
void *gtunet_log_process_thread (void *data);
