/******************************************************************************

	state.c

	�X�e�[�g�Z�[�u/���[�h

******************************************************************************/

#ifdef SAVE_STATE

#include <fcntl.h>
#include <limits.h>
#include <time.h>
#include <zlib.h>
#include "emumain.h"

typedef struct {
	uint16_t year;
	uint16_t month;
	uint16_t day;
	uint16_t hour;
	uint16_t minutes;
	uint16_t seconds;
	uint32_t microseconds;
} stateTime;

/******************************************************************************
	�O���[�o���ϐ�
******************************************************************************/

char date_str[16];
char time_str[16];
char stver_str[16];
int state_version;
uint8_t *state_buffer;
int current_state_version;
#if (EMU_SYSTEM == MVS)
int  state_reload_bios;
#endif


/******************************************************************************
	���[�J���ϐ�
******************************************************************************/

#ifdef ADHOC
static uint8_t state_buffer_base[STATE_BUFFER_SIZE];
#endif

#if (EMU_SYSTEM == CPS1)
static const char *current_version_str = "CPS1SV23";
#elif (EMU_SYSTEM == CPS2)
static const char *current_version_str = "CPS2SV23";
#elif (EMU_SYSTEM == MVS)
static const char *current_version_str = "MVS_SV23";
#elif (EMU_SYSTEM == NCDZ)
static const char *current_version_str = "NCDZSV23";
#endif


/******************************************************************************
	���[�J���֐�
******************************************************************************/

/*------------------------------------------------------
	�T���l�C�������[�N�̈悩��t�@�C���ɕۑ�
------------------------------------------------------*/

static void save_thumbnail(void)
{
	int x, y, w, h;
	uint16_t *src = ((uint16_t *)UI_TEXTURE) + 152;

#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
	if (machine_screen_type)
	{
		w = 112;
		h = 152;
	}
	else
#endif
	{
		w = 152;
		h = 112;
	}

	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			state_save_word(&src[x], 1);
		}
		src += BUF_WIDTH;
	}
}


/*------------------------------------------------------
	�T���l�C�����t�@�C�����烏�[�N�̈�ɓǂݍ���
------------------------------------------------------*/

static void load_thumbnail(FILE *fp)
{
	int x, y, w, h;
	uint16_t *dst = (uint16_t *)UI_TEXTURE;

#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
	if (machine_screen_type)
	{
		w = 112;
		h = 152;
	}
	else
#endif
	{
		w = 152;
		h = 112;
	}

	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
#if (EMU_SYSTEM == NCDZ) || defined(ADHOC)
			fread(&dst[x], 1, 2, fp);
#else
			state_load_word(&dst[x], 1);
#endif
		}
		dst += BUF_WIDTH;
	}
}


/*------------------------------------------------------
	���[�N�̈�̃T���l�C�����N���A
------------------------------------------------------*/

static void clear_thumbnail(void)
{
	int x, y, w, h;
	uint16_t *dst = (uint16_t *)UI_TEXTURE;

#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
	if (machine_screen_type)
	{
		w = 112;
		h = 152;
	}
	else
#endif
	{
		w = 152;
		h = 112;
	}

	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			dst[x] = 0;
		}
		dst += BUF_WIDTH;
	}
}


/******************************************************************************
	�X�e�[�g�Z�[�u/���[�h�֐�
******************************************************************************/

/*------------------------------------------------------
	�X�e�[�g�Z�[�u
------------------------------------------------------*/

static inline void tm_to_stateTime(stateTime *st, struct tm *t)
{
	st->year = t->tm_year;
	st->month = t->tm_mon;
	st->day = t->tm_mday;
	st->hour = t->tm_hour;
	st->minutes = t->tm_min;
	st->seconds = t->tm_sec;
	st->microseconds = 0;
}

