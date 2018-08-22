//#ifndef BLUETOOTH_SERVICE_DEBUG_H
//#define BLUETOOTH_SERVICE_DEBUG_H
//
//#include "Interpreter.h"
//#include "ble/BLE.h"
//
///*!
// * @class BluetoothServiceDebug
// *
// */
//class BluetoothServiceDebug {
//public:
//    /*!
//     * Constructor.
//     * Create a representation of BluetoothServiceDebug
//     * @param interpreter  Reference to an Interpreter
//     */
//    BluetoothServiceDebug(Interpreter& interpreter);
//
//    /*!
//     * Callback. Invoked when any of our attributes are written via BLE.
//     */
//    void onDataWritten(const GattWriteCallbackParams* params);
//
//    /*!
//     * Callback. Invoked when any of our attributes are read via BLE.
//     */
//    void onDataRead(GattReadAuthCallbackParams* params);
//
//private:
//    Interpreter& interpreter;
//    BLEDevice& ble;
//
//    GattAttribute::Handle_t characteristicsHandle;
//    GattCharacteristic characteristic;
//    uint8_t characteristicsBuffer[CHARACTERISTICS_BUFFER_LEN];
//
//    uint8_t selector;
//    uint16_t address;
//};
//
//#endif //BLUETOOTH_SERVICE_DEBUG_H