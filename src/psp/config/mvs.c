/******************************************************************************

	config/mvs.c

	���ץꥱ�`������O���ե�������� (MVS)

******************************************************************************/

#if defined(INCLUDE_INIFILENAME)

/******************************************************************************
	ini�ե�������
******************************************************************************/

static const char *inifile_name = "mvspsp.ini";

#elif defined(INCLUDE_CONFIG_STRUCT)

#define DEFAULT_SAMPLERATE	1	// 22050Hz

/******************************************************************************
	������
******************************************************************************/

static cfg_type gamecfg_mvs[] =
{
	{ CFG_NONE,	"[System Settings]", },
	{ CFG_INT,	"PSPClock",				&platform_cpuclock,	PSPCLOCK_333,	PSPCLOCK_333 },

	{ CFG_NONE,	"[Emulation Settings]", },
	{ CFG_INT,	"NeogeoRegion",			&neogeo_region,			0,	3	},
	{ CFG_INT,	"NeogeoMachineMode",	&neogeo_machine_mode,	0,	2	},
	{ CFG_INT,	"RasterEnable",			&neogeo_raster_enable,	0,	1	},

	{ CFG_NONE,	"[Video Settings]", },
	{ CFG_INT,	"StretchScreen",		&option_stretch,		4,	5	},
	{ CFG_BOOL,	"VideoSync",			&option_vsync,			0,	1	},
	{ CFG_BOOL,	"AutoFrameSkip",		&option_autoframeskip,	0,	1	},
	{ CFG_INT,	"FrameSkipLevel",		&option_frameskip,		0,	11	},
	{ CFG_BOOL,	"ShowFPS",				&option_showfps,		0,	1	},
	{ CFG_BOOL,	"60FPSLimit",			&option_speedlimit,		1,	1	},

	{ CFG_NONE,	"[Audio Settings]", },
	{ CFG_BOOL,	"EnableSound",			&option_sound_enable,	1,	1	},
	{ CFG_INT,	"SampleRate",			&option_samplerate,		DEFAULT_SAMPLERATE,	2	},
	{ CFG_INT,	"SoundVolume",			&option_sound_volume,	10,	10	},

	{ CFG_NONE,	"[Input Settings]", },
	{ CFG_INT,	"Controller",			&option_controller,		0,	1	},

	{ CFG_NONE,	"[NEOGEO Settings]", },
	{ CFG_PAD,	"Up",			&input_map[P1_UP],		PLATFORM_PAD_UP,		0	},
	{ CFG_PAD,	"Down",			&input_map[P1_DOWN],	PLATFORM_PAD_DOWN,		0	},
	{ CFG_PAD,	"Left",			&input_map[P1_LEFT],	PLATFORM_PAD_LEFT,		0	},
	{ CFG_PAD,	"Right",		&input_map[P1_RIGHT],	PLATFORM_PAD_RIGHT,		0	},
	{ CFG_PAD,	"ButtonA",		&input_map[P1_BUTTONA],PLATFORM_PAD_B2,	    0	},
	{ CFG_PAD,	"ButtonB",		&input_map[P1_BUTTONB],PLATFORM_PAD_B1,    0	},
	{ CFG_PAD,	"ButtonC",		&input_map[P1_BUTTONC],PLATFORM_PAD_B3,	0	},
	{ CFG_PAD,	"ButtonD",		&input_map[P1_BUTTOND],PLATFORM_PAD_B4,	0	},
	{ CFG_PAD,	"Start",		&input_map[P1_START],	PLATFORM_PAD_START,		0	},
	{ CFG_PAD,	"Coin",			&input_map[P1_COIN],	PLATFORM_PAD_SELECT,	0	},

	{ CFG_NONE,	"[Switch Settings]", },
	{ CFG_PAD,	"ServiceCoin",	&input_map[SERV_COIN],		0,		0		},
	{ CFG_PAD,	"TestSwitch",	&input_map[TEST_SWITCH],	0,		0		},
	{ CFG_INT,	"DipSwitch",	&neogeo_dipswitch,			0xff,	0xff	},
	{ CFG_INT,	"HardDipSwitch",&neogeo_hard_dipsw,			0,		1		},

	{ CFG_NONE,	"[Autofire Settings]", },
	{ CFG_PAD,	"AutofireA",	&input_map[P1_AF_A],	0,	0	},
	{ CFG_PAD,	"AutofireB",	&input_map[P1_AF_B],	0,	0	},
	{ CFG_PAD,	"AutofireC",	&input_map[P1_AF_C],	0,	0	},
	{ CFG_PAD,	"AutofireD",	&input_map[P1_AF_D],	0,	0	},
	{ CFG_INT,	"AFInterval",	&af_interval,			2,	10	},

	{ CFG_NONE,	"[Hotkey Settings]", },
	{ CFG_PAD,	"HotkeyAB",		&input_map[P1_AB],		0,	0	},
	{ CFG_PAD,	"HotkeyAC",		&input_map[P1_AC],		0,	0	},
	{ CFG_PAD,	"HotkeyAD",		&input_map[P1_AD],		0,	0	},
	{ CFG_PAD,	"HotkeyBC",		&input_map[P1_BC],		0,	0	},
	{ CFG_PAD,	"HotkeyBD",		&input_map[P1_BD],		0,	0	},
	{ CFG_PAD,	"HotkeyCD",		&input_map[P1_CD],		0,	0	},
	{ CFG_PAD,	"HotkeyABC",	&input_map[P1_ABC],	0,	0	},
	{ CFG_PAD,	"HotkeyABD",	&input_map[P1_ABD],	0,	0	},
	{ CFG_PAD,	"HotkeyACD",	&input_map[P1_ACD],	0,	0	},
	{ CFG_PAD,	"HotkeyBCD",	&input_map[P1_BCD],	0,	0	},
	{ CFG_PAD,	"HotkeyABCD",	&input_map[P1_ABCD],	0,	0	},

	{ CFG_NONE,	"[System Key Settings]", },
	{ CFG_PAD,	"Snapshot",		&input_map[SNAPSHOT],		0,	0	},
	{ CFG_PAD,	"SwitchPlayer",	&input_map[SWPLAYER],		0,	0	},
	{ CFG_PAD,	"Commandlist",	&input_map[COMMANDLIST],	0,	0	},

	{ CFG_NONE, NULL, }
};

