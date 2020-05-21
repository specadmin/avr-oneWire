#ifndef ONEWIRE_H
#define ONEWIRE_H
//-----------------------------------------------------------------------------
BYTE one_wire_reset();
BYTE one_wire_read_bit();
void one_wire_write_one();
void one_wire_write_zero();
void one_wire_send_byte(BYTE byte);
BYTE one_wire_read_byte();
WORD one_wire_read_word();
#define one_wire_write_bit(bit)     { if(bit) { one_wire_write_one(); } else { one_wire_write_zero(); } }
//-----------------------------------------------------------------------------
#endif
