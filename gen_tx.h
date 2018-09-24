#ifndef  _GEN_TX_H_
#define  _GEN_TX_H_

#include "includes.h"

int GetLocation();
int GetInv();
int GetInputFrame(uint frame_num, uint frame_length);
int GetSendFrame(uint frame_num, uint frame_length);

#endif
