/*
 *  gtkTtk_Entry.cpp
 * ------------------
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

#include "gtkTtk_Utilities.h"
#include "gtkTtk_TkHeaders.h"
#include "gtkTtk_WidgetDefaults.h"

#if 0
/*
 * Map between Tk/Tile & Gtk/GNOME state flags.
 */
static Ttk_StateTable entry_statemap[] =
{
};
#endif

typedef struct {
} EntryFieldElement;


static Ttk_ElementOptionSpec EntryFieldElementOptions[] = {
    {NULL}
};

static void EntryFieldElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    GTKTTK_WIDGET_CACHE_DEFINITION;
    // GtkBorder border = {0, 0, 0, 0};
    GTKTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = GtkTtk_GetEntry(wc);
    GTKTTK_ENSURE_WIDGET_OK;
    int xt = widget->style->xthickness;
    int yt = widget->style->ythickness;
    *paddingPtr = Ttk_MakePadding(xt + EntryUniformPadding,
                                  yt + EntryUniformPadding,
                                  xt + EntryUniformPadding,
                                  yt + EntryUniformPadding);
    // GtkTtk_gtk_widget_style_get(widget, "inner-border", &border, NULL);
    // GtkTtk_g_object_get(widget, "inner-border", &border, NULL);
    // *paddingPtr = GTKTTK_GTKBORDER_TO_PADDING(border);
}

static void EntryFieldElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTKTTK_GTK_DRAWABLE_DEFINITIONS;
    GTKTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    gboolean hasFrame = TRUE;
    GtkWidget *widget = GtkTtk_GetEntry(wc);
    GTKTTK_ENSURE_WIDGET_OK;
    GTKTTK_DRAWABLE_FROM_WIDGET;
    style = GtkTtk_GetGtkWindowStyle(wc->gtkWindow);
    GTKTTK_DEFAULT_BACKGROUND;
    if (hasFrame) {
      GTKTTK_STYLE_FROM_WIDGET;
      GtkTtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
              GTKTTK_SECTION_ENTRY|GTKTTK_SECTION_ALL);
      GtkTtk_gtk_paint_flat_box(style, gdkDrawable, gtkState, gtkShadow, NULL, widget,
          "entry_bg", 0, 0, b.width, b.height);
      GTKTTK_WIDGET_SET_FOCUS(widget);
            GtkTtk_gtk_paint_shadow(style, gdkDrawable, gtkState, gtkShadow, NULL,
                          widget, "entry", 0, 0, b.width, b.height);
    }
    // GtkTtk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    GtkTtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    GTKTTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec EntryFieldElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(EntryFieldElement),
    EntryFieldElementOptions,
    EntryFieldElementGeometry,
    EntryFieldElementDraw
};

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

int GtkTtk_Init_Entry(Tcl_Interp *interp,
                       GtkTtk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "Entry.field",
            &EntryFieldElementSpec, (void *) wc[0]);
    
    /*
     * Register layouts:
     */

    return TCL_OK;
}; /* GtkTtk_Init_Entry */
