/******************************************************************************

	input.c

    PSP����ȥ�`����������

******************************************************************************/

#include <stddef.h>
#include "input.h"
#include "input_driver.h"
#include "ticker_driver.h"

// TODO: Use video driver function instead
void video_wait_vsync(void);

/******************************************************************************
	��`�������
******************************************************************************/

static uint32_t pad;
static uint8_t pressed_check;
static uint8_t pressed_count;
static uint8_t pressed_delay;
static TICKER curr_time;
static TICKER prev_time;

void *input_info;


/******************************************************************************
	����`�Х��v��
******************************************************************************/

/*--------------------------------------------------------
	�ѥåɤ���ڻ�
--------------------------------------------------------*/

void pad_init(void)
{
	pad = 0;
	pressed_check = 0;
	pressed_count = 0;
	pressed_delay = 0;
	input_info = input_driver->init();
}

void pad_exit(void)
{
	input_driver->free(input_info);
	input_info = NULL;
}

/*--------------------------------------------------------
	�ѥåɤ�Ѻ��״�Bȡ��
--------------------------------------------------------*/

uint32_t poll_gamepad(void)
{
	return input_driver->poll(input_info);
}


/*--------------------------------------------------------
	�ѥåɤ�Ѻ��״�Bȡ�� (MVS / fatfursp����)
--------------------------------------------------------*/

#if (EMU_SYSTEM == MVS)
uint32_t poll_gamepad_fatfursp(void)
{
	return input_driver->pollFatfursp(input_info);
}
#endif


/*--------------------------------------------------------
	�ѥåɤ�Ѻ��״�Bȡ��(���ʥ�)
--------------------------------------------------------*/

#if (EMU_SYSTEM == MVS)
uint32_t poll_gamepad_analog(void)
{
	return input_driver->pollAnalog(input_info);
}
#endif


/*--------------------------------------------------------
	�ѥå�Ѻ��������
--------------------------------------------------------*/

void pad_update(void)
{
	uint32_t data;

	data = poll_gamepad();

	if (data)
	{
		if (!pressed_check)
		{
			pressed_check = 1;
			pressed_count = 0;
			pressed_delay = 8;
			prev_time = ticker_driver->ticker(NULL);
		}
		else
		{
			int count;

			curr_time = ticker_driver->ticker(NULL);
			count = (int)((curr_time - prev_time) / (TICKS_PER_SEC / 60));
			prev_time = curr_time;

			pressed_count += count;

			if (pressed_count > pressed_delay)
			{
				pressed_count = 0;
				if (pressed_delay > 2) pressed_delay -= 2;
			}
			else data = 0;
		}
	}
	else pressed_check = 0;

	pad = data;
}


/*--------------------------------------------------------
	�ܥ���Ѻ��״�B��ȡ��
--------------------------------------------------------*/

bool pad_pressed(uint32_t code)
{
	return (pad & code) != 0;
}


/*--------------------------------------------------------
	ָ�����`�������ȫ�ܥ����Ѻ��״�Bȡ��
--------------------------------------------------------*/

bool pad_pressed_any(uint32_t disable_code)
{
	return (pad & (PLATFORM_PAD_ANY ^ disable_code)) != 0;
}


/*--------------------------------------------------------
	�ܥ����Ѻ��״�B�����ꥢ�����ޤǴ���
--------------------------------------------------------*/

void pad_wait_clear(void)
{
	while (poll_gamepad())
	{
		video_wait_vsync();
		if (!Loop) break;
	}

	pad = 0;
	pressed_check = 0;
}


/*--------------------------------------------------------
	�Τ��ܥ���Ѻ�����ޤǴ���
--------------------------------------------------------*/

void pad_wait_press(int msec)
{
	pad_wait_clear();

	if (msec == PAD_WAIT_INFINITY)
	{
		while (!poll_gamepad())
		{
			video_wait_vsync();
			if (!Loop) break;
		}
	}
	else
	{
		TICKER target = ticker_driver->ticker(NULL) + msec * (TICKS_PER_SEC / 1000);

		while (ticker_driver->ticker(NULL) < target)
		{
			video_wait_vsync();
			if (poll_gamepad()) break;
			if (!Loop) break;
		}
	}

	pad_wait_clear();
}
