/*
 * parsing.c
 *
 *  Created on: 28/03/2023
 *      Author: jhof2002
 */

#include <stdio.h>
#include "parsing.h"
#include <string.h>
#include <stdbool.h>

//struct who go store the data from commands
struct command_list{
	char command[20];
	char command_abbreviation[5];
	int  space_number;
};

struct command_list commands_list[] ={

            {"CONTROL_SYSTEM", "CS", 1},
            {"ENABLE_SYSTEM", "EN", 1},
            {"DEFINE_SAMPLING", "HW", 1},
            {"SAMPLING_NUMBER", "SW", 1},
            {"POSITION_NUMBER", "SPOS", 1},
            {"NORMALIZE_VOLTAGE", "UN", 1},
            {"RESET_POSITION", "IPOS", 0},
            {"PID_SAMPLING_PERIOD", "PIDh", 1},	//7
            {"PID_DEFINE_REF_POS", "PIDyr", 1},
            {"PID_DEFINE_PROP_GAIN", "PIDkp", 1},
            {"PID_DEFINE_INTG_GAIN", "PIDki", 1},
            {"PID_DEFINE_DVT_GAIN", "PIDkd", 1},
            {"PID_DEFINE_LOW_FILTER_CONST", "PIDa", 1},
};

int decide_function(char *part1, char *part2, char *part3, char *part4, uint16_t num_spaces){

	//divide sentence according to spaces
		  part1 = strtok(buffer, " ");
		  part2 = strtok(NULL, " ");
		  part3 = strtok(NULL, " ");
		  part4 = strtok(NULL, " ");


		  //converts the values from char to hexadecimal integer
		  sscanf(part2, "%x", &part_2);
		  sscanf(part3, "%x", &part_3);
		  sscanf(part4, "%x", &part_4);

		  sscanf(part2, "%d", &value);

	//decides which function to execute based on the command abbreviation received via UART
  	if ((strcasecmp(part1, commands_list[0].command_abbreviation) == 0) && (num_spaces == commands_list[0].space_number) && (flag_geral == true)) {
  		control_system(part_2);
  		return 1;
  	}
	else if((strcasecmp(part1, commands_list[1].command_abbreviation) == 0) && (num_spaces == commands_list[1].space_number)){
  		enable_system(part_2);
  		return 1;
  	}
	else if((strcasecmp(part1, commands_list[2].command_abbreviation) == 0) && (num_spaces == commands_list[2].space_number) && (flag_geral == true)){
		int decimal = atoi(part2);
		define_timer_value(decimal);
		return 1;
	  	}
	else if((strcasecmp(part1, commands_list[3].command_abbreviation) == 0) && (num_spaces == commands_list[3].space_number) && (flag_geral == true)){
		define_sampling(part_2);
		return 1;
	  	}
	else if((strcasecmp(part1, commands_list[4].command_abbreviation) == 0) && (num_spaces == commands_list[4].space_number) && (flag_geral == true)){
		 position_number = part_2;
		 return 1;
	  	}
	else if((strcasecmp(part1, commands_list[5].command_abbreviation) == 0) && (num_spaces == commands_list[5].space_number) && (flag_geral == true)){
		 normalize_voltage(value);
		 return 1;
	  	}
	else if((strcasecmp(part1, commands_list[6].command_abbreviation) == 0) && (num_spaces == commands_list[6].space_number) && (flag_geral == true)){
		 reset_position();
		 return 1;
		}
	else if((strcasecmp(part1, commands_list[7].command_abbreviation) == 0) && (num_spaces == commands_list[7].space_number) && (flag_geral == true)){
		int decimal = atoi(part2);
		pid_sampling(decimal);
		return 1;
		}
	else if((strcasecmp(part1, commands_list[8].command_abbreviation) == 0) && (num_spaces == commands_list[8].space_number) && (flag_geral == true)){
		int decimal = atoi(part2);
		pid_define_pos(decimal);
		return 1;
		}
	else if((strcasecmp(part1, commands_list[9].command_abbreviation) == 0) && (num_spaces == commands_list[9].space_number) && (flag_geral == true)){
		int decimal = atoi(part2);
		pid_define_prop_gain(decimal);
		return 1;
		}
	else if((strcasecmp(part1, commands_list[10].command_abbreviation) == 0) && (num_spaces == commands_list[10].space_number) && (flag_geral == true)){
		int decimal = atoi(part2);
		pid_define_intg_gain(decimal);
		return 1;
		}
	else if((strcasecmp(part1, commands_list[11].command_abbreviation) == 0) && (num_spaces == commands_list[11].space_number) && (flag_geral == true)){
		int decimal = atoi(part2);
		pid_define_dvt_gain(decimal);
		return 1;
		}
	else if((strcasecmp(part1, commands_list[12].command_abbreviation) == 0) && (num_spaces == commands_list[12].space_number) && (flag_geral == true)){
		int decimal = atoi(part2);
		pid_define_low_filter_const(decimal);
		return 1;
		}
	else {
  		printf("The entered does not match any command available! \n \r");
  		return 0;
  	}
}

/*
int divide_convert(char *part1,char *part2,char *part3,char *part4){


	  //divide sentence according to spaces
	  part1 = strtok(buffer, " ");
	  part2 = strtok(NULL, " ");
	  part3 = strtok(NULL, " ");
	  part4 = strtok(NULL, " ");


	  //converts the values from char to hexadecimal integer
	  sscanf(part2, "%x", &part_2);
	  sscanf(part3, "%x", &part_3);
	  sscanf(part4, "%x", &part_4);

	  return 0;
}
*/

//will see if there is any non-hexadecimal character
int is_hex(char *str) {
    for (; *str != '\0'; str++) {
        if (!isxdigit(*str)) {
            return 0;		//character non-hexadecimal
        }
    }
    return 1;	//word only has hex characters
}