static cfg_type gamecfg_irrmaze[] =
{
	{ CFG_NONE,	"[System Settings]", },
	{ CFG_INT,	"PSPClock",				&platform_cpuclock,	PSPCLOCK_333,	PSPCLOCK_333 },

	{ CFG_NONE,	"[Emulation Settings]", },
	{ CFG_INT,	"NeogeoRegion",			&neogeo_region,			0,	3	},
	{ CFG_INT,	"NeogeoMachineMode",	&neogeo_machine_mode,	0,	2	},
	{ CFG_INT,	"RasterEnable",			&neogeo_raster_enable,	0,	1	},

	{ CFG_NONE,	"[Video Settings]", },
	{ CFG_INT,	"StretchScreen",		&option_stretch,		4,	5	},
	{ CFG_BOOL,	"VideoSync",			&option_vsync,			0,	0	},
	{ CFG_BOOL,	"AutoFrameSkip",		&option_autoframeskip,	0,	1	},
	{ CFG_INT,	"FrameSkipLevel",		&option_frameskip,		0,	11	},
	{ CFG_BOOL,	"ShowFPS",				&option_showfps,		0,	1	},
	{ CFG_BOOL,	"60FPSLimit",			&option_speedlimit,		1,	1	},

	{ CFG_NONE,	"[Audio Settings]", },
	{ CFG_BOOL,	"EnableSound",			&option_sound_enable,	1,	1	},
	{ CFG_INT,	"SampleRate",			&option_samplerate,		DEFAULT_SAMPLERATE,	2	},
	{ CFG_INT,	"SoundVolume",			&option_sound_volume,	10,	10	},

	{ CFG_NONE,	"[Input Settings]", },
	{ CFG_INT,	"Controller",			&option_controller,		0,	1	},

	{ CFG_NONE,	"[NEOGEO Settings]", },
	{ CFG_PAD,	"Up",			&input_map[P1_UP],		PLATFORM_PAD_UP,		0	},
	{ CFG_PAD,	"Down",			&input_map[P1_DOWN],	PLATFORM_PAD_DOWN,		0	},
	{ CFG_PAD,	"Left",			&input_map[P1_LEFT],	PLATFORM_PAD_LEFT,		0	},
	{ CFG_PAD,	"Right",		&input_map[P1_RIGHT],	PLATFORM_PAD_RIGHT,		0	},
	{ CFG_PAD,	"ButtonA",		&input_map[P1_BUTTONA],PLATFORM_PAD_B2,		0	},
	{ CFG_PAD,	"ButtonB",		&input_map[P1_BUTTONB],PLATFORM_PAD_B1,	0	},
	{ CFG_PAD,	"ButtonC",		&input_map[P1_BUTTONC],PLATFORM_PAD_B3,	0	},
	{ CFG_PAD,	"ButtonD",		&input_map[P1_BUTTOND],PLATFORM_PAD_B4,	0	},
	{ CFG_PAD,	"Start",		&input_map[P1_START],	PLATFORM_PAD_START,		0	},
	{ CFG_PAD,	"Coin",			&input_map[P1_COIN],	PLATFORM_PAD_SELECT,	0	},
	{ CFG_INT,	"Sensitivity",	&analog_sensitivity,	1,					2	},

	{ CFG_NONE,	"[Switch Settings]", },
	{ CFG_PAD,	"ServiceCoin",	&input_map[SERV_COIN],		0,		0		},
	{ CFG_PAD,	"TestSwitch",	&input_map[TEST_SWITCH],	0,		0		},
	{ CFG_INT,	"DipSwitch",	&neogeo_dipswitch,			0xff,	0xff	},
	{ CFG_INT,	"HardDipSwitch",&neogeo_hard_dipsw,			0,		1		},

	{ CFG_NONE,	"[Autofire Settings]", },
	{ CFG_PAD,	"AutofireA",	&input_map[P1_AF_A],	0,	0	},
	{ CFG_PAD,	"AutofireB",	&input_map[P1_AF_B],	0,	0	},
	{ CFG_PAD,	"AutofireC",	&input_map[P1_AF_C],	0,	0	},
	{ CFG_PAD,	"AutofireD",	&input_map[P1_AF_D],	0,	0	},
	{ CFG_INT,	"AFInterval",	&af_interval,			2,	10	},

	{ CFG_NONE,	"[Hotkey Settings]", },
	{ CFG_PAD,	"HotkeyAB",		&input_map[P1_AB],		0,	0	},
	{ CFG_PAD,	"HotkeyAC",		&input_map[P1_AC],		0,	0	},
	{ CFG_PAD,	"HotkeyAD",		&input_map[P1_AD],		0,	0	},
	{ CFG_PAD,	"HotkeyBC",		&input_map[P1_BC],		0,	0	},
	{ CFG_PAD,	"HotkeyBD",		&input_map[P1_BD],		0,	0	},
	{ CFG_PAD,	"HotkeyCD",		&input_map[P1_CD],		0,	0	},
	{ CFG_PAD,	"HotkeyABC",	&input_map[P1_ABC],	0,	0	},
	{ CFG_PAD,	"HotkeyABD",	&input_map[P1_ABD],	0,	0	},
	{ CFG_PAD,	"HotkeyACD",	&input_map[P1_ACD],	0,	0	},
	{ CFG_PAD,	"HotkeyBCD",	&input_map[P1_BCD],	0,	0	},
	{ CFG_PAD,	"HotkeyABCD",	&input_map[P1_ABCD],	0,	0	},

	{ CFG_NONE,	"[System Key Settings]", },
	{ CFG_PAD,	"Snapshot",		&input_map[SNAPSHOT],		0,	0	},
	{ CFG_PAD,	"SwitchPlayer",	&input_map[SWPLAYER],		0,	0	},
	{ CFG_PAD,	"Commandlist",	&input_map[COMMANDLIST],	0,	0	},

	{ CFG_NONE, NULL, }
};

