#include "demo_game.h"

static Eina_Bool
asteroid_motion(void *asteroid_data, double position) {
	Evas_Coord x;
	Evas_Object * asteroid = (Evas_Object *) asteroid_data;
	evas_object_geometry_get(asteroid, &x, NULL, NULL, NULL);
	evas_object_move(asteroid, x, position * screen_height);

	return EINA_TRUE;
}

static void
move_scene(appdata_s *ad) {
	ad->game_position += ad->velocity;
	evas_object_image_fill_set(ad->background, 0,
			ad->game_position % BACKGROUND_IMAGE_HEIGHT, screen_width,
			BACKGROUND_IMAGE_HEIGHT);
}

static Eina_Bool
play_game_cb(void *app_data) {
	static int i = 0;
	appdata_s *ad = (appdata_s *) app_data;

	move_scene(ad);

	if (ad->game_position > 2 * i * screen_height) {
		int k = rand() % ASTEROIDS_NUMBER;
		restart_asteroid_position(ad->asteroid[k]);
		ecore_animator_timeline_add(2 * MINIMUM_VELOCITY / ad->velocity,
				asteroid_motion, ad->asteroid[k]);
		i++;
	}

	return ECORE_CALLBACK_RENEW;
}

static void
ship_rotation(Evas_Object *ship, double angle) {
	Evas_Map *m;
	Evas_Coord x, y, w, h;
	int y_sensitivity = 6;
	int z_sensitivity = 2;
	int focal_distance = 300;

	m = evas_map_new(4);
	evas_object_geometry_get(ship, &x, &y, &w, &h);
	evas_map_util_points_populate_from_object(m, ship);

	double rotation_x = 0;
	double rotation_y = -1 * y_sensitivity * angle;
	double rotation_z = -1 * z_sensitivity * angle;
	int image_ceter_x = x + (w / 2);
	int image_center_y = y + (h / 2);
	int image_center_z = 0;

	int lights_distance = 1000;

	/* center of the image is the center of rotation */
	evas_map_util_3d_rotate(m, rotation_x, rotation_y, rotation_z,
			image_ceter_x, image_center_y, image_center_z);

	evas_map_util_3d_perspective(m, image_ceter_x, image_center_y, 0,
			focal_distance);

	/* Light reflection from left
	 * (255, 255, 255) is RGB color of ambient light,
	 * (0, 0, 0) is RGB color of light  */
	evas_map_util_3d_lighting(m, x - lights_distance, image_center_y, 0, 0, 0,
			0, 255, 255, 255);

	/* Light reflection from right
	 * (255, 255, 255) is RGB color of ambient light,
	 * (255, 99, 71) is RGB color of light  */
	evas_map_util_3d_lighting(m, x + lights_distance, image_center_y, 0, 255,
			99, 71, 255, 255, 255);

	evas_object_map_set(ship, m);
	evas_object_map_enable_set(ship, TRUE);

}

static void
accelerometer_cb(sensor_h sensor, sensor_event_s *event,
		void *app_data) {
	const double g = 9.81; // gravitational acceleration
	Evas_Coord x, y, w, h;
	int minimum_y_pos = 0.4 * screen_height;
	appdata_s *ad = (appdata_s *) app_data;

	evas_object_geometry_get(ad->ship, &x, &y, &w, &h);

	x = x - CONTROL_SENSITIVITY * event->values[0];
	y = y - CONTROL_SENSITIVITY * (event->values[1] - 0.26 * g);

	/* movement restrictions  */
	if (x + w > screen_width)
		x = screen_width - w;

	if (x < 0)
		x = 0;

	if (y < minimum_y_pos)
		y = minimum_y_pos;

	if (y + h > screen_height)
		y = screen_height - h;

	// move object
	evas_object_move(ad->ship, x, y);

	// velocity control
	ad->velocity = MINIMUM_VELOCITY
			* (1 + (float) (screen_height - y) / screen_height);

	// rotate object
	ship_rotation(ad->ship, event->values[0]);

}

static void
set_enemy_sprite_position(Evas_Object *enemy, int position) {
	evas_object_image_fill_set(enemy,
			-1 * (ENEMY_EXPLOSION_IMAGE_WIDTH / ENEMY_EXPLOSION_POS_NUM)
					* position, 0, ENEMY_EXPLOSION_IMAGE_WIDTH,
			ENEMY_EXPLOSION_IMAGE_HEIGHT);
}

static Eina_Bool
enemy_explosion(void *app_data) {
	appdata_s *ad = (appdata_s *) app_data;
	static int frame = 0;
	set_enemy_sprite_position(ad->enemy, frame);
	frame += 1;

	if (frame >= ENEMY_EXPLOSION_POS_NUM) {
		frame = 0;
		restart_enemy(ad->enemy);
		ad->enemy_is_exploded = false;
		return EINA_FALSE;
	}

	return EINA_TRUE;
}

