/* =================================================================================== */
/* Simulation.h header file                                                            */
/* =================================================================================== */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <utils.h>

/* Enumerator for different states of the traffic light simulation */
enum Simulation_State {
    SST_OBJECTS_CREATION, /* Objects creation procedure */
    SST_INITIALIZATION, /* Object initialization procedure */
    SST_INIT_RENDER, /* Initial render procedure */
    SST_SIMULATION, /* Main simulation procedure */
    SST_FINALIZATION /* Finalization procedure */                       
};
typedef enum Simulation_State Simulation_State_t;

/* Main entry point to the simulation of the traffic light */
void simulate_traffic_light(f32 dtime, Key_Pressed_t key_pressed, Render_Buffer_t *render_buffer,
                            b32 *rinning);
                   
#endif // SIMULATION_H_
