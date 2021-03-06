/* Oracle Redo OpCode: 18.1
   Copyright (C) 2018-2020 Adam Leszczynski.

This file is part of Open Log Replicator.

Open Log Replicator is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as published
by the Free Software Foundation; either version 3, or (at your option)
any later version.

Open Log Replicator is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License
along with Open Log Replicator; see the file LICENSE.txt  If not see
<http://www.gnu.org/licenses/>.  */

#include <iostream>
#include <iomanip>
#include "OpCode1801.h"
#include "OracleColumn.h"
#include "OracleObject.h"
#include "OracleReader.h"
#include "RedoLogRecord.h"

using namespace std;

namespace OpenLogReplicator {

    OpCode1801::OpCode1801(OracleReader *oracleReader, RedoLogRecord *redoLogRecord) :
            OpCode(oracleReader, redoLogRecord),
            validDDL(false),
            type(0) {
    }

    OpCode1801::~OpCode1801() {
    }

    void OpCode1801::process() {
        OpCode::process();
        uint64_t fieldPos = redoLogRecord->fieldPos;

        for (uint64_t i = 1; i <= redoLogRecord->fieldCnt; ++i) {
            if (i == 1) {
                redoLogRecord->xid = XID(oracleReader->read16(redoLogRecord->data + fieldPos + 4),
                        oracleReader->read16(redoLogRecord->data + fieldPos + 6),
                        oracleReader->read32(redoLogRecord->data + fieldPos + 8));
                type = oracleReader->read16(redoLogRecord->data + fieldPos + 12);
                uint16_t tmp = oracleReader->read16(redoLogRecord->data + fieldPos + 16);
                //uint16_t seq = oracleReader->read16(redoLogRecord->data + fieldPos + 18);
                //uint16_t cnt = oracleReader->read16(redoLogRecord->data + fieldPos + 20);
                if (type == 85 // truncate table
                        //|| type == 1 //create table
                        || type == 12 // drop table
                        || type == 15 // alter table
                        || type == 86 // truncate partition
                )
                    validDDL = true;

                //temporary object
                if (tmp == 4 || tmp == 5 || tmp == 6 || tmp == 8 || tmp == 9 || tmp == 10) {
                    validDDL = false;
                }
            } else if (i == 12) {
                if (validDDL && redoLogRecord->scn > oracleReader->databaseScn) {
                    redoLogRecord->objn = oracleReader->read32(redoLogRecord->data + fieldPos + 0);
                    if (type == 12 || type == 15) {
                        OracleObject *obj = oracleReader->checkDict(redoLogRecord->objn, 0);
                        if (obj != nullptr)
                            obj->altered = true;
                    }
                }
            }

            fieldPos += (oracleReader->read16(redoLogRecord->data + redoLogRecord->fieldLengthsDelta + i * 2) + 3) & 0xFFFC;
        }
    }
}
