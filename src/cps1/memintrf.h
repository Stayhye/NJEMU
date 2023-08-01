/******************************************************************************

	memintrf.c

	CPS1���ꥤ�󥿥ե��`���v��

******************************************************************************/

#ifndef CPS1_MEMORY_INTERFACE_H
#define CPS1_MEMORY_INTERFACE_H

extern uint8_t *memory_region_cpu1;
extern uint8_t *memory_region_cpu2;
extern uint8_t *memory_region_gfx1;
extern uint8_t *memory_region_sound1;
extern uint8_t *memory_region_user1;
#if !RELEASE
extern uint8_t *memory_region_user2;
#endif
extern uint8_t *memory_region_user3;

extern uint32_t memory_length_cpu1;
extern uint32_t memory_length_cpu2;
extern uint32_t memory_length_gfx1;
extern uint32_t memory_length_sound1;
extern uint32_t memory_length_user1;
#if !RELEASE
extern uint32_t memory_length_user2;
#endif
extern uint32_t memory_length_user3;

extern uint8_t cps1_ram[0x10000];
extern uint16_t cps1_gfxram[0x30000 >> 1];
extern uint16_t cps1_output[0x100 >> 1];

extern uint8_t *qsound_sharedram1;
extern uint8_t *qsound_sharedram2;

int memory_init(void);
void memory_shutdown(void);

uint8_t m68000_read_memory_8(uint32_t offset);
uint16_t m68000_read_memory_16(uint32_t offset);
void m68000_write_memory_8(uint32_t offset, uint8_t data);
void m68000_write_memory_16(uint32_t offset, uint16_t data);

extern uint8_t (*z80_read_memory_8)(uint32_t offset);
extern void (*z80_write_memory_8)(uint32_t offset, uint8_t data);

#ifdef SAVE_STATE
STATE_SAVE( memory );
STATE_LOAD( memory );
#endif

#endif /* CPS1_MEMORY_INTERFACE_H */
