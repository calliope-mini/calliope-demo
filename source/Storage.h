#pragma once

#define KEY_TEST "initial"
#define KEY_DEMO "demo"

extern MicroBit uBit;

inline bool hasStorageKey(const char* name)
{
    void* value = uBit.storage.get(name);
    if (value != NULL) {
      free(value);
      return true;
    } else {
      return false;
    }
}

inline void setStorageKey(const char* name, bool state = true)
{
    if (state) {
        uint8_t done = 1;
        uBit.storage.put(name, &done, sizeof(uint8_t));
    } else {
        uBit.storage.remove(name);
    }
}