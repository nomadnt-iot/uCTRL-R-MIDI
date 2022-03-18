from serial.tools import list_ports
from serial import Serial
import time
import struct

def list_devices():
    """ Return the list of avaiable device """
    devices = []
    for port in list(list_ports.comports()):
        devices.append({
            'name': '{} on {}'.format(port.product  or 'uCtrl-R-midi', port.device),
            'port': port.device
        })
    return devices

class Device:
    def __init__(self):
        self._serial = Serial(timeout=1)

    def connect(self, port, baudrate=9600):
        """ Connect to serial device """

        if not self.is_connected():
            self._serial.port = port
            self._baudrate = baudrate

            self._serial.open()
            #self._serial.flushInput()
            while self._serial.readline(): pass

        return self

    def disconnect(self):
        """ Disconnect from serial device """

        if self.is_connected():
            self._serial.close()

        return self

    def is_connected(self):
        """ return if device connected or not """
        return self._serial.is_open

    def save(self):
        """ save configuration to device """
        self.send('S')
        self._serial.write(struct.pack('4B4B4B4B4B4B4B4B3B3B', *data))
        while True:
            # reading line
            line = self.read()

            print(line)

            # if 'Done' is find or no more lines on the buffer break
            if not line or line == 'Done': break

        return self

    def reset(self):
        """ Resetting device configuration """
        self.send('R')
        # self._serial.flushInput()
        self.read()

        return self

    def load(self):
        """ Load configuration from device """
        self.send('G')

        # unpacking data from device
        data = struct.unpack('<I3h4B3BB4B4BBB4B4BBBB8B8BBBB8B8B', self._serial.read(74))
        magic = hex(data[0])
        buttons = data[1:4]
        slider = data[4:8]
        vslider = data[8:11]
        print(magic, buttons, slider, vslider)

    def send(self, string):
        """ Send string to serial """
        self._serial.write(bytes(string, 'utf-8'))

    def read(self):
        """ Read line from serial buffer """
        return self._serial.readline().decode('utf-8').rstrip('\r\n')

if __name__ == '__main__':
    device = Device()
    device.connect(list_devices()[0]['port'])
    device.load()
    device.disconnect()
