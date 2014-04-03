#include <pebble.h>
#include "bluetooth.h"

Window *window;	
TextLayer *title_layer;
TextLayer *line_layer;
TextLayer *line_layer2;
BitmapLayer *bar_layer;
GBitmap *flag;
TextLayer *round_layer;
TextLayer *racename_layer;
TextLayer *locality_layer;
TextLayer *date_layer;
TextLayer *time_layer;

static AppSync sync;
static uint8_t sync_buffer[256];
static uint32_t raceDateInt;
uint32_t raceRoundNumber; 
const char *raceDateChar; // = "your date here"; 
const char *raceRoundChar; 
const char *seasonChar; 
int raceRound; 
uint32_t offsetInt; 
//const char *diff; 

// Key values for AppMessage Dictionary
enum Settings {customLocation};

//enum someKeys {	STATUS_KEY = 0,	MESSAGE_KEY = 1};

enum raceKeys {
SERIES_KEY 			= 0,   	//	"series": 0,
SEASON_KEY 			= 1,   	//	"season": 1,
ROUND_KEY 			= 2,   	//	"round": 2,
RACENAME_KEY 		= 3,   	//	"round_number": 3,
LOCALITY_KEY 		= 4,   	//	"raceName":4,
DATE_KEY 			= 5,   	//	"locality": 5,
TIME_KEY 			= 6,   	//	"date": 6,
OFFSET_KEY 			= 7,   	//	"time": 7,	
CUSTOMLOCATION_KEY 	= 8,   	//	"offset": 8,
REQUEST_KEY 		= 9,    //	"customLocation": 9,
ROUND_NUMBER_KEY	= 10,   //	"request_key": 10,
SEASON_REQUEST_KEY	= 11,   //	"season_req_key": 11,
MAX_ROUND_KEY		= 12, 	//	"max_round": 12
}; 

// Write message to buffer & send
void send_message(void){
	DictionaryIterator *iter;
	
	app_message_outbox_begin(&iter);
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "sending ROUND_NUMBER_KEY, 9");
//	dict_write_uint8(iter, ROUND_NUMBER_KEY, 9);
	dict_write_end(iter);
 	app_message_outbox_send();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	int raceRoundInt = atoi(raceRoundChar);
	int seasonInt = atoi(seasonChar); 
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "raceRoundChar = %s",  raceRoundChar); 
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "seasonChar = %s",  seasonChar); 
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "currently round %d", raceRoundInt); 
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "creating new round %d = current round - 1", raceRoundInt - 1); 
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "season is %s, sending to URL, check URL receipt", seasonChar); 
	text_layer_set_text(round_layer, "getting race");
	raceRoundInt = raceRoundInt - 1; 
	
	if (raceRoundInt == 0) {
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "raceRoundInt = %d", raceRoundInt); 
//	seasonInt = seasonInt - 1; 
	raceRoundInt = 10;
	seasonInt = seasonInt - 1; 
	APP_LOG(APP_LOG_LEVEL_DEBUG, "round = %d", raceRoundInt); 		
	APP_LOG(APP_LOG_LEVEL_DEBUG, "season = %d", seasonInt); 		
	}
		
	//int i;
	char roundRequestChar[] = "123456";
	char seasonRequestChar[] = "123456"; 
	snprintf(roundRequestChar, sizeof(roundRequestChar), "%d", raceRoundInt);
	snprintf(seasonRequestChar, sizeof(seasonRequestChar), "%d", seasonInt);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "race request string = %%s %s", roundRequestChar);   
	APP_LOG(APP_LOG_LEVEL_DEBUG, "season request string = %%s %s", seasonRequestChar);   
	//text_layer_set_text(&countLayer, buf);
	
	//Tuplet request_tuple = TupletCString(REQUEST_KEY, roundRequestChar);
//	Tuplet request_tuple; 
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	
	if (iter == NULL) {
		return; 
	}
	dict_write_cstring(iter, REQUEST_KEY, roundRequestChar);
	dict_write_cstring(iter, SEASON_REQUEST_KEY, seasonRequestChar); 
