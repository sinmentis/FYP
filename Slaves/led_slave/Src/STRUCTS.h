/*
 * STRUCTS.h
 *
 *  Created on: Sep 23, 2019
 *      Author: Nicole
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_

typedef struct  __attribute__ ((packed)) data_packet_t{
    uint8_t board_info;
    uint8_t board_add;
    uint8_t state;
} data_packet;

union data_in{
	data_packet info_in;
	char info_in_buffer[3];
};

#endif /* STRUCTS_H_ */
