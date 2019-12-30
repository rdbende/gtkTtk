/*
 * $Id: gtkTtk_Init.cpp,v 1.15 2012/07/22 19:26:14 petasis Exp $
 *
 * Copyright (C) 2004-2008 Georgios Petasis
 *
 * The gtkTtk theme is a Tk/Tile theme that uses Gtk/GNOME for drawing.
 */

#include "gtkTtk_Utilities.h"
#include "gtkTtk_TkHeaders.h"
#include <string.h>

static int GtkTtk_GtkAppCreated = 0;

extern GtkTtk_WidgetCache **GtkTtk_CreateGtkApp(Tcl_Interp *interp);
extern void GtkTtk_DestroyGtkApp(void);

static char initScript[] =
  "namespace eval gtkTtk { };"
  "namespace eval ttk::theme::gtkTtk { variable version "
                                                 PACKAGE_VERSION " };"
  "tcl_findLibrary gtkTtk $ttk::theme::gtkTtk::version "
  "$ttk::theme::gtkTtk::version gtkTtk.tcl GTKTTK_LIBRARY gtkTtk::library;";
#ifdef GTKTTK_LOAD_GTK_DYNAMICALLY
static char libsInitScript[] =
  "ttk::theme::gtkTtk::loadLibraries";
#endif /* GTKTTK_LOAD_GTK_DYNAMICALLY */

/*
 * Exit Handler.
 */
static void GtkTtk_ExitProc(ClientData data) {
  //Tcl_MutexLock(&gtkTtkMutex);
  //GtkTtk_DestroyGtkApp();
  //Tcl_MutexUnlock(&gtkTtkMutex);
  Tcl_MutexLock(&gtkTtkMutex);
  // printf("GtkTtk_ExitProc: %d\n", GtkTtk_GtkAppCreated); fflush(NULL);
  if (GtkTtk_GtkAppCreated < 0) {
    Tcl_MutexUnlock(&gtkTtkMutex);
    return;
  }
  --GtkTtk_GtkAppCreated;
  if (GtkTtk_GtkAppCreated == 0) {
    // printf("GtkTtk_ExitProc: %d <- GtkTtk_DestroyGtkApp();\n",
    //       GtkTtk_GtkAppCreated); fflush(NULL);
    GtkTtk_DestroyGtkApp();
  }
  Tcl_MutexUnlock(&gtkTtkMutex);
  return;
}; /* GtkTtk_ExitProc */

/*
 * Helper Functions
 */
int Gtkttk_ThemeName(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  // GtkTtk_WidgetCache **wc_array = (GtkTtk_WidgetCache **) clientData;
  // GtkTtk_WidgetCache *wc;
  // if (!wc_array || !wc_array[0]) {
  //   Tcl_SetResult(interp, (char *) "empty wc_array!", TCL_STATIC);
  //   return TCL_ERROR;
  // }
  // wc = wc_array[0];
  // if (!wc->gtkStyle) {
  //   Tcl_SetResult(interp, (char *) "empty wc->gtkStyle!", TCL_STATIC);
  //   return TCL_ERROR;
  // }
  GtkSettings *settings = NULL;
  gchar       *strval = NULL;
  if (objc != 1) {Tcl_WrongNumArgs(interp, 1, objv, ""); return TCL_ERROR;}

  Tcl_MutexLock(&gtkTtkMutex);
  settings = GtkTtk_gtk_settings_get_default();
  if (settings) {
    GtkTtk_g_object_get(settings, "gtk-theme-name", &strval, NULL);
    if (strval) {
      Tcl_SetResult(interp, (char *) strval, TCL_VOLATILE);
      GtkTtk_g_free(strval);
    }
  }
  Tcl_MutexUnlock(&gtkTtkMutex);
  return TCL_OK;
}; /* Gtkttk_ThemeName */

int Gtkttk_SettingsProperty(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  static const char *Methods[] = {
    "integer", "boolean", "string", (char *) NULL
  };
  enum methods {
    INTEGER, BOOLEAN, STRING
  };
  int type = STRING;
  GtkSettings *settings = NULL;
  gchar       *s_val = NULL;
  gboolean     b_val = FALSE;
  gint         i_val = 0;
  if (objc != 2 && objc != 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "property ?integer|boolean|string?");
    return TCL_ERROR;
  }
  if (objc == 3) {
    if (Tcl_GetIndexFromObj(interp, objv[2], (const char **) Methods,
                            "method", 0, &type) != TCL_OK) {
      return TCL_ERROR;
    }
  }

  Tcl_MutexLock(&gtkTtkMutex);
  settings = GtkTtk_gtk_settings_get_default();
  if (settings) {
    switch ((enum methods) type) {
      case INTEGER:
        GtkTtk_g_object_get(settings, Tcl_GetString(objv[1]), &i_val, NULL);
        Tcl_SetObjResult(interp, Tcl_NewIntObj(i_val));
        break;
      case BOOLEAN:
        GtkTtk_g_object_get(settings, Tcl_GetString(objv[1]), &b_val, NULL);
        if (b_val) Tcl_SetObjResult(interp, Tcl_NewBooleanObj(1));
        else Tcl_SetObjResult(interp, Tcl_NewBooleanObj(0));
        break;
      case STRING:
        GtkTtk_g_object_get(settings, Tcl_GetString(objv[1]), &s_val, NULL);
        if (s_val) {
          Tcl_SetResult(interp, (char *) s_val, TCL_VOLATILE);
          GtkTtk_g_free (s_val);
        }
        break;
    }
  }
  Tcl_MutexUnlock(&gtkTtkMutex);
  return TCL_OK;
}; /* Gtkttk_SettingsProperty */