//	dict_write_tuplet(iter, &request_tuple); 
	dict_write_end(iter); 
	
	app_message_outbox_send(); 
	//send_message(); 
  	//fetch_msg();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	int raceRoundInt = atoi(raceRoundChar);
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "raceRoundChar = %s",  raceRoundChar); 
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "currently round %d", raceRoundInt); 
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "creating new round %d = current round+1", raceRoundInt + 1); 
	text_layer_set_text(round_layer, "getting race");
//	raceRoundInt = raceRoundInt + 1; 
		
	//int i;
	char roundRequestChar[] = "next";
	char seasonRequestChar[] = "current"; 
	//snprintf(roundRequestChar, sizeof(roundRequestChar), "%d", raceRoundInt);
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "race round string = %%s %s", roundRequestChar);   
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "race season string = %%s %s", seasonRequestChar);   
	//text_layer_set_text(&countLayer, buf);
	
	//Tuplet request_tuple = TupletCString(REQUEST_KEY, roundRequestChar);
//	Tuplet request_tuple; 
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	
	if (iter == NULL) {
		return; 
	}
	dict_write_cstring(iter, REQUEST_KEY, roundRequestChar);
	dict_write_cstring(iter, SEASON_REQUEST_KEY, seasonRequestChar); 
//	dict_write_tuplet(iter, &request_tuple); 
	dict_write_end(iter); 
	
	app_message_outbox_send(); 
	//send_message(); 
  	//fetch_msg();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	int raceRoundInt = atoi(raceRoundChar);
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "raceRoundChar = %s",  raceRoundChar); 
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "currently round %d", raceRoundInt); 
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "creating new round %d = current round+1", raceRoundInt + 1); 
	text_layer_set_text(round_layer, "getting race");
	raceRoundInt = raceRoundInt + 1; 
	if (raceRoundInt > 19) {
		raceRoundInt = 19; 
		APP_LOG(APP_LOG_LEVEL_DEBUG, "max round reached"); 
	}
		
	//int i;
	char roundRequestChar[] = "123456";
	snprintf(roundRequestChar, sizeof(roundRequestChar), "%d", raceRoundInt);
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "race round string = %%s %s", roundRequestChar);   
	//text_layer_set_text(&countLayer, buf);
	
	//Tuplet request_tuple = TupletCString(REQUEST_KEY, roundRequestChar);
//	Tuplet request_tuple; 
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	
	if (iter == NULL) {
		return; 
	}
	dict_write_cstring(iter, REQUEST_KEY, roundRequestChar);
//	dict_write_tuplet(iter, &request_tuple); 
	dict_write_end(iter); 
	
	app_message_outbox_send(); 
	//send_message(); 
  	//fetch_msg();
}

static void click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
	window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {
	Tuple *tuple;
	
	tuple = dict_find(received, REQUEST_KEY);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Status: %d", (int)tuple->value->uint32); 
	}
	
	tuple = dict_find(received, ROUND_KEY);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", tuple->value->cstring);
	}
} 


static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Got error: %s", translate_error(app_message_error)); 
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
	switch (key) {
		case SERIES_KEY:
		APP_LOG(APP_LOG_LEVEL_DEBUG, "SERIES_KEY %s", new_tuple->value->cstring);
		text_layer_set_text(title_layer, new_tuple->value->cstring);
		break;
		
		case SEASON_KEY:
		APP_LOG(APP_LOG_LEVEL_DEBUG, "SEASON_KEY %s", new_tuple->value->cstring);
		seasonChar = new_tuple->value->cstring; 
		break; 
		
		case ROUND_KEY: 
		text_layer_set_text(round_layer, new_tuple->value->cstring);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "ROUND_KEY %s", new_tuple->value->cstring);
//		raceRound = new_tuple->value->uint8; APP_LOG(APP_LOG_LEVEL_DEBUG, "raceRound set to %d", raceRound); 
		break; 
		
		case ROUND_NUMBER_KEY:
		APP_LOG(APP_LOG_LEVEL_DEBUG, "ROUND_NUMBER_KEY %s", new_tuple->value->cstring);
