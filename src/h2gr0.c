#include <pebble.h>
  
// This is a custom defined key for saving our count field
#define NUM_OUNCES_PKEY 1

// You can define defaults for values in persistent storage
#define NUM_OUNCES_DEFAULT 0

static Window *window;
static BitmapLayer *image_layer;
static GBitmap *image;

static int num_oz = NUM_OUNCES_DEFAULT;

//updates images
//still kind of really buggy - only wokrs some of the time
//was going to include progress bar but images aren't updating so yeah
static void update_image() {
  if(num_oz <= 8){
    image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BABY_1);
  }
  /*if(num_drinks == 16){
    image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BABY_2);
  }*/
  if(num_oz == 24){
    image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TODDLER_1);
  }
  /*if(num_drinks == 32){
    image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TODDLER_2);
  }*/
  if(num_oz == 40){
    image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TEEN_1);
  }
  /*if(num_drinks == 48){
    image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TEEN_2);
  }*/
  if(num_oz >= 56){
    image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ADULT_1);
  }
  /*if(num_drinks >= 64){
    image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ADULT_2);
  }*/
  bitmap_layer_set_bitmap(image_layer, image);

}

//increments count - updates image appropriately - maybe
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  num_oz+=8;
  update_image();
}
//resets num_oz to 0, image back to the baby plant
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  num_oz = 0;
  image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BABY_1);
  bitmap_layer_set_bitmap(image_layer, image);   
}
//leaving the splash page
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  update_image();
}
//button configuration
static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  
  //splash page
  image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SPLASH);

  // The bitmap layer holds the image for display
  image_layer = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(image_layer, image);
  bitmap_layer_set_alignment(image_layer, GAlignCenter);
  layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));
}

static void window_unload(Window *window) {
  gbitmap_destroy(image);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  num_oz = persist_exists(NUM_OUNCES_PKEY) ? persist_read_int(NUM_OUNCES_PKEY) : NUM_OUNCES_DEFAULT;

  window_stack_push(window, animated);
}

static void deinit(void) {
  persist_write_int(NUM_OUNCES_PKEY, num_oz);
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}