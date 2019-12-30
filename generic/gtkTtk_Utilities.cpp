/*
 *  gtkTtk_Utilities.cpp
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
#include "gtkTtk_TkHeaders.h"
extern gboolean   GtkTtk_GtkInitialisedFlag;
extern GtkWidget *GtkTtk_GtkWindow;

int GtkTtk_GtkInitialised(void) {
  return GtkTtk_GtkInitialisedFlag;
}; /* GtkTtk_GtkInitialised */

GtkWidget *GtkTtk_GetGtkWindow(void) {
  return GtkTtk_GtkWindow;
}; /* GtkTtk_GetGtkWindow */

GtkStyle *GtkTtk_GetGtkWindowStyle(GtkWidget *gtkWindow) {
  if (gtkWindow) {
    return gtkWindow->style;
  }
  return NULL;
}; /* GtkTtk_GetGtkWindowStyle */

GtkStyle *GtkTtk_GetGtkStyle(void) {
  return GtkTtk_GetGtkWindowStyle(GtkTtk_GetGtkWindow());
}; /* GtkTtk_GetGtkStyle */

void GtkTtk_InitialiseGtkWidget(GtkTtk_WidgetCache* wc, GtkWidget* widget) {
  // GtkStyle *style;
  if (!wc || !widget) return;
  if (!wc->protoLayout) {
    wc->protoLayout = GtkTtk_gtk_fixed_new();
    GtkTtk_gtk_container_add((GtkContainer*)(wc->gtkWindow), wc->protoLayout);
  }
  if (!wc->protoLayout) return;
  GtkTtk_gtk_container_add((GtkContainer*)(wc->protoLayout), widget);
  GtkTtk_gtk_widget_realize(widget);
  // style = GtkTtk_gtk_rc_get_style(widget);
  // if (style) GtkTtk_gtk_style_attach(style,
}; /* GtkTtk_InitialiseGtkWidget */

#define GTKTTK_CHECK_WIDGET(widget, allocator_function) \
  if (!wc) return 0; \
  if (!wc->widget) { \
    wc->widget = allocator_function; \
    GtkTtk_InitialiseGtkWidget(wc, wc->widget); \
    return wc->widget; \
  } \
  return wc->widget;

GtkWidget *GtkTtk_GetArrow(GtkTtk_WidgetCache* wc) {
  GTKTTK_CHECK_WIDGET(gtkArrow,
                       GtkTtk_gtk_arrow_new(GTK_ARROW_DOWN, GTK_SHADOW_NONE));
}; /* GtkTtk_GetButton */

GtkWidget *GtkTtk_GetButton(GtkTtk_WidgetCache* wc) {
  GTKTTK_CHECK_WIDGET(gtkButton, GtkTtk_gtk_button_new());
}; /* GtkTtk_GetButton */

GtkWidget *GtkTtk_GetCheckButton(GtkTtk_WidgetCache* wc) {
  GTKTTK_CHECK_WIDGET(gtkCheckButton, GtkTtk_gtk_check_button_new());
}; /* GtkTtk_GetCheckButton */

GtkWidget *GtkTtk_GetRadioButton(GtkTtk_WidgetCache* wc) {
  return GtkTtk_GetCheckButton(wc);
}; /* GtkTtk_GetRadioButton */

GtkWidget *GtkTtk_GetToolBar(GtkTtk_WidgetCache* wc) {
  GTKTTK_CHECK_WIDGET(gtkToolBar, GtkTtk_gtk_toolbar_new());
}; /* GtkTtk_GetToolBar */

GtkWidget *GtkTtk_GetToolButton(GtkTtk_WidgetCache* wc) {
  GTKTTK_CHECK_WIDGET(gtkToolButton, GtkTtk_gtk_button_new());
}; /* GtkTtk_GetToolButton */

GtkWidget *GtkTtk_GetFrame(GtkTtk_WidgetCache* wc) {
  GTKTTK_CHECK_WIDGET(gtkFrame, GtkTtk_gtk_frame_new(NULL));
}; /* GtkTtk_GetFrame */

