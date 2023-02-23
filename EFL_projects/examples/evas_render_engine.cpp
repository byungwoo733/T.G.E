/* Here's an example code for using EFL's Evas render engine */
#include <Evas.h>

int main(int argc, char **argv)
{
   Evas *evas;
   Evas_Object *win, *bg, *rect;

   // Initialize EFL
   evas_init();

   // Create a new Evas instance
   evas = evas_new();

   // Create a new window
   win = evas_object_rectangle_add(evas);
   evas_object_resize(win, 400, 400);
   evas_object_color_set(win, 255, 255, 255, 255);
   evas_object_show(win);

   // Create a background rectangle
   bg = evas_object_rectangle_add(evas);
   evas_object_color_set(bg, 0, 0, 0, 255);
   evas_object_resize(bg, 400, 400);
   evas_object_show(bg);
   evas_object_move(bg, 0, 0);

   // Create a red rectangle
   rect = evas_object_rectangle_add(evas);
   evas_object_color_set(rect, 255, 0, 0, 255);
   evas_object_resize(rect, 50, 50);
   evas_object_show(rect);
   evas_object_move(rect, 50, 50);

   // Add the background and red rectangle to the window
   evas_object_smart_member_add(bg, win);
   evas_object_smart_member_add(rect, win);

   // Render the scene
   evas_render_updates(evas);

   // Free memory and shutdown EFL
   evas_object_del(win);
   evas_free(evas);
   evas_shutdown();

   return 0;
}