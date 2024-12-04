
import serial 
import serial.tools.list_ports #https://pyserial.readthedocs.io/en/latest/tools.html#module-serial.tools.list_ports
import time
import csv

puertos = list(serial.tools.list_ports.comports())

available_ports = list(serial.tools.list_ports.comports())


#for port in available_ports:
#    print(port.description)
#    if port.description.find("Arduino Uno") > 0: 
#        arduinoPort = port.device
#    else: print("N/A")
#print(arduinoPort)


ser = serial.Serial("/dev/ttyACM0", 115200 , timeout = 1)


def insertar_linea(nombre_archivo, texto):
    try:
        # Abre el archivo en modo de escritura al final (append)
        with open(nombre_archivo, 'a') as archivo:
            archivo.write(texto + '\n')  # Escribe la línea con salto de línea
        print(f"Línea añadida: {texto} \n")
    except Exception as e:
        print(f"Ocurrió un error: {e}")

# Uso del código
nombre_del_archivo = 'groundStation.txt'
linea_de_texto = 'Esta es una línea nueva.'

# Llamada para insertar


while True:
	#ser.write(bytes(b"cubesat"))
	line = ser.readline().decode('ascii').rstrip()
	print(line)

	insertar_linea(nombre_del_archivo, line)
	time.sleep(2)
		
