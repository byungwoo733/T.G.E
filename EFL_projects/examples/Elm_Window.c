#include <Elementary.h>


int main(int argc, char **argv)
{
   elm_init(argc, argv);

   Evas_Object *win = elm_win_add(NULL, "Elementray Window", ELM_WIN_BASIC);
   elm_win_title_set(win, "Elementary Window");
   evas_object_resize(win, 480, 400);
   evas_object_move(win, 100, 100);
   evas_object_show(win);

   elm_run();

   elm_shutdown();

   return 0;
}
