/*
 *  gtkTtk_GtkApp.c
 * ---------------
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

#ifdef GTKTTK_ENABLE_GNOME
#include <gnome.h>
GnomeProgram *my_app = NULL;
#endif /* GTKTTK_ENABLE_GNOME */

#include "gtkTtk_Utilities.h"
#include "gtkTtk_TkHeaders.h"
#include <string.h>
gboolean   GtkTtk_GtkInitialisedFlag = FALSE;
static int GtkTtk_xlib_rgb_initialised = 0;
GtkWidget *GtkTtk_GtkWindow = NULL;



/* In the following variable we store the XErrorHandler, before we install our
 * own, which filters out some XErrors... */
static int (*GtkTtk_TkXErrorHandler)(Display *displayPtr,
                                     XErrorEvent *errorPtr);
static int (*GtkTtk_GtkXErrorHandler)(Display *displayPtr,
                                     XErrorEvent *errorPtr);
static int GtkTtk_XErrorHandler(Display *displayPtr, XErrorEvent *errorPtr);

static int  GtkTtk_XEventHandler(ClientData clientdata, XEvent *eventPtr);

/*
 * GtkTtk_InterpDeleteProc:
 * This function will be called when the interpreter gets deleted. It must free
 * all allocated interp-specific memory segments.
 */
static void GtkTtk_InterpDeleteProc(ClientData clientData, Tcl_Interp *interp) {
  GtkTtk_WidgetCache **wc_array = (GtkTtk_WidgetCache **) clientData;
  GtkTtk_WidgetCache *wc = wc_array[0];
  if (wc && wc->gtkWindow) {
    /*This will destroy also ALL children!*/
    GtkTtk_gtk_widget_destroy(wc->gtkWindow);
  }
  // printf("Tk_DeleteGenericHandler: %p\n", interp); fflush(NULL);
  Tk_DeleteGenericHandler(&GtkTtk_XEventHandler, (ClientData) interp);
  Tcl_Free((char *) wc_array[0]);
  Tcl_Free((char *) wc_array[1]);
  Tcl_Free((char *) wc_array);
}; /* GtkTtk_InterpDeleteProc */