static Eina_Bool
enemy_motion(void *enemy_data) {
	int amplitude = 5;
	int period = 100;
	int vertical_speed = 3;

	Evas_Coord x, y, w, h;
	Evas_Object * enemy = (Evas_Object *) enemy_data;
	evas_object_geometry_get(enemy, &x, &y, &w, &h);

	int horizontal_speed = amplitude * sin((double) y / period);

	if (y > screen_height) {
		restart_enemy(enemy);
		return EINA_TRUE;
	}

	x += horizontal_speed;
	y += vertical_speed;

	evas_object_move(enemy, x, y);

	return EINA_TRUE;
}

static Eina_Bool
shot_animation(void *app_data) {
	Evas_Coord x, y, h;
	Evas_Object * laser = (Evas_Object *) app_data;
	evas_object_geometry_get(laser, &x, &y, NULL, &h);

	y -= PROJECTILE_VELOCITY;

	if (y + h < 0) {
		evas_object_del(laser);
		return EINA_FALSE;
	}
	evas_object_move(laser, x, y);

	return EINA_TRUE;
}

static void
increase_score(appdata_s *ad) {
	char buf[PATH_MAX];
	ad->current_score++;
	snprintf(buf, sizeof(buf), "%04d", ad->current_score);
	evas_object_text_text_set(ad->score, buf);
}

static void
mouse_down_cb(void *app_data, Evas *e, Evas_Object *obj,
		void *event_info) {
	Evas_Coord x, y, w, enemy_x, enemy_y, enemy_w, enemy_h;
	appdata_s *ad = app_data;

	evas_object_geometry_get(ad->ship, &x, &y, &w, NULL);
	evas_object_geometry_get(ad->enemy, &enemy_x, &enemy_y, &enemy_w, &enemy_h);

	int ship_center_x = x + w / 2;

	Evas_Object* laser = evas_object_rectangle_add(e);
	/* Color of laser ray in RGB and alpha channel  */
	evas_object_color_set(laser, 255, 50, 50, 255);

	evas_object_move(laser, ship_center_x - (LASER_WIDTH / 2),
			y - LASER_HEIGHT);
	evas_object_resize(laser, LASER_WIDTH, LASER_HEIGHT);
	evas_object_show(laser);
	ecore_animator_add(shot_animation, laser);

	if (ship_center_x >= enemy_x && ship_center_x <= enemy_x + enemy_w
			&& enemy_y < y && !ad->enemy_is_exploded) {
		/* The enemy was shot down */
		increase_score(ad);
		ad->enemy_is_exploded = true;
		ecore_animator_add(enemy_explosion, ad);
	}
}

static int
register_accelerometer_callback(appdata_s *ad) {
	int error;
	bool supported;
	sensor_h accelerometer;
	sensor_listener_h accelerationListener;

	error = sensor_is_supported(SENSOR_ACCELEROMETER, &supported);
	if (error != SENSOR_ERROR_NONE && supported) {
		return error;
	}

	error = sensor_get_default_sensor(SENSOR_ACCELEROMETER, &accelerometer);
	if (error != SENSOR_ERROR_NONE) {
		return error;
	}

	error = sensor_create_listener(accelerometer, &accelerationListener);
	if (error != SENSOR_ERROR_NONE) {
		return error;
	}

	error = sensor_listener_set_event_cb(accelerationListener,
	ACCELEROMETER_INTERVAL_MS, accelerometer_cb, ad);
	if (error != SENSOR_ERROR_NONE) {
		return error;
	}

	error = sensor_listener_start(accelerationListener);

	return SENSOR_ERROR_NONE;
}

static Evas_Object *
add_image(Evas_Object * parent_object,
		const char *filename, int width, int height) {
	char buf[PATH_MAX];
	Evas_Object * image = evas_object_image_filled_add(
			evas_object_evas_get(parent_object));

	snprintf(buf, sizeof(buf), "%s/", app_get_resource_path());
	strcat(buf, filename);
	evas_object_image_file_set(image, buf, NULL);
	evas_object_resize(image, width, height);

	return image;
}

static void
restart_enemy(Evas_Object *enemy) {
	int width = ENEMY_EXPLOSION_IMAGE_WIDTH / ENEMY_EXPLOSION_POS_NUM;
	evas_object_move(enemy, rand() % (screen_width - width),
			-1 * ((rand() % 300) + ENEMY_EXPLOSION_IMAGE_HEIGHT));
	evas_object_image_fill_set(enemy, 0, 0, ENEMY_EXPLOSION_IMAGE_WIDTH,
			ENEMY_EXPLOSION_IMAGE_HEIGHT);
}

static void
restart_asteroid_position(Evas_Object *asteroid) {
	evas_object_move(asteroid, rand() % (screen_width - ASTEROID_WIDTH),
			-1 * ASTEROID_HEIGHT);
}

