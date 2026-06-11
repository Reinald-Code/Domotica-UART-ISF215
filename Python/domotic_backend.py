#!/usr/bin/env python3
"""
@file main.py
@brief Daemon de Control de Seguridad Domótica - Interfaz Base CLI
"""

import serial
import time
import serial.tools.list_ports
import logging

BAUD_RATE = 9600
logging.basicConfig(level=logging.INFO, format='%(asctime)s [%(levelname)s] %(message)s')

class DomoticGateway:
    def __init__(self):
        self.ser = None
        self.is_running = True
        
    def conectar_serial(self):
        try:
            logging.info("Iniciando escaneo automático de puertos seriales...")
            puertos = serial.tools.list_ports.comports()
            puerto_detectado = None
            
            palabras_clave = ["arduino", "uno", "ch340", "usb-serial", "ftdi", "vid_2341"]
            
            for p in puertos:
                descripcion = p.description.lower()
                hardware_id = p.hwid.lower()
                if any(kw in descripcion or kw in hardware_id for kw in palabras_clave):
                    puerto_detectado = p.device
                    logging.info(f"Dispositivo detectado: {p.description} en {p.device}")
                    break
            
            if not puerto_detectado and puertos:
                puerto_detectado = puertos[0].device
            
            if not puerto_detectado:
                raise serial.SerialException("No se detectó ningún dispositivo serial activo.")
                
            self.ser = serial.Serial(puerto_detectado, BAUD_RATE, timeout=0.1)
            time.sleep(2) 
            logging.info(f"Conexión exitosa en: {puerto_detectado}")
            
        except Exception as e:
            logging.error(f"Error de configuración serial: {e}")
            raise e

    def iniciar(self):
        self.conectar_serial()
        try:
            while True:
                print("\n--- Panel de Control de Hardware ---")
                print("1. Consultar Estado")
                print("2. Forzar Apertura de Puerta")
                print("3. Forzar Cierre de Puerta")
                print("4. Forzar Apertura de Ventana")
                print("5. Forzar Cierre de Ventana")
                print("6. Salir")
                opc = input("Seleccione una opción: ")
                
                if opc == '6':
                    break
                else:
                    print(f"Opción {opc} seleccionada (Lógica de envío pendiente de implementación)")
        except KeyboardInterrupt:
            pass
        finally:
            self.is_running = False
            if self.ser:
                self.ser.close()
            logging.info("Apagado del sistema.")

if __name__ == "__main__":
    gateway = DomoticGateway()
    gateway.iniciar()