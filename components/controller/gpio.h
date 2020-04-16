#ifndef GPIO_H
#define GPIO_H

#ifdef __cplusplus
extern "C"{
#endif
/*
 * Return the current lvl of the GPIO pin
 */
int read_gpio(int id);
/*
 * Writes the level to the GPIO pin. Due to lightbleeding, the _off_ lvl
 * is an _input_.
 */
void write_gpio(int id, int level);

#ifdef __cplusplus
}
#endif

#endif