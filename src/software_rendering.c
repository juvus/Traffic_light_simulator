#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <utils.h>
#include <software_rendering.h>

void
clear_full_screen(u32 color, Render_Buffer_t *render_buffer)
{
    /* Function for clear the screen with the desired color */

    u32 *pixel = render_buffer->pixels;
    u32 x, y;

    for (y = 0; y < render_buffer->height; y++) {
        for (x = 0; x < render_buffer->width; x++) {
            *(pixel + x + render_buffer->width * y) = color;
        }
    }
}

void
draw_pixel(u32 x, u32 y, u32 color, Render_Buffer_t *render_buffer)
{
    /* Function for drawing a single pixel on the screen */

    u32 *pixel = render_buffer->pixels;
    *(pixel + x + render_buffer->width * y) = color;
}

void
draw_ver_line(u32 x0, u32 y0, u32 length, u32 color, Render_Buffer_t *render_buffer)
{
    /* Function for drawing the vertical line */
    u32 *pixel = render_buffer->pixels;
    u32 y;

    for (y = y0; y < (y0 + length); ++y) {
        *(pixel + x0 + render_buffer->width * y) = color;
    }
}

void
draw_rect(u32 x0, u32 y0, u32 width, u32 height, u32 color, Render_Buffer_t *render_buffer)
{
    /* Function for drawing the rectangle */

    u32 x, y;
    u32 *pixel = render_buffer->pixels;

    for(y = y0; y < height + y0; y++) {
        for (x = x0; x < width + x0; x++) {
            *(pixel + x + render_buffer->width * y) = color;
        }
    }
}

void
draw_circle(u32 x0, u32 y0, u32 radius, b32 is_filled, u32 color, Render_Buffer_t *render_buffer)
{
    /* Function for drawing a circle (using Brezenhame algorithm) */

    s32 x = 0;
    s32 y = (s32)radius;
    s32 gap = 0;
    s32 delta = (2 - 2 * (s32)radius);

    while (y >= 0) {
        /* Draw points lying on a circle */
        draw_pixel(x0 + (u32)x, y0 + (u32)y, color, render_buffer);
        draw_pixel(x0 + (u32)x, y0 - (u32)y, color, render_buffer);
        draw_pixel(x0 - (u32)x, y0 - (u32)y, color, render_buffer);
        draw_pixel(x0 - (u32)x, y0 + (u32)y, color, render_buffer);

        /* Fill the circle if it is necessary (with lines from top to bottom)*/
        if (is_filled) {    
            draw_ver_line(x0 + (u32)x, y0 - (u32)y, 2 * (u32)y, color, render_buffer);
            draw_ver_line(x0 - (u32)x, y0 - (u32)y, 2 * (u32)y, color, render_buffer);
        }

        gap = 2 * (delta + y) - 1;
        if ((delta < 0) && (gap <= 0)) {
            x++;
            delta += 2 * x + 1;
            continue;
        }

        if ((delta > 0) && (gap > 0)) {
            y--;
            delta -= 2 * y + 1;
            continue;
        }

        x++;
        delta += 2 * (x - y);
        y--;
    }   
}
