
import serial 

ser = serial.Serial('/dev/ttyACM0', 9600, timeout = 1)

def read_arduino_data():
	data = ser.readline().decode('ascii').strip()
	if data:
		sensor_name, value = data.split(':')
		print(f"{sensor_name}: {value}")

while True:
	read_arduino_data()
