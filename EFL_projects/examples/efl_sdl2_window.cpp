/* Here's a sample code snippet showing how to initialize EFL with an SDL2 window */
#include <Ecore_SDL2.h>
#include <Elementary.h>

int main(int argc, char **argv)
{
    Evas_Object *win;
    Ecore_Sdl2_Data *sdl2_data;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return -1;

    if (!ecore_sdl2_init())
        return -1;

    // Create an SDL2 window
    SDL_Window *sdl2_window = SDL_CreateWindow("EFL + SDL2 Window",
                                               SDL_WINDOWPOS_UNDEFINED,
                                               SDL_WINDOWPOS_UNDEFINED,
                                               800, 600,
                                               SDL_WINDOW_OPENGL);

    // Initialize EFL
    ecore_evas_init();

    // Create a new EFL window using the SDL2 window
    sdl2_data = ecore_sdl2_data_get(sdl2_window);
    win = elm_win_add(NULL, "EFL + SDL2 Window", ELM_WIN_BASIC);
    ecore_evas_manual_render_set(sdl2_data->ee, EINA_TRUE);
    elm_win_override_set(win, EINA_TRUE);
    elm_win_alpha_set(win, EINA_TRUE);
    elm_win_indicator_mode_set(win, ELM_WIN_INDICATOR_HIDE);
    elm_win_indicator_opacity_set(win, ELM_WIN_INDICATOR_OPAQUE);
    ecore_evas_data_set(sdl2_data->ee, "sdl2", sdl2_window);
    evas_object_resize(win, 800, 600);
    evas_object_show(win);

    // Enter the EFL main loop
    elm_run();

    // Shutdown EFL and SDL2
    elm_shutdown();
    ecore_evas_shutdown();
    ecore_sdl2_shutdown();
    SDL_Quit();

    return 0;
}