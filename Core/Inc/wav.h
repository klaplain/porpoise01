/*
 * wav.h
 *
 *  Created on: Jun 8, 2023
 *      Author: klapl
 */

#ifndef INC_WAV_H_
#define INC_WAV_H_



/*Standard values for CD-quality audio*/
#define SUBCHUNK1SIZE   (16)
#define AUDIO_FORMAT    (1) /*For PCM*/
#define NUM_CHANNELS    (1)
#define SAMPLE_RATE     (192000)

#define BITS_PER_SAMPLE (16)

#define BYTE_RATE       (SAMPLE_RATE * NUM_CHANNELS * BITS_PER_SAMPLE / 8)
#define BLOCK_ALIGN     (NUM_CHANNELS * BITS_PER_SAMPLE / 8)


#endif /* INC_WAV_H_ */
