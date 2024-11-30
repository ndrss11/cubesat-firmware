import serial
import csv
import time 
from datetime import datetime
# Configuración del puerto serial
#GPSBaud = 38400
#ESP32Baud = 115200
#current_time = datetime.now().strftime("%d_%m_%Y_%H_%M_%S")
filename = f'fcfm.csv'
# Abre el archivo CSV en modo escritura
with open(filename, mode='w', newline='') as file:
    ser = serial.Serial('/dev/ttyACM1',115200,   timeout=1)
    ser.write(bytes(b"cubesat"))
    line = ser.readline().decode('ascii').rstrip()
    writer = csv.writer(file)
    writer.writerow(line) # Encabezados de las columnas
    time.sleep(2)