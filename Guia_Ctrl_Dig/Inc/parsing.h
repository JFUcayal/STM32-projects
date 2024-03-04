#ifndef PARSING_H
#define PARSING_H
#include <stdbool.h>

extern char buffer[255];
extern char *part1, *part2, *part3, *part4;
extern int part_2, part_3, part_4;
extern int num_spaces, value;
extern int position_number;
extern int counter;
extern bool flag_geral;

int decide_function(char *part1, char *part2, char *part3, char *part4, uint16_t num_spaces);

int is_hex(char *str);

int divide_convert(char *part1,char *part2,char *part3,char *part4);

#endif
