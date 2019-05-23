/*!
 * @file Storage.h.
 *
 * Flash Storage functionality for Calliope-demo
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Torsten Curdt		<https://github.com/tcurdt>
 * @author Waldemar Gruenwald 	<https://github.com/gruenwaldi>
 */

#ifndef STORAGE_H
#define STORAGE_H

// Key definitions
#define KEY_TEST "initial"
#define KEY_DEMO "demo"
#define KEY_INTERPRETER "interpreter"

extern MicroBit uBit;

/*!
 * Check for specific storage key
 *
 * @param name Key string to look for
 * @return true, if key is available
 * @return false, if key cannot be found
 */
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

/*!
 * Set a specific storage Key.
 *
 * @param name Key string to set
 * @param value value of the key, default = 1
 */
inline void setStorageKey(const char* name, uint8_t value = 1)
{
	uBit.storage.put(name, &value, sizeof(uint8_t));
}

/*!
 * Remove a specific storage Key.
 *
 * @param name Key string to remove
 */
inline void removeStorageKey(const char* name)
{
	uBit.storage.remove(name);
}

#endif // STORAGE_H