static cfg_type gamecfg_popbounc[] =
{
	{ CFG_NONE,	"[System Settings]", },
	{ CFG_INT,	"PSPClock",				&platform_cpuclock,	PSPCLOCK_333,	PSPCLOCK_333 },

	{ CFG_NONE,	"[Emulation Settings]", },
	{ CFG_INT,	"NeogeoRegion",			&neogeo_region,			0,	3	},
	{ CFG_INT,	"NeogeoMachineMode",	&neogeo_machine_mode,	0,	2	},
	{ CFG_INT,	"RasterEnable",			&neogeo_raster_enable,	0,	1	},

	{ CFG_NONE,	"[Video Settings]", },
	{ CFG_INT,	"StretchScreen",		&option_stretch,		4,	5	},
	{ CFG_BOOL,	"VideoSync",			&option_vsync,			0,	0	},
	{ CFG_BOOL,	"AutoFrameSkip",		&option_autoframeskip,	0,	1	},
	{ CFG_INT,	"FrameSkipLevel",		&option_frameskip,		0,	11	},
	{ CFG_BOOL,	"ShowFPS",				&option_showfps,		0,	1	},
	{ CFG_BOOL,	"60FPSLimit",			&option_speedlimit,		1,	1	},

	{ CFG_NONE,	"[Audio Settings]", },
	{ CFG_BOOL,	"EnableSound",			&option_sound_enable,	1,	1	},
	{ CFG_INT,	"SampleRate",			&option_samplerate,		DEFAULT_SAMPLERATE,	2	},
	{ CFG_INT,	"SoundVolume",			&option_sound_volume,	10,	10	},

	{ CFG_NONE,	"[Input Settings]", },
	{ CFG_INT,	"Controller",			&option_controller,		0,	1	},

	{ CFG_NONE,	"[NEOGEO Settings]", },
	{ CFG_PAD,	"Up",			&input_map[P1_UP],		PLATFORM_PAD_UP,		0	},
	{ CFG_PAD,	"Down",			&input_map[P1_DOWN],	PLATFORM_PAD_DOWN,		0	},
	{ CFG_PAD,	"Left",			&input_map[P1_LEFT],	PLATFORM_PAD_LEFT,		0	},
	{ CFG_PAD,	"Right",		&input_map[P1_RIGHT],	PLATFORM_PAD_RIGHT,		0	},
	{ CFG_PAD,	"ButtonA",		&input_map[P1_BUTTONA],PLATFORM_PAD_B2,		0	},
	{ CFG_PAD,	"ButtonB",		&input_map[P1_BUTTONB],PLATFORM_PAD_B1,	0	},
	{ CFG_PAD,	"ButtonC",		&input_map[P1_BUTTONC],PLATFORM_PAD_B3,	0	},
	{ CFG_PAD,	"ButtonD",		&input_map[P1_BUTTOND],PLATFORM_PAD_B4,	0	},
	{ CFG_PAD,	"Start",		&input_map[P1_START],	PLATFORM_PAD_START,		0	},
	{ CFG_PAD,	"Coin",			&input_map[P1_COIN],	PLATFORM_PAD_SELECT,	0	},
	{ CFG_INT,	"Sensitivity",	&analog_sensitivity,	1,					2	},

	{ CFG_NONE,	"[Switch Settings]", },
	{ CFG_PAD,	"ServiceCoin",	&input_map[SERV_COIN],		0,		0		},
	{ CFG_PAD,	"TestSwitch",	&input_map[TEST_SWITCH],	0,		0		},
	{ CFG_INT,	"DipSwitch",	&neogeo_dipswitch,			0xff,	0xff	},
	{ CFG_INT,	"HardDipSwitch",&neogeo_hard_dipsw,			0,		1		},

	{ CFG_NONE,	"[Autofire Settings]", },
	{ CFG_PAD,	"AutofireA",	&input_map[P1_AF_A],	0,	0	},
	{ CFG_PAD,	"AutofireB",	&input_map[P1_AF_B],	0,	0	},
	{ CFG_PAD,	"AutofireC",	&input_map[P1_AF_C],	0,	0	},
	{ CFG_PAD,	"AutofireD",	&input_map[P1_AF_D],	0,	0	},
	{ CFG_INT,	"AFInterval",	&af_interval,			2,	10	},

	{ CFG_NONE,	"[Hotkey Settings]", },
	{ CFG_PAD,	"HotkeyAB",		&input_map[P1_AB],		0,	0	},
	{ CFG_PAD,	"HotkeyAC",		&input_map[P1_AC],		0,	0	},
	{ CFG_PAD,	"HotkeyAD",		&input_map[P1_AD],		0,	0	},
	{ CFG_PAD,	"HotkeyBC",		&input_map[P1_BC],		0,	0	},
	{ CFG_PAD,	"HotkeyBD",		&input_map[P1_BD],		0,	0	},
	{ CFG_PAD,	"HotkeyCD",		&input_map[P1_CD],		0,	0	},
	{ CFG_PAD,	"HotkeyABC",	&input_map[P1_ABC],	0,	0	},
	{ CFG_PAD,	"HotkeyABD",	&input_map[P1_ABD],	0,	0	},
	{ CFG_PAD,	"HotkeyACD",	&input_map[P1_ACD],	0,	0	},
	{ CFG_PAD,	"HotkeyBCD",	&input_map[P1_BCD],	0,	0	},
	{ CFG_PAD,	"HotkeyABCD",	&input_map[P1_ABCD],	0,	0	},

	{ CFG_NONE,	"[System Key Settings]", },
	{ CFG_PAD,	"Snapshot",		&input_map[SNAPSHOT],		0,	0	},
	{ CFG_PAD,	"SwitchPlayer",	&input_map[SWPLAYER],		0,	0	},
	{ CFG_PAD,	"Commandlist",	&input_map[COMMANDLIST],	0,	0	},

	{ CFG_NONE, NULL, }
};

