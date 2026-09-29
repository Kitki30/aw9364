# AW9364 Library

Simple driver for AW9364 LED controller, used in T-Embed CC1101's

## Instalation
Requires C++ compiler.

Copy this repo to your libraries folder (e.g. /lib/aw9364)

Paste following code into your main CMakeLists.txt
```
# aw9364 lib
list(APPEND EXTRA_COMPONENT_DIRS "${CMAKE_SOURCE_DIR}/lib/aw9364")
```
Replace path with aw9364 lib directory location

## Usage example
```cpp
#include "lib/aw9364/aw9364.hpp"
#include "soc/gpio_num.h"

AW9364 aw9364;
aw9364.init((gpio_num_t)21); // Specify your GPIO number here, needs to have output capabilities

for (int i = 0; i < 16; i++) {
    aw9364.set_level(i); // Set light level to i, light levels can be 0-16, where 0 is disabled, 1 is the brightest level, 16 is the darkest
    ESP_LOGI(TAG, "Current light level: %d", aw9364.get_level()); // Get current light level
    vTaskDelay(300);
}
aw9364.deinit(); // Deinits GPIO pin and disables light
```

Used SensorLib as a reference, awinic docs are really bad