GtkWidget *GtkTtk_GetEntry(GtkTtk_WidgetCache* wc) {
  GTKTTK_CHECK_WIDGET(gtkEntry, GtkTtk_gtk_entry_new());
}; /* GtkTtk_GetEntry */

GtkWidget *GtkTtk_GetCombobox(GtkTtk_WidgetCache* wc) {
  GTKTTK_CHECK_WIDGET(gtkCombobox, GtkTtk_gtk_combo_box_new());
}; /* GtkTtk_GetComboboxEntry */

GtkWidget *GtkTtk_GetComboboxEntry(GtkTtk_WidgetCache* wc) {
  GTKTTK_CHECK_WIDGET(gtkComboboxEntry, GtkTtk_gtk_combo_box_entry_new());
}; /* GtkTtk_GetComboboxEntry */

GtkWidget *GtkTtk_GetHScrollBar(GtkTtk_WidgetCache* wc) {
  GtkAdjustment *adjustment = (GtkAdjustment *)
             GtkTtk_gtk_adjustment_new(0.0, 0.0, 1.0, 0, 0, 0);
  GTKTTK_CHECK_WIDGET(gtkHScrollBar, GtkTtk_gtk_hscrollbar_new(adjustment));
}; /* GtkTtk_GetHScrollBar */

GtkWidget *GtkTtk_GetVScrollBar(GtkTtk_WidgetCache* wc) {
  GtkAdjustment *adjustment = (GtkAdjustment *)
             GtkTtk_gtk_adjustment_new(0.0, 0.0, 1.0, 0, 0, 0);
  GTKTTK_CHECK_WIDGET(gtkVScrollBar, GtkTtk_gtk_vscrollbar_new(adjustment));
}; /* GtkTtk_GetVScrollBar */

GtkWidget *GtkTtk_GetScrollBar(GtkTtk_WidgetCache* wc) {
  if (wc->orientation == TTK_ORIENT_HORIZONTAL) {
    return GtkTtk_GetHScrollBar(wc);
  }
  return GtkTtk_GetVScrollBar(wc);
}; /* GtkTtk_GetScrollBar */

GtkWidget *GtkTtk_GetHScale(GtkTtk_WidgetCache* wc) {
  GTKTTK_CHECK_WIDGET(gtkHScale, GtkTtk_gtk_hscale_new_with_range(0,1,0.001));
}; /* GtkTtk_GetHScale */

GtkWidget *GtkTtk_GetVScale(GtkTtk_WidgetCache* wc) {
  GTKTTK_CHECK_WIDGET(gtkVScale, GtkTtk_gtk_vscale_new_with_range(0,1,0.001));
}; /* GtkTtk_GetVScale */

GtkWidget *GtkTtk_GetScale(GtkTtk_WidgetCache* wc) {
  if (wc->orientation == TTK_ORIENT_HORIZONTAL) {
    return GtkTtk_GetHScale(wc);
  }
  return GtkTtk_GetVScale(wc);
}; /* GtkTtk_GetScale */

GtkWidget *GtkTtk_GetHProgressBar(GtkTtk_WidgetCache* wc) {
  GTKTTK_CHECK_WIDGET(gtkHProgressBar, GtkTtk_gtk_progress_bar_new());
  GtkTtk_gtk_progress_bar_set_orientation(
     (GtkProgressBar*) wc->gtkHProgressBar, GTK_PROGRESS_LEFT_TO_RIGHT);
  GtkTtk_gtk_progress_bar_set_fraction(
     (GtkProgressBar *) wc->gtkHProgressBar, 1);
}; /* GtkTtk_GetHProgressBar */

GtkWidget *GtkTtk_GetVProgressBar(GtkTtk_WidgetCache* wc) {
  GTKTTK_CHECK_WIDGET(gtkVProgressBar, GtkTtk_gtk_progress_bar_new());
  GtkTtk_gtk_progress_bar_set_orientation(
     (GtkProgressBar*) wc->gtkVProgressBar, GTK_PROGRESS_BOTTOM_TO_TOP);
  GtkTtk_gtk_progress_bar_set_fraction(
     (GtkProgressBar *) wc->gtkVProgressBar, 1);
}; /* GtkTtk_GetVProgressBar */

