#define TARGET_EXTENSION 1
#include "C_API/pd_api.h"

void logToConsole(PlaydateAPI *pd, const char* fmt) {
  pd->system->logToConsole(fmt);
}

void errorToConsole(PlaydateAPI *pd, const char* fmt) {
  pd->system->error(fmt);
}
