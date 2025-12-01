// ===========================
// PEPPERMINT CANDY CANE CHASE
// MAROON CHASE ANIMATION
// ===========================

struct MaroonChaseAnim {
  unsigned long lastUpdate = 0;
  unsigned long interval = 210;
  int offset = 0;
};

MaroonChaseAnim chase;

uint32_t chaseColors[] = {
  strip.Color(245, 10, 10),
  strip.Color(210, 210, 210),
  strip.Color(128, 10, 10) // MAROON
};

void updateMaroonChase() {
  if (millis() - chase.lastUpdate < chase.interval) return;
  chase.lastUpdate = millis();

  chase.offset++;

  for (int i = 0; i < strip.numPixels(); i++) {
    int colorIndex = (i + chase.offset) % 3;
    strip.setPixelColor(i, chaseColors[colorIndex]);
  }
  strip.show();
}

// ===========================
// TWO COLOR FLASH 
// ===========================

struct CandyCaneAnim {
  unsigned long lastUpdate = 0;
  unsigned long interval = 400;
  bool toggle = false;
};

CandyCaneAnim candy;

void updateCandyCane() {
  if (millis() - candy.lastUpdate < candy.interval) return;
  candy.lastUpdate = millis();
  candy.toggle = !candy.toggle;

  for (int i = 0; i < strip.numPixels(); i++) {
    if ((i + candy.toggle) % 2 == 0)
      strip.setPixelColor(i, strip.Color(255, 0, 0));
    else
      strip.setPixelColor(i, strip.Color(0, 255, 0));
  }
  strip.show();
}

// ===========================
// TREE TWINKLE
// ===========================

const uint32_t TREE_COLORS[] = {
  strip.Color(255, 0, 0),
  strip.Color(0, 255, 0),
  strip.Color(255, 180, 0),
  strip.Color(0, 0, 255)
};
const int NUM_TREE_COLORS = 4;

struct TreeLight {
  int pixel;
  int brightness;
  int direction;
  int colorIndex;
};

#define NUM_LIGHTS 20
TreeLight lights[NUM_LIGHTS];

unsigned long lastTreeUpdate = 0;
unsigned long treeInterval = 20;

void setupChristmasTree() {
  for (int i = 0; i < NUM_COLORS; i++) {
    lights[i].pixel = random(strip.numPixels());
    lights[i].brightness = random(0, 255);
    lights[i].direction = random(0, 2) * 2 - 1; // -1 or 1
    lights[i].colorIndex = random(NUM_TREE_COLORS);
  }
}

void updateChristmasTree() {
  unsigned long now = millis();
  if (now - lastTreeUpdate < treeInterval) return;
  lastTreeUpdate = now;

  strip.clear();

  for (int i = 0; i < NUM_LIGHTS; i++) {
    TreeLight &light = lights[i];

    light.brightness += light.direction * 5;

    if (light.brightness >= 255) {
      light.direction = -1;
    }
    if (light.brightness <= 0) {
      light.pixel = random(strip.numPixels());
      light.colorIndex = random(NUM_TREE_COLORS);
      light.direction = 1;
      light.brightness = 0;
    }

    uint32_t c = TREE_COLORS[light.colorIndex];

    uint8_t r = ((c >> 16) & 0xFF) * light.brightness / 255;
    uint8_t g = ((c >> 8) & 0xFF) * light.brightness / 255;
    uint8_t b = (c & 0xFF) * light.brightness / 255;

    strip.setPixelColor(light.pixel, strip.Color(r, g, b));
  }

  strip.show();
}