#define GETPROPERTY_GTK_WIDGET_GET       0
#define GETPROPERTY_GTK_WIDGET_STYLE_GET 1
int Gtkttk_GetProperty(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[], int gtkMethod) {
  static const char *Methods[] = {
    "integer", "boolean", "string", (char *) NULL
  };
  enum methods {
    INTEGER, BOOLEAN, STRING
  };
  static const char *Widgets[] = {
    "GtkHScrollbar" , "GtkVScrollbar", "GtkNotebook", "GtkButton",
    (char *) NULL
  };
  enum widgets {
    W_HSCROLLBAR, W_VSCROLLBAR, W_NOTEBOOK, W_BUTTON
  };
  int          type  = STRING;
  gchar       *s_val = NULL;
  gboolean     b_val = FALSE;
  gint         i_val = 0;
  GtkWidget   *widget = NULL;
  GtkTtk_WidgetCache **wc_array = (GtkTtk_WidgetCache **) clientData;
  GtkTtk_WidgetCache *wc;
  if (!wc_array || !wc_array[0]) {
    Tcl_SetResult(interp, (char *) "empty wc_array!", TCL_STATIC);
    return TCL_ERROR;
  }
  wc = wc_array[0];
  if (objc != 3 && objc != 4) {
    Tcl_WrongNumArgs(interp, 1, objv,
       "widget property ?integer|boolean|string?");
    return TCL_ERROR;
  }
  /* Get widget... */
  if (Tcl_GetIndexFromObj(interp, objv[1], (const char **) Widgets,
                                 "widget", 0, &type) != TCL_OK) {
    return TCL_ERROR;
  }
  switch ((enum widgets) type) {
    case W_HSCROLLBAR: {widget = GtkTtk_GetHScrollBar(wc); break;}
    case W_VSCROLLBAR: {widget = GtkTtk_GetVScrollBar(wc); break;}
    case W_NOTEBOOK:   {widget = GtkTtk_GetNotebook(wc);   break;}
    case W_BUTTON:     {widget = GtkTtk_GetButton(wc);     break;}
  }
  /* Get property type, which defaults to "string"... */
  if (objc == 4) {
    if (Tcl_GetIndexFromObj(interp, objv[3], (const char **) Methods,
                            "method", 0, &type) != TCL_OK) {
      return TCL_ERROR;
    }
  }

  Tcl_MutexLock(&gtkTtkMutex);
  if (widget) {
    switch ((enum methods) type) {
      case INTEGER:
        switch (gtkMethod) {
          case GETPROPERTY_GTK_WIDGET_GET:
            GtkTtk_gtk_object_get((GtkObject *) widget,
                Tcl_GetString(objv[2]), &i_val, NULL);
          break;
          case GETPROPERTY_GTK_WIDGET_STYLE_GET:
            GtkTtk_gtk_widget_style_get(widget, Tcl_GetString(objv[2]),
                                         &i_val, NULL);
            break;
        }
        Tcl_SetObjResult(interp, Tcl_NewIntObj(i_val));
        break;
      case BOOLEAN:
        switch (gtkMethod) {
          case GETPROPERTY_GTK_WIDGET_GET:
            GtkTtk_gtk_object_get((GtkObject *) widget,
                Tcl_GetString(objv[2]), &b_val, NULL);
            break;
          case GETPROPERTY_GTK_WIDGET_STYLE_GET:
            GtkTtk_gtk_widget_style_get(widget, Tcl_GetString(objv[2]),
                                         &b_val, NULL);
            break;
        }
        if (b_val) Tcl_SetObjResult(interp, Tcl_NewBooleanObj(1));
        else Tcl_SetObjResult(interp, Tcl_NewBooleanObj(0));
        break;
      case STRING:
        switch (gtkMethod) {
          case GETPROPERTY_GTK_WIDGET_GET:
            GtkTtk_gtk_object_get((GtkObject *) widget,
                Tcl_GetString(objv[2]), &s_val, NULL);
            break;
          case GETPROPERTY_GTK_WIDGET_STYLE_GET:
            GtkTtk_gtk_widget_style_get(widget, Tcl_GetString(objv[2]),
                                         &s_val, NULL);
            break;
        }
        if (s_val) {
          Tcl_SetResult(interp, (char *) s_val, TCL_VOLATILE);
          GtkTtk_g_free (s_val);
        }
        break;
    }
  }
  Tcl_MutexUnlock(&gtkTtkMutex);
  return TCL_OK;
}; /* Gtkttk_GetProperty */

int Gtkttk_WidgetProperty(ClientData clientData, Tcl_Interp *interp,
                          int objc, Tcl_Obj *const objv[]) {
  return Gtkttk_GetProperty(clientData, interp, objc, objv,
                            GETPROPERTY_GTK_WIDGET_GET);
}; /* Gtkttk_WidgetProperty */

int Gtkttk_WidgetStyleProperty(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  return Gtkttk_GetProperty(clientData, interp, objc, objv,
                            GETPROPERTY_GTK_WIDGET_STYLE_GET);
}; /* Gtkttk_WidgetStyleProperty */

