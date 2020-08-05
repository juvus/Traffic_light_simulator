/* =================================================================================== */
/* Realization of the traffic_light methods                                            */
/* =================================================================================== */

#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <software_rendering.h>
#include <traffic_light.h>
#include <utils.h>

/* Traffic_light methods  */
Traffic_Light_t*
traffic_light_constructor(void)
{
    /* Constructor of the traffic light object */
    Traffic_Light_t *traffic_light = (Traffic_Light_t*) malloc (1 * sizeof(Traffic_Light_t)); 
    if (traffic_light == NULL) {
        assert(0 && "ERROR: Error in memory allocation for the traffic_light object");
    }
    return traffic_light;
}

void
traffic_light_destructor(Traffic_Light_t *traffic_light)
{
    /* Destructor of the traffic light object */
    free(traffic_light);
}

void
traffic_light_init(Traffic_Light_t *traffic_light, const Render_Buffer_t *render_buffer, const u32 width,
                   const u32 height, const u32 bkg_color, const u32 green_color, const u32 yellow_color,
                   const u32 red_color, const u32 off_color, const u32 light_radius,
                   const f32 *times_array, const b32 *green_lights_array, const b32 *yellow_lights_array,
                   const b32 *red_lights_array)
{
    u32 world_width = render_buffer->width;
    u32 world_height = render_buffer->height;
    u32 dist;
    u32 i;
    
    /* Initialization of the traffic light object */
    *(u32 *)&traffic_light->width = width; /* Traffic light width */
    *(u32 *)&traffic_light->height = height; /* Traffic light height */
    *(u32 *)&traffic_light->bkg_color = bkg_color; /* Traffic light height */
    *(u32 *)&traffic_light->green_color = green_color; /* Color of the green light */
    *(u32 *)&traffic_light->yellow_color = yellow_color; /* Color of the yellow light */
    *(u32 *)&traffic_light->red_color = red_color; /* Color of the red light */
    *(u32 *)&traffic_light->off_color = off_color; /* Color of the red light */
    *(u32 *)&traffic_light->light_radius = light_radius; /* Color of the red light */

    /* Calculation the position of the bottom-left corner of the traffic light */
    traffic_light->BL.x = (world_width - traffic_light->width) / 2;
    traffic_light->BL.y = (world_height - traffic_light->height) / 2;

    /* Calculation of the light centers */
    dist = (traffic_light->height - 6 * traffic_light->light_radius) / 4;   
    traffic_light->green_light_center.x = traffic_light->BL.x + traffic_light->width / 2;
    traffic_light->green_light_center.y = traffic_light->BL.y + dist + traffic_light->light_radius;
    traffic_light->yellow_light_center.x = traffic_light->BL.x + traffic_light->width / 2;
    traffic_light->yellow_light_center.y = traffic_light->BL.y + dist * 2 + traffic_light->light_radius * 3;
    traffic_light->red_light_center.x = traffic_light->BL.x + traffic_light->width / 2;
    traffic_light->red_light_center.y = traffic_light->BL.y + dist * 3 + traffic_light->light_radius * 5;

    /* Filling the traffic light different arrays */
    for (i = 0; i < NUM_STATES; ++i) {
        traffic_light->times_array[i] = times_array[i];
        traffic_light->green_lights_array[i] = green_lights_array[i];
        traffic_light->yellow_lights_array[i] = yellow_lights_array[i];
        traffic_light->red_lights_array[i] = red_lights_array[i];
    }

    /* Default state and working mode */
    traffic_light->work_mode = WM_MANUAL;

    /* Deafault lights states */
    traffic_light->green_light_state = 0;
    traffic_light->yellow_light_state = 0;
    traffic_light->red_light_state = 0;
}

void
traffic_light_set_auto_mode(Traffic_Light_t *traffic_light) {
    /* Method for set the auto mode of the traffic light */

    traffic_light->work_mode = WM_AUTO;
    traffic_light->current_state = GREEN_ON;
    traffic_light->passed_time = 0.0f;

    /* Initial turn of green light */
    traffic_light->green_light_state = 1;
    traffic_light->yellow_light_state = 0;
    traffic_light->red_light_state = 0;
}

void
traffic_light_set_manual_mode(Traffic_Light_t *traffic_light) {
    /* Method for set the manual mode of the traffic light */

    traffic_light->work_mode = WM_MANUAL;
}

void
traffic_light_toggle_green(Traffic_Light_t *traffic_light) {
    /* Method for toggle the green light of the traffic light */

    if (traffic_light->green_light_state == 0) {
        traffic_light->green_light_state = 1;
    }
    else {
        traffic_light->green_light_state = 0;
    }
}

void
traffic_light_toggle_yellow(Traffic_Light_t *traffic_light) {
    /* Method for toggle the yellow light of the traffic light */

    if (traffic_light->yellow_light_state == 0) {
        traffic_light->yellow_light_state = 1;
    }
    else {
        traffic_light->yellow_light_state = 0;
    }
}

void
traffic_light_toggle_red(Traffic_Light_t *traffic_light) {
    /* Method for toggle the red light of the traffic light */

    if (traffic_light->red_light_state == 0) {
        traffic_light->red_light_state = 1;
    }
    else {
        traffic_light->red_light_state = 0;
    }
}

void
traffic_light_render(Traffic_Light_t *traffic_light, Render_Buffer_t *render_buffer)
{
    /* Method for render the traffic light on the screen */

    /* Render the traffic light frame */
    draw_rect(traffic_light->BL.x, traffic_light->BL.y, traffic_light->width, traffic_light->height,
              traffic_light->bkg_color, render_buffer);
    
    /* Render the green light */
    if (traffic_light->green_light_state == 1) {
        draw_circle(traffic_light->green_light_center.x, traffic_light->green_light_center.y,
                    traffic_light->light_radius, 1, traffic_light->green_color, render_buffer);
    }
    else {
        draw_circle(traffic_light->green_light_center.x, traffic_light->green_light_center.y,
                traffic_light->light_radius, 1, traffic_light->off_color, render_buffer);
    }

    /* Render the yellow light */
    if (traffic_light->yellow_light_state == 1) {
        draw_circle(traffic_light->yellow_light_center.x, traffic_light->yellow_light_center.y,
                    traffic_light->light_radius, 1, traffic_light->yellow_color, render_buffer);
    }
    else {
        draw_circle(traffic_light->yellow_light_center.x, traffic_light->yellow_light_center.y,
                traffic_light->light_radius, 1, traffic_light->off_color, render_buffer);
    }

    /* Render the red light */
    if (traffic_light->red_light_state == 1) {
        draw_circle(traffic_light->red_light_center.x, traffic_light->red_light_center.y,
                    traffic_light->light_radius, 1, traffic_light->red_color, render_buffer);
    }
    else {
        draw_circle(traffic_light->red_light_center.x, traffic_light->red_light_center.y,
                traffic_light->light_radius, 1, traffic_light->off_color, render_buffer);
    }
}