GtkTtk_WidgetCache **GtkTtk_CreateGtkApp(Tcl_Interp *interp) {
  /*
   * The first step is to initialise the gtk library. This must be done once
   * in the application lifetime.
   */
  Tcl_MutexLock(&gtkTtkMutex);
  if (!GtkTtk_GtkInitialisedFlag) {
#ifdef GTKTTK_ENABLE_GNOME
    gchar **remaining_args = NULL;
    GOptionEntry option_entries[] = {
      {G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &remaining_args,
       "Special option that collects any remaining arguments for us"},
      { NULL }
    };
    GOptionContext *option_context;
#endif /* GTKTTK_ENABLE_GNOME */
    int argc = 1;
    char **argv = GtkTtk_g_new0(char*, 2);
    argv[0] = (char *) Tcl_GetNameOfExecutable();

#ifdef GTKTTK_INSTALL_XERROR_HANDLER
    GtkTtk_TkXErrorHandler = XSetErrorHandler(GtkTtk_XErrorHandler);
#endif /* GTKTTK_INSTALL_XERROR_HANDLER */

#ifdef GTKTTK_ENABLE_GNOME
    option_context = GtkTtk_g_option_context_new("tile-gtk");
    GtkTtk_g_option_context_add_main_entries(option_context,
                                              option_entries, NULL);
    /* We assume PACKAGE and VERSION are set to the program name and version
     * number respectively. Also, assume that 'option_entries' is a global
     * array of GOptionEntry structures.
     */
    my_app = gnome_program_init(PACKAGE_NAME, PACKAGE_VERSION,
                                LIBGNOMEUI_MODULE, argc, argv,
                                GNOME_PARAM_GOPTION_CONTEXT, option_context,
                                GNOME_PARAM_NONE);
    if (my_app) GtkTtk_GtkInitialisedFlag = TRUE;
    if (remaining_args != NULL) {
      GtkTtk_g_strfreev(remaining_args);
      remaining_args = NULL;
    }
#else  /* GTKTTK_ENABLE_GNOME */
    GtkTtk_gtk_disable_setlocale();
    GtkTtk_GtkInitialisedFlag = GtkTtk_gtk_init_check(&argc, &argv);
#endif /* GTKTTK_ENABLE_GNOME */
    GtkTtk_g_free(argv);
    if (!GtkTtk_GtkInitialisedFlag) {
      Tcl_MutexUnlock(&gtkTtkMutex);
      return NULL;
    }
    /* Initialise GtkTtk_GtkWindow... */
    GtkTtk_GtkWindow = GtkTtk_gtk_window_new(GTK_WINDOW_POPUP);
    GtkTtk_gtk_widget_realize(GtkTtk_GtkWindow);
#ifdef GTKTTK_INSTALL_XERROR_HANDLER
    /*
     * GTK+ xerror handler will terminate the application.
     * Just get rid of that...
     */
    GtkTtk_GtkXErrorHandler = XSetErrorHandler(GtkTtk_XErrorHandler);
#endif /* GTKTTK_INSTALL_XERROR_HANDLER */

#ifdef GTKTTK_SYNCHRONIZE
    XSynchronize(Tk_Display(Tk_MainWindow(interp)), true);
#endif /* GTKTTK_SYNCHRONIZE */
  }
  Tcl_MutexUnlock(&gtkTtkMutex);

  /*
   * Allocate the widget cache. We keep a widget cache per interpreter.
   * Each cache is an array of two elements, one for each orientation.
   */

  GtkTtk_WidgetCache **wc_array = (GtkTtk_WidgetCache **) 
                           Tcl_Alloc(sizeof(GtkTtk_WidgetCache*)*2);
  wc_array[0] = (GtkTtk_WidgetCache *) 
                           Tcl_Alloc(sizeof(GtkTtk_WidgetCache));
  wc_array[1] = (GtkTtk_WidgetCache *) 
                           Tcl_Alloc(sizeof(GtkTtk_WidgetCache));
  Tcl_SetAssocData(interp, "gtkTtkTtk_gtk_widget_cache",
                   &GtkTtk_InterpDeleteProc, (ClientData) wc_array);
  GtkTtk_WidgetCache *wc = wc_array[0];
  memset(wc, 0, sizeof(GtkTtk_WidgetCache));
  /*
   * Initialise the widget cache.
   */
  wc->GtkTtk_MainInterp  = interp;
  wc->GtkTtk_tkwin       = Tk_MainWindow(interp);
  if (wc->GtkTtk_tkwin != NULL && wc->GtkTtk_MainDisplay == None) {
    Tk_MakeWindowExist(wc->GtkTtk_tkwin);
    wc->GtkTtk_MainDisplay = Tk_Display(wc->GtkTtk_tkwin);
  }
  if (wc->GtkTtk_MainDisplay == None) {
    Tcl_MutexUnlock(&gtkTtkMutex);
    Tcl_Free((char *) wc_array[0]);
    Tcl_Free((char *) wc_array[1]);
    Tcl_Free((char *) wc_array);
    return NULL;
  }
#ifndef __WIN32__
  wc->gdkDisplay = GtkTtk_gdk_x11_lookup_xdisplay(wc->GtkTtk_MainDisplay);
#endif
  if (!wc->gdkDisplay) {
    wc->gdkDisplay = GtkTtk_gdk_display_get_default();
  }
  wc->gtkWindow = GtkTtk_gtk_window_new(GTK_WINDOW_POPUP);
  if (wc->gtkWindow) GtkTtk_gtk_widget_realize(wc->gtkWindow);
  wc->protoLayout = GtkTtk_gtk_fixed_new();
  GtkTtk_gtk_container_add((GtkContainer*)(wc->gtkWindow), wc->protoLayout);
  memcpy(wc_array[1], wc_array[0], sizeof(GtkTtk_WidgetCache));
  wc_array[0]->orientation    = TTK_ORIENT_HORIZONTAL;
  wc_array[1]->orientation    = TTK_ORIENT_VERTICAL;
  wc_array[0]->gtkOrientation = GTK_ORIENTATION_HORIZONTAL;
  wc_array[1]->gtkOrientation = GTK_ORIENTATION_VERTICAL;

#ifndef __WIN32__
  Tcl_MutexLock(&gtkTtkMutex);
  if (!GtkTtk_xlib_rgb_initialised) {
    GtkTtk_xlib_rgb_init(wc->GtkTtk_MainDisplay,Tk_Screen(wc->GtkTtk_tkwin));
    GtkTtk_xlib_rgb_initialised = 1;
  }
  Tcl_MutexUnlock(&gtkTtkMutex);
#endif
  return wc_array;
}; /* GtkTtk_CreateGtkApp */

