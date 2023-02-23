/**
 * This file contains the exported symbol.
 */
#include "tge_main_loop.h"

// This is an example of an exported method.
bool
tizentge_main_loop(void)
{


running = true;

// While application is running
while(!running){
	update();
	render();
}
	// Handle events on queue

	// Pass event to engine

// Update the engine
	update();

// Render the engine
	render();

// quit engine
	quit();

	return true;
}

// Main loop
int main(){

// initialize the engine
bool engine_init(){

	return true;
}

void engine_update(){

}

void engine_render(){

}

void engine_quit(){

}
