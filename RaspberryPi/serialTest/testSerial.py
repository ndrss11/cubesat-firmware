
import serial 
import serial.tools.list_ports #https://pyserial.readthedocs.io/en/latest/tools.html#module-serial.tools.list_ports

puertos = list(serial.tools.list_ports.comports())

available_ports = list(serial.tools.list_ports.comports())
for port in available_ports:
    print(port.description)
    if port.description.find("R4") > 0: 
        arduinoPort = port.device
    else: print("N/A")
print(arduinoPort)


ser = serial.Serial(arduinoPort, 9600, timeout = 1)

def read_arduino_data():
	data = ser.readline().decode('ascii').strip()
	if data:
		sensor_name, value = data.split(':')
		print(f"{sensor_name}: {value}")

while True:
	read_arduino_data()