void GtkTtk_DestroyGtkApp(void) {
  Tcl_MutexLock(&gtkTtkMutex);
  if (GtkTtk_GtkInitialisedFlag) {
    // XSetErrorHandler(GtkTtk_TkXErrorHandler);
    GtkTtk_GtkInitialisedFlag = FALSE;
  }
  Tcl_MutexUnlock(&gtkTtkMutex);
}; /* GtkTtk_DestroyGtkApp */

/*
 * GtkTtk_XErrorHandler:
 * This XError handler just prints some debug information and then calls
 * Tk's XError handler...
 */
static int GtkTtk_XErrorHandler(Display *displayPtr, XErrorEvent *errorPtr) {
#ifdef GTKTTK_VERBOSE_XERROR_HANDLER
  char buf[64];
  XGetErrorText (displayPtr, errorPtr->error_code, buf, 63);
  printf("===============================================================\n");
  printf("  GtkTtk_XErrorHandler:\n");
  printf("    error_code   = %s (%d)\n", buf, errorPtr->error_code);
  printf("    request_code = %d\n", errorPtr->request_code);
  printf("    minor_code   = %d\n", errorPtr->minor_code);
  printf("===============================================================\n");
#endif /* GTKTTK_VERBOSE_XERROR_HANDLER */
  return GtkTtk_TkXErrorHandler(displayPtr, errorPtr);
}; /* GtkTtk_XErrorHandler */

static int GtkTtk_XEventHandler(ClientData clientData, XEvent *eventPtr) {
  const char *tcl_callback;
  int status;
  if (eventPtr->type != ClientMessage) return 0;
  // Atom GtkTtk_KIPC_COMM_ATOM = XInternAtom(eventPtr->xclient.display,
  //                                          "KIPC_COMM_ATOM" , false);
  Atom GtkTtk_KIPC_COMM_ATOM = None;
  if (eventPtr->xclient.message_type != GtkTtk_KIPC_COMM_ATOM) return 0;
  /* The following data variable contains the type of the KIPC message,
   * As defined in gnomelibs/gnomecore/kipc.h:
   * PaletteChanged      = 0
   * StyleChanged        = 2
   * ToolbarStyleChanged = 6
   */
  switch (eventPtr->xclient.data.l[0]) {
    case 0:   /* PaletteChanged      */
      tcl_callback = "tile::theme::gtkTtk::gnomePaletteChangeNotification";
      break;
    case 2:   /* StyleChanged        */
    case 6: { /* ToolbarStyleChanged */
      tcl_callback = "tile::theme::gtkTtk::gnomeStyleChangeNotification";
      break;
    }
    default: {
      return 0;
    }
  }
  Tcl_Interp *interp = (Tcl_Interp *) clientData;
  if (interp == NULL) return 0;
  // printf("GtkTtk_XEventHandler: %p\n", interp); fflush(NULL);
  /* Notify the tile engine about the change... */
  status = Tcl_Eval(interp, tcl_callback);
  if (status != TCL_OK) Tcl_BackgroundError(interp);
  /* Do not remove True: As many interpreters may have registered this event
   * handler, allow Tk to call all of them! */
  return True;
} /* GtkTtk_XEventHandler */
