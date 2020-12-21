/*
 *	pluginsdk.h
 *	The header which all plugins must include
 *	in order to develop, extend, and enhance the program.
 *	
 *	Note that plugin developers *must* compile their code with -shared
 *	and -fPIC, or their MSVC equivalents for an NT port.
 * 
 *	Author:
 *		Brian Schnepp (Oct 29, 2019)
 */

#ifndef PLUGINSDK_H
#define PLUGINSDK_H

/* Boilerplate portable code stuff. */
#include <stdint.h>

#include <tools.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * The type of the plugin.
 * This should maximize at 2^16 - 1 different
 * possibilities, and retain this specific size for
 * future ABI compatibility.
 *
 * Plugin hooks should never be reorganized for backwards
 * compatibility reasons.
 */
typedef enum PluginHook
{
	PLUGIN_HOOK_INVALID = 0x0000,
	PLUGIN_HOOK_BRUSH,
	PLUGIN_HOOK_CANVAS_BRUSH,
	PLUGIN_HOOK_CANVAS_OPTION,
	PLUGIN_HOOK_MAX = 0xFFFF
}PluginHook;

/*
 * The purpose of the callback.
 * This should maximize at 2^16 - 1 different
 * possibilities, and retain this specific size for
 * future ABI compatibility.
 *
 * Reasons should never be reorganized for backwards
 * compatibility reasons. A plugin compiled for version 0.1
 * should remain compatible for version 1.0, 2.0, 3.0, 5.0,
 * and 99999999999999999.0.
 */
typedef enum PluginDispatchReason
{
	PLUGIN_DISPATCH_REASON_INVALID = 0x0000,
	PLUGIN_DISPATCH_REASON_BRUSH_SELECTED,
	/* DEPRECATED: Deselection event never happens. */
	PLUGIN_DISPATCH_REASON_BRUSH_DESELECTED,
	/* Resize isn't implemented, but may be in the future? */
	PLUGIN_DISPATCH_REASON_BRUSH_RESIZED,
	
	/* 
	 * These aren't implemented, but reserved for if done.
	 * Capability for it to be useful for adding a menu
	 * widget for something like mirroring.
	 */
	PLUGIN_DISPATCH_REASON_CANVAS_OPTION_SELECTED,
	PLUGIN_DISPATCH_REASON_CANVAS_OPTION_DESELECTED,
	
	/* 
	 * A "Canvas Brush" is a brush that wants to be aware of the canvas.
	 * ie, this would be useful for something like a fill bucket, stamp
	 * tool, or other feature like that.
	 */
	PLUGIN_DISPATCH_REASON_CANVAS_BRUSH_SELECTED,
	PLUGIN_DISPATCH_REASON_CANVAS_BRUSH_DESELECTED,
	
	PLUGIN_DISPATCH_REASON_MAX = 0xFFFF
}PluginDispatchReason;

typedef struct BrushResizeObject
{
	float NewSize;
	Brush *CurBrush;
}BrushResizeObject;

typedef struct CanvasBrushObject
{
	Pixel *Mapping;
	Brush *CurBrush;
}CanvasBrushObject;


/*
 * When the plugin is first loaded,
 * declare the type of plugin this is.
 */
PluginHook PluginInit(void);

/*
 * Callback system: when something goes on,
 * then this should get called.
 */
void PluginDispatch(PluginDispatchReason Reason, void *Data);


/*
 * Called when exiting the plugin (ie, closing).
 */
void PluginDeinit(void);


#ifdef __cplusplus
}
#endif

#endif
