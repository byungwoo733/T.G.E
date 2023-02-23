#ifndef __spriteexample_H__
#define __spriteexample_H__

#include <app.h>
#include <Elementary.h>
#include <runtime_info.h>
#include <efl_extension.h>
#include <sensor.h>
#include <dlog.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "spriteexample"

#if !defined(PACKAGE)
#define PACKAGE "org.tizen.spriteexample"
#endif

#define KEY_END "XF86Stop"

#define ACCELEROMETER_INTERVAL_MS 20
#define CONTROL_SENSITIVITY 2.0
#define MINIMUM_VELOCITY 5

/* background image properties */
#define BACKGROUND_IMAGE_HEIGHT 2160

/* ship image properties */
#define SHIP_IMAGE_WIDTH 90
#define SHIP_IMAGE_HEIGHT 80
#define LASER_WIDTH 4
#define LASER_HEIGHT 100

/* enemy image properties */
#define ENEMY_EXPLOSION_IMAGE_WIDTH 1980
#define ENEMY_EXPLOSION_IMAGE_HEIGHT 180
#define ENEMY_EXPLOSION_POS_NUM 11

/* Asteroids properties */
#define ASTEROIDS_NUMBER 3
#define ASTEROID_WIDTH 100
#define ASTEROID_HEIGHT 80
#define ASTEROID_TO_BACKGROUND_SPEED_RATIO 2

/* Shot speed */
#define PROJECTILE_VELOCITY 100

/* SCORE  */
#define SCORE_FRAME_WIDTH 120
#define SCORE_FRAME_HEIGHT 60
#define SCORE_BORDER_WIDTH 30
#define SCORE_FRAME_POS_X 20
#define SCORE_FRAME_POS_Y 20
#define SCORE_TEXT_SIZE 40


typedef struct appdata {
	Evas_Object *win;
	Evas_Object *background;
	Evas_Object *ship;
	Evas_Object *enemy;
	Evas_Object *score;
	Evas_Object * asteroid[ASTEROIDS_NUMBER];
	int current_score;
	bool enemy_is_exploded;
	int velocity;
	int game_position;
} appdata_s;

static int screen_width;
static int screen_height;

static void win_delete_request_cb(void * , Evas_Object * , void *);
static Eina_Bool keydown_cb(void * , int , void *);
static void app_pause(void *);
static void app_resume(void *);
static void app_terminate(void *);
static bool app_create(void *);
static void create_base_gui(appdata_s *);

static Eina_Bool play_game_cb(void *);
static void move_scene(appdata_s *);
static Eina_Bool enemy_explosion(void *);
static Eina_Bool shot_animation(void *);
static Eina_Bool asteroid_motion(void *, double );
static void mouse_down_cb(void *, Evas * , Evas_Object * , void *);
static int register_accelerometer_callback(appdata_s *);
static void accelerometer_cb(sensor_h , sensor_event_s *, void *);
static void ship_rotation(Evas_Object *, double );
static void set_enemy_sprite_position(Evas_Object *, int );
static void restart_enemy(Evas_Object *);
static void restart_asteroid_position(Evas_Object * );

#endif /* __spriteexample_H__ */
