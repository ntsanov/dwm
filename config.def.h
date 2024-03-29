/* See LICENSE file for copyright and license details. */


#include <X11/XF86keysym.h>
#include "colors.h"

/* appearance */
static const unsigned int borderpx  = 4;       /* border pixel of windows */
static const unsigned int gappx     = 0;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=14" };
static const char dmenufont[]       = "monospace:size=14";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { solarized_base00, solarized_base03, solarized_base00 },
	[SchemeSel]  = { solarized_orange, solarized_base02, solarized_orange  },
};

/* staticstatus */
static const int statmonval = 0;

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      			            instance    title       tags mask     isfloating   isterminal noswallow monitor float x,y,w,h  floatborderpx */
	{ "Gimp",     				         NULL,       NULL,       0,            1,           0,         0,        -1, 50,50,500,500, 5 },
	{ "st",       				         NULL,       NULL,       0,            0,           1,         1,        -1, 50,50,500,500, 5},
	{ "Skype",    				         NULL,       NULL,       1 << 8,       1,           0,         0,        1, 0,0,1200,700, 5},
	{ "ViberPC",  				         NULL,       NULL,       1 << 8,       1,           0,         0,        1, 0,732,1200,700, 5},
	{ "Chromium", 				         NULL,       NULL,       0,            0,           0,         0,        -1, 50,50,500,500, 5},
	{ "Microsoft Teams - Preview", 	NULL,       NULL,       1 << 8,       1,           0,         0,        1, 1205,0,1300,1400, 5},
	{ "obs",						         NULL,		   NULL,		   0,            0,           0,         1,        -1,0,0,1920,1080,0}
   { "trayer",                      NULL,       NULL,       1 << 9        1,           0,         1,        1, 300,0,1620,30, 0}
};

// GIST - https://gist.github.com/palopezv/efd34059af6126ad970940bcc6a90f2e
/* sound */
//static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "0", "+5%",     NULL };
//static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "0", "-5%",     NULL };
//static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "0", "toggle",  NULL };

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]",      tile },    /* first entry is default */
	{ "[]",      NULL },    /* no layout function means floating behavior */
	{ "[]",      monocle },
};

/* key definitions */
//#define MODKEY Mod1Mask
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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", solarized_base03, "-nf", solarized_base00, "-sb", solarized_base02, "-sf", solarized_orange, "-l", "12", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char upvol[]   = 	{ "pamixer --allow-boost -i 5; pkill -RTMIN+1 dwmblocks" };
static const char downvol[] = 	{ "pamixer --allow-boost -d 5; pkill -RTMIN+1 dwmblocks" };
static const char mutevol[] = 	{ "pamixer -t; pkill -RTMIN+1 dwmblocks" };
static const char mon_br_up[] = 	{ "xbacklight -inc 10" };
static const char mon_br_down[] = 	{ "xbacklight -dec 10" };
static const char kb_layout[] = { "setxkbmap -query | grep -q 'us' && setxkbmap bg phonetic || setxkbmap us; pkill -RTMIN+2 dwmblocks" };
static const char prtscr_win[] = { "scrot -u '/tmp/%F_%T_$wx$h.png' -e 'xclip -selection clipboard -target image/png -i $f' && notify-send 'window captured to clipboard'" };
static const char *passmenu[] = {"passmenu", "-fn", dmenufont, "-nb", solarized_base03, "-nf", solarized_base00, "-sb", solarized_base02, "-sf", solarized_orange, "-l", "12", NULL  };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      					spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, 					spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      					togglebar,      {0} },
	{ MODKEY,                       XK_j,      					focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      					focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      					incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      					incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      					setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      					setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, 					zoom,           {0} },
	{ MODKEY,                       XK_Tab,    					view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      					killclient,     {0} },
	{ MODKEY,                       XK_t,      					setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      					setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      					setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  					setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  					togglefloating, {0} },
	{ MODKEY,                       XK_0,      					view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      					tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  					focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, 					focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  					tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, 					tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  					setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  					setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  					setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_b,      					spawn, SHCMD("$BROWSER") },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ 0,							XK_ISO_Next_Group, 			spawn, SHCMD(kb_layout) },
	{ 0,                       		XF86XK_AudioLowerVolume, 	spawn, SHCMD(downvol) },
	{ 0,                       		XF86XK_AudioMute, 			spawn, SHCMD(mutevol) },
	{ 0,                       		XF86XK_AudioRaiseVolume, 	spawn, SHCMD(upvol) },
	{ 0,                       		XF86XK_MonBrightnessUp, 	spawn, SHCMD(mon_br_up) },
	{ 0,                       		XF86XK_MonBrightnessDown, 	spawn, SHCMD(mon_br_down) },
	{ 0,                       		XK_Print, 					spawn, SHCMD("flameshot gui") },
	{ ControlMask|ShiftMask,		XK_Print, 					spawn, SHCMD(prtscr_win) },
	{ MODKEY|ShiftMask,             XK_l, 						spawn, SHCMD("slock") },
	{ MODKEY|ShiftMask,             XK_s, 						spawn, SHCMD("rofi -monitor -4 -show ssh") },
	{ MODKEY|ShiftMask,             XK_p, 						spawn, {.v = passmenu } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