int Gtkttk_GtkEnum(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  static const char *Methods[] = {
    "GtkPositionType", (char *) NULL
  };
  enum methods {
    GTKPOSITIONTYPE,
  };
  int index, v;
  const char *n = NULL;
  if (objc != 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "gtk_enum_type value");
    return TCL_ERROR;
  }
  /* Get widget... */
  if (Tcl_GetIndexFromObj(interp, objv[1], (const char **) Methods,
                                 "gtk_enum_type", 0, &index) != TCL_OK) {
    return TCL_ERROR;
  }
  if (Tcl_GetIntFromObj(interp, objv[2], &v) != TCL_OK) {
    return TCL_ERROR;
  }
  switch ((enum methods) index) {
    case GTKPOSITIONTYPE: {
      if      (v == GTK_POS_LEFT)   n = "GTK_POS_LEFT";
      else if (v == GTK_POS_RIGHT)  n = "GTK_POS_RIGHT";
      else if (v == GTK_POS_TOP)    n = "GTK_POS_TOP";
      else if (v == GTK_POS_BOTTOM) n = "GTK_POS_BOTTOM";
      break;
    }
  }
  if (!n) {
    Tcl_SetResult(interp, (char *) "value out of range", TCL_STATIC);
    return TCL_ERROR;
  }
  Tcl_SetResult(interp, (char *) n, TCL_STATIC);
  return TCL_OK;
}; /* Gtkttk_GtkEnum */

int Gtkttk_GtkDirectory(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  static const char *Methods[] = {
    "theme", "default_files", (char *) NULL
  };
  enum methods {
    THEME, DEFAULT_FILES
  };
  int type;
  gchar *dir = NULL, **dirs = NULL;
  
  if (objc != 2 && objc != 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "category ?value?");
    return TCL_ERROR;
  }
  if (Tcl_GetIndexFromObj(interp, objv[1], (const char **) Methods,
                          "method", 0, &type) != TCL_OK) {
    return TCL_ERROR;
  }

  Tcl_MutexLock(&gtkTtkMutex);
  switch ((enum methods) type) {
    case THEME:
      dir = GtkTtk_gtk_rc_get_theme_dir();
      break;
    case DEFAULT_FILES:
      if (objc == 3) {
        int mobjc; Tcl_Obj **mobjv;
        if (Tcl_ListObjGetElements(interp, objv[2], &mobjc, &mobjv) != TCL_OK) {
          return TCL_ERROR;
        }
        dirs = GtkTtk_g_new0(gchar *, mobjc+1);
        for (int i = 0; i < mobjc; ++i) {
          Tcl_IncrRefCount(mobjv[i]);
          dirs[i] = Tcl_GetString(mobjv[i]);
        }
        GtkTtk_gtk_rc_set_default_files(dirs);
        for (int i = 0; i < mobjc; ++i) {
          Tcl_DecrRefCount(mobjv[i]);
        }
        GtkTtk_g_free(dirs); dirs = NULL;
      } else {
        dirs = GtkTtk_gtk_rc_get_default_files();
      }
      break;
  }
  if (dir) {
    Tcl_SetResult(interp, (char *) dir, TCL_VOLATILE);
    GtkTtk_g_free(dir);
  }
  if (dirs) {
    Tcl_Obj *list = Tcl_NewListObj(0, NULL);
    while (*dirs) {
      Tcl_ListObjAppendElement(NULL, list, Tcl_NewStringObj(*dirs, -1));
      ++dirs;
    }
    Tcl_SetObjResult(interp, list);
  }
  Tcl_MutexUnlock(&gtkTtkMutex);
  return TCL_OK;
}; /* Gtkttk_GtkDirectory */

int Gtkttk_gtk_method(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  static const char *Methods[] = {
    "gtk_rc_reparse_all_for_settings", "gtk_rc_reset_styles", (char *) NULL
  };
  enum methods {
    GTK_RC_REPARSE_ALL_FOR_SETTINGS, GTK_RC_RESET_STYLES
  };
  int type;
  
  if (objc != 2) {
    Tcl_WrongNumArgs(interp, 1, objv, "method"); return TCL_ERROR;
  }
  if (Tcl_GetIndexFromObj(interp, objv[1], (const char **) Methods,
                          "method", 0, &type) != TCL_OK) {
    return TCL_ERROR;
  }

  Tcl_MutexLock(&gtkTtkMutex);
  switch ((enum methods) type) {
    case GTK_RC_REPARSE_ALL_FOR_SETTINGS:
      GtkTtk_gtk_rc_reparse_all_for_settings(
              GtkTtk_gtk_settings_get_default(), TRUE);
      break;
    case GTK_RC_RESET_STYLES:
      GtkTtk_gtk_rc_reset_styles(GtkTtk_gtk_settings_get_default());
      break;
  }
  Tcl_MutexUnlock(&gtkTtkMutex);
  return TCL_OK;
}; /* Gtkttk_gtk_method */

