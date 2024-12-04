def insertar_linea(nombre_archivo, texto):
    try:
        # Abre el archivo en modo de escritura al final (append)
        with open(nombre_archivo, 'a') as archivo:
            archivo.write(texto + '\n')  # Escribe la línea con salto de línea
        print(f"Línea añadida: {texto}")
    except Exception as e:
        print(f"Ocurrió un error: {e}")

# Uso del código
nombre_del_archivo = 'mi_archivo.txt'
linea_de_texto = 'Esta es una línea nueva.'

# Llamada para insertar
insertar_linea(nombre_del_archivo, linea_de_texto)