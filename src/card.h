#pragma once
#include "helpers.h"
#include "poll.h"

#ifdef __cplusplus
extern "C" {
#endif
void attach (callbackAttach callback, i32 *data);
void touch (callbackTouch callback, u64 data, Keybindings insertBind);
void touchCancel ();
#ifdef __cplusplus
}
#endif
