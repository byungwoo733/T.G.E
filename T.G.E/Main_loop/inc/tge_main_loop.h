#ifndef _TGE_MAIN_LOOP_H_
#define _TGE_MAIN_LOOP_H_

/**
 * This header file is included to define _EXPORT_.
 */
#include <stdbool.h>
#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

// This method is exported from tge_main_loop.so
EXPORT_API bool tizentge_main_loop(void);

#ifdef __cplusplus
}
#endif
#endif // _TGE_MAIN_LOOP_H_