GtkWidget *GtkTtk_GetProgressBar(GtkTtk_WidgetCache* wc) {
  if (wc->orientation == TTK_ORIENT_HORIZONTAL) {
    return GtkTtk_GetHProgressBar(wc);
  }
  return GtkTtk_GetVProgressBar(wc);
}; /* GtkTtk_GetProgressBar */

GtkWidget *GtkTtk_GetStatusBar(GtkTtk_WidgetCache* wc) {
  GTKTTK_CHECK_WIDGET(gtkStatusBar, GtkTtk_gtk_statusbar_new());
}; /* GtkTtk_GetStatusBar */

GtkWidget *GtkTtk_GetHPaned(GtkTtk_WidgetCache* wc) {
  GTKTTK_CHECK_WIDGET(gtkHPaned, GtkTtk_gtk_hpaned_new());
}; /* GtkTtk_GetHPaned */

GtkWidget *GtkTtk_GetVPaned(GtkTtk_WidgetCache* wc) {
  GTKTTK_CHECK_WIDGET(gtkVPaned, GtkTtk_gtk_vpaned_new());
}; /* GtkTtk_GetVPaned */

GtkWidget *GtkTtk_GetPaned(GtkTtk_WidgetCache* wc) {
  if (wc->orientation == TTK_ORIENT_HORIZONTAL) {
    return GtkTtk_GetHPaned(wc);
  }
  return GtkTtk_GetVPaned(wc);
}; /* GtkTtk_GetPaned */

GtkWidget *GtkTtk_GetNotebook(GtkTtk_WidgetCache* wc) {
  GTKTTK_CHECK_WIDGET(gtkNotebook, GtkTtk_gtk_notebook_new());
}; /* GtkTtk_GetNotebook */


const char *GtkTtk_GtkStateStr(GtkStateType gtkState) {
  switch ((GtkStateType) gtkState) {
    case GTK_STATE_NORMAL:      return "GTK_STATE_NORMAL";
    case GTK_STATE_ACTIVE:      return "GTK_STATE_ACTIVE";
    case GTK_STATE_PRELIGHT:    return "GTK_STATE_PRELIGHT";
    case GTK_STATE_SELECTED:    return "GTK_STATE_SELECTED";
    case GTK_STATE_INSENSITIVE: return "GTK_STATE_INSENSITIVE";
  }
  return "invalid!";
}; /* GtkTtk_GtkStateStr */
const char *GtkTtk_GtkShadowStr(GtkShadowType gtkShadow) {
  switch ((GtkShadowType) gtkShadow) {
    case GTK_SHADOW_NONE:       return "GTK_SHADOW_NONE";
    case GTK_SHADOW_IN:         return "GTK_SHADOW_IN";
    case GTK_SHADOW_OUT:        return "GTK_SHADOW_OUT";
    case GTK_SHADOW_ETCHED_IN:  return "GTK_SHADOW_ETCHED_IN";
    case GTK_SHADOW_ETCHED_OUT: return "GTK_SHADOW_ETCHED_OUT";
  }
  return "invalid!";
}; /* GtkTtk_GtkShadowStr */

