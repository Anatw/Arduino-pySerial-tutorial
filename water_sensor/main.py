import serial
from vpython import cylinder, color, label, vector, rate
import io

# Serial() open a serial communication with the port specified. The Python program locks
# the specified port specified for its usage.
# The baudrate argument must be identical to the baud rate defined in the Arduino sketch,
# otherwise the Python program will not be able to read the data.
board_ser = serial.Serial(port='COM3', baudrate=9600, timeout=30)
# While we send data to the Arduino using the Serial method, we need a way to flush that
# data so that the Arduino will know when each serial message is ending. For this, we use
# TextIOWrapper.
sio = io.TextIOWrapper(io.BufferedRWPair(board_ser, board_ser))
# Two virtual objects will be created - a cylinder to represent the water level, and a
# label to output the water level in text.
water_object = cylinder(color=color.blue, radio=5, Length=5, axis=vector(0, 1, 0))
label = label(text="Water level is: ", color=color.white, pos=vector(0, -.2, 0))


def send_serial_to_arduino(byte_to_send):
    board_ser.write(byte_to_send)
    sio.flush()


while True:
    if board_ser.inWaiting() > 0:  # Wait until there is serial data to read - inWaiting() return the
        water_sensor_value = board_ser.readline().strip().decode('utf-8')
        if water_sensor_value.isdigit():
            wsv_float = float(water_sensor_value.strip())
            water_level = round(((6. / 641.) * wsv_float), 1)
            if water_level <= 1:
                send_serial_to_arduino(b'l')
            elif 1 < water_level <= 4.5:
                send_serial_to_arduino(b'm')
            elif 4.5 < water_level:
                send_serial_to_arduino(b'h')

            label.text = water_level
            # If the water_object length is set to 0 it disappears, and then it will not
            # return even if the water level gets higher. For this reason, it should be
            # set to 0.1 is it equals 0.
            if water_level == 0.0:
                wsv_float = 0.1
            water_object.length = water_level
