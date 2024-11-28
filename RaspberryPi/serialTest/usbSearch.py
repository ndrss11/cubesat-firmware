#Este codigo identifica el puerto en el que esta conectado el arduino UNO
import serial.tools.list_ports

available_ports = list(serial.tools.list_ports.comports())
for port in available_ports:
    print(port.description)
    if port.description.find("R4") > 0: 
        arduinoPort = port.device
    else: print("N/A")
print(arduinoPort)