int state_save(int slot)
{
	int32_t fd = -1;
   	stateTime nowtime;
	char path[PATH_MAX];
	char error_mes[128];
	char buf[128];
#if (EMU_SYSTEM == NCDZ)
	uint8_t *inbuf, *outbuf;
	unsigned long insize, outsize;
#else
#ifndef ADHOC
	uint8_t *state_buffer_base;
#endif
	uint32_t size;
#endif

	sprintf(path, "%sstate/%s.sv%d", launchDir, game_name, slot);
	remove(path);

	sprintf(buf, TEXT(STATE_SAVING), game_name, slot);
	init_progress(6, buf);

	time_t now = time(NULL);
   	struct tm *t = localtime(&now);
	tm_to_stateTime(&nowtime, t);

	if ((fd = open(path, O_WRONLY|O_CREAT, 0777)) >= 0)
#if (EMU_SYSTEM == NCDZ)
	{
		if ((inbuf = malloc(STATE_BUFFER_SIZE)) == NULL)
		{
			strcpy(error_mes, TEXT(COULD_NOT_ALLOCATE_STATE_BUFFER));
			goto error;
		}
		memset(inbuf, 0, STATE_BUFFER_SIZE);
		state_buffer = inbuf;

		state_save_byte(current_version_str, 8);
		state_save_byte(&nowtime, 16);
		update_progress();

		save_thumbnail();
		update_progress();

		write(fd, inbuf, (uint32_t)state_buffer - (uint32_t)inbuf);
		update_progress();

		memset(inbuf, 0, STATE_BUFFER_SIZE);
		state_buffer = inbuf;

		state_save_memory();
		state_save_m68000();
		state_save_z80();
		state_save_input();
		state_save_timer();
		state_save_driver();
		state_save_video();
		state_save_ym2610();
		state_save_cdda();
		state_save_cdrom();
		update_progress();

		insize = (uint32_t)state_buffer - (uint32_t)inbuf;
		outsize = insize * 1.1 + 12;
		if ((outbuf = malloc(outsize)) == NULL)
		{
			strcpy(error_mes, TEXT(COULD_NOT_ALLOCATE_STATE_BUFFER));
			free(inbuf);
			goto error;
		}
		memset(outbuf, 0, outsize);

		if (compress(outbuf, &outsize, inbuf, insize) != Z_OK)
		{
			strcpy(error_mes, TEXT(COULD_NOT_COMPRESS_STATE_DATA));
			free(inbuf);
			free(outbuf);
			goto error;
		}
		free(inbuf);
		update_progress();

		write(fd, &outsize, 4);
		write(fd, outbuf, outsize);
		close(fd);
		free(outbuf);
		update_progress();

		show_progress(buf);
		return 1;
	}
#else
	{
#ifdef ADHOC
		state_buffer = state_buffer_base;
#else
#if (EMU_SYSTEM == CPS1 || (EMU_SYSTEM == CPS2 && defined(LARGE_MEMORY)))
		state_buffer = state_buffer_base = malloc(STATE_BUFFER_SIZE);
#else
		state_buffer = state_buffer_base = cache_alloc_state_buffer(STATE_BUFFER_SIZE);
#endif
		if (!state_buffer)
		{
			strcpy(error_mes, TEXT(COULD_NOT_ALLOCATE_STATE_BUFFER));
			goto error;
		}
#endif
		memset(state_buffer, 0, STATE_BUFFER_SIZE);
		update_progress();

		state_save_byte(current_version_str, 8);
		state_save_byte(&nowtime, 16);
		update_progress();

		save_thumbnail();
		update_progress();

		state_save_memory();
		state_save_m68000();
		state_save_z80();
		state_save_input();
		state_save_timer();
		state_save_driver();
		state_save_video();
#if (EMU_SYSTEM == CPS1)
		state_save_coin();
		switch (machine_driver_type)
		{
		case MACHINE_qsound:
			state_save_qsound();
			state_save_eeprom();
			break;

		case MACHINE_pang3:
			state_save_eeprom();

		default:
			state_save_ym2151();
			break;
		}
#elif (EMU_SYSTEM == CPS2)
		state_save_coin();
		state_save_qsound();
		state_save_eeprom();
#elif (EMU_SYSTEM == MVS)
		state_save_ym2610();
		state_save_pd4990a();
#endif
		update_progress();

		size = (uint32_t)state_buffer - (uint32_t)state_buffer_base;
		write(fd, state_buffer_base, size);
		close(fd);
		update_progress();

#ifndef ADHOC
#if (EMU_SYSTEM == CPS1 || (EMU_SYSTEM == CPS2 && defined(LARGE_MEMORY)))
		free(state_buffer_base);
#else
		cache_free_state_buffer(STATE_BUFFER_SIZE);
#endif
#endif
		update_progress();

		show_progress(buf);
		return 1;
	}
#endif
	else
	{
		sprintf(error_mes, TEXT(COULD_NOT_CREATE_STATE_FILE), game_name, slot);
	}

#if !defined(ADHOC) || (EMU_SYSTEM == NCDZ)
error:
#endif
	if (fd >= 0)
	{
		close(fd);
		remove(path);
	}
	show_progress(error_mes);
	pad_wait_press(PAD_WAIT_INFINITY);

	return 0;
}


