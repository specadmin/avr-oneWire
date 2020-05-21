//-----------------------------------------------------------------------------
#include "../avr-misc/avr-misc.h"
#include "oneWire.h"
#include "config.h"
//-----------------------------------------------------------------------------
#define OW_T_SLOT               70
#define OW_T_LOW0               70
#define OW_T_LOW1               10
#define OW_T_RDV                1
#define OW_T_RST_LO             500
#define OW_T_RST_HI             500
#define OW_T_PD_LO              100
#define OW_T_PD_HI              70
#define OW_T_REC                2
//-----------------------------------------------------------------------------
__inline void pull_bus_down(WORD delay)
{
    // wait for the bus to become free
    while(!test_bit(ONE_WIRE_PIN, ONE_WIRE_BIT));

    // ensure we are going to send a low-level to the bus
    clr_bit(ONE_WIRE_PORT, ONE_WIRE_BIT);

    // attach to the bus and release it after delay
    set_bit(ONE_WIRE_DDR, ONE_WIRE_BIT);
    udelay(delay);
    clr_bit(ONE_WIRE_DDR, ONE_WIRE_BIT);
}
//-----------------------------------------------------------------------------
void one_wire_write_one()
{
    pull_bus_down(OW_T_LOW1);
    udelay(OW_T_SLOT - OW_T_LOW1);
}
//-----------------------------------------------------------------------------
void one_wire_write_zero()
{
    pull_bus_down(OW_T_LOW0);
    udelay(OW_T_REC);
}
//-----------------------------------------------------------------------------
BYTE one_wire_reset()
{
    pull_bus_down(OW_T_RST_LO);
    udelay(OW_T_PD_HI);
    BYTE tmp = test_bit(ONE_WIRE_PIN, ONE_WIRE_BIT);
    udelay(OW_T_RST_HI - OW_T_PD_HI);
    return tmp;
}
//-----------------------------------------------------------------------------
BYTE one_wire_read_bit()
{
    disable_interrupts();
    pull_bus_down(OW_T_RDV);
    resume_interrupts();
    udelay(OW_T_REC);
    BYTE tmp = test_bit(ONE_WIRE_PIN, ONE_WIRE_BIT);
    udelay(OW_T_SLOT - OW_T_RDV);
    return tmp;
}
//-----------------------------------------------------------------------------
void one_wire_send_byte(BYTE byte)
{
    for(BYTE i = 0; i < 8; i++)
    {
        if(byte & 0x01)
        {
            one_wire_write_one();
        }
        else
        {
            one_wire_write_zero();
        }
        byte >>= 1;
    }
}
//-----------------------------------------------------------------------------
BYTE one_wire_read_byte()
{
    BYTE tmp = 0;
    for(BYTE i = 0; i < 8; i++)
    {
        tmp >>= 1;
        tmp |= one_wire_read_bit() ? 0x80 : 0;
    }
    return tmp;
}
//-----------------------------------------------------------------------------
WORD one_wire_read_word()
{
    WORD tmp = 0;
    for(BYTE i = 0; i < 16; i++)
    {
        tmp >>= 1;
        tmp |= one_wire_read_bit() ? 0x8000 : 0;
    }
    return tmp;
}
//-----------------------------------------------------------------------------
