#ifndef SOFTWARE_RENDERING_H_
#define SOFTWARE_RENDERING_H_

#include <utils.h>

/* Function for clear the screen with the desired color */
void clear_full_screen(u32 color, Render_Buffer_t *render_buffer);

/* Function for drawing a single pixel on the screen */
void draw_pixel(u32 x, u32 y, u32 color, Render_Buffer_t *render_buffer);

/* Function for drawing the vertical line */
void draw_ver_line(u32 x0, u32 y0, u32 length, u32 color, Render_Buffer_t *render_buffer);

/* Function for drawing the simple rectangle */
void draw_rect(u32 x0, u32 y0, u32 width, u32 height, u32 color, Render_Buffer_t *render_buffer);

/* Function for drawing a circle (using Brezenhame algorithm) */
void draw_circle(u32 x0, u32 y0, u32 radius, b32 is_filled ,u32 color, Render_Buffer_t *render_buffer);

#endif /* SOFTWARE_RENDERING_H_ */
