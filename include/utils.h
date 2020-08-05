/* =================================================================================== */
/* Set different unilities for the si,ulation                                          */
/* =================================================================================== */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>
#include <windows.h>

/* Type synonims of stdint for convinient usage */
typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;
typedef int b32;
typedef float f32;
typedef double f64;

/* Boolean synonims */
#define true 1
#define false 0

/* Enumerator for the identifiers of the key commands */
enum Key_Pressed {
    KP_NONE, /* Nothing was pressed */
    KP_AUTO_WORK_MODE, /* Ctrl + A */
    KP_MANUAL_WORK_MODE, /* Ctrl + M */
    KP_TOGGLE_GREEN, /* F1 */
    KP_TOGGLE_YELLOW, /* F2 */
    KP_TOGGLE_RED, /* F3 */
    KP_ESCAPE /* Esc */
};
typedef enum Key_Pressed Key_Pressed_t;

/* Vector of two u32 elements with iterative possibility */
struct V2_u32 {
    union {
        struct {
            u32 x;
            u32 y;
        };
        u32 e[2];
    };
};
typedef struct V2_u32 V2_u32_t;

/* Structure for render buffer */
struct Render_Buffer {
    u32 width; /* Buffer width */
    u32 height; /* Buffer height */
    u32 *pixels; /* Array of pixels data */
    BITMAPINFO bitmap_info; /* Bitmapinfo structure */
};
typedef struct Render_Buffer Render_Buffer_t;

#endif // UTILS_H_
