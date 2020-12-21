#ifndef INTERNALDRAW_H
#define INTERNALDRAW_H

#include <pluginsdk/pluginsdk.h>

/* A wrapper class to "fake" a plugin into the resulting binary. */
class InternalDraw
{
public:
	static PluginHook PluginInit(void);
	static void PluginDeinit(void);
	static void PluginDispatch(PluginDispatchReason Reason, void *Data);
private:
	InternalDraw();
	~InternalDraw();
};

#endif