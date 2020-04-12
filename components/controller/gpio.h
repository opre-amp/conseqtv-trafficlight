#ifndef GPIO_H
#define GPIO_H

#ifdef __cplusplus
extern "C"{
#endif

int read_gpio(int id);
void write_gpio(int id, int level);

#ifdef __cplusplus
}
#endif

#endif