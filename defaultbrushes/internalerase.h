#ifndef INTERNALERASE_H
#define INTERNALERASE_H

#include <pluginsdk/pluginsdk.h>

/* A wrapper class to "fake" a plugin into the resulting binary. */
class InternalErase
{
public:
	static PluginHook PluginInit(void);
	static void PluginDeinit(void);
	static void PluginDispatch(PluginDispatchReason Reason, void *Data);
private:
	InternalErase();
	~InternalErase();
};

#endif