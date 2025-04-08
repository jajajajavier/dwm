/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* alt-tab configuration */
static const unsigned int tabModKey 		= 0x40;	/* if this key is hold the alt-tab functionality stays acitve. This key must be the same as key that is used to active functin altTabStart `*/
static const unsigned int tabCycleKey 		= 0x17;	/* if this key is hit the alt-tab program moves one position forward in clients stack. This key must be the same as key that is used to active functin altTabStart */
static const unsigned int tabPosY 			= 1;	/* tab position on Y axis, 0 = bottom, 1 = center, 2 = top */
static const unsigned int tabPosX 			= 1;	/* tab position on X axis, 0 = left, 1 = center, 2 = right */
static const unsigned int maxWTab 			= 400;	/* tab menu width */
static const unsigned int maxHTab 			= 200;	/* tab menu height */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 64;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft  = 0;   /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 8;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int vertpad            = 0;       /* vertical padding of bar */
static const int sidepad            = 0;       /* horizontal padding of bar */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int horizpadbar        = 6;        /* horizontal padding for statusbar */
static const int vertpadbar         = 12;        /* vertical padding for statusbar */
static const char *fonts[]          = { "JetBrainsMonoNF:size=11.5" };
static const char dmenufont[]       = "JetBrainsNF:size=11";
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */

static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";

static const char col_black[]       = "#5c6370";
static const char col_blue[]        = "#61afef";
static const char col_cyan[]        = "#56b6c2";
static const char col_green[]       = "#98c379";
static const char col_magenta[]     = "#c678dd";
static const char col_red[]         = "#e06c75";
static const char col_yellow[]      = "#d19a66";
static const char col_white[]       = "#ffffff";

static const char col_background[]  = "#282c34";
static const char col_foreground[]  = "#abb2bf";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_foreground, col_background, col_background },
	[SchemeSel]  = { col_cyan, col_gray2,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "", "󰈹", "", "󰈙", "", "", "", "󰱒", ""};

static const unsigned int ulinepad	= 5;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 3;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 0;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

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
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	// { ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "kitty", NULL };

static const char *rofi_launcher[]  = { "/home/javier/.local/bin/rofi-launcher", NULL };
static const char *rofi_power[]  = { "/home/javier/.local/bin/rofi-power", NULL };
static const char *rofi_screenshot[]  = { "/home/javier/.local/bin/rofi-screenshot", NULL };

static const char *file_manager[] = { "pcmanfm", NULL };
static const char *browser[] = { "qutebrowser", NULL };

static const char *brightness_up[] = { "brightnessctl", "s", "5%+", NULL};
static const char *brightness_down[] = { "brightnessctl", "s", "5%-", NULL};

static const char *volume_up[] = { "pamixer", "-i", "5", NULL};
static const char *volume_down[] = { "pamixer", "-d", "5", NULL};
static const char *mute[] = { "pamixer", "-t", NULL};
static const char *toggle_play[] = { "playerctl", "play-pause", NULL };
static const char *toggle_shuffle[] = { "playerctl", "shuffle", "Toggle", NULL };

#include "movestack.c"
static const Key keys[] = {
	/* modifier                     key        function        argument */
	// { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,    spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_BackSpace, spawn,          {.v = rofi_power } },
	{ MODKEY,                       XK_r,         spawn,          {.v = rofi_launcher } },
	{ MODKEY|ShiftMask,             XK_s,         spawn,          {.v = rofi_screenshot } },
	{ MODKEY,                       XK_f,         spawn,          {.v = file_manager } },
	{ MODKEY,                       XK_b,         spawn,          {.v = browser } },

	{ MODKEY,                       XK_F11,       togglebar,      {0} },
	{ MODKEY,                       XK_m,         zoom,           {0} },
	{ MODKEY,                       XK_t,         togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_Tab,	      view,           {0} },
	{ Mod1Mask,             		    XK_Tab,       altTabStart,	  {0} },
	{ MODKEY,                       XK_0,         view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,         tag,            {.ui = ~0 } },

	{ MODKEY,                       XK_j,         focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,         focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,         movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,         movestack,      {.i = -1 } },

	{ MODKEY,                       XK_q,         killclient,     {0} },
	{ MODKEY|ControlMask,           XK_q,         quit,           {0} },
	{ MODKEY|ControlMask,           XK_r,         quit,           {1} }, 

	{ MODKEY|ControlMask,           XK_z,         setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,		          XK_space,     cyclelayout,    {.i = -1 } },
	{ MODKEY,                       XK_space,     cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_comma,     focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,    focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,     tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,    tagmon,         {.i = +1 } },

	{ MODKEY,                       XK_i,         incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,         incnmaster,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_h,         setmfact,       {.f = -0.05} },
	{ MODKEY|ControlMask,           XK_l,         setmfact,       {.f = +0.05} },
	{ MODKEY|ControlMask,           XK_k,         setcfact,       {.f = +0.25} },
	{ MODKEY|ControlMask,           XK_j,         setcfact,       {.f = -0.25} },
	{ MODKEY|ControlMask,           XK_n,         setcfact,       {.f =  0.00} },

	{ MODKEY|Mod1Mask,              XK_k,         incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_j,         incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_h,         incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_l,         incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_l,         incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_h,         incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,         togglegaps,     {0} },
	{ MODKEY|Mod1Mask,              XK_n,         defaultgaps,    {0} },

  { 0, XF86XK_MonBrightnessUp,                  spawn,          {.v = brightness_up } },
  { 0, XF86XK_MonBrightnessDown,                spawn,          {.v = brightness_down } },
  { 0, XF86XK_AudioRaiseVolume,                 spawn,          {.v = volume_up } },
  { 0, XF86XK_AudioLowerVolume,                 spawn,          {.v = volume_down } },
  { 0, XF86XK_AudioMute,                        spawn,          {.v = mute } },
	{ MODKEY,                       XK_p,         spawn,          {.v = toggle_play } },
	{ MODKEY|ShiftMask,             XK_p,         spawn,          {.v = toggle_shuffle } },

	TAGKEYS( XK_1, 0)
	TAGKEYS( XK_2, 1)
	TAGKEYS( XK_3, 2)
	TAGKEYS( XK_4, 3)
	TAGKEYS( XK_5, 4)
	TAGKEYS( XK_6, 5)
	TAGKEYS( XK_7, 6)
	TAGKEYS( XK_8, 7)
	TAGKEYS( XK_9, 8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	// { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
