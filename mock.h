#ifndef MOCK_H
#define MOCK_H

#include "global.h"

// Mock lighting
void mock_setAll(EloColor color);
void mock_show();
void mock_broadcastState();

void mock_setIndicator(int i, int s, int l);
void mock_configIndicator(int i, int s, int l);
void mock_loadIndicators(byte *bufPtr);
int mock_getIndicatorCount();
byte *mock_getIndicatorBuffer();
int mock_stateHasChanged();
void mock_resetVersion();
#endif
