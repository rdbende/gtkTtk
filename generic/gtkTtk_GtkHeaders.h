/*
 *  gtkTtk_GtkHeaders.h
 * --------------------
 *
 * This file is part of the gtkTtk package, a Tk/Tile based theme that uses
 * Gtk/GNOME for drawing.
 *
 * Copyright (C) 2004-2008 by:
 * Georgios Petasis, petasis@iit.demokritos.gr,
 * Software and Knowledge Engineering Laboratory,
 * Institute of Informatics and Telecommunications,
 * National Centre for Scientific Research (NCSR) "Demokritos",
 * Aghia Paraskevi, 153 10, Athens, Greece.
 */
#ifndef _GTKTTK_GTKHEADERS
#define _GTKTTK_GTKHEADERS

#include <tk.h>
#include "gtkTtk_Symbols.h"

/* Convenience memory allocators
 */
#define GtkTtk_g_new0(struct_type, n_structs) \
    ((struct_type *) GtkTtk_g_malloc0 (((gsize) sizeof (struct_type)) * ((gsize) (n_structs))))

typedef struct GtkTtk_WidgetCache {
  Tk_Window      GtkTtk_tkwin;
  Display*       GtkTtk_MainDisplay;
  Tcl_Interp*    GtkTtk_MainInterp;
  int            orientation;
  GtkOrientation gtkOrientation;
  GdkDisplay*    gdkDisplay;
  GtkWidget*     gtkArrow;
  GtkWidget*     gtkNotebook;
  GtkWidget*     gtkEntry;
  GtkWidget*     gtkHScale;
  GtkWidget*     gtkVScale;
  GtkWidget*     gtkHProgressBar;
  GtkWidget*     gtkVProgressBar;
  GtkWidget*     gtkCombobox;
  GtkWidget*     gtkComboboxEntry;
  GtkWidget*     gtkHScrollBar;
  GtkWidget*     gtkVScrollBar;
  GtkWidget*     gtkButton;
  GtkWidget*     gtkCheckButton;
  GtkWidget*     gtkSpinButton;
  GtkWidget*     gtkToolBar;
  GtkWidget*     gtkToolButton;
  GtkWidget*     gtkTreeView;
  GtkWidget*     gtkTreeHeader;
  GtkWidget*     gtkFrame;
  GtkWidget*     gtkStatusBar;
  GtkWidget*     gtkHPaned;
  GtkWidget*     gtkVPaned;
  GtkWidget*     gtkWindow;
  GtkWidget*     protoLayout;
} GtkTtk_WidgetCache;

typedef struct {
    GtkStateType  state;    /* State value to return if this entry matches */
    GtkShadowType shadow;   /* Shadow value to return if this entry matches */
    unsigned int  value;    /* Value to return if this entry matches */
    unsigned int  onBits;   /* Bits which must be set */
    unsigned int  offBits;  /* Bits which must be cleared */
    unsigned int  section;  /* Table section to match */
} GtkTtk_StateTable;

#define GTKTTK_SECTION_ALL       (1<<0)
#define GTKTTK_SECTION_BUTTONS   (1<<1)
#define GTKTTK_SECTION_ENTRY     (1<<2)
#define GTKTTK_SECTION_SCROLLBAR (1<<3)
#define GTKTTK_SECTION_STEPPERS  (1<<4)
#define GTKTTK_SECTION_TROUGH    (1<<5)
#define GTKTTK_SECTION_SASH      (1<<6)
#define GTKTTK_SECTION_TABS      (1<<7)

#endif
