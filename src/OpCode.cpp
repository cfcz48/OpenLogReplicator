/* Oracle Redo Generic OpCode
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
#include <string.h>
#include "OpCode.h"
#include "OracleReader.h"
#include "RedoLogRecord.h"

using namespace std;

namespace OpenLogReplicator {

    OpCode::OpCode(OracleReader *oracleReader, RedoLogRecord *redoLogRecord):
            oracleReader(oracleReader),
            redoLogRecord(redoLogRecord) {
    }

    OpCode::~OpCode() {
    }

    typeop1 OpCode::getOpCode(void) {
        return redoLogRecord->opCode;
    }

    void OpCode::process() {
        if (oracleReader->dumpRedoLog >= 1) {
            bool encrypted = false;
            if ((redoLogRecord->typ & 0x80) != 0)
                encrypted = true;

            if (oracleReader->version < 0x12000) {
                if (redoLogRecord->typ == 6)
                    oracleReader->dumpStream << "CHANGE #" << dec << (uint64_t)redoLogRecord->vectorNo <<
                        " MEDIA RECOVERY MARKER" <<
                        " SCN:" << PRINTSCN48(redoLogRecord->scnRecord) <<
                        " SEQ:" << dec << (uint64_t)redoLogRecord->seq <<
                        " OP:" << (uint64_t)(redoLogRecord->opCode >> 8) << "." << (uint64_t)(redoLogRecord->opCode & 0xFF) <<
                        " ENC:" << dec << (uint64_t)encrypted << endl;
                else
                    oracleReader->dumpStream << "CHANGE #" << dec << (uint64_t)redoLogRecord->vectorNo <<
                        " TYP:" << (uint64_t)redoLogRecord->typ <<
                        " CLS:" << redoLogRecord->cls <<
                        " AFN:" << redoLogRecord->afn <<
                        " DBA:0x" << setfill('0') << setw(8) << hex << redoLogRecord->dba <<
                        " OBJ:" << dec << redoLogRecord->recordObjd <<
                        " SCN:" << PRINTSCN48(redoLogRecord->scnRecord) <<
                        " SEQ:" << dec << (uint64_t)redoLogRecord->seq <<
                        " OP:" << (uint64_t)(redoLogRecord->opCode >> 8) << "." << (uint64_t)(redoLogRecord->opCode & 0xFF) <<
                        " ENC:" << dec << (uint64_t)encrypted <<
                        " RBL:" << dec << redoLogRecord->rbl << endl;
            } else if (oracleReader->version < 0x12200) {
                if (redoLogRecord->typ == 6)
                    oracleReader->dumpStream << "CHANGE #" << dec << (uint64_t)redoLogRecord->vectorNo <<
                        " MEDIA RECOVERY MARKER" <<
                        " CON_ID:" << redoLogRecord->conId <<
                        " SCN:" << PRINTSCN48(redoLogRecord->scnRecord) <<
                        " SEQ:" << dec << (uint64_t)redoLogRecord->seq <<
                        " OP:" << (uint64_t)(redoLogRecord->opCode >> 8) << "." << (uint64_t)(redoLogRecord->opCode & 0xFF) <<
                        " ENC:" << dec << (uint64_t)encrypted <<
                        " FLG:0x" << setw(4) << hex << redoLogRecord->flgRecord << endl;
                else
                    oracleReader->dumpStream << "CHANGE #" << dec << (uint64_t)redoLogRecord->vectorNo <<
                        " CON_ID:" << redoLogRecord->conId <<
                        " TYP:" << (uint64_t)redoLogRecord->typ <<
                        " CLS:" << redoLogRecord->cls <<
                        " AFN:" << redoLogRecord->afn <<
                        " DBA:0x" << setfill('0') << setw(8) << hex << redoLogRecord->dba <<
                        " OBJ:" << dec << redoLogRecord->recordObjd <<
                        " SCN:" << PRINTSCN48(redoLogRecord->scnRecord) <<
                        " SEQ:" << dec << (uint64_t)redoLogRecord->seq <<
                        " OP:" << (uint64_t)(redoLogRecord->opCode >> 8) << "." << (uint64_t)(redoLogRecord->opCode & 0xFF) <<
                        " ENC:" << dec << (uint64_t)encrypted <<
                        " RBL:" << dec << redoLogRecord->rbl <<
                        " FLG:0x" << setw(4) << hex << redoLogRecord->flgRecord << endl;
            } else {
                if (redoLogRecord->typ == 6)
                    oracleReader->dumpStream << "CHANGE #" << dec << (uint64_t)redoLogRecord->vectorNo <<
                        " MEDIA RECOVERY MARKER" <<
                        " CON_ID:" << redoLogRecord->conId <<
                        " SCN:" << PRINTSCN64(redoLogRecord->scnRecord) <<
                        " SEQ:" << dec << (uint64_t)redoLogRecord->seq <<
                        " OP:" << (uint64_t)(redoLogRecord->opCode >> 8) << "." << (uint64_t)(redoLogRecord->opCode & 0xFF) <<
                        " ENC:" << dec << (uint64_t)encrypted <<
                        " FLG:0x" << setw(4) << hex << redoLogRecord->flgRecord << endl;
                else
                    oracleReader->dumpStream << "CHANGE #" << dec << (uint64_t)redoLogRecord->vectorNo <<
                        " CON_ID:" << redoLogRecord->conId <<
                        " TYP:" << (uint64_t)redoLogRecord->typ <<
                        " CLS:" << redoLogRecord->cls <<
                        " AFN:" << redoLogRecord->afn <<
                        " DBA:0x" << setfill('0') << setw(8) << hex << redoLogRecord->dba <<
                        " OBJ:" << dec << redoLogRecord->recordObjd <<
                        " SCN:" << PRINTSCN64(redoLogRecord->scnRecord) <<
                        " SEQ:" << dec << (uint64_t)redoLogRecord->seq <<
                        " OP:" << (uint64_t)(redoLogRecord->opCode >> 8) << "." << (uint64_t)(redoLogRecord->opCode & 0xFF) <<
                        " ENC:" << dec << (uint64_t)encrypted <<
                        " RBL:" << dec << redoLogRecord->rbl <<
                        " FLG:0x" << setw(4) << hex << redoLogRecord->flgRecord << endl;
            }
        }

        if (oracleReader->dumpRawData)
            redoLogRecord->dumpHex(oracleReader->dumpStream, oracleReader);
    }

    void OpCode::ktbRedo(uint64_t fieldPos, uint64_t fieldLength) {
        if (fieldLength < 8) {
            oracleReader->dumpStream << "ERROR: too short field KTB Redo: " << dec << fieldLength << endl;
            return;
        }

        if (redoLogRecord->opc == 0x0A16)
            oracleReader->dumpStream << "index undo for leaf key operations" << endl;
        else if (redoLogRecord->opc == 0x0B01)
            oracleReader->dumpStream << "KDO undo record:" << endl;

        int8_t op = redoLogRecord->data[fieldPos + 0];
        uint8_t flg = redoLogRecord->data[fieldPos + 1];
        if (oracleReader->dumpRedoLog >= 1) {
            uint8_t ver = flg & 0x03;
            uint64_t padding = 1;

            oracleReader->dumpStream << "KTB Redo " << endl;
            oracleReader->dumpStream << "op: 0x" << setfill('0') << setw(2) << hex << (int32_t)op << " " <<
                    " ver: 0x" << setfill('0') << setw(2) << hex << (uint64_t)ver << "  " << endl;
            oracleReader->dumpStream << "compat bit: " << dec << (uint64_t)(flg & 0x04) << " ";
            if ((flg & 0x04) != 0)
                oracleReader->dumpStream << "(post-11)";
            else
                oracleReader->dumpStream << "(pre-11)";
            oracleReader->dumpStream << " padding: " << padding << endl;
        }
        char opCode = '?';

        if ((op & 0x0F) == KTBOP_C) {
            if (fieldLength < 16) {
                oracleReader->dumpStream << "ERROR: too short field KTB Redo C: " << dec << fieldLength << endl;
                return;
            }

            opCode = 'C';
            redoLogRecord->uba = oracleReader->read56(redoLogRecord->data + fieldPos + 8);
            if (oracleReader->dumpRedoLog >= 1) {
                oracleReader->dumpStream << "op: " << opCode << " " << " uba: " << PRINTUBA(redoLogRecord->uba) << endl;
            }
        } else if ((op & 0x0F) == KTBOP_Z) {
            opCode = 'Z';
            if (oracleReader->dumpRedoLog >= 1) {
                oracleReader->dumpStream << "op: " << opCode << endl;
            }
        } else if ((op & 0x0F) == KTBOP_L) {
            opCode = 'L';
            if ((flg & 0x08) == 0) {
                if (fieldLength < 28) {
                    oracleReader->dumpStream << "ERROR: too short field KTB Redo L: " << dec << fieldLength << endl;
                    return;
                }
                redoLogRecord->uba = oracleReader->read56(redoLogRecord->data + fieldPos + 12);

                if (oracleReader->dumpRedoLog >= 1) {
                    typexid itlXid = XID(oracleReader->read16(redoLogRecord->data + fieldPos + 4),
                            oracleReader->read16(redoLogRecord->data + fieldPos + 6),
                            oracleReader->read32(redoLogRecord->data + fieldPos + 8));

                    oracleReader->dumpStream << "op: " << opCode << " " <<
                            " itl:" <<
                            " xid:  " << PRINTXID(itlXid) <<
                            " uba: " << PRINTUBA(redoLogRecord->uba) << endl;

                    uint8_t lkc = oracleReader->read16(redoLogRecord->data + fieldPos + 20);
                    uint8_t flag = redoLogRecord->data[fieldPos + 19];
                    char flagStr[5] = "----";
                    if ((flag & 0x80) != 0) flagStr[0] = 'C';
                    if ((flag & 0x40) != 0) flagStr[1] = 'B';
                    if ((flag & 0x20) != 0) flagStr[2] = 'U';
                    if ((flag & 0x10) != 0) flagStr[3] = 'T';
                    typescn scnx = oracleReader->readSCNr(redoLogRecord->data + fieldPos + 26);

                    if (oracleReader->version < 0x12200)
                        oracleReader->dumpStream << "                     " <<
                                " flg: " << flagStr << "   " <<
                                " lkc:  " << (uint64_t)lkc << "    " <<
                                " fac: " << PRINTSCN48(scnx) << endl;
                    else
                        oracleReader->dumpStream << "                     " <<
                                " flg: " << flagStr << "   " <<
                                " lkc:  " << (uint64_t)lkc << "    " <<
                                " fac:  " << PRINTSCN64(scnx) << endl;
                }

            } else {
                if (fieldLength < 32) {
                    oracleReader->dumpStream << "ERROR: too short field KTB Redo: " << dec << fieldLength << endl;
                    return;
                }
                redoLogRecord->uba = oracleReader->read56(redoLogRecord->data + fieldPos + 16);

                if (oracleReader->dumpRedoLog >= 1) {
                    typexid itlXid = XID(oracleReader->read16(redoLogRecord->data + fieldPos + 8),
                            oracleReader->read16(redoLogRecord->data + fieldPos + 10),
                            oracleReader->read32(redoLogRecord->data + fieldPos + 12));

                    oracleReader->dumpStream << "op: " << opCode << " " <<
                            " itl:" <<
                            " xid:  " << PRINTXID(itlXid) <<
                            " uba: " << PRINTUBA(redoLogRecord->uba) << endl;

                    uint8_t lkc = 0;
                    uint8_t flag = redoLogRecord->data[fieldPos + 25];
                    char flagStr[5] = "----";
                    if ((flag & 0x80) != 0) flagStr[0] = 'C';
                    if ((flag & 0x40) != 0) flagStr[1] = 'B';
                    if ((flag & 0x20) != 0) flagStr[2] = 'U';
                    if ((flag & 0x10) != 0) flagStr[3] = 'T';
                    typescn scnx = oracleReader->readSCNr(redoLogRecord->data + fieldPos + 26);

                    if (oracleReader->version < 0x12200)
                        oracleReader->dumpStream << "                     " <<
                                " flg: " << flagStr << "   " <<
                                " lkc:  " << (uint64_t)lkc << "    " <<
                                " scn: " << PRINTSCN48(scnx) << endl;
                    else
                        oracleReader->dumpStream << "                     " <<
                                " flg: " << flagStr << "   " <<
                                " lkc:  " << (uint64_t)lkc << "    " <<
                                " scn:  " << PRINTSCN64(scnx) << endl;
                }
            }

        } else if ((op & 0x0F) == KTBOP_N) {
            opCode = 'N';
            if (oracleReader->dumpRedoLog >= 1) {
                oracleReader->dumpStream << "op: " << opCode << endl;
            }

        } else if ((op & 0x0F) == KTBOP_F) {
            if (fieldLength < 24) {
                oracleReader->dumpStream << "ERROR: too short field KTB Redo F: " << dec << fieldLength << endl;
                return;
            }

            opCode = 'F';
            redoLogRecord->xid = XID(oracleReader->read16(redoLogRecord->data + fieldPos + 8),
                    oracleReader->read16(redoLogRecord->data + fieldPos + 10),
                    oracleReader->read32(redoLogRecord->data + fieldPos + 12));
            redoLogRecord->uba = oracleReader->read56(redoLogRecord->data + fieldPos + 16);

            if (oracleReader->dumpRedoLog >= 1) {

                oracleReader->dumpStream << "op: " << opCode << " " <<
                        " xid:  " << PRINTXID(redoLogRecord->xid) <<
                        "    uba: " << PRINTUBA(redoLogRecord->uba) << endl;
            }
        }

        //block cleanout record
        if ((op & KTBOP_BLOCKCLEANOUT) != 0) {
            if (oracleReader->dumpRedoLog >= 1) {
                typescn scn = oracleReader->readSCN(redoLogRecord->data + fieldPos + 48);
                uint8_t opt = redoLogRecord->data[fieldPos + 44];
                uint8_t ver = redoLogRecord->data[fieldPos + 46];
                uint8_t entries = redoLogRecord->data[fieldPos + 45];

                if (oracleReader->version < 0x12200)
                    oracleReader->dumpStream << "Block cleanout record, scn: " <<
                            " " << PRINTSCN48(scn) <<
                            " ver: 0x" << setfill('0') << setw(2) << hex << (uint64_t)ver <<
                            " opt: 0x" << setfill('0') << setw(2) << hex << (uint64_t)opt <<
                            ", entries follow..." << endl;
                else {
                    char bigscn = 'N', compat = 'N';
                    if ((ver & 0x08) != 0)
                        bigscn = 'Y';
                    if ((ver & 0x04) != 0)
                        compat = 'Y';
                    uint32_t spare = 0; //FIXME
                    ver &= 0x03;
                    oracleReader->dumpStream << "Block cleanout record, scn: " <<
                            " " << PRINTSCN64(scn) <<
                            " ver: 0x" << setfill('0') << setw(2) << hex << (uint64_t)ver <<
                            " opt: 0x" << setfill('0') << setw(2) << hex << (uint64_t)opt <<
                            " bigscn: " << bigscn <<
                            " compact: " << compat <<
                            " spare: " << setfill('0') << setw(8) << hex << spare <<
                            ", entries follow..." << endl;
                }

                if (fieldLength < 56 + entries * (uint64_t)8) {
                    oracleReader->dumpStream << "ERROR: too short field KTB Redo F 0x11: " << dec << fieldLength << endl;
                    return;
                }

                for (uint64_t j = 0; j < entries; ++j) {
                    uint8_t itli = redoLogRecord->data[fieldPos + 56 + j * 8];
                    uint8_t flg = redoLogRecord->data[fieldPos + 57 + j * 8];
                    typescn scn = oracleReader->readSCNr(redoLogRecord->data + fieldPos + 58 + j * 8);
                    if (oracleReader->version < 0x12100)
                        oracleReader->dumpStream << "  itli: " << dec << (uint64_t)itli << " " <<
                                " flg: " << (uint64_t)flg << " " <<
                                " scn: " << PRINTSCN48(scn) << endl;
                    else if (oracleReader->version < 0x12200)
                        oracleReader->dumpStream << "  itli: " << dec << (uint64_t)itli << " " <<
                                " flg: (opt=" << (uint64_t)(flg & 0x03) << " whr=" << (uint64_t)(flg >>2) << ") " <<
                                " scn: " << PRINTSCN48(scn) << endl;
                    else {
                        uint8_t opt = flg & 0x03;
                        uint8_t whr = flg >> 2;
                        oracleReader->dumpStream << "  itli: " << dec << (uint64_t)itli << " " <<
                                " flg: (opt=" << (uint64_t)opt << " whr=" << (uint64_t)whr << ") " <<
                                " scn:  " << PRINTSCN64(scn) << endl;
                    }
                }
            }
        }
    }

    void OpCode::kdoOpCodeIRP(uint64_t fieldPos, uint64_t fieldLength) {
        if (fieldLength < 48) {
            oracleReader->dumpStream << "ERROR: too short field KDO OpCode IRP: " << dec << fieldLength << endl;
            return;
        }

        redoLogRecord->fb = redoLogRecord->data[fieldPos + 16];
        redoLogRecord->cc = redoLogRecord->data[fieldPos + 18];
        redoLogRecord->slot = oracleReader->read16(redoLogRecord->data + fieldPos + 42);
        redoLogRecord->tabn = redoLogRecord->data[fieldPos + 44];

        if ((redoLogRecord->fb & FB_L) == 0) {
            redoLogRecord->nridBdba = oracleReader->read32(redoLogRecord->data + fieldPos + 28);
            redoLogRecord->nridSlot = oracleReader->read16(redoLogRecord->data + fieldPos + 32);
        }

        if (fieldLength < 45 + ((uint64_t)redoLogRecord->cc + 7) / 8) {
            oracleReader->dumpStream << "ERROR: too short field KDO OpCode IRP for nulls: " << dec << fieldLength <<
                    " (cc: " << redoLogRecord->cc << ")" << endl;
            return;
        }
        redoLogRecord->nullsDelta = fieldPos + 45;

        if (oracleReader->dumpRedoLog >= 1) {
            uint16_t sizeDelt = oracleReader->read16(redoLogRecord->data + fieldPos + 40);
            oracleReader->dumpStream << "tabn: " << (uint64_t)redoLogRecord->tabn <<
                    " slot: " << dec << (uint64_t)redoLogRecord->slot << "(0x" << hex << redoLogRecord->slot << ")" <<
                    " size/delt: " << dec << sizeDelt << endl;

            char fbStr[9] = "--------";
            processFbFlags(redoLogRecord->fb, fbStr);
            uint8_t lb = redoLogRecord->data[fieldPos + 17];

            oracleReader->dumpStream << "fb: " << fbStr <<
                    " lb: 0x" << hex << (uint64_t)lb << " " <<
                    " cc: " << dec << (uint64_t)redoLogRecord->cc;
            if (fbStr[1] == 'C') {
                uint8_t cki = redoLogRecord->data[fieldPos + 19];
                oracleReader->dumpStream << " cki: " << dec << (uint64_t)cki << endl;
            } else
                oracleReader->dumpStream << endl;

            if ((redoLogRecord->fb & FB_F) != 0  && (redoLogRecord->fb & FB_H) == 0) {
                typedba hrid1 = oracleReader->read32(redoLogRecord->data + fieldPos + 20);
                typeslot hrid2 = oracleReader->read16(redoLogRecord->data + fieldPos + 24);
                oracleReader->dumpStream << "hrid: 0x" << setfill('0') << setw(8) << hex << hrid1 << "." << hex << hrid2 << endl;
            }

            //next DBA/SLT
            if ((redoLogRecord->fb & FB_L) == 0) {
                oracleReader->dumpStream << "nrid:  0x" << setfill('0') << setw(8) << hex << redoLogRecord->nridBdba << "." << hex << redoLogRecord->nridSlot << endl;
            }

            if ((redoLogRecord->fb & FB_K) != 0) {
                uint8_t curc = 0; //FIXME
                uint8_t comc = 0; //FIXME
                uint32_t pk = oracleReader->read32(redoLogRecord->data + fieldPos + 20);
                uint8_t pk1 = redoLogRecord->data[fieldPos + 24];
                uint32_t nk = oracleReader->read32(redoLogRecord->data + fieldPos + 28);
                uint8_t nk1 = redoLogRecord->data[fieldPos + 32];

                oracleReader->dumpStream << "curc: " << dec << (uint64_t)curc <<
                        " comc: " << dec << (uint64_t)comc <<
                        " pk: 0x" << setfill('0') << setw(8) << hex << pk << "." << hex << (uint64_t)pk1 <<
                        " nk: 0x" << setfill('0') << setw(8) << hex << nk << "." << hex << (uint64_t)nk1 << endl;
            }

            oracleReader->dumpStream << "null:";
            if (redoLogRecord->cc >= 11)
                oracleReader->dumpStream << endl << "01234567890123456789012345678901234567890123456789012345678901234567890123456789" << endl;
            else
                oracleReader->dumpStream << " ";

            uint8_t *nulls = redoLogRecord->data + redoLogRecord->nullsDelta, bits = 1;
            for (uint64_t i = 0; i < redoLogRecord->cc; ++i) {

                if ((*nulls & bits) != 0)
                    oracleReader->dumpStream << "N";
                else
                    oracleReader->dumpStream << "-";
                if ((i % 80) == 79 && i < redoLogRecord->cc)
                    oracleReader->dumpStream << endl;

                bits <<= 1;
                if (bits == 0) {
                    bits = 1;
                    ++nulls;
                }
            }
            oracleReader->dumpStream << endl;
        }
    }

    void OpCode::kdoOpCodeDRP(uint64_t fieldPos, uint64_t fieldLength) {
        if (fieldLength < 20) {
            oracleReader->dumpStream << "ERROR: too short field KDO OpCode DRP: " << dec << fieldLength << endl;
            return;
        }

        redoLogRecord->slot = oracleReader->read16(redoLogRecord->data + fieldPos + 16);
        redoLogRecord->tabn = redoLogRecord->data[fieldPos + 18];

        if (oracleReader->dumpRedoLog >= 1) {
            oracleReader->dumpStream << "tabn: " << (uint64_t)redoLogRecord->tabn <<
                    " slot: " << dec << (uint64_t)redoLogRecord->slot << "(0x" << hex << redoLogRecord->slot << ")" << endl;
        }
    }

    void OpCode::kdoOpCodeLKR(uint64_t fieldPos, uint64_t fieldLength) {
        if (fieldLength < 20) {
            oracleReader->dumpStream << "ERROR: too short field KDO OpCode LKR: " << dec << fieldLength << endl;
            return;
        }

        redoLogRecord->slot = oracleReader->read16(redoLogRecord->data + fieldPos + 16);
        redoLogRecord->tabn = redoLogRecord->data[fieldPos + 18];

        if (oracleReader->dumpRedoLog >= 1) {
            uint8_t to = redoLogRecord->data[fieldPos + 19];
            oracleReader->dumpStream << "tabn: "<< (uint64_t)redoLogRecord->tabn <<
                " slot: " << dec << redoLogRecord->slot <<
                " to: " << dec << (uint64_t)to << endl;
        }
    }

    void OpCode::kdoOpCodeURP(uint64_t fieldPos, uint64_t fieldLength) {
        if (fieldLength < 28) {
            oracleReader->dumpStream << "ERROR: too short field KDO OpCode URP: " << dec << fieldLength << endl;
            return;
        }

        redoLogRecord->fb = redoLogRecord->data[fieldPos + 16];
        redoLogRecord->slot = oracleReader->read16(redoLogRecord->data + fieldPos + 20);
        redoLogRecord->nullsDelta = fieldPos + 26;

        if (fieldLength < 26 + ((uint64_t)redoLogRecord->cc + 7) / 8) {
            oracleReader->dumpStream << "ERROR: too short field KDO OpCode IRP for nulls: " << dec <<
                    fieldLength << " (cc: " << redoLogRecord->cc << ")" << endl;
            return;
        }

        redoLogRecord->tabn = redoLogRecord->data[fieldPos + 19];
        redoLogRecord->cc = redoLogRecord->data[fieldPos + 23];

        if (oracleReader->dumpRedoLog >= 1) {
            uint8_t lock = redoLogRecord->data[fieldPos + 17];
            uint8_t ckix = redoLogRecord->data[fieldPos + 18];
            uint8_t ncol = redoLogRecord->data[fieldPos + 22];
            int16_t size = oracleReader->read16(redoLogRecord->data + fieldPos + 24); //signed

            oracleReader->dumpStream << "tabn: "<< (uint64_t)redoLogRecord->tabn <<
                    " slot: " << dec << redoLogRecord->slot << "(0x" << hex << redoLogRecord->slot << ")" <<
                    " flag: 0x" << setfill('0') << setw(2) << hex << (uint64_t)redoLogRecord->fb <<
                    " lock: " << dec << (uint64_t)lock <<
                    " ckix: " << dec << (uint64_t)ckix << endl;
            oracleReader->dumpStream << "ncol: " << dec << (uint64_t)ncol <<
                    " nnew: " << dec << (uint64_t)redoLogRecord->cc <<
                    " size: " << size << endl;
        }
    }

    void OpCode::kdoOpCodeCFA(uint64_t fieldPos, uint64_t fieldLength) {
        if (fieldLength < 32) {
            oracleReader->dumpStream << "ERROR: too short field KDO OpCode ORP: " << dec << fieldLength << endl;
            return;
        }

        redoLogRecord->nridBdba = oracleReader->read32(redoLogRecord->data + fieldPos + 16);
        redoLogRecord->nridSlot = oracleReader->read16(redoLogRecord->data + fieldPos + 20);
        redoLogRecord->slot = oracleReader->read16(redoLogRecord->data + fieldPos + 24);
        redoLogRecord->tabn = redoLogRecord->data[fieldPos + 27];

        if (oracleReader->dumpRedoLog >= 1) {
            uint8_t flag = redoLogRecord->data[fieldPos + 26];
            uint8_t lock = redoLogRecord->data[fieldPos + 28];
            oracleReader->dumpStream <<
                    "tabn: " << dec << (uint64_t)redoLogRecord->tabn <<
                    " slot: " << dec << redoLogRecord->slot << "(0x" << hex << redoLogRecord->slot << ")" <<
                    " flag: 0x" << setfill('0') << setw(2) << hex << (uint64_t)flag << endl <<
                    "lock: " << dec << (uint64_t)lock <<
                    " nrid: 0x" << setfill('0') << setw(8) << hex << redoLogRecord->nridBdba << "." << hex << redoLogRecord->nridSlot << endl;
        }
    }

    void OpCode::kdoOpCodeSKL(uint64_t fieldPos, uint64_t fieldLength) {
        if (fieldLength < 20) {
            oracleReader->dumpStream << "ERROR: too short field KDO OpCode SKL: " << dec << fieldLength << endl;
            return;
        }

        redoLogRecord->slot = redoLogRecord->data[fieldPos + 27];

        if (oracleReader->dumpRedoLog >= 1) {
            uint8_t flagStr[3] = "--";
            uint8_t lock = redoLogRecord->data[fieldPos + 29];
            uint8_t flag = redoLogRecord->data[fieldPos + 28];
            if ((flag & 0x01) != 0) flagStr[0] = 'F';
            if ((flag & 0x02) != 0) flagStr[1] = 'B';

            oracleReader->dumpStream << "flag: " << flagStr <<
                    " lock: " << dec << (uint64_t)lock <<
                    " slot: " << dec << redoLogRecord->slot << "(0x" << hex << redoLogRecord->slot << ")" << endl;

            if ((flag & 0x01) != 0) {
                uint8_t fwd[4];
                uint16_t fwd2 = oracleReader->read16(redoLogRecord->data + fieldPos + 20);
                memcpy(fwd, redoLogRecord->data + fieldPos + 16, 4);
                oracleReader->dumpStream << "fwd: 0x" <<
                        setfill('0') << setw(2) << hex << (uint64_t)fwd[0] <<
                        setfill('0') << setw(2) << hex << (uint64_t)fwd[1] <<
                        setfill('0') << setw(2) << hex << (uint64_t)fwd[2] <<
                        setfill('0') << setw(2) << hex << (uint64_t)fwd[3] << "." <<
                        dec << fwd2 << " " << endl;
            }

            if ((flag & 0x02) != 0) {
                uint8_t bkw[4];
                uint16_t bkw2 = oracleReader->read16(redoLogRecord->data + fieldPos + 26);
                memcpy(bkw, redoLogRecord->data + fieldPos + 22, 4);
                oracleReader->dumpStream << "bkw: 0x" <<
                        setfill('0') << setw(2) << hex << (uint64_t)bkw[0] <<
                        setfill('0') << setw(2) << hex << (uint64_t)bkw[1] <<
                        setfill('0') << setw(2) << hex << (uint64_t)bkw[2] <<
                        setfill('0') << setw(2) << hex << (uint64_t)bkw[3] << "." <<
                        dec << bkw2 << endl;
            }
        }
    }

    void OpCode::kdoOpCodeORP(uint64_t fieldPos, uint64_t fieldLength) {
        if (fieldLength < 48) {
            oracleReader->dumpStream << "ERROR: too short field KDO OpCode ORP: " << dec << fieldLength << endl;
            return;
        }

        redoLogRecord->fb = redoLogRecord->data[fieldPos + 16];
        redoLogRecord->cc = redoLogRecord->data[fieldPos + 18];
        redoLogRecord->slot = oracleReader->read16(redoLogRecord->data + fieldPos + 42);
        redoLogRecord->tabn = redoLogRecord->data[fieldPos + 44];
        redoLogRecord->nullsDelta = fieldPos + 45;

        if (fieldLength < 45 + ((uint64_t)redoLogRecord->cc + 7) / 8) {
            oracleReader->dumpStream << "ERROR: too short field KDO OpCode ORP for nulls: " << dec << fieldLength <<
                    " (cc: " << redoLogRecord->cc << ")" << endl;
            return;
        }

        if ((redoLogRecord->fb & FB_L) == 0) {
            redoLogRecord->nridBdba = oracleReader->read32(redoLogRecord->data + fieldPos + 28);
            redoLogRecord->nridSlot = oracleReader->read16(redoLogRecord->data + fieldPos + 32);
        }

        if (oracleReader->dumpRedoLog >= 1) {
            uint16_t sizeDelt = oracleReader->read16(redoLogRecord->data + fieldPos + 40);
            oracleReader->dumpStream << "tabn: "<< (uint64_t)redoLogRecord->tabn <<
                " slot: " << dec << (uint64_t)redoLogRecord->slot << "(0x" << hex << (uint64_t)redoLogRecord->slot << ")" <<
                " size/delt: " << dec << sizeDelt << endl;

            char fbStr[9] = "--------";
            processFbFlags(redoLogRecord->fb, fbStr);
            uint8_t lb = redoLogRecord->data[fieldPos + 17];

            oracleReader->dumpStream << "fb: " << fbStr <<
                    " lb: 0x" << hex << (uint64_t)lb << " " <<
                    " cc: " << dec << (uint64_t)redoLogRecord->cc;
            if (fbStr[1] == 'C') {
                uint8_t cki = redoLogRecord->data[fieldPos + 19];
                oracleReader->dumpStream << " cki: " << dec << (uint64_t)cki << endl;
            } else
                oracleReader->dumpStream << endl;

            if ((redoLogRecord->fb & FB_L) == 0) {
                oracleReader->dumpStream << "nrid:  0x" << setfill('0') << setw(8) << hex << redoLogRecord->nridBdba << "." << hex << redoLogRecord->nridSlot << endl;
            }

            oracleReader->dumpStream << "null:";
            if (redoLogRecord->cc >= 11)
                oracleReader->dumpStream << endl << "01234567890123456789012345678901234567890123456789012345678901234567890123456789" << endl;
            else
                oracleReader->dumpStream << " ";

            uint8_t *nulls = redoLogRecord->data + redoLogRecord->nullsDelta, bits = 1;
            for (uint64_t i = 0; i < redoLogRecord->cc; ++i) {

                if ((*nulls & bits) != 0)
                    oracleReader->dumpStream << "N";
                else
                    oracleReader->dumpStream << "-";
                if ((i % 80) == 79 && i < redoLogRecord->cc)
                    oracleReader->dumpStream << endl;

                bits <<= 1;
                if (bits == 0) {
                    bits = 1;
                    ++nulls;
                }
            }
            oracleReader->dumpStream << endl;
        }
    }

    void OpCode::kdoOpCodeQM(uint64_t fieldPos, uint64_t fieldLength) {
        if (fieldLength < 24) {
            oracleReader->dumpStream << "ERROR: too short field KDO OpCode QMI (1): " << dec << fieldLength << endl;
            return;
        }

        redoLogRecord->tabn = redoLogRecord->data[fieldPos + 16];
        redoLogRecord->nrow = oracleReader->read16(redoLogRecord->data + fieldPos + 18);
        redoLogRecord->slotsDelta = fieldPos + 20;

        if (oracleReader->dumpRedoLog >= 1) {
            uint8_t lock = redoLogRecord->data[fieldPos + 17];

            oracleReader->dumpStream << "tabn: "<< (uint64_t)redoLogRecord->tabn <<
                " lock: " << dec << (uint64_t)lock <<
                " nrow: " << dec << redoLogRecord->nrow << endl;

            if (fieldLength < 22 + (uint64_t)redoLogRecord->nrow * 2) {
                oracleReader->dumpStream << "ERROR: too short field KDO OpCode QMI (2): " << dec << fieldLength << ", " <<
                        redoLogRecord->nrow << endl;
                return;
            }
        }
    }

    void OpCode::kdoOpCode(uint64_t fieldPos, uint64_t fieldLength) {
        if (fieldLength < 16) {
            oracleReader->dumpStream << "ERROR: too short field KDO OpCode: " << dec << fieldLength << endl;
            return;
        }

        redoLogRecord->bdba = oracleReader->read32(redoLogRecord->data + fieldPos + 0);
        redoLogRecord->op = redoLogRecord->data[fieldPos + 10];
        redoLogRecord->flags = redoLogRecord->data[fieldPos + 11];
        redoLogRecord->itli = redoLogRecord->data[fieldPos + 12];

        if (oracleReader->dumpRedoLog >= 1) {
            typedba hdba = oracleReader->read32(redoLogRecord->data + fieldPos + 4);
            uint16_t maxFr = oracleReader->read16(redoLogRecord->data + fieldPos + 8);
            uint8_t ispac = redoLogRecord->data[fieldPos + 13];

            const char* opCode = "???";
            switch (redoLogRecord->op & 0x1F) {
                case OP_IUR: opCode = "IUR"; break; //Interpret Undo Redo
                case OP_IRP: opCode = "IRP"; break; //Insert Row Piece
                case OP_DRP: opCode = "DRP"; break; //Delete Row Piece
                case OP_LKR: opCode = "LKR"; break; //LocK Row
                case OP_URP: opCode = "URP"; break; //Update Row Piece
                case OP_ORP: opCode = "ORP"; break; //Overwrite Row Piece
                case OP_MFC: opCode = "MFC"; break; //Manipulate First Column
                case OP_CFA: opCode = "CFA"; break; //Change Forwarding Address
                case OP_CKI: opCode = "CKI"; break; //Change Cluster key Index
                case OP_SKL: opCode = "SKL"; break; //Set Key Links
                case OP_QMI: opCode = "QMI"; break; //Quick Multi-row Insert
                case OP_QMD: opCode = "QMD"; break; //Quick Multi-row Delete
                case OP_DSC: opCode = "DSC"; break;
                case OP_LMN: opCode = "LMN"; break;
                case OP_LLB: opCode = "LLB"; break;
                case OP_SHK: opCode = "SHK"; break;
                case OP_CMP: opCode = "CMP"; break;
                case OP_DCU: opCode = "DCU"; break;
                case OP_MRK: opCode = "MRK"; break;
                default:
                    opCode = "XXX";
                    if (oracleReader->dumpRedoLog >= 1)
                        oracleReader->dumpStream << "DEBUG op: " << dec << (uint64_t)(redoLogRecord->op & 0x1F) << endl;
            }

            string xtype = "0";
            string rtype = "";
            switch (redoLogRecord->flags & 0x03) {
            case FLAGS_XA:
                xtype = "XA"; //redo
                break;
            case FLAGS_XR:
                xtype = "XR"; //rollback
                break;
            case FLAGS_CR:
                xtype = "CR"; //unknown
                break;
            }
            redoLogRecord->flags &= 0xFC;

            if ((redoLogRecord->flags & FLAGS_KDO_KDOM2) != 0)
                rtype = "xtype KDO_KDOM2";

            string rowDependencies = "Disabled";
            if ((redoLogRecord->op & OP_ROWDEPENDENCIES) != 0)
                rowDependencies = "Enabled";

            oracleReader->dumpStream << "KDO Op code: " << opCode << " row dependencies " << rowDependencies << endl;
            oracleReader->dumpStream << "  xtype: " << xtype << rtype <<
                    " flags: 0x" << setfill('0') << setw(8) << hex << (uint64_t)redoLogRecord->flags << " " <<
                    " bdba: 0x" << setfill('0') << setw(8) << hex << redoLogRecord->bdba << " " <<
                    " hdba: 0x" << setfill('0') << setw(8) << hex << hdba << endl;
            oracleReader->dumpStream << "itli: " << dec << (uint64_t)redoLogRecord->itli << " " <<
                    " ispac: " << dec << (uint64_t)ispac << " " <<
                    " maxfr: " << dec << (uint64_t)maxFr << endl;
        }

        switch (redoLogRecord->op & 0x1F) {
        case OP_IRP: kdoOpCodeIRP(fieldPos, fieldLength);
                     break;
        case OP_DRP: kdoOpCodeDRP(fieldPos, fieldLength);
                     break;
        case OP_LKR: kdoOpCodeLKR(fieldPos, fieldLength);
                     break;
        case OP_URP: kdoOpCodeURP(fieldPos, fieldLength);
                     break;
        case OP_ORP: kdoOpCodeORP(fieldPos, fieldLength);
                     break;
        case OP_CKI: kdoOpCodeSKL(fieldPos, fieldLength);
                     break;
        case OP_CFA: kdoOpCodeCFA(fieldPos, fieldLength);
                     break;
        case OP_QMI:
        case OP_QMD: kdoOpCodeQM(fieldPos, fieldLength);
                     break;
        }
    }

    void OpCode::ktub(uint64_t fieldPos, uint64_t fieldLength) {
        if (fieldLength < 24) {
            oracleReader->dumpStream << "ERROR: too short field ktub: " << dec << fieldLength << endl;
            return;
        }

        redoLogRecord->objn = oracleReader->read32(redoLogRecord->data + fieldPos + 0);
        redoLogRecord->objd = oracleReader->read32(redoLogRecord->data + fieldPos + 4);
        redoLogRecord->tsn = oracleReader->read32(redoLogRecord->data + fieldPos + 8);
        redoLogRecord->undo = oracleReader->read32(redoLogRecord->data + fieldPos + 12);
        redoLogRecord->opc = (((typeop1)redoLogRecord->data[fieldPos + 16]) << 8) | redoLogRecord->data[fieldPos + 17];
        redoLogRecord->slt = redoLogRecord->data[fieldPos + 18];
        redoLogRecord->rci = redoLogRecord->data[fieldPos + 19];
        redoLogRecord->flg = oracleReader->read16(redoLogRecord->data + fieldPos + 20);

        string ktuType = "ktubu", prevObj = "", postObj = "";
        bool isKtubl = false;
        if ((redoLogRecord->flg & FLG_KTUBL) != 0) {
            isKtubl = true;
            ktuType = "ktubl";
            if (oracleReader->version < 0x19000) {
                prevObj = "[";
                postObj = "]";
            }
        }

        if (oracleReader->version < 0x19000) {
            oracleReader->dumpStream <<
                    ktuType << " redo:" <<
                    " slt: " << dec << (uint64_t)redoLogRecord->slt <<
                    " rci: " << dec << (uint64_t)redoLogRecord->rci <<
                    " opc: " << dec << (uint64_t)(redoLogRecord->opc >> 8) << "." << (uint64_t)(redoLogRecord->opc & 0xFF) <<
                    " " << prevObj << "objn: " << dec << redoLogRecord->objn <<
                    " objd: " << dec << redoLogRecord->objd <<
                    " tsn: " << dec << redoLogRecord->tsn << postObj << endl;
        } else {
            typedba prevDba = oracleReader->read32(redoLogRecord->data + fieldPos + 12);
            uint16_t wrp = oracleReader->read16(redoLogRecord->data + fieldPos + 22);

            oracleReader->dumpStream <<
                    ktuType << " redo:" <<
                    " slt: "  << dec << (uint64_t)redoLogRecord->slt <<
                    " wrp: " << dec << wrp <<
                    " flg: 0x" << setfill('0') << setw(4) << hex << redoLogRecord->flg <<
                    " prev dba:  0x" << setfill('0') << setw(8) << hex << prevDba <<
                    " rci: " << dec << (uint64_t)redoLogRecord->rci <<
                    " opc: " << dec << (uint64_t)(redoLogRecord->opc >> 8) << "." << (uint64_t)(redoLogRecord->opc & 0xFF) <<
                    " [objn: " << dec << redoLogRecord->objn <<
                    " objd: " << dec << redoLogRecord->objd <<
                    " tsn: " << dec << redoLogRecord->tsn << "]" << endl;
        }

        string lastBufferSplit;
        if ((redoLogRecord->flg & FLG_LASTBUFFERSPLIT) != 0)
            lastBufferSplit = "Yes";
        else {
            if (oracleReader->version < 0x19000)
                lastBufferSplit = "No";
            else
                lastBufferSplit = " No";
        }

        string userUndoDone;
        if ((redoLogRecord->flg & FLG_USERUNDODDONE) != 0)
            userUndoDone = "Yes";
        else {
            if (oracleReader->version < 0x19000)
                userUndoDone = "No";
            else
                userUndoDone = " No";
        }

        string undoType;
        if (oracleReader->version < 0x12200) {
            if ((redoLogRecord->flg & FLG_MULTIBLOCKUNDOHEAD) != 0)
                undoType = "Multi-block undo - HEAD";
            else if ((redoLogRecord->flg & FLG_MULTIBLOCKUNDOTAIL) != 0)
                undoType = "Multi-Block undo - TAIL";
            else if ((redoLogRecord->flg & FLG_MULTIBLOCKUNDOMID) != 0)
                undoType = "Multi-block undo - MID";
            else
                undoType = "Regular undo      ";
        } else if (oracleReader->version < 0x19000) {
            if ((redoLogRecord->flg & FLG_MULTIBLOCKUNDOHEAD) != 0)
                undoType = "Multi-Block undo - HEAD";
            else if ((redoLogRecord->flg & FLG_MULTIBLOCKUNDOTAIL) != 0)
                undoType = "Multi-Block undo - TAIL";
            else if ((redoLogRecord->flg & FLG_MULTIBLOCKUNDOMID) != 0)
                undoType = "Multi-Block undo - MID";
            else
                undoType = "Regular undo";
        } else {
            if ((redoLogRecord->flg & FLG_MULTIBLOCKUNDOHEAD) != 0)
                undoType = "MBU - HEAD  ";
            else if ((redoLogRecord->flg & FLG_MULTIBLOCKUNDOTAIL) != 0)
                undoType = "MBU - TAIL  ";
            else if ((redoLogRecord->flg & FLG_MULTIBLOCKUNDOMID) != 0)
                undoType = "MBU - MID   ";
            else
                undoType = "Regular undo";
        }

        string tempObject;
        if ((redoLogRecord->flg & FLG_ISTEMPOBJECT) != 0)
            tempObject = "Yes";
        else {
            if (oracleReader->version < 0x19000)
                tempObject = "No";
            else
                tempObject = " No";
        }

        string tablespaceUndo;
        if ((redoLogRecord->flg & FLG_TABLESPACEUNDO) != 0)
            tablespaceUndo = "Yes";
        else {
            if (oracleReader->version < 0x19000)
                tablespaceUndo = "No";
            else
                tablespaceUndo = " No";
        }

        string userOnly = " No";
        if ((redoLogRecord->flg & FLG_USERONLY) != 0)
            userOnly = "Yes";
        else {
            if (oracleReader->version < 0x19000)
                userOnly = "No";
            else
                userOnly = " No";
        }

        if (isKtubl) {
            //KTUBL
            if (fieldLength < 28) {
                oracleReader->dumpStream << "too short field ktubl: " << dec << fieldLength << endl;
                return;
            }

            if (fieldLength == 28) {
                if (oracleReader->dumpRedoLog >= 1) {
                    uint16_t flg2 = oracleReader->read16(redoLogRecord->data + fieldPos + 24);
                    int16_t buExtIdx = oracleReader->read16(redoLogRecord->data + fieldPos + 26);

                    if (oracleReader->version < 0x12200) {
                        oracleReader->dumpStream <<
                                "Undo type:  " << undoType << "  " <<
                                "Begin trans    Last buffer split:  " << lastBufferSplit << " " << endl <<
                                "Temp Object:  " << tempObject << " " << endl <<
                                "Tablespace Undo:  " << tablespaceUndo << " " << endl <<
                                "             0x" << setfill('0') << setw(8) << hex << redoLogRecord->undo << " " << endl;

                        oracleReader->dumpStream <<
                                " BuExt idx: " << dec << buExtIdx <<
                                " flg2: " << hex << flg2 << endl;
                    } else if (oracleReader->version < 0x19000) {
                        oracleReader->dumpStream <<
                                "Undo type:  " << undoType <<
                                "        Begin trans    Last buffer split:  " << lastBufferSplit << " " << endl <<
                                "Temp Object:  " << tempObject << " " << endl <<
                                "Tablespace Undo:  " << tablespaceUndo << " " << endl <<
                                "             0x" << setfill('0') << setw(8) << hex << redoLogRecord->undo << " " << endl;

                        oracleReader->dumpStream <<
                                " BuExt idx: " << dec << buExtIdx <<
                                " flg2: " << hex << flg2 << endl;
                    } else {
                        oracleReader->dumpStream <<
                                "[Undo type  ] " << undoType << " " <<
                                " [User undo done   ] " << userUndoDone << " " <<
                                " [Last buffer split] " << lastBufferSplit << " " << endl <<
                                "[Temp object]          " << tempObject << " " <<
                                " [Tablespace Undo  ] " << tablespaceUndo << " " <<
                                " [User only        ] " << userOnly << " " << endl <<
                                "Begin trans    " << endl;

                        oracleReader->dumpStream <<
                                "BuExt idx: " << dec << buExtIdx <<
                                " flg2: " << hex << flg2 << endl;
                    }
                }
            } else if (fieldLength >= 76) {
                if (oracleReader->dumpRedoLog >= 1) {
                    uint16_t flg2 = oracleReader->read16(redoLogRecord->data + fieldPos + 24);
                    int16_t buExtIdx = oracleReader->read16(redoLogRecord->data + fieldPos + 26);
                    typeuba prevCtlUba = oracleReader->read64(redoLogRecord->data + fieldPos + 28);
                    typescn prevCtlMaxCmtScn = oracleReader->readSCN(redoLogRecord->data + fieldPos + 36);
                    typescn prevTxCmtScn = oracleReader->readSCN(redoLogRecord->data + fieldPos + 44);
                    typescn txStartScn = oracleReader->readSCN(redoLogRecord->data + fieldPos + 56);
                    uint32_t prevBrb = oracleReader->read32(redoLogRecord->data + fieldPos + 64);
                    uint32_t prevBcl = oracleReader->read32(redoLogRecord->data + fieldPos + 68);
                    uint32_t logonUser = oracleReader->read32(redoLogRecord->data + fieldPos + 72);

                    if (oracleReader->version < 0x12200) {
                        oracleReader->dumpStream <<
                                "Undo type:  " << undoType << "  " <<
                                "Begin trans    Last buffer split:  " << lastBufferSplit << " " << endl <<
                                "Temp Object:  " << tempObject << " " << endl <<
                                "Tablespace Undo:  " << tablespaceUndo << " " << endl <<
                                "             0x" << setfill('0') << setw(8) << hex << redoLogRecord->undo << " " <<
                                " prev ctl uba: " << PRINTUBA(prevCtlUba) << " " << endl <<
                                "prev ctl max cmt scn:  " << PRINTSCN48(prevCtlMaxCmtScn) << " " <<
                                " prev tx cmt scn:  " << PRINTSCN48(prevTxCmtScn) << " " << endl;

                        oracleReader->dumpStream <<
                                "txn start scn:  " << PRINTSCN48(txStartScn) << " " <<
                                " logon user: " << dec << logonUser << " " <<
                                " prev brb: " << prevBrb << " " <<
                                " prev bcl: " << dec << prevBcl;

                        oracleReader->dumpStream <<
                                " BuExt idx: " << dec << buExtIdx <<
                                " flg2: " << hex << flg2 << endl;
                    } else if (oracleReader->version < 0x19000) {
                        oracleReader->dumpStream <<
                                "Undo type:  " << undoType <<
                                "        Begin trans    Last buffer split:  " << lastBufferSplit << " " << endl <<
                                "Temp Object:  " << tempObject << " " << endl <<
                                "Tablespace Undo:  " << tablespaceUndo << " " << endl <<
                                "             0x" << setfill('0') << setw(8) << hex << redoLogRecord->undo << " " <<
                                " prev ctl uba: " << PRINTUBA(prevCtlUba) << " " << endl <<
                                "prev ctl max cmt scn:  " << PRINTSCN64(prevCtlMaxCmtScn) << " " <<
                                " prev tx cmt scn:  " << PRINTSCN64(prevTxCmtScn) << " " << endl;

                        oracleReader->dumpStream <<
                                "txn start scn:  " << PRINTSCN64(txStartScn) << " " <<
                                " logon user: " << dec << logonUser << " " <<
                                " prev brb: " << prevBrb << " " <<
                                " prev bcl: " << dec << prevBcl;

                        oracleReader->dumpStream <<
                                " BuExt idx: " << dec << buExtIdx <<
                                " flg2: " << hex << flg2 << endl;
                    } else {
                        oracleReader->dumpStream <<
                                "[Undo type  ] " << undoType << " " <<
                                " [User undo done   ] " << userUndoDone << " " <<
                                " [Last buffer split] " << lastBufferSplit << " " << endl <<
                                "[Temp object]          " << tempObject << " " <<
                                " [Tablespace Undo  ] " << tablespaceUndo << " " <<
                                " [User only        ] " << userOnly << " " << endl <<
                                "Begin trans    " << endl <<
                                " prev ctl uba: " << PRINTUBA(prevCtlUba) <<
                                " prev ctl max cmt scn:  " << PRINTSCN64(prevCtlMaxCmtScn) << " " << endl <<
                                " prev tx cmt scn:  " << PRINTSCN64(prevTxCmtScn) << " " << endl;

                        oracleReader->dumpStream <<
                                " txn start scn:  " << PRINTSCN64(txStartScn) <<
                                "  logon user: " << dec << logonUser << endl <<
                                " prev brb:  0x" << setfill('0') << setw(8) << hex << prevBrb <<
                                "  prev bcl:  0x" << setfill('0') << setw(8) << hex << prevBcl << endl;

                        oracleReader->dumpStream <<
                                "BuExt idx: " << dec << buExtIdx <<
                                " flg2: " << hex << flg2 << endl;
                    }
                }
            }
        } else {
            //KTUBU
            if (oracleReader->dumpRedoLog >= 1) {
                if (oracleReader->version < 0x19000) {
                    oracleReader->dumpStream <<
                            "Undo type:  " << undoType << " " <<
                            "Undo type:  " << getUndoType() <<
                            "Last buffer split:  " << lastBufferSplit << " " << endl <<
                            "Tablespace Undo:  " << tablespaceUndo << " " << endl <<
                            "             0x" << setfill('0') << setw(8) << hex << redoLogRecord->undo << endl;
                } else {
                    oracleReader->dumpStream <<
                            "[Undo type  ] " << undoType << " " <<
                            " [User undo done   ] " << userUndoDone << " " <<
                            " [Last buffer split] " << lastBufferSplit << " " << endl <<
                            "[Temp object]          " << tempObject << " " <<
                            " [Tablespace Undo  ] " << tablespaceUndo << " " <<
                            " [User only        ] " << userOnly << " " << endl;
                }
            }
        }
    }


    const char* OpCode::getUndoType() {
        return "";
    }

    void OpCode::dumpColsVector(uint8_t *data, uint64_t colnum, uint16_t fieldLength) {
        uint64_t pos = 0;

        oracleReader->dumpStream << "Vector content: " << endl;

        for (uint64_t k = 0; k < redoLogRecord->cc; ++k) {
            uint16_t fieldLength = data[pos];
            ++pos;
            uint8_t isNull = (fieldLength == 0xFF);

            if (fieldLength == 0xFE) {
                fieldLength = oracleReader->read16(data + pos);
                pos += 2;
            }

            dumpCols(data + pos, colnum + k, fieldLength, isNull);

            if (!isNull)
                pos += fieldLength;
        }
    }

    void OpCode::dumpCols(uint8_t *data, uint64_t colnum, uint16_t fieldLength, uint8_t isNull) {
        if (isNull) {
            oracleReader->dumpStream << "col " << setfill(' ') << setw(2) << dec << colnum << ": *NULL*" << endl;
        } else {
            oracleReader->dumpStream << "col " << setfill(' ') << setw(2) << dec << colnum << ": " <<
                    "[" << setfill(' ') << setw(2) << dec << fieldLength << "]";

            if (fieldLength <= 20)
                oracleReader->dumpStream << " ";
            else
                oracleReader->dumpStream << endl;

            for (uint64_t j = 0; j < fieldLength; ++j) {
                oracleReader->dumpStream << " " << setfill('0') << setw(2) << hex << (uint64_t)data[j];
                if ((j % 25) == 24 && j != (uint64_t)fieldLength - 1)
                    oracleReader->dumpStream << endl;
            }

            oracleReader->dumpStream << endl;
        }
    }

    void OpCode::dumpRows(uint8_t *data) {
        if (oracleReader->dumpRedoLog >= 1) {
            uint64_t pos = 0;
            char fbStr[9] = "--------";

            for (uint64_t r = 0; r < redoLogRecord->nrow; ++r) {
                oracleReader->dumpStream << "slot[" << dec << r << "]: " << dec << oracleReader->read16(redoLogRecord->data + redoLogRecord->slotsDelta + r * 2) << endl;
                processFbFlags(data[pos + 0], fbStr);
                uint8_t lb = data[pos + 1];
                uint8_t jcc = data[pos + 2];
                uint16_t tl = oracleReader->read16(redoLogRecord->data + redoLogRecord->rowLenghsDelta + r * 2);

                oracleReader->dumpStream << "tl: " << dec << tl <<
                        " fb: " << fbStr <<
                        " lb: 0x" << hex << (uint64_t)lb << " " <<
                        " cc: " << dec << (uint64_t)jcc << endl;
                pos += 3;

                if ((redoLogRecord->op & OP_ROWDEPENDENCIES) != 0) {
                    if (oracleReader->version < 0x12200)
                        pos += 6;
                    else
                        pos += 8;
                }

                for (uint64_t k = 0; k < jcc; ++k) {
                    uint16_t fieldLength = data[pos];
                    ++pos;
                    uint8_t isNull = (fieldLength == 0xFF);

                    if (fieldLength == 0xFE) {
                        fieldLength = oracleReader->read16(data + pos);
                        pos += 2;
                    }

                    dumpCols(data + pos, k, fieldLength, isNull);

                    if (!isNull)
                        pos += fieldLength;
                }
            }
        }
    }

    void OpCode::dumpVal(uint64_t fieldPos, uint64_t fieldLength, string msg) {
        if (oracleReader->dumpRedoLog >= 1) {
            oracleReader->dumpStream << msg;
            for (uint64_t j = 0; j < fieldLength; ++j)
                oracleReader->dumpStream << redoLogRecord->data[fieldPos + j];
            oracleReader->dumpStream << endl;
        }
    }

    void OpCode::processFbFlags(uint8_t fb, char *fbStr) {
        if ((fb & FB_N) != 0) fbStr[7] = 'N'; else fbStr[7] = '-'; //last column continues in Next piece
        if ((fb & FB_P) != 0) fbStr[6] = 'P'; else fbStr[6] = '-'; //first column continues from Previous piece
        if ((fb & FB_L) != 0) fbStr[5] = 'L'; else fbStr[5] = '-'; //Last data piece
        if ((fb & FB_F) != 0) fbStr[4] = 'F'; else fbStr[4] = '-'; //First data piece
        if ((fb & FB_D) != 0) fbStr[3] = 'D'; else fbStr[3] = '-'; //Deleted row
        if ((fb & FB_H) != 0) fbStr[2] = 'H'; else fbStr[2] = '-'; //Head piece of row
        if ((fb & FB_C) != 0) fbStr[1] = 'C'; else fbStr[1] = '-'; //Clustered table member
        if ((fb & FB_K) != 0) fbStr[0] = 'K'; else fbStr[0] = '-'; //cluster Key
        fbStr[8] = 0;
    }
}