int Gtkttk_ThemeColour(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  static const char *Methods[] = {
    "fg(NORMAL)",           "fg(PRELIGHT)",           "fg(ACTIVE)",
    "fg(SELECTED)",         "fg(INSENSITIVE)",
    "bg(NORMAL)",           "bg(PRELIGHT)",           "bg(ACTIVE)",
    "bg(SELECTED)",         "bg(INSENSITIVE)",
    "base(NORMAL)",         "base(PRELIGHT)",         "base(ACTIVE)",
    "base(SELECTED)",       "base(INSENSITIVE)",
    "text(NORMAL)",         "text(PRELIGHT)",         "text(ACTIVE)",
    "text(SELECTED)",       "text(INSENSITIVE)",
    "light(NORMAL)",        "light(PRELIGHT)",        "light(ACTIVE)",
    "light(SELECTED)",      "light(INSENSITIVE)",
    "mid(NORMAL)",          "mid(PRELIGHT)",          "mid(ACTIVE)",
    "mid(SELECTED)",        "mid(INSENSITIVE)",
    "dark(NORMAL)",         "dark(PRELIGHT)",         "dark(ACTIVE)",
    "dark(SELECTED)",       "dark(INSENSITIVE)",
    "text_aa(NORMAL)",      "text_aa(PRELIGHT)",      "text_aa(ACTIVE)",
    "text_aa(SELECTED)",    "text_aa(INSENSITIVE)",
    (char *) NULL
  };
  enum methods {
    FG_NORMAL,        FG_PRELIGHT,        FG_ACTIVE,
    FG_SELECTED,      FG_INSENSITIVE,
    BG_NORMAL ,       BG_PRELIGHT,        BG_ACTIVE,
    BG_SELECTED,      BG_INSENSITIVE,
    BASE_NORMAL,      BASE_PRELIGHT,      BASE_ACTIVE,
    BASE_SELECTED,    BASE_INSENSITIVE,
    TEXT_NORMAL,      TEXT_PRELIGHT,      TEXT_ACTIVE,
    TEXT_SELECTED,    TEXT_INSENSITIVE,
    LIGHT_NORMAL,     LIGHT_PRELIGHT,     LIGHT_ACTIVE,
    LIGHT_SELECTED,   LIGHT_INSENSITIVE,
    MID_NORMAL,       MID_PRELIGHT,       MID_ACTIVE,
    MID_SELECTED,     MID_INSENSITIVE,
    DARK_NORMAL,      DARK_PRELIGHT,      DARK_ACTIVE,
    DARK_SELECTED,    DARK_INSENSITIVE,
    TAA_NORMAL,       TAA_PRELIGHT,       TAA_ACTIVE,
    TAA_SELECTED,     TAA_INSENSITIVE
  };
  GtkTtk_WidgetCache **wc = (GtkTtk_WidgetCache **) clientData;
  GdkColor colour;
  gchar* colour_str = NULL;
  GtkStyle *style;
  int index;
  if (!wc) {
    Tcl_SetResult(interp, (char *) "empty wc!", TCL_STATIC);
    return TCL_ERROR;
  }
  style = GtkTtk_GetGtkWindowStyle(wc[0]->gtkWindow);
  if (!style) {
    Tcl_SetResult(interp, (char *) "empty style!", TCL_STATIC);
    return TCL_ERROR;
  }
  if (objc != 2) {
    Tcl_WrongNumArgs(interp, 1, objv, "colour");
    return TCL_ERROR;
  }

  if (Tcl_GetIndexFromObj(interp, objv[1], (const char **) Methods,
                          "method", 0, &index) == TCL_OK) {
  switch ((enum methods) index) {
    case FG_NORMAL:         colour = style->fg[GTK_STATE_NORMAL];         break;
    case FG_PRELIGHT:       colour = style->fg[GTK_STATE_PRELIGHT];       break;
    case FG_ACTIVE:         colour = style->fg[GTK_STATE_ACTIVE];         break;
    case FG_SELECTED:       colour = style->fg[GTK_STATE_SELECTED];       break;
    case FG_INSENSITIVE:    colour = style->fg[GTK_STATE_INSENSITIVE];    break;
    case BG_NORMAL:         colour = style->bg[GTK_STATE_NORMAL];         break;
    case BG_PRELIGHT:       colour = style->bg[GTK_STATE_PRELIGHT];       break;
    case BG_ACTIVE:         colour = style->bg[GTK_STATE_ACTIVE];         break;
    case BG_SELECTED:       colour = style->bg[GTK_STATE_SELECTED];       break;
    case BG_INSENSITIVE:    colour = style->bg[GTK_STATE_INSENSITIVE];    break;
    case BASE_NORMAL:       colour = style->base[GTK_STATE_NORMAL];       break;
    case BASE_PRELIGHT:     colour = style->base[GTK_STATE_PRELIGHT];     break;
    case BASE_ACTIVE:       colour = style->base[GTK_STATE_ACTIVE];       break;
    case BASE_SELECTED:     colour = style->base[GTK_STATE_SELECTED];     break;
    case BASE_INSENSITIVE:  colour = style->base[GTK_STATE_INSENSITIVE];  break;
    case TEXT_NORMAL:       colour = style->text[GTK_STATE_NORMAL];       break;
    case TEXT_PRELIGHT:     colour = style->text[GTK_STATE_PRELIGHT];     break;
    case TEXT_ACTIVE:       colour = style->text[GTK_STATE_ACTIVE];       break;
    case TEXT_SELECTED:     colour = style->text[GTK_STATE_SELECTED];     break;
    case TEXT_INSENSITIVE:  colour = style->text[GTK_STATE_INSENSITIVE];  break;
    case LIGHT_NORMAL:      colour = style->light[GTK_STATE_NORMAL];      break;
    case LIGHT_PRELIGHT:    colour = style->light[GTK_STATE_PRELIGHT];    break;
    case LIGHT_ACTIVE:      colour = style->light[GTK_STATE_ACTIVE];      break;
    case LIGHT_SELECTED:    colour = style->light[GTK_STATE_SELECTED];    break;
    case LIGHT_INSENSITIVE: colour = style->light[GTK_STATE_INSENSITIVE]; break;
    case DARK_NORMAL:       colour = style->dark[GTK_STATE_NORMAL];       break;
    case DARK_PRELIGHT:     colour = style->dark[GTK_STATE_PRELIGHT];     break;
    case DARK_ACTIVE:       colour = style->dark[GTK_STATE_ACTIVE];       break;
    case DARK_SELECTED:     colour = style->dark[GTK_STATE_SELECTED];     break;
    case DARK_INSENSITIVE:  colour = style->dark[GTK_STATE_INSENSITIVE];  break;
    case MID_NORMAL:        colour = style->mid[GTK_STATE_NORMAL];        break;
    case MID_PRELIGHT:      colour = style->mid[GTK_STATE_PRELIGHT];      break;
    case MID_ACTIVE:        colour = style->mid[GTK_STATE_ACTIVE];        break;
    case MID_SELECTED:      colour = style->mid[GTK_STATE_SELECTED];      break;
    case MID_INSENSITIVE:   colour = style->mid[GTK_STATE_INSENSITIVE];   break;
    case TAA_NORMAL:        colour = style->text_aa[GTK_STATE_NORMAL];    break;
    case TAA_PRELIGHT:      colour = style->text_aa[GTK_STATE_PRELIGHT];  break;
    case TAA_ACTIVE:        colour = style->text_aa[GTK_STATE_ACTIVE];    break;
    case TAA_SELECTED:      colour = style->text_aa[GTK_STATE_SELECTED];  break;
    case TAA_INSENSITIVE:   colour = style->text_aa[GTK_STATE_INSENSITIVE];
  }
    colour_str = GtkTtk_gdk_color_to_string(&colour);
  } else {
    if (GtkTtk_gtk_style_lookup_color(style, Tcl_GetString(objv[1]), &colour)){
      colour_str = GtkTtk_gdk_color_to_string(&colour);
    }
  }

  if (colour_str) {
    Tcl_SetResult(interp, (char *) colour_str, TCL_VOLATILE);
    GtkTtk_g_free(colour_str);
    return TCL_OK;
  }
  Tcl_SetResult(interp, (char *) "colour not found: ", TCL_STATIC);
  Tcl_AppendResult(interp, (char *) Tcl_GetString(objv[1]), NULL);
  return TCL_ERROR;
}; /* Gtkttk_ThemeColour */

