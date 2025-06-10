/*
 * parser_parse.h
 *
 *  Created on: Jun 4, 2025
 *      Author: jakub
 */

#ifndef INC_PARSER_COMPLEX_H_
#define INC_PARSER_COMPLEX_H_


#define ENDLINE '\n'

void ParserTakeLine(TRingBuffer *Buff, uint8_t *FinalTable);
void ParserParse(uint8_t *FinalTable);
void ParserParseComplex (uint8_t *FinalTable);



#endif /* INC_PARSER_COMPLEX_H_ */
