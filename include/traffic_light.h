/* =================================================================================== */
/* traffic_light.h header file                                                         */
/* =================================================================================== */

#ifndef TRAFFIC_LIGHT_H_
#define TRAFFIC_LIGHT_H_

#include <utils.h>

/* Enumerator to denote the section */
enum Traffic_Light_Section {
    TLSEC_GREEN, /* Traffic light green section */
    TLSEC_YELLOW, /* Traffic light yellow section */
    TLSEC_RED /* Traffic light red section */                            
};
typedef enum Traffic_Light_Section Traffic_Light_Section_t;

/* Enumerator with 2 different working modes of the traffic light */
enum Traffic_Light_Work_Mode {
    WM_AUTO, /* Work in automatic mode */
    WM_MANUAL /* Work in manual mode */
};
typedef enum Traffic_Light_Work_Mode Traffic_Light_Work_Mode_t;

/* Enumerator with different possible states of traffic_light (in automatic worlk mode) */
enum Traffic_Light_State {
    GREEN_ON, /* Green light on */
    GREEN_OFF_1, /* Green light off 1st time */
    GREEN_ON_1, /* Green light on 1st time */
    GREEN_OFF_2, /* Green light off 2nd time */
    GREEN_ON_2, /* Green light on 2nd time */
    GREEN_OFF_3, /* Green light off 3rd time */
    GREEN_ON_3, /* Green light on 3rd time */
    YELLOW_ON, /* Green light off and tellow light on */
    RED_ON, /* Yellow light off and red light on */
    RED_YELLOW_ON, /* Red and yellow lights on */
    NUM_STATES /* Total number of different states */
};
typedef enum Traffic_Light_State Traffic_Light_State_t;

/* Structure of the traffic_light with corresponding data */
struct Traffic_Light {
    const u32 width; /* Traffic light width */
    const u32 height; /* Traffic light height */
    const u32 pnl_color; /* Color of the traffic light front pannel */
    const u32 pnl_light_color; /* Color of the pannel light part */
    const u32 pnl_shadow_color; /* Color of the pannel shadow part */
    const u32 green_color; /* Color of the green light */
    const u32 yellow_color; /* Color of the yellow light */
    const u32 red_color; /* Color of the red light */
    const u32 off_color; /* Color of the turned off light */
    const u32 light_radius; /* Radius of the light */
    V2_u32_t BL; /* Bottom left corner possition of the traffic light */
    V2_u32_t green_light_center; /* Center of the green light */
    V2_u32_t yellow_light_center; /* Center of the yellow light */
    V2_u32_t red_light_center; /* Center of the red light */
    Traffic_Light_Work_Mode_t work_mode; /* Current work mode of the traffic light */
    Traffic_Light_State_t current_state; /* Current traffic light state */
    f32 passed_time; /* Time, passed from the last changed state */
    f32 times_array[NUM_STATES]; /* Array with times for every traffic light state */
    b32 green_lights_array[NUM_STATES]; /* Array of the green light work in every state */
    b32 yellow_lights_array[NUM_STATES]; /* Array of the yellow light work in every state */
    b32 red_lights_array[NUM_STATES]; /* Array of the red light work in every state */
    b32 green_light_state; /* Current state of the green light (0 - off, 1 - on) */
    b32 yellow_light_state; /* Current state of the yellow light (0 - off, 1 - on) */
    b32 red_light_state; /* Current state of the red light (0 - off, 1 - on) */
};
typedef struct Traffic_Light Traffic_Light_t;

/* Constructor of the traffic light object */
Traffic_Light_t* traffic_light_constructor(void);

/* Destructor of the traffic light object */
void traffic_light_destructor(Traffic_Light_t *traffic_light);

/* Initialization of the traffic light object */
void traffic_light_init(Traffic_Light_t *traffic_light, const Render_Buffer_t *render_buffer);

/* Method for set the auto mode of the traffic light */
void traffic_light_set_auto_mode(Traffic_Light_t *traffic_light);

/* Method for set the manual mode of the traffic light */
void traffic_light_set_manual_mode(Traffic_Light_t *traffic_light);

/* Method for toggle the green light of the traffic light */
void traffic_light_toggle_green(Traffic_Light_t *traffic_light);

/* Method for toggle the yellow light of the traffic light */
void traffic_light_toggle_yellow(Traffic_Light_t *traffic_light);

/* Method for toggle the red light of the traffic light */
void traffic_light_toggle_red(Traffic_Light_t *traffic_light);

/* Method for render the traffic light on the screen */
void traffic_light_render(Traffic_Light_t *traffic_light, Render_Buffer_t *render_buffer);
                   
#endif // TRAFFIC_LIGHT_H_