#ifndef GTKTTK_LOAD_GTK_DYNAMICALLY
#ifndef GTK_STYLE_GET_PRIVATE
struct _GtkStylePrivate {
  GSList *color_hashes;
};
typedef struct _GtkStylePrivate GtkStylePrivate;
#define GTK_STYLE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), GTK_TYPE_STYLE, GtkStylePrivate))
#endif
#endif /* GTKTTK_LOAD_GTK_DYNAMICALLY */

int Gtkttk_ColourKeys(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  GtkTtk_WidgetCache **wc = (GtkTtk_WidgetCache **) clientData;
  GtkStyle *style;
  if (!wc) {
    Tcl_SetResult(interp, (char *) "empty wc!", TCL_STATIC);
    return TCL_ERROR;
  }
  style = GtkTtk_GetGtkWindowStyle(wc[0]->gtkWindow);
  if (!style) {
    Tcl_SetResult(interp, (char *) "empty style!", TCL_STATIC);
    return TCL_ERROR;
  }
#ifndef GTKTTK_LOAD_GTK_DYNAMICALLY
  GtkStylePrivate *priv = GTK_STYLE_GET_PRIVATE(style);
  GSList *iter;
  Tcl_Obj *list = Tcl_NewListObj(0, NULL);
  for (iter = priv->color_hashes; iter != NULL; iter = iter->next) {
    GHashTable *hash    = (GHashTable *) iter->data;
    GList *keys = GtkTtk_g_hash_table_get_keys(hash);
    for (; keys != NULL; keys = keys->next) {
      Tcl_ListObjAppendElement(NULL, list,
                               Tcl_NewStringObj((char *) keys->data, -1));
    }
  }
  Tcl_SetObjResult(interp, list);
#endif /* GTKTTK_LOAD_GTK_DYNAMICALLY */
  return TCL_OK;
}; /* Gtkttk_ColourKeys */