static cfg_type gamecfg_vliner[] =
{
	{ CFG_NONE,	"[System Settings]", },
	{ CFG_INT,	"PSPClock",				&platform_cpuclock,	PSPCLOCK_333,	PSPCLOCK_333 },

	{ CFG_NONE,	"[Emulation Settings]", },
	{ CFG_INT,	"NeogeoRegion",			&neogeo_region,			0,	3	},
	{ CFG_INT,	"NeogeoMachineMode",	&neogeo_machine_mode,	0,	2	},
	{ CFG_INT,	"RasterEnable",			&neogeo_raster_enable,	0,	1	},

	{ CFG_NONE,	"[Video Settings]", },
	{ CFG_INT,	"StretchScreen",		&option_stretch,		4,	5	},
	{ CFG_BOOL,	"VideoSync",			&option_vsync,			0,	0	},
	{ CFG_BOOL,	"AutoFrameSkip",		&option_autoframeskip,	0,	1	},
	{ CFG_INT,	"FrameSkipLevel",		&option_frameskip,		0,	11	},
	{ CFG_BOOL,	"ShowFPS",				&option_showfps,		0,	1	},
	{ CFG_BOOL,	"60FPSLimit",			&option_speedlimit,		1,	1	},

	{ CFG_NONE,	"[Audio Settings]", },
	{ CFG_BOOL,	"EnableSound",			&option_sound_enable,	1,	1	},
	{ CFG_INT,	"SampleRate",			&option_samplerate,		DEFAULT_SAMPLERATE,	2	},
	{ CFG_INT,	"SoundVolume",			&option_sound_volume,	10,	10	},

	{ CFG_NONE,	"[Input Settings]", },
	{ CFG_INT,	"Controller",			&option_controller,		0,	1	},

	{ CFG_NONE,	"[NEOGEO Settings]", },
	{ CFG_PAD,	"Up",			&input_map[P1_UP],		PLATFORM_PAD_UP,		0	},
	{ CFG_PAD,	"Down",			&input_map[P1_DOWN],	PLATFORM_PAD_DOWN,		0	},
	{ CFG_PAD,	"Left",			&input_map[P1_LEFT],	PLATFORM_PAD_LEFT,		0	},
	{ CFG_PAD,	"Right",		&input_map[P1_RIGHT],	PLATFORM_PAD_RIGHT,		0	},
	{ CFG_PAD,	"Big",			&input_map[P1_BUTTONA],PLATFORM_PAD_B2,		0	},
	{ CFG_PAD,	"Small",		&input_map[P1_BUTTONB],PLATFORM_PAD_B1,	0	},
	{ CFG_PAD,	"DoubleUp",		&input_map[P1_BUTTONC],PLATFORM_PAD_B3,	0	},
	{ CFG_PAD,	"Payout",       &input_map[P1_BUTTOND],PLATFORM_PAD_B4,	0	},
	{ CFG_PAD,	"Start_Collect",&input_map[P1_START],	PLATFORM_PAD_START,		0	},
	{ CFG_PAD,	"Coin",			&input_map[P1_COIN],	PLATFORM_PAD_SELECT,	0	},

	{ CFG_NONE,	"[Switch Settings]", },
	{ CFG_PAD,	"ServiceCoin",	&input_map[SERV_COIN],		0,		0		},
	{ CFG_PAD,	"TestSwitch",	&input_map[TEST_SWITCH],	0,		0		},
	{ CFG_INT,	"DipSwitch",	&neogeo_dipswitch,			0xff,	0xff	},
	{ CFG_INT,	"HardDipSwitch",&neogeo_hard_dipsw,			0,		1		},
	{ CFG_PAD,	"OperatorMenu",	&input_map[OTHER1],		0,		0		},
	{ CFG_PAD,	"Clear Credit",	&input_map[OTHER2],		0,		0		},
	{ CFG_PAD,	"Hopper Out",	&input_map[OTHER3],		0,		0		},

	{ CFG_NONE,	"[Autofire Settings]", },
	{ CFG_PAD,	"AutofireA",	&input_map[P1_AF_A],	0,	0	},
	{ CFG_PAD,	"AutofireB",	&input_map[P1_AF_B],	0,	0	},
	{ CFG_PAD,	"AutofireC",	&input_map[P1_AF_C],	0,	0	},
	{ CFG_PAD,	"AutofireD",	&input_map[P1_AF_D],	0,	0	},
	{ CFG_INT,	"AFInterval",	&af_interval,			2,	10	},

	{ CFG_NONE,	"[Hotkey Settings]", },
	{ CFG_PAD,	"HotkeyAB",		&input_map[P1_AB],		0,	0	},
	{ CFG_PAD,	"HotkeyAC",		&input_map[P1_AC],		0,	0	},
	{ CFG_PAD,	"HotkeyAD",		&input_map[P1_AD],		0,	0	},
	{ CFG_PAD,	"HotkeyBC",		&input_map[P1_BC],		0,	0	},
	{ CFG_PAD,	"HotkeyBD",		&input_map[P1_BD],		0,	0	},
	{ CFG_PAD,	"HotkeyCD",		&input_map[P1_CD],		0,	0	},
	{ CFG_PAD,	"HotkeyABC",	&input_map[P1_ABC],	0,	0	},
	{ CFG_PAD,	"HotkeyABD",	&input_map[P1_ABD],	0,	0	},
	{ CFG_PAD,	"HotkeyACD",	&input_map[P1_ACD],	0,	0	},
	{ CFG_PAD,	"HotkeyBCD",	&input_map[P1_BCD],	0,	0	},
	{ CFG_PAD,	"HotkeyABCD",	&input_map[P1_ABCD],	0,	0	},

	{ CFG_NONE,	"[System Key Settings]", },
	{ CFG_PAD,	"Snapshot",		&input_map[SNAPSHOT],		0,	0	},
	{ CFG_PAD,	"SwitchPlayer",	&input_map[SWPLAYER],		0,	0	},
	{ CFG_PAD,	"Commandlist",	&input_map[COMMANDLIST],	0,	0	},

	{ CFG_NONE, NULL, }
};