/*------------------------------------------------------
	�X�e�[�g���[�h
------------------------------------------------------*/

int state_load(int slot)
{
#if defined(ADHOC) || (EMU_SYSTEM == NCDZ)
	int32_t fd;
#else
	FILE *fp;
#endif
	char path[PATH_MAX];
	char error_mes[128];
	char buf[128];
#if (EMU_SYSTEM == NCDZ)
	uint8_t *inbuf, *outbuf;
	unsigned long insize, outsize;
#endif

	sprintf(path, "%sstate/%s.sv%d", launchDir, game_name, slot);

#if (EMU_SYSTEM == MVS)
	state_reload_bios = 0;
#endif

	sprintf(buf, TEXT(STATE_LOADING), game_name, slot);
#if (EMU_SYSTEM == NCDZ)
	init_progress(6, buf);
#else
	init_progress(4, buf);
#endif

#if (EMU_SYSTEM == NCDZ)
	if ((fd = open(path, O_RDONLY, 0777)) >= 0)
	{
		lseek(fd, (8+16) + (152*112*2), SEEK_SET);
		update_progress();

		read(fd, &insize, 4);
		if ((inbuf = malloc(insize)) == NULL)
		{
			strcpy(error_mes, TEXT(COULD_NOT_ALLOCATE_STATE_BUFFER));
			close(fd);
			goto error;
		}
		memset(inbuf, 0, insize);
		update_progress();

		read(fd, inbuf, insize);
		close(fd);
		update_progress();

		outsize = STATE_BUFFER_SIZE;
		if ((outbuf = malloc(outsize)) == NULL)
		{
			strcpy(error_mes, TEXT(COULD_NOT_ALLOCATE_STATE_BUFFER));
			free(inbuf);
			goto error;
		}
		memset(outbuf, 0, outsize);

		if (uncompress(outbuf, &outsize, inbuf, insize) != Z_OK)
		{
			strcpy(error_mes, TEXT(COULD_NOT_UNCOMPRESS_STATE_DATA));
			free(inbuf);
			free(outbuf);
			goto error;
		}
		free(inbuf);
		update_progress();

		state_buffer = outbuf;

		state_load_memory();
		state_load_m68000();
		state_load_z80();
		state_load_input();
		state_load_timer();
		state_load_driver();
		state_load_video();
		state_load_ym2610();
		state_load_cdda();
		state_load_cdrom();
		update_progress();

		free(outbuf);

		if (mp3_get_status() == MP3_SEEK)
		{
			mp3_seek_start();

			while (mp3_get_status() == MP3_SEEK)
				video_driver->waitVsync(video_data);
		}
		update_progress();

		show_progress(buf);
		return 1;
	}
#else
#ifdef ADHOC
	if ((fd = open(path, O_RDONLY, 0777)) >= 0)
	{
		int size;

		size = lseek(fd, 0, SEEK_END);
		lseek(fd, 0, SEEK_SET);
		read(fd, state_buffer_base, size);
		close(fd);

		state_buffer = state_buffer_base;

		state_load_skip((8+16));
		update_progress();

		state_load_skip((152*112*2));
		update_progress();

		state_load_memory();
		state_load_m68000();
		state_load_z80();
		state_load_input();
		state_load_timer();
		state_load_driver();
		state_load_video();
#if (EMU_SYSTEM == CPS1)

		state_load_coin();
		switch (machine_driver_type)
		{
		case MACHINE_qsound:
			state_load_qsound();
			state_load_eeprom();
			break;

		case MACHINE_pang3:
			state_load_eeprom();

		default:
			state_load_ym2151();
			break;
		}
#elif (EMU_SYSTEM == CPS2)
		state_load_coin();
		state_load_qsound();
		state_load_eeprom();
#elif (EMU_SYSTEM == MVS)
		state_load_ym2610();
		state_load_pd4990a();

		if (state_reload_bios)
		{
			state_reload_bios = 0;

			if (!reload_bios())
			{
				show_progress(TEXT(COULD_NOT_RELOAD_BIOS));
				pad_wait_press(PAD_WAIT_INFINITY);
				Loop = LOOP_BROWSER;
				return 0;
			}
		}
#endif
#else
	if ((fp = fopen(path, "rb")) != NULL)
	{
		state_load_skip((8+16));
		update_progress();

		state_load_skip((152*112*2));
		update_progress();

		state_load_memory(fp);
		state_load_m68000(fp);
		state_load_z80(fp);
		state_load_input(fp);
		state_load_timer(fp);
		state_load_driver(fp);
		state_load_video(fp);
#if (EMU_SYSTEM == CPS1)

		state_load_coin(fp);
		switch (machine_driver_type)
		{
		case MACHINE_qsound:
			state_load_qsound(fp);
			state_load_eeprom(fp);
			break;

		case MACHINE_pang3:
			state_load_eeprom(fp);

		default:
			state_load_ym2151(fp);
			break;
		}
		fclose(fp);
#elif (EMU_SYSTEM == CPS2)
		state_load_coin(fp);
		state_load_qsound(fp);
		state_load_eeprom(fp);
		fclose(fp);
#elif (EMU_SYSTEM == MVS)
		state_load_ym2610(fp);
		state_load_pd4990a(fp);
		fclose(fp);

		if (state_reload_bios)
		{
			state_reload_bios = 0;

			if (!reload_bios())
			{
				show_progress(TEXT(COULD_NOT_RELOAD_BIOS));
				pad_wait_press(PAD_WAIT_INFINITY);
				Loop = LOOP_BROWSER;
				return 0;
			}
		}
#endif
#endif

		update_progress();

		show_progress(buf);
		return 1;
	}
#endif
	else
	{
		sprintf(error_mes, TEXT(COULD_NOT_OPEN_STATE_FILE), game_name, slot);
	}

#if (EMU_SYSTEM == NCDZ)
error:
#endif
	show_progress(error_mes);
	pad_wait_press(PAD_WAIT_INFINITY);

	return 0;
}


/*------------------------------------------------------
	�T���l�C���쐬
------------------------------------------------------*/

void state_make_thumbnail(void)
{
	uint16_t *tex = UI_TEXTURE;

	{
#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
		RECT clip1 = { 64, 16, 64 + 384, 16 + 224 };

		if (machine_screen_type)
		{
			RECT clip2 = { 152, 0, 152 + 112, 152 };
			video_driver->copyRectRotate(video_data, work_frame, tex, &clip1, &clip2);
		}
		else
		{
			RECT clip2 = { 152, 0, 152 + 152, 112 };
			video_driver->copyRect(video_data, work_frame, tex, &clip1, &clip2);
		}
#elif (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		RECT clip1 = { 24, 16, 336, 240 };
		RECT clip2 = { 152, 0, 152 + 152, 112 };

		video_driver->copyRect(video_data, work_frame, tex, &clip1, &clip2);
#endif
	}
}


/*------------------------------------------------------
	�T���l�C���ǂݍ���
------------------------------------------------------*/

int state_load_thumbnail(int slot)
{
	FILE *fp;
	char path[PATH_MAX];

	clear_thumbnail();

	sprintf(path, "%sstate/%s.sv%d", launchDir, game_name, slot);

	if ((fp = fopen(path, "rb")) != NULL)
	{
		stateTime t;

		memset(stver_str, 0, 16);

		fread(stver_str, 1, 8, fp);
		fread(&t, 1, 16, fp);
		load_thumbnail(fp);
		fclose(fp);

		current_state_version = current_version_str[7] - '0';
		state_version = stver_str[7] - '0';

		sprintf(date_str, "%04d/%02d/%02d", t.year, t.month, t.day);
		sprintf(time_str, "%02d:%02d:%02d", t.hour, t.minutes, t.seconds);

		return 1;
	}

	ui_popup(TEXT(COULD_NOT_OPEN_STATE_FILE), game_name, slot);

	return 0;
}


/*------------------------------------------------------
	�T���l�C������
------------------------------------------------------*/

void state_clear_thumbnail(void)
{
	strcpy(date_str, "----/--/--");
	strcpy(time_str, "--:--:--");
	strcpy(stver_str, "--------");

	state_version = 0;

	clear_thumbnail();
}

/******************************************************************************
	AdHoc�p�X�e�[�g����M�֐�
******************************************************************************/

#ifdef ADHOC

/*
	�f�[�^�T�C�Y�� ((���f�[�^�T�C�Y / 0x3ff) + 1) * 0x3ff �ŋ��߂邱��

	0x3ff = 0x400 bytes (���M�o�b�t�@�T�C�Y) - 1 byte(�f�[�^���ʃR�[�h)
*/

#if (EMU_SYSTEM == CPS1)
#define ADHOC_STATE_SIZE	0x452eb		// CPS1 adhoc: 0x450d3
#elif (EMU_SYSTEM == CPS2)
#define ADHOC_STATE_SIZE	0x4b2d3		// CPS2 adhoc: 0x4b1f7
#elif (EMU_SYSTEM == MVS)
#define ADHOC_STATE_SIZE	0x46ee4		// MVS adhoc: 0x46da2
#endif

/*------------------------------------------------------
	�X�e�[�g���M
------------------------------------------------------*/

int adhoc_send_state(uint32_t *frame)
{
	int error = 0;
	int retry_count = 10;

	state_buffer = state_buffer_base;

	memset(state_buffer, 0, STATE_BUFFER_SIZE);

	if (frame != NULL)
		*(uint32_t *)state_buffer = *frame;

	state_buffer += 4;

	state_save_memory();
	state_save_m68000();
	state_save_z80();
	state_save_input();
	state_save_timer();
	state_save_driver();
	state_save_video();

#if (EMU_SYSTEM == CPS1)
	state_save_coin();
	switch (machine_driver_type)
	{
	case MACHINE_qsound:
		state_save_qsound();
		state_save_eeprom();
		break;

	case MACHINE_pang3:
		state_save_eeprom();

	default:
		state_save_ym2151();
		break;
	}

#elif (EMU_SYSTEM == CPS2)
	state_save_coin();
	state_save_qsound();
	state_save_eeprom();

#elif (EMU_SYSTEM == MVS)
	state_save_ym2610();
	state_save_pd4990a();
#endif

#if 0
	{
		int size = (uint32_t)state_buffer - (uint32_t)state_buffer_base;
		ui_popup("size = %08x (%08x)", size, ((size / 0x3ff) + 1) * 0x3ff);
	}
#endif

retry:
	adhocWait(ADHOC_DATASIZE_SYNC);
	if (adhocSync() < 0)
	{
		return 0;
	}
	if ((error = adhocSendRecvAck(state_buffer_base, ADHOC_STATE_SIZE, ADHOC_TIMEOUT, ADHOC_DATATYPE_STATE)) != ADHOC_STATE_SIZE)
	{
		if (error == (int)0x80410715)	// �^�C���A�E�g
		{
			if (Loop != LOOP_EXEC) return 1;
			if (--retry_count) goto retry;
		}
		return 0;
	}

	return 1;
}


/*------------------------------------------------------
	�X�e�[�g��M
------------------------------------------------------*/

int adhoc_recv_state(uint32_t *frame)
{
	int error = 0;
	int retry_count = 10;

retry:
	adhocWait(ADHOC_DATASIZE_SYNC);
	if (adhocSync() < 0)
	{
		return 0;
	}

	adhocWait(ADHOC_DATASIZE_STATE);
	if ((error = adhocRecvSendAck(state_buffer_base, ADHOC_STATE_SIZE, ADHOC_TIMEOUT, ADHOC_DATATYPE_STATE)) != ADHOC_STATE_SIZE)
	{
		if (error == -1)
		{
			if (Loop != LOOP_EXEC) return 1;
			goto retry;	// �f�[�^�^�C�v�s��v
		}
		else if (error == (int)0x80410715)	// �^�C���A�E�g
		{
			if (Loop != LOOP_EXEC) return 1;
			if (--retry_count) goto retry;
		}
		return 0;
	}

	state_buffer = state_buffer_base;

	if (frame != NULL)
		*frame = *(uint32_t *)state_buffer;

	state_buffer += 4;

	state_load_memory();
	state_load_m68000();
	state_load_z80();
	state_load_input();
	state_load_timer();
	state_load_driver();
	state_load_video();

#if (EMU_SYSTEM == CPS1)
	state_load_coin();
	switch (machine_driver_type)
	{
	case MACHINE_qsound:
		state_load_qsound();
		state_load_eeprom();
		break;

	case MACHINE_pang3:
		state_load_eeprom();

	default:
		state_load_ym2151();
		break;
	}

#elif (EMU_SYSTEM == CPS2)
	state_load_coin();
	state_load_qsound();
	state_load_eeprom();

#elif (EMU_SYSTEM == MVS)
	state_load_ym2610();
	state_load_pd4990a();
#endif

	if (adhoc_server)
		option_controller = INPUT_PLAYER1;
	else
		option_controller = INPUT_PLAYER2;

	return 1;
}

#endif

#endif /* SAVE_STATE */