void GtkTtk_StateInfo(int state, GtkStateType gtkState,
             GtkShadowType gtkShadow, Tk_Window tkwin, GtkWidget *widget)
{
    printf("Widget: %s\n  ", Tk_PathName(tkwin));
    if (state & TTK_STATE_ACTIVE) {
      printf("TTK_STATE_ACTIVE %d ", TTK_STATE_ACTIVE);
    } 
    if (state & TTK_STATE_DISABLED) {
      printf("TTK_STATE_DISABLED %d ", TTK_STATE_DISABLED);
    } 
    if (state & TTK_STATE_FOCUS) {
      printf("TTK_STATE_FOCUS %d ", TTK_STATE_FOCUS);
    } 
    if (state & TTK_STATE_PRESSED) {
      printf("TTK_STATE_PRESSED %d ", TTK_STATE_PRESSED);
    } 
    if (state & TTK_STATE_SELECTED) {
      printf("TTK_STATE_SELECTED %d ", TTK_STATE_SELECTED);
    } 
    if (state & TTK_STATE_BACKGROUND) {
      printf("TTK_STATE_BACKGROUND %d ", TTK_STATE_BACKGROUND);
    } 
    if (state & TTK_STATE_ALTERNATE) {
      printf("TTK_STATE_ALTERNATE %d ", TTK_STATE_ALTERNATE);
    } 
    if (state & TTK_STATE_INVALID) {
      printf("TTK_STATE_INVALID %d ", TTK_STATE_INVALID);
    } 
    if (state & TTK_STATE_READONLY) {
      printf("TTK_STATE_READONLY %d ", TTK_STATE_READONLY);
    } 
    if (state & TTK_STATE_USER1) {
      printf("TTK_STATE_USER1 %d ", TTK_STATE_USER1);
    } 
    if (state & TTK_STATE_USER2) {
      printf("TTK_STATE_USER2 %d ", TTK_STATE_USER2);
    } 
    if (state & TTK_STATE_USER3) {
      printf("TTK_STATE_USER3 %d ", TTK_STATE_USER3);
    } 
    if (state & TTK_STATE_USER4) {
      printf("TTK_STATE_USER4 %d ", TTK_STATE_USER4);
    } 
    if (state & TTK_STATE_USER5) {
      printf("TTK_STATE_USER5 %d ", TTK_STATE_USER5);
    } 
    if (state & TTK_STATE_USER6) {
      printf("TTK_STATE_USER6 %d ", TTK_STATE_USER6);
    } 
#ifdef TTK_STATE_USER7
    if (state & TTK_STATE_USER7) {
      printf("TTK_STATE_USER7 %d ", TTK_STATE_USER7);
    }
#endif /* TTK_STATE_USER7 */
    printf(" state=%d\n", state);
    printf("  GTK state: %s, shadow: %s",
      GtkTtk_GtkStateStr(gtkState), GtkTtk_GtkShadowStr(gtkShadow));
#ifndef GTKTTK_LOAD_GTK_DYNAMICALLY
    if (widget) {
      printf(", focus: %d, default: %d", GTK_WIDGET_HAS_FOCUS(widget),
                                         GTK_WIDGET_HAS_DEFAULT(widget));
    }
#endif /* GTKTTK_LOAD_GTK_DYNAMICALLY */
    printf("\n");
    fflush(0);
}; /* GtkTtk_StateInfo */

void GtkTtk_CopyGtkPixmapOnToDrawable(GdkPixmap *gdkDrawable, Drawable d,
            Tk_Window tkwin, int x, int y, int w, int h, int x1, int x2)
{
#ifdef __WIN32__
#define gc_usage ((GdkGCValuesMask) \
    (GDK_GC_FOREGROUND | GDK_GC_BACKGROUND | GDK_GC_EXPOSURES))
    XGCValues gcValues;
    gcValues.graphics_exposures = False;
    GC gc = Tk_GetGC(tkwin, GCForeground | GCBackground | GCGraphicsExposures,
                     &gcValues);
    GdkGC *gdkGC = GtkTtk_gdk_gc_new(gdkDrawable);
    HDC hdcSrc = GtkTtk_gdk_win32_hdc_get(gdkDrawable, gdkGC, gc_usage);
    /* Create a Tk Drawable from the HDC... */
    TkWinDrawable gtkD;
    gtkD.type = TWD_WINDC;
    gtkD.winDC.hdc = hdcSrc;
    XCopyArea(Tk_Display(tkwin), (Drawable) &gtkD, d, gc, x, y, w, h, x1, x2);
    GtkTtk_gdk_win32_hdc_release(gdkDrawable, gdkGC, gc_usage);
    if (gdkGC) GtkTtk_g_object_unref(gdkGC);
    Tk_FreeGC(Tk_Display(tkwin), gc);
#else