static cfg_type gamecfg_jockeygp[] =
{
	{ CFG_NONE,	"[System Settings]", },
	{ CFG_INT,	"PSPClock",				&platform_cpuclock,	PSPCLOCK_333,	PSPCLOCK_333 },

	{ CFG_NONE,	"[Emulation Settings]", },
	{ CFG_INT,	"NeogeoRegion",			&neogeo_region,			0,	3	},
	{ CFG_INT,	"NeogeoMachineMode",	&neogeo_machine_mode,	0,	2	},
	{ CFG_INT,	"RasterEnable",			&neogeo_raster_enable,	0,	1	},

	{ CFG_NONE,	"[Video Settings]", },
	{ CFG_INT,	"StretchScreen",		&option_stretch,		4,	5	},
	{ CFG_BOOL,	"VideoSync",			&option_vsync,			0,	0	},
	{ CFG_BOOL,	"AutoFrameSkip",		&option_autoframeskip,	0,	1	},
	{ CFG_INT,	"FrameSkipLevel",		&option_frameskip,		0,	11	},
	{ CFG_BOOL,	"ShowFPS",				&option_showfps,		0,	1	},
	{ CFG_BOOL,	"60FPSLimit",			&option_speedlimit,		1,	1	},

	{ CFG_NONE,	"[Audio Settings]", },
	{ CFG_BOOL,	"EnableSound",			&option_sound_enable,	1,	1	},
	{ CFG_INT,	"SampleRate",			&option_samplerate,		DEFAULT_SAMPLERATE,	2	},
	{ CFG_INT,	"SoundVolume",			&option_sound_volume,	10,	10	},

	{ CFG_NONE,	"[Input Settings]", },
	{ CFG_INT,	"Controller",			&option_controller,		0,	1	},

	{ CFG_NONE,	"[NEOGEO Settings]", },
	{ CFG_PAD,	"Up",			&input_map[P1_UP],		PLATFORM_PAD_UP,		0	},
	{ CFG_PAD,	"Down",			&input_map[P1_DOWN],	PLATFORM_PAD_DOWN,		0	},
	{ CFG_PAD,	"Left",			&input_map[P1_LEFT],	PLATFORM_PAD_LEFT,		0	},
	{ CFG_PAD,	"Right",		&input_map[P1_RIGHT],	PLATFORM_PAD_RIGHT,		0	},
	{ CFG_PAD,	"Bet",			&input_map[P1_BUTTONA],PLATFORM_PAD_B2,		0	},
	{ CFG_PAD,	"Cancel",		&input_map[P1_BUTTONB],PLATFORM_PAD_B1,	0	},
	{ CFG_PAD,	"BetCancelAll",	&input_map[P1_BUTTONC],PLATFORM_PAD_B3,	0	},
	{ CFG_PAD,	"Payout",		&input_map[P1_BUTTOND],PLATFORM_PAD_B4,	0	},
	{ CFG_PAD,	"Start",		&input_map[P1_START],	PLATFORM_PAD_START,		0	},
	{ CFG_PAD,	"Coin",			&input_map[P1_COIN],	PLATFORM_PAD_SELECT,	0	},

	{ CFG_NONE,	"[Switch Settings]", },
	{ CFG_PAD,	"ServiceCoin",	&input_map[SERV_COIN],		0,		0		},
	{ CFG_PAD,	"TestSwitch",	&input_map[TEST_SWITCH],	0,		0		},
	{ CFG_INT,	"DipSwitch",	&neogeo_dipswitch,			0xff,	0xff	},
	{ CFG_INT,	"HardDipSwitch",&neogeo_hard_dipsw,			0,		1		},

	{ CFG_NONE,	"[Autofire Settings]", },
	{ CFG_PAD,	"AutofireA",	&input_map[P1_AF_A],	0,	0	},
	{ CFG_PAD,	"AutofireB",	&input_map[P1_AF_B],	0,	0	},
	{ CFG_PAD,	"AutofireC",	&input_map[P1_AF_C],	0,	0	},
	{ CFG_PAD,	"AutofireD",	&input_map[P1_AF_D],	0,	0	},
	{ CFG_INT,	"AFInterval",	&af_interval,			2,	10	},

	{ CFG_NONE,	"[Hotkey Settings]", },
	{ CFG_PAD,	"HotkeyAB",		&input_map[P1_AB],		0,	0	},
	{ CFG_PAD,	"HotkeyAC",		&input_map[P1_AC],		0,	0	},
	{ CFG_PAD,	"HotkeyAD",		&input_map[P1_AD],		0,	0	},
	{ CFG_PAD,	"HotkeyBC",		&input_map[P1_BC],		0,	0	},
	{ CFG_PAD,	"HotkeyBD",		&input_map[P1_BD],		0,	0	},
	{ CFG_PAD,	"HotkeyCD",		&input_map[P1_CD],		0,	0	},
	{ CFG_PAD,	"HotkeyABC",	&input_map[P1_ABC],	0,	0	},
	{ CFG_PAD,	"HotkeyABD",	&input_map[P1_ABD],	0,	0	},
	{ CFG_PAD,	"HotkeyACD",	&input_map[P1_ACD],	0,	0	},
	{ CFG_PAD,	"HotkeyBCD",	&input_map[P1_BCD],	0,	0	},
	{ CFG_PAD,	"HotkeyABCD",	&input_map[P1_ABCD],	0,	0	},

	{ CFG_NONE,	"[System Key Settings]", },
	{ CFG_PAD,	"Snapshot",		&input_map[SNAPSHOT],		0,	0	},
	{ CFG_PAD,	"SwitchPlayer",	&input_map[SWPLAYER],		0,	0	},
	{ CFG_PAD,	"Commandlist",	&input_map[COMMANDLIST],	0,	0	},

	{ CFG_NONE, NULL, }
};

#elif defined(INCLUDE_SETUP_CONFIG_STRUCT)

/******************************************************************************
	config��������O��
******************************************************************************/

	switch (neogeo_ngh)
	{
	case NGH_irrmaze:
		gamecfg = gamecfg_irrmaze;
		break;

	case NGH_popbounc:
		gamecfg = gamecfg_popbounc;
		break;

	case NGH_vliner:
		gamecfg = gamecfg_vliner;
		break;

	case NGH_jockeygp:
		gamecfg = gamecfg_jockeygp;
		break;

	default:
		gamecfg = gamecfg_mvs;
		break;
	}

#endif
