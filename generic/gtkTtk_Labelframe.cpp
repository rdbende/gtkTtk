/*
 *  gtkTtk_Labelframe.cpp
 * -----------------------
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
static Ttk_StateTable labelframe_statemap[] =
{
};
#endif

typedef struct {
} LabelframeBorderElement;


static Ttk_ElementOptionSpec LabelframeBorderElementOptions[] = {
    {NULL}
};

static void LabelframeBorderElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    *paddingPtr = Ttk_UniformPadding(LabelframeUniformPadding);
}

static void LabelframeBorderElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTKTTK_GTK_DRAWABLE_DEFINITIONS;
    GTKTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    /* GTKTTK_SETUP_GTK_DRAWABLE; */
    GtkWidget *widget = GtkTtk_GetFrame(wc);
    GTKTTK_ENSURE_WIDGET_OK;
    GTKTTK_STYLE_FROM_WIDGET;
    GTKTTK_DRAWABLE_FROM_WIDGET;
    // GTKTTK_ATTACH_STYLE_TO_WIDGET;
    // GTKTTK_SETUP_WIDGET_SIZE(b.width, b.height);
    // GTKTTK_WIDGET_SET_FOCUS(widget);
    GTKTTK_DEFAULT_BACKGROUND;
    // GtkTtk_gtk_paint_flat_box(style, gdkDrawable, gtkState, gtkShadow, NULL,
    //                            widget, "frame", 0, 0, b.width, b.height);
    GtkTtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, 0, 0);
    GtkTtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            GTKTTK_SECTION_BUTTONS|GTKTTK_SECTION_ALL);
    GtkTtk_gtk_paint_shadow(style, gdkDrawable, gtkState, gtkShadow, NULL, widget,
                     "frame", 0, 0, b.width, b.height);
    GtkTtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    GTKTTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec LabelframeBorderElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(LabelframeBorderElement),
    LabelframeBorderElementOptions,
    LabelframeBorderElementGeometry,
    LabelframeBorderElementDraw
};

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

TTK_BEGIN_LAYOUT(LabelframeLayout)
    /* Note: labelframe widget does its own layout */
    TTK_NODE("Labelframe.border", TTK_FILL_BOTH)
TTK_END_LAYOUT

int GtkTtk_Init_Labelframe(Tcl_Interp *interp,
                       GtkTtk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "Labelframe.border",
            &LabelframeBorderElementSpec, (void *) wc[0]);
    
    /*
     * Register layouts:
     */
    Ttk_RegisterLayout(themePtr, "TLabelframe", LabelframeLayout);

    return TCL_OK;
}; /* GtkTtk_Init_Labelframe */