#ifdef GTKTTK_USE_XCOPY
    XGCValues gcValues;
    gcValues.graphics_exposures = False;
    GC gc = Tk_GetGC(tkwin, GCForeground | GCBackground | GCGraphicsExposures,
                     &gcValues);
    XCopyArea(Tk_Display(tkwin), GDK_DRAWABLE_XID(pixmap), d, gc,
              x, y, w, h, x1, x2);
    Tk_FreeGC(Tk_Display(tkwin), gc);
    XGCValues gcValues;
    GC gc;

    gcValues.function = GXcopy;
    gcValues.graphics_exposures = False;
    gc = Tk_GetGC(tkwin, GCFunction|GCGraphicsExposures, &gcValues);

    XCopyArea(Tk_Display(tkwin), d, Tk_WindowId(tkwin), gc,
            0, 0, (unsigned) Tk_Width(tkwin), (unsigned) Tk_Height(tkwin),
            0, 0);

    Tk_FreePixmap(Tk_Display(tkwin), d);
    Tk_FreeGC(Tk_Display(tkwin), gc);
#else
    GdkPixbuf *imgb;
    XGCValues gcValues;
    gcValues.graphics_exposures = False;
    GC gc = Tk_GetGC(tkwin, GCForeground | GCBackground | GCGraphicsExposures,
                     &gcValues);
    imgb = GtkTtk_gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, w, h);
    if (!imgb) {
      Tk_FreeGC(Tk_Display(tkwin), gc);
      return;
    }
    imgb = GtkTtk_gdk_pixbuf_get_from_drawable(imgb, gdkDrawable,
                                                NULL, x, y, 0, 0, w, h);
    GtkTtk_gdk_pixbuf_xlib_render_to_drawable(imgb, d, gc,
         0, 0, x1, x2, w, h, XLIB_RGB_DITHER_MAX, 0, 0);
    GtkTtk_gdk_pixbuf_unref(imgb);
    Tk_FreeGC(Tk_Display(tkwin), gc);
#endif

/*
 * How to convert an X drawable to a GtkDrawable!
 *
  GdkPixmap    *gdkDrawable = NULL; \
  GtkStyle     *style       = NULL; \
  GdkScreen    *screen      = NULL; \
  GdkColormap  *cmap        = NULL; \
  screen = GtkTtk_gdk_display_get_screen(wc->gdkDisplay, \
                                          Tk_ScreenNumber(tkwin)); \
  printf("Widget: %s, d=%d, p=%p\n", Tk_PathName(tkwin), d, \
          gdk_xid_table_lookup((XID) d));fflush(0); \
  gdkDrawable = GtkTtk_gdk_pixmap_foreign_new_for_screen(screen, d, \
          Tk_Width(tkwin), Tk_Height(tkwin), \
          DefaultDepthOfScreen(Tk_Screen(tkwin))); \
  if (!gdkDrawable) return; \
  GdkVisual *visual = GtkTtk_gdk_x11_screen_lookup_visual(screen,
                              XVisualIDFromVisual(Tk_Visual(tkwin))); \
  cmap = GtkTtk_gdk_x11_colormap_foreign_new(visual, Tk_Colormap(tkwin)); \
  GtkTtk_gdk_drawable_set_colormap(gdkDrawable, cmap);\
  printf("d=%d, gdkDrawable=%p, XID=%d, p=%p\n", d, gdkDrawable, \
          gdk_x11_drawable_get_xid(gdkDrawable), \
          gdk_xid_table_lookup((XID) d));fflush(0);
 */
#endif
}; /* GtkTtk_CopyGtkPixmapOnToDrawable */

void GtkTtk_StoreStyleNameLowers(GtkTtk_WidgetCache *wc) {
}; /* GtkTtk_StoreStyleName */

bool GtkTtk_ThemeIs(GtkTtk_WidgetCache *wc, const char* name) {
  return false;
}; /* GtkTtk_ThemeIs */

/*
 * GtkTtk_StateTableLookup --
 * Look up an index from a statically allocated state table.
 */
unsigned int GtkTtk_StateTableLookup(Ttk_StateTable *map, unsigned int state) {
    return Ttk_StateTableLookup(map, state);
    int value = 0;
    while (map->onBits || map->offBits) {
      if (state & map->onBits) value |= map->index;
      ++map;
    }
    return value;
}; /* GtkTtk_StateTableLookup */

