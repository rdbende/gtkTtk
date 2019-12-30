/*
 *  gtkTtk_Utilities.h
 * ----------------------
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

#include "gtkTtk_GtkHeaders.h"
#include "gtkTtk_Elements.h"
#include <string.h>

#define NO_GTK_STYLE_ENGINE {/*printf("NULL qApp\n");fflush(NULL);*/return;}

#define GTKTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE \
  if (!GtkTtk_GtkInitialised()) return;

#define GTKTTK_ENSURE_WIDGET_OK \
  if (!widget) return;

#define GTKTTK_ATTACH_STYLE_TO_WIDGET \
  style = GtkTtk_gtk_style_attach(style, widget->window);

#define GTKTTK_WIDGET_CACHE_DEFINITION \
  GtkTtk_WidgetCache *wc = (GtkTtk_WidgetCache *) clientData;

#define GTKTTK_ORIENTATION_DEFINITION \
  int orientation = wc->orientation;

#define GTKTTK_GTK_DRAWABLE_DEFINITIONS \
  GtkTtk_WidgetCache *wc = (GtkTtk_WidgetCache *) clientData; \
  GdkPixmap    *gdkDrawable = NULL; \
  GtkStyle     *style       = NULL; \
  GtkStateType  gtkState    = GTK_STATE_NORMAL; \
  GtkShadowType gtkShadow   = GTK_SHADOW_NONE;

#define GTKTTK_SETUP_GTK_DRAWABLE \
  GTKTTK_SETUP_GTK_DRAWABLE_PIXMAP_SIZE(b.width, b.height)

#define GTKTTK_SETUP_GTK_DRAWABLE_PIXMAP_SIZE(pw, ph) \
  if (!wc) return; \
  style = GtkTtk_GetGtkWindowStyle(wc->gtkWindow); \
  if (!style) return; \
  gdkDrawable = GtkTtk_gdk_pixmap_new(wc->gtkWindow->window, pw, ph, -1); \
  style = GtkTtk_GetGtkWindowStyle(wc->gtkWindow);

#define GTKTTK_STYLE_BACKGROUND_DEFAULT \
  if (wc && wc->gtkWindow) style = GtkTtk_GetGtkWindowStyle(wc->gtkWindow); \
  if (!style) return;

#define GTKTTK_STYLE_FROM_WIDGET \
  style = GtkTtk_GetGtkWindowStyle(widget); \
  if (!style) style = GtkTtk_GetGtkWindowStyle(wc->gtkWindow); \
  if (!style) return;

#define GTKTTK_DRAWABLE_FROM_WIDGET_SIZE(pw, ph) \
  gdkDrawable = GtkTtk_gdk_pixmap_new(widget->window, pw, ph, -1);

#define GTKTTK_DRAWABLE_FROM_WIDGET \
  GTKTTK_DRAWABLE_FROM_WIDGET_SIZE(b.width, b.height)

// #define GTKTTK_DRAWABLE_FROM_WIDGET \
//   gdkDrawable = gdk_pixmap_foreign_new(Tk_WindowId(tkwin)); \
//   GdkColormap gdkColormap = gdk_x11_colormap_foreign_new(gdkx_visual_get(Tk_Visual(tkwin)), Tk_Colormap(tkwin)); \
//   gdk_drawable_set_colormap(gdkDrawable, gdkColormap);

#define GTKTTK_DEFAULT_BACKGROUND_SIZE(pw, ph) \
  GtkTtk_gtk_style_apply_default_background(style, gdkDrawable, TRUE, \
               gtkState, NULL, 0, 0, pw, ph);

#define GTKTTK_DEFAULT_BACKGROUND \
  GTKTTK_DEFAULT_BACKGROUND_SIZE(b.width, b.height)

#define GTKTTK_CLEANUP_GTK_DRAWABLE \
  if (gdkDrawable) GtkTtk_g_object_unref(gdkDrawable);

#define GTKTTK_SETUP_STATE_SHADOW(statemap, shadowmap) \
    gtkState  = (GtkStateType) \
       GtkTtk_StateTableLookup(statemap,  state); \
    gtkShadow = (GtkShadowType) \
       GtkTtk_StateTableLookup(shadowmap, state);

#define GTKTTK_SETUP_WIDGET_SIZE(width, height) \
  GtkTtk_gtk_widget_set_size_request(widget, width, height);

#define GTKTTK_GET_WIDGET_SIZE(widthPtr, heightPtr)  \
  if (widget) { \
    GtkRequisition size; \
    GtkTtk_gtk_widget_size_request(widget, &size); \
    widthPtr  = size.width; \
    heightPtr = size.height; \
  }

