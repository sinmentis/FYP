/*
 * STRUCTS.h
 *
 *  Created on: Sep 22, 2019
 *      Author: Nicole
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_


typedef struct  __attribute__ ((packed)) data_packet_t{
    uint8_t board_info;
    uint8_t board_add;
    uint8_t state;
} data_packet;

typedef struct  __attribute__ ((packed)) board_info_t{
    int board_type_1 : 1;
    int board_type_2 : 1;
    int board_type_3 : 1;
    int board_type_4 : 1;

    int board_num_1 : 1;
	int board_num_2 : 1;
	int board_num_3 : 1;
	int board_num_4 : 1;
} board_info;


union board_info_data{
	board_info board_info_raw;
	uint8_t board_info_char;
};

union data_in{
	data_packet info_in;
	char info_in_buffer[3];
};


#endif /* STRUCTS_H_ */