//		APP_LOG(APP_LOG_LEVEL_DEBUG, "raceRoundChar cstring is %s", new_tuple->value->cstring); 
		break; 
		
		case RACENAME_KEY:
		APP_LOG(APP_LOG_LEVEL_DEBUG, "RACENAME_KEY %s", new_tuple->value->cstring);
		text_layer_set_text(racename_layer, new_tuple->value->cstring);
		break; 
		
		case LOCALITY_KEY:
		APP_LOG(APP_LOG_LEVEL_DEBUG, "LOCALITY_KEY %s", new_tuple->value->cstring);
		text_layer_set_text(locality_layer, new_tuple->value->cstring);
		break; 
		
		case DATE_KEY:
		raceDateInt = new_tuple->value->uint32;
		APP_LOG(APP_LOG_LEVEL_DEBUG, "DATE_KEY (uint32) %lu", new_tuple->value->uint32);
		raceDateChar = new_tuple->value->cstring;
		APP_LOG(APP_LOG_LEVEL_DEBUG, "DATE_KEY (cstring) %s", new_tuple->value->cstring);
		break; 
		
		case TIME_KEY: 
		APP_LOG(APP_LOG_LEVEL_DEBUG, "TIME_KEY %s", new_tuple->value->cstring);
		text_layer_set_text(time_layer, new_tuple->value->cstring);
//		text_layer_set_text(date_layer, new_tuple->value->cstring);
		break; 
		
		case OFFSET_KEY:
		APP_LOG(APP_LOG_LEVEL_DEBUG, "OFFSET_KEY %s", new_tuple->value->cstring);
		offsetInt = new_tuple->value->uint32; 
		break; 
		
	}
}

static void handle_second_tick(struct tm* tick_time, TimeUnits units_changed) {
	//static char staleString[] = "1234567890";   //        static char count_text[] = "123456";

	static char diff[] = "365 Days, 24 hours, 59 minutes, 59 seconds"; 
	time_t nowInt = time(NULL) + (3600 * offsetInt);
	int timeInt = nowInt; 
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "offsetInt = %lu ", offsetInt); 

	int diffInt;
	int diffIntSec; 
	int diffIntMin; 
	int diffIntHr; 
	int diffIntDay; 
	diffInt = (raceDateInt - nowInt); // + (3600 * 8) - raceDateInt
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "diffint %d = (raceDate %lu - nowInt %d", diffInt, raceDateInt, timeInt); 
	diffIntSec = diffInt % 60; 
	diffIntMin = (diffInt % 3600) / 60; 
	diffIntHr = (diffInt % 86400) / 3600;
	diffIntDay = diffInt / 86400; 
	
//	snprintf(diff, sizeof(diff), "%dD %dH %dM %dS", diffIntDay, diffIntHr, diffIntMin, diffIntSec); ;
	snprintf(diff, sizeof(diff), "%dd %dhr %dm %ds", diffIntDay, diffIntHr, diffIntMin, diffIntSec); ;
	text_layer_set_text(date_layer, diff); 
}

// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Dropped!");
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Failed to Send!");
}

static void app_message_init(void) {
  // Register message handlers
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_failed(out_failed_handler);
  // Init buffers
  app_message_open(64, 64);
  //fetch_msg();
}


static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window); 
	
		Tuplet initial_values[] = {
			TupletCString(SERIES_KEY, "Formula 1"),
			//TupletInteger(ROUND_NUMBER_KEY, (int) 9),
			TupletCString(ROUND_NUMBER_KEY, "current"),
			TupletCString(SEASON_KEY, "2014"),
			TupletCString(ROUND_KEY, "getting sched"),
			TupletCString(RACENAME_KEY, "Finding Next Grand Prix"),
			TupletCString(LOCALITY_KEY, "Locating next circuit and location; continent"),
			TupletCString(DATE_KEY, "countwown to next race"),
			TupletCString(TIME_KEY, "Time key time key"),
			TupletInteger(OFFSET_KEY, (int) 7), 
	};

	GFont round_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ARIAL_17));
	GFont racename_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ARIAL_BOLD_22));

	
	//create layers x, y, w, h
	title_layer = text_layer_create(GRect(0, 0, 144, 50));
	text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
	text_layer_set_background_color(title_layer, GColorClear);
	bar_layer = bitmap_layer_create(GRect(0, 33, 144, 10));
	flag = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHEQUERED_FLAG);
	bitmap_layer_set_bitmap(bar_layer, flag);
