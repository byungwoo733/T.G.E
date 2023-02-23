#include <Elementary.h>


void _key_down_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   Evas_Event_Key_Down *ev = event_info;

   if (!strcmp(ev->keyname, "Escape"))
     {
        printf("Good Bye!\n");
        elm_exit();
     }
}

int main(int argc, char **argv)
{
   elm_init(argc, argv);

   Evas_Object *win = elm_win_add(NULL, "Elementray Window", ELM_WIN_BASIC);
   elm_win_title_set(win, "Elementary Window");
   evas_object_resize(win, 480, 400);
   evas_object_move(win, 100, 100);
   evas_object_show(win);
   evas_object_event_callback_add(win, EVAS_CALLBACK_KEY_DOWN, _key_down_cb, NULL);

   Evas *e = evas_object_evas_get(win);

   Evas_Object *rect = evas_object_rectangle_add(e);
   evas_object_resize(rect, 480, 400);
   evas_object_color_set(rect, 255, 255, 255, 255);
   evas_object_show(rect);

   Evas_Object *label = elm_label_add(win);
   elm_object_text_set(label, "Hello E World!");
   evas_object_move(label, 100, 100);
   evas_object_resize(label, 200, 100);
   evas_object_show(label);

   elm_run();

   elm_shutdown();

   return 0;
}
