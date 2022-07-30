import psutil
import time
import serial

ser = serial.Serial("COM11", 115200, timeout=5)

while True:
    time.sleep(0.1)
    cpu = psutil.cpu_percent(interval=None)
    mem = psutil.virtual_memory()
    print(cpu, mem.percent)
    ser.write(f"dm {int(cpu)} {int(mem.percent)}\n".encode('utf-8'))