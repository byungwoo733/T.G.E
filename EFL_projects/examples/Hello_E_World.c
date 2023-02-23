#include <Elementary.h>


int main(int argc, char **argv)
{
   elm_init(argc, argv);

   printf("Hello E World!\n");

   elm_run();

   elm_shutdown();

   return 0;
}