#define GTKTTK_WIDGET_SETUP_DEFAULT(obj) \
  int defaultState  = TTK_BUTTON_DEFAULT_DISABLED; \
  int has_default = (defaultState == TTK_BUTTON_DEFAULT_ACTIVE); \
  /*Ttk_GetButtonDefaultStateFromObj(NULL, obj, &defaultState);*/

#ifdef GTKTTK_LOAD_GTK_DYNAMICALLY

#define GTKTTK_WIDGET_SET_FOCUS(widget)

#define GTKTTK_WIDGET_SET_DEFAULT(widget, obj) \
  int defaultState  = TTK_BUTTON_DEFAULT_DISABLED; \
  int has_default = (defaultState == TTK_BUTTON_DEFAULT_ACTIVE);

#else /* GTKTTK_LOAD_GTK_DYNAMICALLY */

#define GTKTTK_WIDGET_SET_FOCUS(widget) \
  if (state & TTK_STATE_FOCUS) { \
    GTK_WIDGET_SET_FLAGS(widget,   GTK_HAS_FOCUS); \
  } else { \
    GTK_WIDGET_UNSET_FLAGS(widget, GTK_HAS_FOCUS); \
  }

#define GTKTTK_WIDGET_SET_DEFAULT(widget, obj) \
  int defaultState  = TTK_BUTTON_DEFAULT_DISABLED; \
  int has_default = (defaultState == TTK_BUTTON_DEFAULT_ACTIVE); \
  /*Ttk_GetButtonDefaultStateFromObj(NULL, obj, &defaultState);*/ \
  if (has_default) { \
    GTK_WIDGET_SET_FLAGS(widget,   GTK_HAS_DEFAULT); \
  } else { \
    GTK_WIDGET_UNSET_FLAGS(widget, GTK_HAS_DEFAULT); \
  } 
#endif /* GTKTTK_LOAD_GTK_DYNAMICALLY */

#define GTKTTK_DEBUG_PRINT_BOX \
  printf("x=%d, y=%d, w=%d, h=%d\n", b.x, b.y, b.width, b.height); \
  fflush(0);

#define GTKTTK_DEBUG_PRINT_TK_WIDGET \
  printf("Widget: %s,p=%p\n", Tk_PathName(tkwin), tkwin); \
  fflush(0);

#define GTKTTK_GTKBORDER_TO_PADDING(border) \
  Ttk_MakePadding(border.left, border.top, border.right, border.bottom)

TCL_DECLARE_MUTEX(gtkTtkMutex);

/* Global Symbols */

/* Helper Functions */
extern int        GtkTtk_GtkInitialised(void);
extern GtkWidget *GtkTtk_GetGtkWindow(void);
extern GtkStyle  *GtkTtk_GetGtkWindowStyle(GtkWidget *gtkWindow);
extern GtkStyle  *GtkTtk_GetGtkStyle(void);

extern unsigned int GtkTtk_StateTableLookup(Ttk_StateTable *, unsigned int);
extern void GtkTtk_CopyGtkPixmapOnToDrawable(GdkPixmap *, Drawable, Tk_Window,
                                            int, int, int, int, int, int);
extern void GtkTtk_StateInfo(int, GtkStateType,
                    GtkShadowType, Tk_Window, GtkWidget *widget = NULL);

extern GtkWidget *GtkTtk_GetArrow(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetButton(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetCheckButton(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetRadioButton(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetToolBar(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetToolButton(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetFrame(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetEntry(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetCombobox(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetComboboxEntry(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetHScrollBar(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetVScrollBar(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetScrollBar(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetHScale(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetVScale(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetScale(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetHProgressBar(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetVProgressBar(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetProgressBar(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetStatusBar(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetPaned(GtkTtk_WidgetCache* wc);
extern GtkWidget *GtkTtk_GetNotebook(GtkTtk_WidgetCache* wc);
#if 0
extern void GtkTtk_StoreStyleNameLowers(GtkTtk_WidgetCache *wc);
extern bool GtkTtk_ThemeIs(GtkTtk_WidgetCache *wc, const char* name);
extern void GtkTtk_SetFocus(bool focus);
#endif

extern unsigned int GtkTtk_StateShadowTableLookup(GtkTtk_StateTable*,
       unsigned int, GtkStateType&, GtkShadowType&,
       unsigned int section = GTKTTK_SECTION_ALL);
extern double GtkTtk_ValueFromSlider(GtkTtk_WidgetCache *wc, Tk_Window tkwin,
                               Ttk_Box b);
