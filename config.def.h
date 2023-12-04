/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 6;        /* gap pixel between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "Iosevka:style=Medium:size=10" };
static const char dmenufont[]       = "Iosevka Nerd Font Mono:style=Medium:size=10";

static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */

/**********/
/* COLORS */
/**********/

/* Polar Night */
static const char nord0[]  = "#2e3440";
static const char nord1[]  = "#3b4252";
static const char nord2[]  = "#434c5e";
static const char nord3[]  = "#4c566a";
/* Snow Storm */
static const char nord4[]  = "#d8dee9";
static const char nord5[]  = "#e5e9f0";
static const char nord6[]  = "#eceff4";
/* Frost */
static const char nord7[]  = "#8fbcbb";
static const char nord8[]  = "#88c0d0";
static const char nord9[]  = "#81a1c1";
static const char nord10[] = "#5e81ac";
/* Aurora */
static const char nord11[] = "#bf616a";  /* Red */
static const char nord12[] = "#d08770";  /* Orange */
static const char nord13[] = "#ebcb8b";  /* Yellow */
static const char nord14[] = "#a3be8c";  /* Green */
static const char nord15[] = "#b48ead";  /* Purple */

static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#4c566a";  /* bar main color */

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, nord0, col_gray2 },
	[SchemeSel]  = { col_gray4, nord3,  nord10 },
};


/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "st",       NULL,       NULL,       0,            1,           -1 },
	{ "thunar",       NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "TIL",      tile },    /* first entry is default */
	{ "FLO",      NULL },    /* no layout function means floating behavior */
	{ "MON",      monocle },
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


/************/
/* COMMANDS */
/************/

static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */

static const char *dmenucmd[]           = { "/home/geoffrey/.scripts/dmenu.sh", NULL };
static const char *termcmd[]            = { "alacritty", NULL };
static const char *dmenudesktopcmd[]    = { "/home/geoffrey/.scripts/j4-dmenu-desktop.sh", NULL };
static const char *powerman[]           = { "rofi_run", "-l", NULL };
static const char *filecmd[]            = { "thunar", NULL };
static const char *filescreenshotscmd[] = {
    "thunar", "/home/geoffrey/Pictures/Screenshots", NULL,
};
static const char *lockcmd[]            = { "/home/geoffrey/.scripts/i3lock.sh", NULL };
static const char *scrotcmd[]           = { "/home/geoffrey/.scripts/scrot.sh", NULL };
static const char *monitorcmd[]         = { "/home/geoffrey/.scripts/monitor.sh", NULL };
static const char *battcheckcmd[]       = { "/home/geoffrey/.scripts/disenablebattcheck.sh", NULL };

/* volume */
static const char *upvol[]   = { "/home/geoffrey/.scripts/sound.sh", "inc", NULL };
static const char *downvol[] = { "/home/geoffrey/.scripts/sound.sh", "dec", NULL };
static const char *mutevol[] = { "/home/geoffrey/.scripts/sound.sh", "mut", NULL };
static const char *mutemic[] = { "/home/geoffrey/.scripts/mic.sh",   "mut", NULL };

static Key keys[] = {
	/* modifier         key        function        argument */
	{ MODKEY,           XK_d,      spawn,          {.v = dmenudesktopcmd } },
	{ MODKEY,           XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,           XK_e,      spawn,          {.v = filecmd } },
	{ MODKEY|ShiftMask, XK_e,      spawn,          {.v = filescreenshotscmd } },
	{ MODKEY,           XK_b,      togglebar,      {0} },
	{ MODKEY,           XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,           XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask, XK_j,      pushstack,      {.i = INC(+1) } },
	{ MODKEY|ShiftMask, XK_k,      pushstack,      {.i = INC(-1) } },
	{ MODKEY,           XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,           XK_p,      incnmaster,     {.i = -1 } },
	{ MODKEY,           XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,           XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask, XK_Return, zoom,           {0} },
	{ MODKEY,           XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask, XK_q,      killclient,     {0} },
	{ MODKEY,           XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,           XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,           XK_f,      fullscreen,     {0} },
	{ MODKEY|ShiftMask, XK_space,  togglefloating, {0} },
	{ MODKEY,           XK_s,      togglesticky,   {0} },
	{ MODKEY|ShiftMask, XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,           XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,           XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask, XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask, XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(            XK_1,                      0)
	TAGKEYS(            XK_2,                      1)
	TAGKEYS(            XK_3,                      2)
	TAGKEYS(            XK_4,                      3)
	TAGKEYS(            XK_5,                      4)
	TAGKEYS(            XK_6,                      5)
	TAGKEYS(            XK_7,                      6)
	TAGKEYS(            XK_8,                      7)
	TAGKEYS(            XK_9,                      8)
	TAGKEYS(            XK_0,                      9)
	{ MODKEY|ShiftMask, XK_e,      spawn,          {.v = powerman} },
	{ MODKEY|ShiftMask, XK_r,      quit,           {1} },
	{ MODKEY|ShiftMask, XK_d,      spawn,           {.v = dmenucmd} },
	{ MODKEY|ShiftMask, XK_x,      spawn,           {.v = lockcmd} },
	{ MODKEY|ShiftMask, XK_s,      spawn,           {.v = scrotcmd} },
	{ MODKEY|ShiftMask, XK_p,      spawn,           {.v = monitorcmd} },
	{ MODKEY|ShiftMask, XK_c,      spawn,           {.v = battcheckcmd} },
    /* sound control */
    { 0, XF86XK_AudioLowerVolume, spawn, {.v = downvol} },
    { 0, XF86XK_AudioRaiseVolume, spawn, {.v = upvol} },
    { 0, XF86XK_AudioMute,        spawn, {.v = mutevol} },
    { 0, XF86XK_AudioMicMute,     spawn, {.v = mutemic} },
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
