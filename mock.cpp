#include "mock.h"

// Lighting Provider Mocks
void mock_setAll(EloColor color) {

}

void mock_show() {

}

void mock_broadcastState() {

}

void mock_setIndicator(int i, int s, int l) {
}

void mock_configIndicator(int i, int s, int l) {
}

void mock_loadIndicators(byte *bufPtr) {

}

int mock_getIndicatorCount() {
    return 0;
}

byte *mock_getIndicatorBuffer() {
    return (byte *)0;
}

int mock_stateHasChanged()
{
    return 0;
}

void mock_resetVersion(){

}