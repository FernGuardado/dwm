/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int gappx = 20;           /* gap pixel between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "NotoMono Nerd Font:size=12", "JoyPixels:pixelsize=12:antialias=true:autohint=true" };
static const char dmenufont[]       = "monospace:size=13";
static const char col_fg1[]	    = "#78a1c2";
static const char col_fg2[]	    = "#0f3d77";
static const char col_bg1[]     = "#28303d";
static const char col_bg2[]     = "#12161c";
static const char col_border1[]  = "#232b36";
static const char col_border2[]  = "#425167";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_fg1, col_bg1, col_border1 },
	[SchemeSel]  = { col_fg1, col_bg2, col_border2  },
};

/* pulseaudio controls */
static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "alsa_output.pci-0000_2f_00.4.analog-stereo", "+5%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "alsa_output.pci-0000_2f_00.4.analog-stereo", "-5%",     NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "alsa_output.pci-0000_2f_00.4.analog-stereo", "toggle",  NULL };

/* betterlockscreen */
static const char *lockscr[] = { "/usr/bin/betterlockscreen", "-l", NULL, NULL, NULL };

/* tagging */
//static const char *tags[] = { "","", "", "", "", "", "", "", "" };
static const char *tags[] = { "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },         
	/* { "VirtualBox", NULL,	  NULL,       0,            1,           -1 }, */
};

/* layout(s) */
static const float mfact     = 0.48; /* factor of master area size [0.05..0.95] */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int nmaster = 1;  /* default number of clients in the master area */

/* #include "nmaster-ncol.c" */
#include "grid.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },/* first entry is default */
	{ "><>",      NULL }, /* no layout function means floating behavior */        
	/* { "-|=",      ntile }, */      
	{ "[M]",      monocle },
	{ "HHH",      grid }, 
 	{ "|||",      col },  
	/* { "-|-",      nbstack }, */
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-l", "20", "-fn", dmenufont, "-nb", col_fg2, "-nf", col_fg2, "-sb", col_bg1, "-sf", col_bg2, NULL };
static const char *jgmenucmd[] = { "jgmenu_run", NULL, NULL, NULL, NULL  };
static const char *termcmd[]  = { "alacritty", NULL };

#include "shiftview.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	// { MODKEY,               	XK_F11,    spawn,	   {.v = downvol } },
	// { MODKEY,                       XK_F9,     spawn,          {.v = mutevol } },
	// { MODKEY,                       XK_F12,    spawn,          {.v = upvol   } },
	{ MODKEY,			            XK_F6,	      spawn,		SHCMD("pamixer --allow-boost -d 5; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,		        XK_F6,	      spawn,		SHCMD("pamixer --allow-boost -d 15; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,			            XK_F7,	      spawn,		SHCMD("pamixer --allow-boost -i 5; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,		        XK_F7,	      spawn,		SHCMD("pamixer --allow-boost -i 15; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,			            XK_F5,	      spawn,		SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,                       XK_p,      spawn,          SHCMD("~/.local/bin/rofi_bar") },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,		        XK_l,      spawn, 	   {.v = lockscr } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_comma, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,			            XK_Left,  shiftview,      { .i = -1 } },
	{ MODKEY,			            XK_Right, shiftview,      { .i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	{ MODKEY|ShiftMask,		        XK_q,	   spawn,	   SHCMD("~/.local/bin/powermenu") },
	TAGKEYS(                        XK_1,                      0)  
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

