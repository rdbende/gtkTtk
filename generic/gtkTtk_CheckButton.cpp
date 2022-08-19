/*
 *  gtkTtk_CheckButton.cpp
 * ------------------------
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

typedef struct {
} CheckButtonIndicatorElement;


static Ttk_ElementOptionSpec CheckButtonIndicatorElementOptions[] = {
    {NULL}
};

static void CheckButtonIndicatorElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    GTKTTK_WIDGET_CACHE_DEFINITION;
    gint size, spacing = CheckButtonHorizontalPadding;
    gint focus_width, focus_pad;
    GTKTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = GtkTtk_GetCheckButton(wc);
    GTKTTK_ENSURE_WIDGET_OK;
    GtkTtk_gtk_widget_style_get(widget,
           "indicator-size",    &size,
           "indicator-spacing", &spacing,
           "focus-line-width",  &focus_width,
           "focus-padding",     &focus_pad, NULL);
    *widthPtr = *heightPtr = spacing*2+size+focus_width+focus_pad;
    size = focus_width;
    *paddingPtr = Ttk_MakePadding(*widthPtr,
				  size,
				  *widthPtr,
				  size);
}

static void CheckButtonIndicatorElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTKTTK_GTK_DRAWABLE_DEFINITIONS;
    gint indicator_size, x, y;
    GTKTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = GtkTtk_GetCheckButton(wc);
    GTKTTK_ENSURE_WIDGET_OK;
    GTKTTK_DRAWABLE_FROM_WIDGET;
    style = GtkTtk_GetGtkWindowStyle(wc->gtkWindow);
    GTKTTK_DEFAULT_BACKGROUND;
    GTKTTK_STYLE_FROM_WIDGET;
    GTKTTK_WIDGET_SET_FOCUS(widget);
    GtkTtk_gtk_widget_style_get(widget,
           "indicator-size", &indicator_size, NULL);
    GtkTtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            GTKTTK_SECTION_BUTTONS|GTKTTK_SECTION_ALL);
    // GtkTtk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    x = (b.width  - indicator_size) / 2;
    y = (b.height - indicator_size) / 2;
    GtkTtk_gtk_paint_check(style, gdkDrawable, gtkState, gtkShadow, NULL,
            widget, "checkbutton", x, y, indicator_size, indicator_size);
    GtkTtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
            0, 0, b.width, b.height, b.x, b.y);
    GTKTTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec CheckButtonIndicatorElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(CheckButtonIndicatorElement),
    CheckButtonIndicatorElementOptions,
    CheckButtonIndicatorElementGeometry,
    CheckButtonIndicatorElementDraw
};

typedef struct {
} CheckButtonBorderElement;


static Ttk_ElementOptionSpec CheckButtonBorderElementOptions[] = {
    {NULL}
};

static void CheckButtonBorderElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    GTKTTK_WIDGET_CACHE_DEFINITION;
    gint focus_width;
    gint focus_pad;
    GTKTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = GtkTtk_GetCheckButton(wc);
    GTKTTK_ENSURE_WIDGET_OK;
    GtkTtk_gtk_widget_style_get(widget,
           "focus-line-width", &focus_width,
           "focus-padding",    &focus_pad, NULL);
    *paddingPtr = Ttk_UniformPadding(focus_width + focus_pad);
}

static void CheckButtonBorderElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTKTTK_GTK_DRAWABLE_DEFINITIONS;
    GTKTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = GtkTtk_GetCheckButton(wc);
    GTKTTK_ENSURE_WIDGET_OK;
    GTKTTK_DRAWABLE_FROM_WIDGET;
    GTKTTK_STYLE_BACKGROUND_DEFAULT;
    GTKTTK_DEFAULT_BACKGROUND;
    if (state & TTK_STATE_FOCUS) {
      gint border_width = ((GtkContainer*) widget)->border_width;
      gint focus_width;
      gint focus_pad;
      GTKTTK_STYLE_FROM_WIDGET;
      GTKTTK_WIDGET_SET_FOCUS(widget);
      GtkTtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            GTKTTK_SECTION_BUTTONS|GTKTTK_SECTION_ALL);
      GtkTtk_gtk_widget_style_get(widget,
           "focus-line-width", &focus_width,
           "focus-padding",    &focus_pad, NULL);
      GtkTtk_gtk_paint_focus(style, gdkDrawable, gtkState, NULL, widget,
              "checkbutton", b.x + border_width, b.y + border_width,
               b.width - 2 * border_width, b.height - 2 * border_width);
    }
    GtkTtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    GTKTTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec CheckButtonBorderElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(CheckButtonBorderElement),
    CheckButtonBorderElementOptions,
    CheckButtonBorderElementGeometry,
    CheckButtonBorderElementDraw
};

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

/*
 * TTK_BEGIN_LAYOUT(CheckbuttonLayout)
 *      TTK_GROUP("Checkbutton.border", TTK_FILL_BOTH,
 *          TTK_GROUP("Checkbutton.padding", TTK_FILL_BOTH,
 *              TTK_NODE("Checkbutton.indicator", TTK_PACK_LEFT)
 *              TTK_GROUP("Checkbutton.focus", TTK_PACK_LEFT | TTK_STICK_W,
 *                  TTK_NODE("Checkbutton.label", TTK_FILL_BOTH))))
 * TTK_END_LAYOUT
 */
TTK_BEGIN_LAYOUT(CheckbuttonLayout)
  TTK_GROUP("Checkbutton.border", TTK_FILL_BOTH,
    TTK_GROUP("Checkbutton.padding", TTK_FILL_BOTH,
      TTK_NODE("Checkbutton.indicator", TTK_PACK_LEFT)
      TTK_NODE("Checkbutton.label", TTK_PACK_LEFT|TTK_STICK_W|TTK_FILL_BOTH)))
TTK_END_LAYOUT

int GtkTtk_Init_CheckButton(Tcl_Interp *interp,
                       GtkTtk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "Checkbutton.border",
            &CheckButtonBorderElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Checkbutton.indicator",
            &CheckButtonIndicatorElementSpec, (void *) wc[0]);
    
    /*
     * Register layouts:
     */
    Ttk_RegisterLayout(themePtr, "TCheckbutton", CheckbuttonLayout);
    
    return TCL_OK;
}; /* GtkTtk_Init_CheckButton */