static void
create_base_gui(appdata_s *ad) {
	srand(time(NULL));

	/* init game parameters */
	ad->velocity = MINIMUM_VELOCITY;

	/* Window */
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_wm_desktop_layout_support_set(ad->win, EINA_TRUE);
	elm_win_autodel_set(ad->win, EINA_TRUE);
	evas_object_smart_callback_add(ad->win, "delete,request",
			win_delete_request_cb, NULL);

	/* Get device parameters */
	elm_win_screen_size_get(ad->win, NULL, NULL, &screen_width, &screen_height);

	/* Add scrollable background */
	ad->background = add_image(ad->win, "starfield_small.jpg", screen_width,
			screen_height);
	evas_object_move(ad->background, 0, 0);
	evas_object_image_fill_set(ad->background, 0, 0, screen_width,
	BACKGROUND_IMAGE_HEIGHT);

	/* Add asteroids */
	for (int i = 0; i < ASTEROIDS_NUMBER; i++) {
		char buf[PATH_MAX];
		snprintf(buf, sizeof(buf), "asteroid%d.png", i);
		ad->asteroid[i] = add_image(ad->win, buf, ASTEROID_WIDTH,
				ASTEROID_HEIGHT);
		restart_asteroid_position(ad->asteroid[i]);
		evas_object_show(ad->asteroid[i]);
	}

	/* Add ship */
	ad->ship = add_image(ad->win, "ship.png", SHIP_IMAGE_WIDTH,
			SHIP_IMAGE_HEIGHT);
	evas_object_move(ad->ship, 0.5 * screen_width - SHIP_IMAGE_WIDTH / 2,
			0.7 * screen_height);

	/* Add enemy */
	int enemy_width = ENEMY_EXPLOSION_IMAGE_WIDTH / ENEMY_EXPLOSION_POS_NUM;
	ad->enemy = add_image(ad->win, "ship_explode.png", enemy_width,
			ENEMY_EXPLOSION_IMAGE_HEIGHT);
	restart_enemy(ad->enemy);

	/* add score */
	Evas_Object *score_frame = add_image(ad->win, "score_frame.png",
			SCORE_FRAME_WIDTH, SCORE_FRAME_HEIGHT);
	evas_object_move(score_frame, SCORE_FRAME_POS_X, SCORE_FRAME_POS_Y);
	evas_object_image_border_set(score_frame, SCORE_BORDER_WIDTH,
	SCORE_BORDER_WIDTH, SCORE_BORDER_WIDTH, SCORE_BORDER_WIDTH);
	evas_object_image_border_scale_set(score_frame, 0.5);

	ad->score = evas_object_text_add(evas_object_evas_get(score_frame));
	evas_object_text_font_set(ad->score, "Vera", SCORE_TEXT_SIZE);
	evas_object_resize(ad->score, SCORE_FRAME_WIDTH, SCORE_FRAME_HEIGHT);
	evas_object_move(ad->score, 35, 24);
	char buf[PATH_MAX];
	snprintf(buf, sizeof(buf), "%04d", ad->current_score);
	evas_object_text_text_set(ad->score, buf);

	/* show evas objects */
	evas_object_show(ad->background);
	evas_object_show(ad->ship);
	evas_object_show(ad->enemy);
	evas_object_show(score_frame);
	evas_object_show(ad->score);

	/* Register callbacks to game control */
	ecore_animator_add(play_game_cb, ad);

	evas_object_event_callback_add(ad->background, EVAS_CALLBACK_MOUSE_DOWN,
			mouse_down_cb, ad);
	register_accelerometer_callback(ad);
	ecore_animator_add(enemy_motion, ad->enemy);

	/* Show window after base gui is set up */
	evas_object_show(ad->win);

	ecore_event_handler_add(ECORE_EVENT_KEY_DOWN, keydown_cb, NULL);
}

static void
win_delete_request_cb(void *data, Evas_Object *obj,
		void *event_info) {
	elm_exit();
}

static Eina_Bool
keydown_cb(void *data, int type, void *event) {
	appdata_s *ad = data;
	Ecore_Event_Key *ev = event;
	if (!strcmp(ev->keyname, KEY_END)) {
		/* Let window go to hide state. */
		elm_win_lower(ad->win);
		return ECORE_CALLBACK_DONE;
	}
	return ECORE_CALLBACK_PASS_ON;
}

static bool
app_create(void *app_data) {
	appdata_s *ad = app_data;
	create_base_gui(ad);
	return true;
}

static void
app_pause(void *data) {
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data) {
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data) {
	/* Release all resources. */
}

int
main(int argc, char *argv[]) {
	appdata_s *ad = { 0, };
	app_event_callback_s event_callback;

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = NULL;
	event_callback.language_changed = NULL;

	event_callback.low_memory = NULL;
	event_callback.low_battery = NULL;
	event_callback.device_orientation = NULL;
	event_callback.region_format_changed = NULL;

	memset(&ad, 0x0, sizeof(appdata_s));

	int ret = app_efl_main(&argc, &argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		LOGE("app_efl_main() is failed. err = %d", ret);
	}

	return ret;
}
