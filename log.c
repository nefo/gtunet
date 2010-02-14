/*
 * log.c
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

#include "mytunet/logs.h"

#include "gtunet.h"
#include "utils.h"
#include "log.h"

/*
 * Since mytunet doesn't have a log transmitting function for gtunet,
 * We simply use the function for qtunet.
 */
VOID mytunetsvc_transmit_log_qt (LOG *log)
{
  GtunetLog *glog;
  
  glog = gtunet_create_log_item (log);
  if (glog)
    {
      gtunet_add_to_log_list (glog);
    }
}

