/** -*- c++ -*-
 * Copyright (C) 2008 Doug Judd (Zvents, Inc.)
 *
 * This file is part of Hypertable.
 *
 * Hypertable is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2 of the
 * License, or any later version.
 *
 * Hypertable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "Common/Compat.h"
#include "Common/Error.h"
#include "Common/Logger.h"

#include "AsyncComm/ResponseCallback.h"
#include "Common/Serialization.h"

#include "Hypertable/Lib/Types.h"

#include "RangeServer.h"
#include "RequestHandlerCompact.h"

using namespace Hypertable;
using namespace Serialization;

/**
 *
 */
void RequestHandlerCompact::run() {
  ResponseCallback cb(m_comm, m_event_ptr);
  TableIdentifier table;
  RangeSpec range;
  uint8_t compaction_type = 0;
  size_t remaining = m_event_ptr->message_len - 2;
  const uint8_t *msg = m_event_ptr->message + 2;

  try {
    table.decode(&msg, &remaining);
    range.decode(&msg, &remaining);
    compaction_type = decode_i8(&msg, &remaining);
    m_range_server->compact(&cb, &table, &range, compaction_type);
  }
  catch (Exception &e) {
    HT_ERROR_OUT << e << HT_END;
    cb.error(Error::PROTOCOL_ERROR, "Error handling compact message");
  }
}
