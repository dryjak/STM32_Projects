/*
 * ring_buffer.h
 *
 *  Created on: Jun 1, 2025
 *      Author: jakub
 */

#ifndef INC_RING_BUFFER_H_
#define INC_RING_BUFFER_H_

//size
#define RING_BUFFER_SIZE 16

//typedef enum
typedef enum
{
	RB_OK 		= 0,
	RB_ERROR 	= 1
}RBStatus;

//obiekt
typedef struct
{
	uint16_t Head;
	uint16_t Tail;
	uint8_t Buffer[RING_BUFFER_SIZE];
}TRingBuffer;

//functions
//read
RBStatus RB_Read(TRingBuffer *Buff, uint8_t *Value);
//write
RBStatus RB_Write(TRingBuffer *Buff, uint8_t Value);
//flush
void RB_Flush(TRingBuffer *Buff);



#endif /* INC_RING_BUFFER_H_ */
