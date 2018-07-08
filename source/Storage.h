#ifndef STORAGE_H
#define STORAGE_H

#define KEY_TEST "initial"
#define KEY_DEMO "demo"
#define KEY_INTERPRETER "interpreter"

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

inline void setStorageKey(const char* name, uint8_t value = 1)
{
    uBit.storage.put(name, &value, sizeof(uint8_t));
}

inline void removeStorageKey(const char* name)
{
    uBit.storage.remove(name);
}

#endif // STORAGE_H