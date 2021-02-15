import sensor
import image
import lcd
import time
from machine import I2C,UART

from fpioa_manager import fm

#uart initial
fm.register(35, fm.fpioa.UART2_TX, force=True)
fm.register(34, fm.fpioa.UART2_RX, force=True)

uart_Port = UART(UART.UART2, 115200,8,0,0, timeout=1000, read_buf_len= 4096)


clock = time.clock()
lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_vflip(1)
sensor.set_hmirror(1)
sensor.run(1)
sensor.skip_frames(20)
while True:
    clock.tick()
    img = sensor.snapshot()
    res = img.find_barcodes()
    fps =clock.fps()
    if len(res) > 0:
        data = res[0].payload()
        print(data)
        uart_Port.write(data)
        time.sleep(2)
    lcd.display(img)
