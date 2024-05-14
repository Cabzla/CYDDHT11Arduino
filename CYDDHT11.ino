#include <TFT_eSPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <lvgl.h>
#include <XPT2046_Touchscreen.h>

// DHT11 Configuration
#define DHTPIN 27
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Display Configuration
TFT_eSPI tft = TFT_eSPI();
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[240 * 10];
static lv_color_t buf2[240 * 10];
lv_disp_drv_t disp_drv;
lv_indev_drv_t indev_drv;

// Touchscreen Configuration
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);

// LVGL Objects
lv_obj_t *temp_label;
lv_obj_t *humid_label;
lv_obj_t *chart;
lv_chart_series_t *ser1;
lv_chart_series_t *ser2;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize the DHT sensor
  dht.begin();

  // Initialize the display
  tft.begin();
  tft.setRotation(1);

  // Initialize LVGL
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, 240 * 10);
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 320;
  disp_drv.ver_res = 240;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // Initialize touch screen
  pinMode(XPT2046_IRQ, INPUT);
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  // Create interface
  create_ui();
}

void loop() {
  lv_timer_handler();
  delay(5);

  // Update sensor readings every second
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 1000) {
    lastUpdate = millis();
    update_sensor_readings();
  }
}

void update_sensor_readings() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  char tempStr[10]; // Platz für die Temperatur als String
  char humidStr[10]; // Platz für die Luftfeuchtigkeit als String
  // Temperatur und Luftfeuchtigkeit in Strings umwandeln
  dtostrf(temperature, 4, 2, tempStr);
  dtostrf(humidity, 4, 2, humidStr);

  // Update LVGL objects with new sensor data
  lv_label_set_text_fmt(temp_label, "Temp: %s C", tempStr);
  lv_label_set_text_fmt(humid_label, "Humidity: %s %%", humidStr);

    // Update max/min labels and chart
  lv_chart_set_next_value(chart, ser1, temperature);
  lv_chart_set_next_value(chart, ser2, humidity);
}

void create_ui() {
  // Create temperature and humidity labels
  temp_label = lv_label_create(lv_scr_act());
  lv_label_set_text(temp_label, "Temp: -- C");
  lv_obj_align(temp_label, LV_ALIGN_TOP_LEFT, 10, 10);

  humid_label = lv_label_create(lv_scr_act());
  lv_label_set_text(humid_label, "Humidity: -- %");
  lv_obj_align(humid_label, LV_ALIGN_TOP_LEFT, 10, 10);

  // Create chart for 24-hour temperature and humidity values
  chart = lv_chart_create(lv_scr_act());
  lv_obj_set_size(chart, 200, 150);
  lv_obj_align(chart, LV_ALIGN_TOP_RIGHT, -10, 10);
  lv_chart_set_type(chart, LV_CHART_TYPE_LINE); 
  ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
  ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);
}

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors(&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  if (digitalRead(XPT2046_IRQ) == LOW) {
TS_Point p = ts.getPoint();
if (p.z > 0) {
data->state = LV_INDEV_STATE_PR;
data->point.x = p.x;
data->point.y = p.y;
}
} else {
data->state = LV_INDEV_STATE_REL;
}
}
