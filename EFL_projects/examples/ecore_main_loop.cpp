/* Here's an example code for using EFL's Ecore main loop */
#include <Ecore.h>

Eina_Bool timer_cb(void *data)
{
   printf("Timer called!\n");

   return ECORE_CALLBACK_RENEW;
}

int main(void)
{
   if (!ecore_init())
      return EXIT_FAILURE;

   Ecore_Timer *timer = ecore_timer_add(2.0, timer_cb, NULL);

   ecore_main_loop_begin();

   ecore_timer_del(timer);

   ecore_shutdown();

   return EXIT_SUCCESS;
}