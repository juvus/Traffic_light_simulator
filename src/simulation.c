/* =================================================================================== */
/* Realization of the simulation procedure                                             */
/* =================================================================================== */

#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <simulation.h>
#include <software_rendering.h>
#include <traffic_light.h>
#include <utils.h>

/* Define different simulation constants */
/* World constants */
const u32 WORLD_BKG_COLOR = 0xefefef; /* World background color */

/* Traffic light constants */
const u32 TLIGHT_WIDTH = 180; /* Width of the traffic light */
const u32 TLIGHT_HEIGHT = 540; /* Height of the traffic light (3 times gires than width)*/
const u32 TLIGHT_PNL_COLOR = 0x323232; /* Color of the traffic light front pannel */
const u32 TLIGHT_PNL_LIGHT_COLOR = 0x787878; /* Color of the pannel light part */
const u32 TLIGHT_PNL_SHADOW_COLOR = 0x191919; /* Color of the pannel shadow part */
const u32 TLIGHT_GREEN_COLOR = 0x00ff00; /* Color of the green light */
const u32 TLIGHT_YELLOW_COLOR = 0xffff00; /* Color of the yellow light */
const u32 TLIGHT_RED_COLOR = 0xff0000; /* Color of the red light */
const u32 TLIGHT_OFF_COLOR = 0x000000; /* Color of the turned off color */
const u32 TLIGHT_LIGHT_RADIUS = 70; /* Radius of the light */
const f32 TLight_times_array[10] = {7.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 3.0f, 10.0f, 3.0f};
const b32 TLight_green_lights_array[10]  = {1, 0, 1, 0, 1, 0, 1, 0, 0, 0};
const b32 TLight_yellow_lights_array[10] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 1};
const b32 TLight_red_lights_array[10]    = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1};

/* Define different object and variables necessary for the simulation */
static Simulation_State_t simulation_state = SST_OBJECTS_CREATION; /* Current simulation state */
static Traffic_Light_t *traffic_light;
static b32 need_lights_change; /* Flag showind the need in lights change */

void
simulate_traffic_light(f32 dtime, Key_Pressed_t key_pressed, Render_Buffer_t *render_buffer,
                       b32 *running)
{
    /* Main function for the simulation of the traffic light */

    /* Set the constant value for dtime for testing purposes */
    //dtime = 0.01;
    
    switch (simulation_state) {
    case SST_OBJECTS_CREATION: {
        /* =========================================================================================== */
        /* Creation of different objects (Memory allocation)                                           */
        /* =========================================================================================== */

        traffic_light = traffic_light_constructor();
            
        /* Jump to the next simulation stage */
        simulation_state = SST_INITIALIZATION;
        break;
    }
    case SST_INITIALIZATION: {
        /* =========================================================================================== */
        /* Initialization and reset procedure                                                          */
        /* =========================================================================================== */

        traffic_light_init(traffic_light, render_buffer);
        
        /* Jump to the next simulation stage */
	simulation_state = SST_INIT_RENDER;
        break;
    }
    case SST_INIT_RENDER: {
        /* =========================================================================================== */
        /* Initial render of the objects                                                               */
        /* =========================================================================================== */

        clear_full_screen(WORLD_BKG_COLOR, render_buffer);
        traffic_light_render(traffic_light, render_buffer);
        
        /* Jump to the next simulation stage */
        simulation_state = SST_SIMULATION;
        break;
    }
    case SST_SIMULATION: {
        /* =========================================================================================== */
        /* Simulation run logic procedure                                                              */
        /* =========================================================================================== */

        /* Change the mode of the traffic light work */
        if (key_pressed == KP_AUTO_WORK_MODE) {
            traffic_light_set_auto_mode(traffic_light);
        }
        if (key_pressed == KP_MANUAL_WORK_MODE) {
            traffic_light_set_manual_mode(traffic_light);
        }

        /* Toggle traffic light colors in case of manual mode */
        if (traffic_light->work_mode == WM_MANUAL) {
            if (key_pressed == KP_TOGGLE_GREEN) {
                traffic_light_toggle_green(traffic_light);
            }
            if (key_pressed == KP_TOGGLE_YELLOW) {
                traffic_light_toggle_yellow(traffic_light);
            }
            if (key_pressed == KP_TOGGLE_RED) {
                traffic_light_toggle_red(traffic_light);
            }
        }

        /* Process the traffic light lights states in case of auto mode */
        if (traffic_light->work_mode == WM_AUTO) {
            /* Check if the traffic light state should be changed */
            
            if (traffic_light->passed_time >= traffic_light->times_array[traffic_light->current_state]) {
                traffic_light->current_state += 1;
                /* Check if we moved to 1 state over the available states */
                if (traffic_light->current_state == NUM_STATES) {
                    traffic_light->current_state = GREEN_ON;
                }
                need_lights_change = 1;
                traffic_light->passed_time = 0.0f;
            }
            else {
                traffic_light->passed_time += dtime;
            }

            /* Set the light according to the traffic light state */
            if (need_lights_change) {
                /* Check the green light */
                traffic_light->green_light_state =
                    traffic_light->green_lights_array[traffic_light->current_state];
                /* Check the yellow light */
                traffic_light->yellow_light_state =
                    traffic_light->yellow_lights_array[traffic_light->current_state];
                /* Check the red light */
                traffic_light->red_light_state =
                    traffic_light->red_lights_array[traffic_light->current_state];

                /* Turn off the next time lights change */
                need_lights_change = 0;
            }
        }
        
        /* Render the traffic light */
        traffic_light_render(traffic_light, render_buffer);
        
        /* Jump to the next simulation stage or exit */
        if (key_pressed == KP_ESCAPE) {
            simulation_state = SST_FINALIZATION;
        }
        else {
            simulation_state = SST_SIMULATION;
        }
        break;
    } 

    case SST_FINALIZATION: {
        /* =========================================================================================== */
        /* Simulation finished procesure                                                               */
        /* =========================================================================================== */
        *running = false;
        traffic_light_destructor(traffic_light);
        break;
    }}
}