int Gtkttk_InitialiseLibrary(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  static const char *Methods[] = {
    "required",
    "gdk",  "gdk_pixbuf", "gdk_pixbuf_xlib",
    "glib", "gobject",
    "gtk",
    (char *) NULL
  };
  enum methods {
    L_REQUIRED,
    L_GDK,  L_GDK_PIXBUF, L_GDK_PIXBUF_XLIB,
    L_GLIB, L_GOBJECT,
    L_GTK
  };
  int index, status;
  if (objc != 2 && objc != 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "required|library ?filename?");
    return TCL_ERROR;
  }
  if (Tcl_GetIndexFromObj(interp, objv[1], (const char **) Methods,
                          "method", 0, &index) != TCL_OK)
    return TCL_ERROR;
  Tcl_MutexLock(&gtkTtkMutex);
  switch ((enum methods) index) {
    case L_REQUIRED:
#ifdef GTKTTK_LOAD_GTK_DYNAMICALLY
      Tcl_SetObjResult(interp, Tcl_NewBooleanObj(1));
#else
      Tcl_SetObjResult(interp, Tcl_NewBooleanObj(0));
#endif /* GTKTTK_LOAD_GTK_DYNAMICALLY */
      Tcl_MutexUnlock(&gtkTtkMutex);
      return TCL_OK;
#ifdef GTKTTK_LOAD_GTK_DYNAMICALLY
    case L_GDK:
      if (!GTKTTK_LAST_SYMBOL_gdk && objc > 2) {
        status = GtkTtk_InitialiseSymbols_gdk(interp, objv[2]);
      }
      break;
    case L_GDK_PIXBUF:
      if (!GTKTTK_LAST_SYMBOL_gdk_pixbuf && objc > 2) {
        status = GtkTtk_InitialiseSymbols_gdk_pixbuf(interp, objv[2]);
      }
      break;
    case L_GDK_PIXBUF_XLIB:
#ifndef __WIN32__
      if (!GTKTTK_LAST_SYMBOL_gdk_pixbuf_xlib && objc > 2) {
        status = GtkTtk_InitialiseSymbols_gdk_pixbuf_xlib(interp, objv[2]);
      }
#endif
      break;
    case L_GLIB:
      if (!GTKTTK_LAST_SYMBOL_glib && objc > 2) {
        status = GtkTtk_InitialiseSymbols_glib(interp, objv[2]);
      }
      break;
    case L_GOBJECT:
      if (!GTKTTK_LAST_SYMBOL_gobject && objc > 2) {
        status = GtkTtk_InitialiseSymbols_gobject(interp, objv[2]);
      }
      break;
    case L_GTK:
      if (!GTKTTK_LAST_SYMBOL_gtk && objc > 2) {
        status = GtkTtk_InitialiseSymbols_gtk(interp, objv[2]);
      }
      break;
#endif /* GTKTTK_LOAD_GTK_DYNAMICALLY */
  }
  Tcl_MutexUnlock(&gtkTtkMutex);
  return status;
}; /* Gtkttk_InitialiseLibrary */

int Gtkttk_SetPalette(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
#if 0
  static const char *Methods[] = {
    "-background",       "-foreground",
    "-buttonBackground", "-buttonForeground",
    "-selectBackground", "-selectForeground",
    "-windowBackground", "-windowForeground",
    "-linkColor",        "-visitedLinkColor",
    "-contrast",
    (char *) NULL
  };
  enum methods {
    CLR_background,       CLR_foreground,
    CLR_buttonBackground, CLR_buttonForeground,
    CLR_selectBackground, CLR_selectForeground,
    CLR_windowBackground, CLR_windowForeground,
    CLR_linkColor,        CLR_visitedLinkColor,
    CLR_contrast
  };
  int index, contrast_;
  char *value;
  if ((objc-1)%2) {
    Tcl_WrongNumArgs(interp, 1, objv, "?-key value?");
    return TCL_ERROR;
  }
  Tcl_MutexLock(&gtkTtkMutex);
  Tcl_MutexUnlock(&gtkTtkMutex);
#endif
  return TCL_OK;
}; /* Gtkttk_SetPalette */

