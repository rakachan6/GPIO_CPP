#ifndef GPIO_CPP_APP_H
#define GPIO_CPP_APP_H

#pragma once

#ifdef __cplusplus

enum class Mode {off, steady, flash_double, print_help};

extern "C" {
#endif

void app_init(void);
void app_main_while(void);

#ifdef __cplusplus
}
#endif

#endif //GPIO_CPP_APP_H
