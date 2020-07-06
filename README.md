###BLE

A shell script that "Access GATT services from BLE Device" using 
bluetoothctl commands.


### Acceptance Criteria 

- The script connect to a BLE device and show all its available services
- This script will read a service from the device.
- This script will write values to a service.

### For example:
Given that there is a BLE device in range and paired
#### When the shell script `./script show gatt` is run the script should print all the available GATT services on the device.

### Reference links
Accessing GATT services - https://docs.ubuntu.com/core/en/stacks/bluetooth/bluez/docs/reference/gatt-services
