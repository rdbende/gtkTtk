/*
 *  gtkTtk_SizeGrip.cpp
 * ---------------------
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
static Ttk_StateTable size_grip_statemap[] =
{
};
#endif

typedef struct {
} SizeGripElement;

static Ttk_ElementOptionSpec SizeGripElementOptions[] = {
    {NULL}
};

static void SizeGripElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    GTKTTK_WIDGET_CACHE_DEFINITION;
    GTKTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = GtkTtk_GetStatusBar(wc);
    *widthPtr  = 18;
    *heightPtr = 18;
    GTKTTK_ENSURE_WIDGET_OK;
    *paddingPtr = Ttk_MakePadding(0, 0, widget->style->xthickness,
                                        widget->style->ythickness);
}

static void SizeGripElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTKTTK_GTK_DRAWABLE_DEFINITIONS;
    GTKTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    /* GTKTTK_SETUP_GTK_DRAWABLE; */
    GtkWidget *widget = wc->gtkWindow;
    GTKTTK_ENSURE_WIDGET_OK;
    GTKTTK_STYLE_FROM_WIDGET;
    GTKTTK_DRAWABLE_FROM_WIDGET;
    // GtkTtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
    //         GTKTTK_SECTION_ALL);
    GTKTTK_DEFAULT_BACKGROUND;
    // GtkTtk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    GtkTtk_gtk_paint_resize_grip(style, gdkDrawable, GTK_STATE_NORMAL, NULL, widget,
       "window", GDK_WINDOW_EDGE_SOUTH_EAST, 0, 0, b.width, b.height);
    GtkTtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    GTKTTK_CLEANUP_GTK_DRAWABLE;
}; /* SizeGripElementDraw */

static Ttk_ElementSpec SizeGripElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(SizeGripElement),
    SizeGripElementOptions,
    SizeGripElementGeometry,
    SizeGripElementDraw
};

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

TTK_BEGIN_LAYOUT(SizegripLayout)
    TTK_NODE("Sizegrip.sizegrip", TTK_PACK_BOTTOM|TTK_STICK_S|TTK_STICK_E)
TTK_END_LAYOUT

int GtkTtk_Init_SizeGrip(Tcl_Interp *interp,
                       GtkTtk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "sizegrip",
            &SizeGripElementSpec, (void *) wc[0]);
    
    /*
     * Register layouts:
     */
    Ttk_RegisterLayout(themePtr, "TSizegrip", SizegripLayout);

    return TCL_OK;
}; /* GtkTtk_Init_SizeGrip */