int Gtkttk_SetStyle(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  if (objc != 2) {Tcl_WrongNumArgs(interp, 1, objv, "style"); return TCL_ERROR;}
  Tcl_MutexLock(&gtkTtkMutex);
#if 0
  GtkTtk_WidgetCache **wc_array = (GtkTtk_WidgetCache **) clientData;
  GtkTtk_WidgetCache *wc = wc_array[0];
  if (qApp) {
    int len;
    const char* str = Tcl_GetStringFromObj(objv[1], &len);
    QString style = QString::fromUtf8(str, len);
    QStyle *new_style = QStyleFactory::create(style);
    QStyle *todelete = NULL;
    if (new_style == NULL) {
      Tcl_SetResult(interp, (char *) "unknwon style: \"", TCL_STATIC);
      Tcl_AppendResult(interp, str, "\"", NULL);
      Tcl_MutexUnlock(&gtkTtkMutex);
      return TCL_ERROR;
    }
    //qApp->setStyle(style);
    /* Is this style the qApp style? */
    if (wc->GtkTtk_Style_Owner) todelete = wc->GtkTtk_Style;

#ifdef GTKTTK_GTK_VERSION_3                    
    if (strcmp(qApp->style().name(), str) == 0) {
      wc->GtkTtk_Style = &(qApp->style());
#endif /* GTKTTK_GTK_VERSION_3 */
#ifdef GTKTTK_GTK_VERSION_4                    
    if (qApp->style()->objectName() == style) {
      wc->GtkTtk_Style = qApp->style();
#endif /* GTKTTK_GTK_VERSION_4 */
      wc->GtkTtk_Style_Owner = false;
    } else {
      wc->GtkTtk_Style = QStyleFactory::create(style);
      wc->GtkTtk_Style_Owner = true;
    }
    GtkTtk_StoreStyleNameLowers(wc);
    wc->GtkTtk_QScrollBar_Widget->setStyle(wc->GtkTtk_Style);
    wc->GtkTtk_QComboBox_RW_Widget->setStyle(wc->GtkTtk_Style);
    wc->GtkTtk_QComboBox_RO_Widget->setStyle(wc->GtkTtk_Style);
    wc->GtkTtk_QWidget_WidgetParent->setStyle(wc->GtkTtk_Style);
    wc->GtkTtk_QWidget_Widget->setStyle(wc->GtkTtk_Style);
#ifdef GTKTTK_GTK_VERSION_3
    wc->GtkTtk_QWidget_Widget->polish();
#endif /* GTKTTK_GTK_VERSION_3 */
    wc->GtkTtk_QSlider_Hor_Widget->setStyle(wc->GtkTtk_Style);
    wc->GtkTtk_QSlider_Ver_Widget->setStyle(wc->GtkTtk_Style);
    wc->GtkTtk_QProgressBar_Hor_Widget->setStyle(wc->GtkTtk_Style);
    wc->GtkTtk_GTKabWidget_Widget->setStyle(wc->GtkTtk_Style);
    wc->GtkTtk_QPixmap_BackgroundTile = 
#ifdef GTKTTK_GTK_VERSION_3
                     (wc->GtkTtk_QWidget_Widget)->paletteBackgroundPixmap();
#endif /* GTKTTK_GTK_VERSION_3 */
#ifdef GTKTTK_GTK_VERSION_4
                     (wc->GtkTtk_QWidget_Widget)->palette().window().texture();
#endif /* GTKTTK_GTK_VERSION_4 */
    wc->GtkTtk_Style->polish(wc->GtkTtk_QWidget_Widget);
    if (todelete) delete todelete;
#if 0
    // Print Scrollbar statistics...
#define SC_PRINT_INFO(subcontrol) \
    wc->GtkTtk_Style->querySubControlMetrics(QStyle::CC_ScrollBar,\
        wc->GtkTtk_QScrollBar_Widget, subcontrol).x(),\
    wc->GtkTtk_Style->querySubControlMetrics(QStyle::CC_ScrollBar,\
        wc->GtkTtk_QScrollBar_Widget, subcontrol).y(),\
    wc->GtkTtk_Style->querySubControlMetrics(QStyle::CC_ScrollBar,\
        wc->GtkTtk_QScrollBar_Widget, subcontrol).width(),\
    wc->GtkTtk_Style->querySubControlMetrics(QStyle::CC_ScrollBar,\
        wc->GtkTtk_QScrollBar_Widget, subcontrol).height()
    printf("SC_ScrollBarAddLine: x=%d, y=%d, w=%d, h=%d\n", SC_PRINT_INFO(QStyle::SC_ScrollBarAddLine));
    printf("SC_ScrollBarSubLine: x=%d, y=%d, w=%d, h=%d\n", SC_PRINT_INFO(QStyle::SC_ScrollBarSubLine));
    printf("SC_ScrollBarAddPage: x=%d, y=%d, w=%d, h=%d\n", SC_PRINT_INFO(QStyle::SC_ScrollBarAddPage));
    printf("SC_ScrollBarSubPage: x=%d, y=%d, w=%d, h=%d\n", SC_PRINT_INFO(QStyle::SC_ScrollBarSubPage));
    printf("SC_ScrollBarFirst: x=%d, y=%d, w=%d, h=%d\n", SC_PRINT_INFO(QStyle::SC_ScrollBarFirst));
    printf("SC_ScrollBarLast: x=%d, y=%d, w=%d, h=%d\n", SC_PRINT_INFO(QStyle::SC_ScrollBarLast));
#endif
  }
  memcpy(wc_array[1], wc_array[0], sizeof(GtkTtk_WidgetCache));
  wc_array[0]->orientation = TTK_ORIENT_HORIZONTAL;
  wc_array[1]->orientation = TTK_ORIENT_VERTICAL;
  /* Save the name of the current theme... */
  Tcl_SetVar(interp, "ttk::theme::gtkTtk::theme",
#ifdef GTKTTK_GTK_VERSION_3                    
             wc->GtkTtk_Style->name(), TCL_GLOBAL_ONLY);
#endif /* GTKTTK_GTK_VERSION_3 */
#ifdef GTKTTK_GTK_VERSION_4                    
             wc->GtkTtk_Style->objectName().toUtf8().data(), TCL_GLOBAL_ONLY);
#endif /* GTKTTK_GTK_VERSION_4 */
#endif
  Tcl_MutexUnlock(&gtkTtkMutex);
  return TCL_OK;
}; /* Gtkttk_SetStyle */

extern "C" int DLLEXPORT
Gtkttk_Init(Tcl_Interp *interp)
{
    Ttk_Theme themePtr;
    Tk_Window tkwin;
    char tmpScript[1024];
    GtkTtk_WidgetCache **wc = NULL;
    GtkSettings *settings = NULL;
    gchar       *strval = NULL;

    if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL)
        return TCL_ERROR;
    if (Tk_InitStubs(interp,  TK_VERSION,  0) == NULL)
        return TCL_ERROR;
    if (Ttk_InitStubs(interp) == NULL)
        return TCL_ERROR;

    tkwin = Tk_MainWindow(interp);
    if (tkwin == NULL) return TCL_ERROR;

    themePtr  = Ttk_CreateTheme(interp, "gtkTtk", NULL);
    if (!themePtr) return TCL_ERROR;

    Tcl_CreateObjCommand(interp,
                         "ttk::theme::gtkTtk::initialiseLibrary",
                         Gtkttk_InitialiseLibrary, (ClientData) wc, NULL);
    if (Tcl_Eval(interp, initScript) != TCL_OK) {
      return TCL_ERROR;
    }
#ifdef GTKTTK_LOAD_GTK_DYNAMICALLY
    if (!GtkTtk_GtkAppCreated) {
      if (Tcl_Eval(interp, libsInitScript) != TCL_OK) {
        return TCL_ERROR;
      }
    }