//	line_layer = text_layer_create(GRect(0, 36, 144, 1));
//	line_layer2 = text_layer_create(GRect(0, 39, 144, 1));
//	text_layer_set_background_color(line_layer, GColorBlack);
//	text_layer_set_background_color(line_layer2, GColorBlack);
	round_layer = text_layer_create(GRect(0, 42, 144, 50));
	text_layer_set_background_color(round_layer, GColorClear);
	text_layer_set_text_alignment(round_layer, GTextAlignmentCenter); 
	text_layer_set_overflow_mode(round_layer, GTextOverflowModeFill);
	text_layer_set_font(round_layer, round_font);
//	text_layer_set_font(round_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	racename_layer = text_layer_create(GRect(2, 57, 142, 150));
//	text_layer_set_font(racename_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_font(racename_layer, racename_font); 
	text_layer_set_background_color(racename_layer, GColorClear);
	locality_layer = text_layer_create(GRect(2, 101, 242, 50));
//	text_layer_set_font(locality_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	text_layer_set_font(locality_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_overflow_mode(locality_layer, GTextOverflowModeWordWrap);
	text_layer_set_text_alignment(locality_layer, GTextAlignmentLeft);
	text_layer_set_overflow_mode(locality_layer, GTextOverflowModeTrailingEllipsis);
	text_layer_set_background_color(locality_layer, GColorClear);
	date_layer = text_layer_create(GRect(0, 137, 144, 80));
//	text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
	text_layer_set_overflow_mode(date_layer, GTextOverflowModeWordWrap);
	text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_background_color(date_layer, GColorClear);
	text_layer_set_text_alignment(date_layer, GTextAlignmentCenter); 
	time_layer = text_layer_create(GRect(0-20, 168-40, 184, 20));
	text_layer_set_background_color(time_layer, GColorClear);
	text_layer_set_overflow_mode(time_layer, GTextOverflowModeTrailingEllipsis); 
	
	
	
	GFont custom_font_title = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TAHOMA_BOLD_28));
	text_layer_set_font(title_layer, custom_font_title);
	
	layer_add_child(window_layer, text_layer_get_layer(title_layer));
//	layer_add_child(window_layer, text_layer_get_layer(line_layer));
//	layer_add_child(window_layer, text_layer_get_layer(line_layer2));
	layer_add_child(window_layer, bitmap_layer_get_layer(bar_layer));
	layer_add_child(window_layer, text_layer_get_layer(round_layer));
	layer_add_child(window_layer, text_layer_get_layer(racename_layer));
	layer_add_child(window_layer, text_layer_get_layer(locality_layer));
	layer_add_child(window_layer, text_layer_get_layer(date_layer));
//	layer_add_child(window_layer, text_layer_get_layer(time_layer));	
	
	app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
	sync_tuple_changed_callback, sync_error_callback, NULL);
	
    tick_timer_service_subscribe(SECOND_UNIT, &handle_second_tick);
}

static void window_unload(Window *window) {
}

void init(void) {
	window = window_create();
	window_set_click_config_provider(window, click_config_provider);
	window_set_background_color(window, GColorWhite);
	window_set_fullscreen(window, true);
	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload
	}); 
	
	const bool animated = true;
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);
		
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
	send_message();	
	window_stack_push(window, animated);
}

void deinit(void) {
	app_message_deregister_callbacks();
	window_destroy(window);
	text_layer_destroy(title_layer);
	text_layer_destroy(line_layer);
	text_layer_destroy(line_layer2);
	text_layer_destroy(round_layer);
	text_layer_destroy(racename_layer); 
	text_layer_destroy(locality_layer); 
	text_layer_destroy(date_layer);
	text_layer_destroy(time_layer); 
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}