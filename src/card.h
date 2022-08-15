#include "helpers.h"

#ifdef __cplusplus
extern "C" {
#endif
void attach (callbackAttach callback, i32 *data);
void touch (callbackTouch callback, u64 data);
#ifdef __cplusplus
}
#endif
