/*
 * ring_buffer.c
 *
 *  Created on: Jun 1, 2025
 *      Author: jakub
 */

#include <main.h>
#include <ring_buffer.h>

//read function
RBStatus RB_Read(TRingBuffer *Buff, uint8_t *Value)
{
	if(Buff->Head == Buff->Tail)
	{
		return RB_ERROR;
	}

	*Value = Buff->Buffer[Buff->Tail];
	Buff->Tail = (Buff->Tail + 1) % RING_BUFFER_SIZE;

	return RB_OK;
}

//write function
RBStatus RB_Write(TRingBuffer *Buff, uint8_t Value)
{
	uint16_t HeadTmp = (Buff->Head + 1) % RING_BUFFER_SIZE;
	if(HeadTmp == Buff->Tail)
	{
		return RB_ERROR;
	}

	Buff->Buffer[Buff->Head] = Value;
	Buff->Head = HeadTmp;

	return RB_OK;
}
//Flush
void RB_Flush(TRingBuffer *Buff)
{
	Buff->Head = 0;
	Buff->Tail = 0;
}