/*
 * GTK_STATE_NORMAL:      State during normal operation.
 * GTK_STATE_ACTIVE:      State of a currently active widget, such as a
 *                        depressed button.
 * GTK_STATE_PRELIGHT:    State indicating that the mouse pointer is over the
 *                        widget and the widget will respond to mouse clicks.
 * GTK_STATE_SELECTED:    State of a selected item, such the selected row in
 *                        a list.
 * GTK_STATE_INSENSITIVE: State indicating that the widget is unresponsive
 *                        to user actions.
 *
 * GTK_SHADOW_NONE:       No outline.
 * GTK_SHADOW_IN:         The outline is bevelled inwards.
 * GTK_SHADOW_OUT:        The outline is bevelled outwards like a button.
 * GTK_SHADOW_ETCHED_IN:  The outline has a sunken 3d appearance.
 * GTK_SHADOW_ETCHED_OUT: The outline has a raised 3d appearance
 *
 * TTK_STATE_ACTIVE
 * TTK_STATE_DISABLED
 * TTK_STATE_FOCUS
 * TTK_STATE_PRESSED
 * TTK_STATE_SELECTED
 * TTK_STATE_BACKGROUND
 * TTK_STATE_ALTERNATE
 * TTK_STATE_INVALID
 * TTK_STATE_READONLY
 */
