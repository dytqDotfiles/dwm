/* See LICENSE file for copyright and license details. */

#include "fibonacci.c"
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=15","fontawesome:size:15","JoyPixels:pixelsize=15:antialias=true:autohint=true" };
static const char dmenufont[]       = "monospace:size=15";
/* default color */
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
/*retro color scheme */
static const char col_backW[]       = "#202732";
static const char col_bordunfocW[]  = "#526667";
static const char col_bordfocusW[]  = "#8F6A61";
static const char col_bar[]         = "#8CD2EE";
static const char col_txt[]         = "#FFFFFF";
static const char col_bartxt[]      = "#FFFFFF";

static const char *colors[][3]      = {
	/*               fg          bg         border   */
	[SchemeNorm] = { col_bartxt, col_backW, col_bordunfocW },
	[SchemeSel]  = { col_txt,    col_bar,   col_bordfocusW },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[T]",      tile },    /* first entry is default */
	{ "[F]",      NULL },    /* no layout function means floating behavior */
	{ "[@]",      spiral }, /* fibo */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, "-l", "20", NULL };
static const char *termcmd[]  = { "alacritty", NULL };

/* microphone */
static const char *mic_mut[] = { "amixer", "set", "Capture", "toggle", NULL};
static const char *mic_register[] = {NULL}; //to do
/* lock screen */
static const char *lockscr[] = { "slock",NULL};

/* Special Keys define XF86 lib */
#define XK_AudioM XF86XK_AudioMute 
#define XK_AudioL XF86XK_AudioLowerVolume 
#define XK_AudioR XF86XK_AudioRaiseVolume	
#define XK_BriUp  XF86XK_MonBrightnessUp 
#define XK_BriDo  XF86XK_MonBrightnessDown
#define XK_Mic    XF86XK_AudioMicMute

/* install brightnessctl */ 

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ 0,				XK_AudioM, spawn,          SHCMD("sh ~/.scripts/volume_mute.sh")},
       	{ 0, 				XK_AudioL, spawn, 	   SHCMD("sh ~/.scripts/volume_down.sh")},
	{ 0,				XK_AudioR, spawn, 	   SHCMD("sh ~/.scripts/volume_up.sh")},	
	{ MODKEY,			XK_AudioM, spawn,          SHCMD("mpc prev")},
       	{ MODKEY,			XK_AudioL, spawn, 	   SHCMD("mpc toggle")},
	{ MODKEY,			XK_AudioR, spawn, 	   SHCMD("mpc next")},
	{ MODKEY|ShiftMask,		XK_AudioL, spawn,	   SHCMD("mpc stop")},	
	{ 0,	 			XK_BriUp,  spawn, 	   SHCMD("brightnessctl --quiet set +10%")},
	{ 0, 				XK_BriDo,  spawn, 	   SHCMD("brightnessctl --quiet set 10%-")},
	{ 0, 				XK_Print,  spawn,	   SHCMD("scrot -d0 ~/Image/%Y-%m-%d-%s_$wx$h.jpg") }, 
	{ 0,				XK_Mic,    spawn,	   {.v = mic_mut } },
	{ MODKEY,			XK_Mic,    spawn,	   {.v = mic_register } }, 
	{ Mod4Mask,			XK_l,      spawn,	   {.v = lockscr } },
	{ MODKEY|ShiftMask,		XK_s,	   spawn,	   SHCMD("~/.scripts/dmenu/shutdown.sh") },
	{ MODKEY|ShiftMask,		XK_r,	   spawn, 	   SHCMD("~/.scripts/dmenu/reboot.sh") },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	//{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_k,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0 } },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_s,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_ampersand,                     0)
	TAGKEYS(                        XK_eacute,                        1)
	TAGKEYS(                        XK_quotedbl,                      2)
	TAGKEYS(                        XK_apostrophe,                    3)
	TAGKEYS(                        XK_parenleft,                     4)
	TAGKEYS(                        XK_minus,                         5)
	TAGKEYS(                        XK_egrave,                        6)
	TAGKEYS(                        XK_underscore,                    7)
	TAGKEYS(                        XK_ccedilla,                      8)
	
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};
/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

