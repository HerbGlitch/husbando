#ifndef ARC_STD_ERRNO_H_
#define ARC_STD_ERRNO_H_

#include <stdint.h>

#define ARC_ERRNO_NULL       -0x01
#define ARC_ERRNO_DATA       -0x02
#define ARC_ERRNO_COPY       -0x03
#define ARC_ERRNO_EXISTS     -0x04
#define ARC_ERRNO_OVERFLOW   -0x05
#define ARC_ERRNO_INIT       -0x06
#define ARC_ERRNO_CONNECTION -0x07

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
static int32_t arc_errno = 0;
#pragma GCC diagnostic pop


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#ifdef ARC_DEBUG
#   include <stdio.h>
#   define ARC_DEBUG_LOG(ERR, STR, ...) printf("[ERROR %d] " STR "\n", ERR, __VA_ARGS__)
#   define ARC_DEBUG_ERR(STR) printf("[ERROR %d]" STR "\n", arc_errno)
#else
#   define ARC_DEBUG_LOG(ERR, STR, ...)
#   define ARC_DEBUG_ERR(STR)
#endif

#define ARC_ERR_CHECK(FUNC) FUNC; if(arc_errno){ ARC_DEBUG_LOG(arc_errno, "%s", #FUNC); return; }

#endif //ARC_STD_ERRNO_H_