unsigned int GtkTtk_StateShadowTableLookup(GtkTtk_StateTable *map,
             unsigned int state, GtkStateType& gtkState,
             GtkShadowType& gtkShadow, unsigned int section) {
  static GtkTtk_StateTable default_map[] = {
    /* Section for: Buttons */
    /* Section common to all widgets */
    {GTK_STATE_ACTIVE,      GTK_SHADOW_NONE, 0,
     TTK_STATE_ACTIVE,      0,               GTKTTK_SECTION_ALL},
    {GTK_STATE_INSENSITIVE, GTK_SHADOW_NONE, 0,
     TTK_STATE_DISABLED,    0,               GTKTTK_SECTION_ALL},
    {GTK_STATE_PRELIGHT,    GTK_SHADOW_NONE, 0,
     TTK_STATE_FOCUS,       0,               GTKTTK_SECTION_ALL},
    {GTK_STATE_SELECTED,    GTK_SHADOW_NONE, 0,
     TTK_STATE_PRESSED,     0,               GTKTTK_SECTION_ALL},
    {GTK_STATE_SELECTED,    GTK_SHADOW_NONE, 0,
     TTK_STATE_SELECTED,    0,               GTKTTK_SECTION_ALL},
    {GTK_STATE_NORMAL,      GTK_SHADOW_NONE, 0,
     TTK_STATE_BACKGROUND,  0,               GTKTTK_SECTION_ALL},
    {GTK_STATE_NORMAL,      GTK_SHADOW_NONE, 0,
     TTK_STATE_ALTERNATE,   0,               GTKTTK_SECTION_ALL},
    {GTK_STATE_INSENSITIVE, GTK_SHADOW_NONE, 0,
     TTK_STATE_INVALID,     0,               GTKTTK_SECTION_ALL},
    {GTK_STATE_INSENSITIVE, GTK_SHADOW_NONE, 0,
     TTK_STATE_READONLY,    0,               GTKTTK_SECTION_ALL},
    {GTK_STATE_NORMAL,      GTK_SHADOW_NONE, 0,
     0,                     0,               GTKTTK_SECTION_ALL}
  };
  unsigned int value = 0;
  gtkState  = GTK_STATE_NORMAL; 
  gtkShadow = GTK_SHADOW_NONE;
  if (!map) {
    map = default_map;
    /* Instead of writting huge tables, do some checks here... */
    if (section & GTKTTK_SECTION_STEPPERS) {
      gtkShadow = GTK_SHADOW_OUT;
      if (state & TTK_STATE_DISABLED) {
        gtkState = GTK_STATE_INSENSITIVE;
      } else if (state & TTK_STATE_PRESSED) {
        gtkState = GTK_STATE_ACTIVE;
        gtkShadow = GTK_SHADOW_IN;
      } else if (state & TTK_STATE_ACTIVE) {
        gtkState = GTK_STATE_PRELIGHT;
      }
      map = NULL; /* Do not search the table */
    } else if (section & GTKTTK_SECTION_BUTTONS ||
               section & GTKTTK_SECTION_ENTRY ||
               section & GTKTTK_SECTION_SCROLLBAR) {
      /* Whether the button is drawn pressed or not, is defined by shadow. */
      if (state & TTK_STATE_PRESSED || state & TTK_STATE_SELECTED) {
        gtkShadow = GTK_SHADOW_IN;
      } else {
        gtkShadow = GTK_SHADOW_OUT;
        // if (state & TTK_STATE_DISABLED || state & TTK_STATE_READONLY)
        //                                    gtkState  = GTK_STATE_INSENSITIVE;
        // else if (state & TTK_STATE_ACTIVE) gtkState  = GTK_STATE_PRELIGHT;
        // else if (state & TTK_STATE_FOCUS)  gtkState  = GTK_STATE_ACTIVE;
      }
      if ((state & TTK_STATE_ACTIVE) &&
             (!(state & TTK_STATE_PRESSED) && !(state & TTK_STATE_SELECTED))) {
        gtkState = GTK_STATE_PRELIGHT;
      } else {
        if (!(section & GTKTTK_SECTION_SCROLLBAR) &&
	    state & TTK_STATE_DISABLED || state & TTK_STATE_READONLY)
          gtkState  = GTK_STATE_INSENSITIVE;
        else if (state & TTK_STATE_PRESSED) gtkState  = GTK_STATE_ACTIVE;
        else if (state & TTK_STATE_DISABLED)
          gtkState  = GTK_STATE_INSENSITIVE;
	}
      map = NULL; /* Do not search the table */
    } else if (section & GTKTTK_SECTION_TROUGH) {
      if (state & TTK_STATE_PRESSED) {
        gtkState = GTK_STATE_ACTIVE;
      } else {
        gtkState = GTK_STATE_INSENSITIVE;
      }
      gtkShadow = GTK_SHADOW_IN;
      map = NULL; /* Do not search the table */
    } else if (section & GTKTTK_SECTION_SASH) {
      if (state & TTK_STATE_DISABLED || state & TTK_STATE_READONLY)
                                         gtkState  = GTK_STATE_INSENSITIVE;
      else if (state & TTK_STATE_FOCUS)  gtkState  = GTK_STATE_SELECTED;
      else if (state & TTK_STATE_ACTIVE) gtkState  = GTK_STATE_PRELIGHT;
      map = NULL; /* Do not search the table */
    } else if (section & GTKTTK_SECTION_TABS) {
      gtkState  = GTK_STATE_ACTIVE;
      gtkShadow = GTK_SHADOW_OUT;
      if (state & TTK_STATE_DISABLED || state & TTK_STATE_READONLY)
                                            gtkState  = GTK_STATE_INSENSITIVE;
      else if (state & TTK_STATE_SELECTED)  gtkState  = GTK_STATE_NORMAL;
      else if (state & TTK_STATE_ACTIVE)    gtkState  = GTK_STATE_PRELIGHT;
      else if (state & TTK_STATE_FOCUS)     gtkState  = GTK_STATE_ACTIVE;
      map = NULL; /* Do not search the table */
    }
  }
  if (map) {
    while ( (state & map->onBits)  != map->onBits  ||
           (~state & map->offBits) != map->offBits ||
           !(section & map->section)) {
      ++map; ++value;
    }
    gtkState  = map->state;
    gtkShadow = map->shadow;
  }
  return (map)? map->value : value;
}; /* GtkTtk_StateShadowTableLookup */

double GtkTtk_ValueFromSlider(GtkTtk_WidgetCache *wc, Tk_Window tkwin,
                               Ttk_Box b) {
  double value = 0.0;
  double ww = Tk_Width(tkwin), wh = Tk_Height(tkwin), dx = 0.0, x;
  if (wc->orientation == TTK_ORIENT_HORIZONTAL) {
    dx = b.width / 2.0;
    x = b.x + dx;
    value = x / ww;
  } else {
    dx = b.height / 2.0;
    x = b.y + dx;
    value = x / wh;
  }
  if (value < 0.0) value = 0.0;
  if (value > 1.0) value = 1.0;
  // printf("ww:%f, b.x:%d, x:%f, dx:%f, v:%f\n", ww, b.x, x, dx, value);
  return value;
}; /* GtkTtk_ValueFromSlider */
