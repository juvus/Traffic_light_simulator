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

/* Externally determined constants (in file simulation.c) */
extern const u32 TLIGHT_WIDTH; /* Width of the traffic light */
extern const u32 TLIGHT_HEIGHT; /* Height of the traffic light (3 times gires than width)*/
extern const u32 TLIGHT_PNL_COLOR; /* Color of the traffic light front pannel */
extern const u32 TLIGHT_PNL_LIGHT_COLOR; /* Color of the pannel light part */
extern const u32 TLIGHT_PNL_SHADOW_COLOR; /* Color of the pannel shadow part */
extern const u32 TLIGHT_GREEN_COLOR; /* Color of the green light */
extern const u32 TLIGHT_YELLOW_COLOR; /* Color of the yellow light */
extern const u32 TLIGHT_RED_COLOR; /* Color of the red light */
extern const u32 TLIGHT_OFF_COLOR; /* Color of the turned off color */
extern const u32 TLIGHT_LIGHT_RADIUS; /* Radius of the light */
extern const f32 TLight_times_array[10];
extern const b32 TLight_green_lights_array[10];
extern const b32 TLight_yellow_lights_array[10];
extern const b32 TLight_red_lights_array[10];

/* Declaration of a static functions */
static void traffic_light_render_section(Traffic_Light_t *traffic_light, Render_Buffer_t *render_buffer,
                                          Traffic_Light_Section_t section);

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
traffic_light_init(Traffic_Light_t *traffic_light, const Render_Buffer_t *render_buffer)
{
    u32 world_width = render_buffer->width;
    u32 world_height = render_buffer->height;
    u32 dist;
    u32 i;
    
    /* Initialization of the traffic light object */
    *(u32 *)&traffic_light->width = TLIGHT_WIDTH; /* Traffic light width */
    *(u32 *)&traffic_light->height = TLIGHT_HEIGHT; /* Traffic light height */
    *(u32 *)&traffic_light->pnl_color = TLIGHT_PNL_COLOR; /* Traffic light pannel color */
    *(u32 *)&traffic_light->pnl_light_color = TLIGHT_PNL_LIGHT_COLOR; /* Color of the light part of the pannel */
    *(u32 *)&traffic_light->pnl_shadow_color = TLIGHT_PNL_SHADOW_COLOR; /* Color of the shadow part of the pannel */
    *(u32 *)&traffic_light->green_color = TLIGHT_GREEN_COLOR; /* Color of the green light */
    *(u32 *)&traffic_light->yellow_color = TLIGHT_YELLOW_COLOR; /* Color of the yellow light */
    *(u32 *)&traffic_light->red_color = TLIGHT_RED_COLOR; /* Color of the red light */
    *(u32 *)&traffic_light->off_color = TLIGHT_OFF_COLOR; /* Color when the light is off */
    *(u32 *)&traffic_light->light_radius = TLIGHT_LIGHT_RADIUS; /* Light radius */

    /* Calculation the position of the bottom-left corner of the traffic light */
    traffic_light->BL.x = (world_width - traffic_light->width) / 2;
    traffic_light->BL.y = (world_height - traffic_light->height) / 2;

    /* Calculation of the light centers */
    dist = 10; /* Windth of the pannel margins */   
    traffic_light->green_light_center.x = traffic_light->BL.x + dist * 2 + traffic_light->light_radius;
    traffic_light->green_light_center.y = traffic_light->BL.y + dist * 2 + traffic_light->light_radius;
    traffic_light->yellow_light_center.x = traffic_light->BL.x + dist * 2 + traffic_light->light_radius;
    traffic_light->yellow_light_center.y = traffic_light->BL.y + dist * 6 + traffic_light->light_radius * 3;
    traffic_light->red_light_center.x = traffic_light->BL.x + dist * 2 + traffic_light->light_radius;
    traffic_light->red_light_center.y = traffic_light->BL.y + dist * 10 + traffic_light->light_radius * 5;

    /* Filling the traffic light different arrays */
    for (i = 0; i < NUM_STATES; ++i) {
        traffic_light->times_array[i] = TLight_times_array[i];
        traffic_light->green_lights_array[i] = TLight_green_lights_array[i];
        traffic_light->yellow_lights_array[i] = TLight_yellow_lights_array[i];
        traffic_light->red_lights_array[i] = TLight_red_lights_array[i];
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

    /* Render the green section of the reaffic_light */
    traffic_light_render_section(traffic_light, render_buffer, TLSEC_GREEN);

    /* Render the yellow section of the reaffic_light */
    traffic_light_render_section(traffic_light, render_buffer, TLSEC_YELLOW);

    /* Render the red section of the reaffic_light */
    traffic_light_render_section(traffic_light, render_buffer, TLSEC_RED);    
}

static void
traffic_light_render_section(Traffic_Light_t *traffic_light, Render_Buffer_t *render_buffer,
                                          Traffic_Light_Section_t section)
{
    /* Method for render the particular traffic light section */

    u32 x0 = traffic_light->BL.x; /* BL x coordinate of the section */
    u32 y0; /* BL y coordinate of the section */
    u32 sec_width = traffic_light->width; /* Section width */
    u32 sec_height = traffic_light->height / 3; /* Section height */
    u32 i; /* Temp variable */
    
    /* Determine the y0 parameter of the section */
    switch (section) {
    case TLSEC_GREEN: {
        y0 = traffic_light->BL.y;
        break;
    }
    case TLSEC_YELLOW: {
        y0 = traffic_light->BL.y + (traffic_light->height / 3);
        break;
    }
    case TLSEC_RED: {
        y0 = traffic_light->BL.y + (traffic_light->height / 3) * 2;
        break;
    }}
    
    /* Fill the whole section with traffic light pannel color */
    draw_rect(x0, y0, sec_width, sec_height, traffic_light->pnl_color, render_buffer);

    /* Draw bottom shadow part of the pannel */
    draw_rect(x0, y0, sec_width, 10, traffic_light->pnl_shadow_color, render_buffer);

    /* Draw left shadow part of the pannel */
    draw_rect(x0, y0, 10, sec_height, traffic_light->pnl_shadow_color, render_buffer);

    /* Draw top light part of the pannel */
    draw_rect(x0, y0 + sec_height - 10, sec_width, 10, traffic_light->pnl_light_color, render_buffer);

    /* Draw top right part of the pannel */
    draw_rect(x0 + sec_width - 10, y0, 10, sec_height, traffic_light->pnl_light_color, render_buffer);

    /* Draw "Bottom - Left" and "Top - Right" corners */
    for (i = 0; i < 10; ++i) {
        draw_pixel(x0 + i, y0 + i, traffic_light->pnl_color, render_buffer);
        draw_pixel(x0 + sec_width - 10 + i, y0 + sec_height - 10 + i, traffic_light->pnl_color, render_buffer);
    }

    /* Draw "Top - Left" and "Bottom - Right" corners */
    for (i = 0; i < 10; ++i) {
        draw_ver_line(x0 + i, y0 + sec_height - 10, 10 - i, traffic_light->pnl_shadow_color, render_buffer);
        draw_pixel(x0 + i, y0 + sec_height - i - 1, traffic_light->pnl_color, render_buffer);
        draw_ver_line(x0 + sec_width - 10 + i, y0, 10 - i, traffic_light->pnl_shadow_color, render_buffer);
        draw_pixel(x0 + sec_width - 10 + i, y0 + 10 - i - 1, traffic_light->pnl_color, render_buffer);
    }

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


