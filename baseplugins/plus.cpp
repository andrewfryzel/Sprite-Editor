/*
 *	plus.cpp
 *	An example plugin implementing a multi-pixel
 *	brush.
 *	Author:
 *		Brian Schnepp (Nov 1, 2019)
 */

#include <pluginsdk/pluginsdk.h>

#include <iostream>

#include <stdlib.h>

PluginHook PluginInit(void)
{
	std::cout << "Proof this init function got called!" << std::endl;
	return PLUGIN_HOOK_BRUSH;
}

/* Helper function to automatically round a float to the nearest integer. */
uint32_t InternalRoundNumber(float Val)
{
	/* 
	 * Int cast always floors. If adding 0.5 brings it up
	 * to the next integer, then it was in [X.5, (X+1).0), 
	 * where X is some whole integer. 
	 */
	return static_cast<uint32_t>(Val + 0.5f);
}

/* Handles the brush selected event. */
void HandleBrushSelected(Brush *NewBrush)
{
	NewBrush->IsEraser = false;
	NewBrush->Width = 3;
	NewBrush->Height = 3;
	for (uint32_t Idx = 0; Idx < 9; ++Idx)
	{
		if (Idx == 1 
			|| Idx == 3 
			|| Idx == 4 
			|| Idx == 5 
			|| Idx == 7)
		{
			NewBrush->Map[Idx] = 0xFF;
		}
	}
}

/* Handles the brush deselected event. */
void HandleBrushDeselected(Brush *OldBrush)
{
	free(OldBrush->Map);
}

/* Handles the resizing of the brush. */
void HandleBrushResize(BrushResizeObject *BrRzObj)
{
	/* 
	 * We create a new map, of the size of the old one,
	 * and then overwrite the old map. There's some manual
	 * memory management here for us, in C, we'd realloc(),
	 * but here we'd just use new and delete[].
	 */
	 
	/* Get the original brush. */
	Brush *brush = BrRzObj->CurBrush;
	/* Calculate new buffer size. */
	uint32_t NewWidth = InternalRoundNumber(
		brush->Width * BrRzObj->NewSize
	);
	
	uint32_t NewHeight = InternalRoundNumber(
		brush->Height*BrRzObj->NewSize
	);
	
    /* Probably a working nearest-neighbor? */
	for (uint32_t CurWidth = 0; CurWidth < NewWidth; ++CurWidth)
	{
		for (uint32_t CurHeight = 0; CurHeight < NewHeight; ++CurHeight)
		{
			uint32_t OldWidth = static_cast<uint32_t>(
				CurWidth / BrRzObj->NewSize
			);
			
			uint32_t OldHeight = static_cast<uint32_t>(
				CurHeight / BrRzObj->NewSize
			);
			/* Like the VGA graphics unit, 
			 * cell = (curRow * rowSize) + curCol; */
			 uint32_t Index = 
				((brush->Width) * (OldWidth)) + OldHeight;
				
			brush->Map[(CurWidth * NewWidth) + CurHeight] =
				brush->Map[Index];
		}
	}
	brush->Width = NewWidth;
	brush->Height = NewHeight;
}

/*
 * Callback system: when something goes on,
 * then this should get called.
 */
void PluginDispatch(PluginDispatchReason Reason, void *Data)
{
	std::cout << "Proof this dispatch function got called!" << std::endl;
	if (Reason == PLUGIN_DISPATCH_REASON_BRUSH_SELECTED)
	{
		/* We know data is a brush. */
		Brush *brush = reinterpret_cast<Brush*>(Data);
		HandleBrushSelected(brush);
	} else if (Reason == PLUGIN_DISPATCH_REASON_BRUSH_DESELECTED) {
		/* We know data is a brush. */
		Brush *brush = reinterpret_cast<Brush*>(Data);
		HandleBrushDeselected(brush);
	} else if (Reason == PLUGIN_DISPATCH_REASON_BRUSH_RESIZED) {
		/* We know data is some structure with the Brush,
		 * and the new size. */
		BrushResizeObject *ResizeObj =
			reinterpret_cast<BrushResizeObject*>(ResizeObj);
	}
}


/*
 * Called when exiting the plugin (ie, closing).
 */
void PluginDeinit(void)
{
	std::cout << "Proof this deinit function got called!" << std::endl;
	/* 
	 * There's nothing here to deallocate.
	 * This should only do something if there's
	 * some sort of static allocation, or there's some
	 * kind of other external code called to initialize this.
	 */
}