#endif /* GTKTTK_LOAD_GTK_DYNAMICALLY */

    /*
     * Initialise Gtk:
     */
    Tcl_MutexLock(&gtkTtkMutex);
    wc = GtkTtk_CreateGtkApp(interp);
    ++GtkTtk_GtkAppCreated;

    /*
     * Register the various widgets...
     */
    GtkTtk_Init_Background(interp, wc, themePtr);
    GtkTtk_Init_Button(interp, wc, themePtr);
    GtkTtk_Init_CheckButton(interp, wc, themePtr);
    GtkTtk_Init_RadioButton(interp, wc, themePtr);
    GtkTtk_Init_ToolButton(interp, wc, themePtr);
    GtkTtk_Init_Labelframe(interp, wc, themePtr);
    GtkTtk_Init_Entry(interp, wc, themePtr);
    GtkTtk_Init_Menubutton(interp, wc, themePtr);
    GtkTtk_Init_Scrollbar(interp, wc, themePtr);
    GtkTtk_Init_Scale(interp, wc, themePtr);
    GtkTtk_Init_Progress(interp, wc, themePtr);
    GtkTtk_Init_SizeGrip(interp, wc, themePtr);
    GtkTtk_Init_Paned(interp, wc, themePtr);
    GtkTtk_Init_Notebook(interp, wc, themePtr);
    GtkTtk_Init_Combobox(interp, wc, themePtr);
#if 0
    GtkTtk_Init_TreeView(interp, wc, themePtr);
    //GtkTtk_Init_Separator(interp, wc, themePtr);
    //GtkTtk_Init_Arrows(interp, wc, themePtr);
#endif
    Tcl_CreateExitHandler(&GtkTtk_ExitProc, 0);
    //Tcl_CreateThreadExitHandler(&GtkTtk_ExitProc, 0);
    
    /*
     * Register the TileGtk package...
     */
    Tcl_CreateObjCommand(interp, "ttk::theme::gtkTtk::gtkEnum",
                         Gtkttk_GtkEnum, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp, "ttk::theme::gtkTtk::settingsProperty",
                         Gtkttk_SettingsProperty, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp, "ttk::theme::gtkTtk::widgetStyleProperty",
                         Gtkttk_WidgetStyleProperty, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp, "ttk::theme::gtkTtk::widgetProperty",
                         Gtkttk_WidgetProperty, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp, "ttk::theme::gtkTtk::currentThemeName",
                         Gtkttk_ThemeName, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp, "ttk::theme::gtkTtk::gtkDirectory",
                         Gtkttk_GtkDirectory, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp, "ttk::theme::gtkTtk::setStyle",
                         Gtkttk_SetStyle, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp, "ttk::theme::gtkTtk::gtk_method",
                         Gtkttk_gtk_method, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp,
                         "ttk::theme::gtkTtk::currentThemeColour",
                         Gtkttk_ThemeColour, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp,
                         "ttk::theme::gtkTtk::currentThemeColourKeys",
                         Gtkttk_ColourKeys, (ClientData) wc, NULL);
#if 0
    Tcl_CreateObjCommand(interp,
                         "ttk::theme::gtkTtk::setPalette",
                         Gtkttk_SetPalette, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp,
                         "ttk::theme::gtkTtk::getPixelMetric",
                         Gtkttk_GetPixelMetric, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp,
                         "ttk::theme::gtkTtk::getStyleHint",
                         Gtkttk_GetStyleHint, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp,
                         "ttk::theme::gtkTtk::getSubControlMetrics",
                         Gtkttk_GetSubControlMetrics, (ClientData) wc, NULL);
#endif
    /* Save the name of the current theme... */
    strcpy(tmpScript, "namespace eval ttk::theme::gtkTtk { variable theme ");
    settings = GtkTtk_gtk_settings_get_default();
    if (settings) {
      GtkTtk_g_object_get(settings, "gtk-theme-name", &strval, NULL);
      strcat(tmpScript, "{");
      if (strval) {
        strcat(tmpScript, strval);
        GtkTtk_g_free(strval);
      }
      strcat(tmpScript, "}");
    } else {
      strcat(tmpScript, "{}");
    }
    strcat(tmpScript, " };");
    Tcl_MutexUnlock(&gtkTtkMutex);
    
    if (Tcl_Eval(interp, tmpScript) != TCL_OK) {
      return TCL_ERROR;
    }
    if (Tcl_Eval(interp, "ttk::theme::gtkTtk::init") != TCL_OK) {
      return TCL_ERROR;
    }
    Tcl_PkgProvide(interp, "ttk::theme::gtkTtk", PACKAGE_VERSION);
    Tcl_PkgProvide(interp, PACKAGE_NAME, PACKAGE_VERSION);
    return TCL_OK;
}; /* GtkTtk_Init */

int DLLEXPORT
GtkTtk_Finish(Tcl_Interp *interp)
{
    Tcl_MutexLock(&gtkTtkMutex);
    if (GtkTtk_GtkAppCreated < 0) {Tcl_MutexUnlock(&gtkTtkMutex); return 0;}
    --GtkTtk_GtkAppCreated;
    if (GtkTtk_GtkAppCreated == 0) GtkTtk_DestroyGtkApp();
    Tcl_MutexUnlock(&gtkTtkMutex);
    return 0;
}; /* GtkTtk_Finish */
