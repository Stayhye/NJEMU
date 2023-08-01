/******************************************************************************

	ui.c

	��`�����󥿥ե��`���I��

******************************************************************************/

#ifndef PSP_UI_H
#define PSP_UI_H

#define UI_FULL_REFRESH		1
#define UI_PARTIAL_REFRESH	2

/*------------------------------------------------------
	���������ʾ
------------------------------------------------------*/

#if !VIDEO_32BPP
enum
{
	WP_LOGO = 0,
	WP_FILER,
	WP_GAMECFG,
	WP_KEYCFG,
	WP_STATE,
	WP_COLORCFG,
	WP_DIPSW,
	WP_CMDLIST,
	NUM_WALLPAPERS
};
#endif

enum
{
	BG_DEFAULT = 0,
	BG_USER,
	BG_LOGOONLY,
	BG_DISABLE,
	BG_MAX
};

#if VIDEO_32BPP
extern int bgimage_type;
extern int bgimage_blightness;
#endif

void load_background(int number);
void show_background(void);


/*------------------------------------------------------
	�Хåƥ�`״�B��ʾ
------------------------------------------------------*/

int draw_battery_status(int draw);


/*------------------------------------------------------
	�ᥤ��ܥ��`���ʾ
------------------------------------------------------*/

int draw_volume_status(int draw);


/*------------------------------------------------------
	���������ܥå�����ʾ
------------------------------------------------------*/

void draw_dialog(int sx, int sy, int ex, int ey);


/*------------------------------------------------------
	������`��Щ`��ʾ
------------------------------------------------------*/

void draw_scrollbar(int sx, int sy, int ex, int ey, int disp_lines, int total_lines, int current_line);


/*------------------------------------------------------
	�ݥåץ��åץ�å��`����ʾ
------------------------------------------------------*/

void ui_popup_reset(void);
void ui_popup(const char *text, ...);
int ui_show_popup(int draw);


/*------------------------------------------------------
	��ʽ�������б�ʾ
------------------------------------------------------*/

void msg_screen_init(int wallpaper, int icon, const char *title);
void msg_screen_clear(void);
void msg_set_text_color(uint32_t color);
void msg_printf(const char *text, ...);



/*------------------------------------------------------
	�ץ��쥹�Щ`��ʾ
------------------------------------------------------*/

void init_progress(int total, const char *text);
void update_progress(void);
void show_progress(const char *text);


/*--------------------------------------------------------
	��å��`���ܥå�����ʾ
--------------------------------------------------------*/

enum
{
	MB_STARTEMULATION = 0,
#ifdef ADHOC
	MB_STARTEMULATION_ADHOC,
#endif
	MB_EXITEMULATION,
	MB_RETURNTOFILEBROWSER,
	MB_RESETEMULATION,
	MB_RESTARTEMULATION,
#if (EMU_SYSTEM != NCDZ)
	MB_GAMENOTWORK,
#endif
	MB_SETSTARTUPDIR,
#ifdef LARGE_MEMORY
	MB_PSPVERSIONERROR,
#endif
#ifdef SAVE_STATE
	MB_STARTSAVESTATE,
	MB_FINISHSAVESTATE,
	MB_STARTLOADSTATE,
	MB_FINISHLOADSTATE,
	MB_DELETESTATE,
#endif
#if (EMU_SYSTEM == NCDZ)
	MB_STARTEMULATION_NOMP3,
	MB_BOOTBIOS,
	MB_BIOSNOTFOUND,
	MB_BIOSINVALID,
#endif
	MB_NUM_MAX
};

int messagebox(int number);


/*--------------------------------------------------------
	�إ�ױ�ʾ
--------------------------------------------------------*/

enum
{
	HELP_FILEBROWSER = 0,
	HELP_MAINMENU,
#if (EMU_SYSTEM == MVS)
	HELP_SELECTBIOS,
#endif
	HELP_GAMECONFIG,
	HELP_KEYCONFIG,
#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == MVS)
	HELP_DIPSWITCH,
#endif
#ifdef SAVE_STATE
	HELP_STATE,
#endif
#if VIDEO_32BPP
	HELP_COLORSETTINGS,
#endif
#ifdef COMMAND_LIST
	HELP_COMMANDLIST,
#endif
	HELP_CHEATCONFIG,
	HELP_NUM_MAX
};

int help(int number);


#endif /* PSP_UI_H */
