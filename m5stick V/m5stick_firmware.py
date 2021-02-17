import sensor,image,lcd
from fpioa_manager import *
from Maix import I2S, GPIO
from board import board_info
import audio
import time
from machine import I2C, UART
import re

##UART
fm.register(35, fm.fpioa.UART2_TX, force=True)
fm.register(34, fm.fpioa.UART2_RX, force=True)
uart_Port = UART(UART.UART2, 9600,8,0,0, timeout=1000, read_buf_len= 4096)

##UART
##audio
fm.register(board_info.SPK_SD, fm.fpioa.GPIO0)
spk_sd=GPIO(GPIO.GPIO0, GPIO.OUT)
spk_sd.value(1)
fm.register(board_info.SPK_DIN,fm.fpioa.I2S0_OUT_D1)
fm.register(board_info.SPK_BCLK,fm.fpioa.I2S0_SCLK)
fm.register(board_info.SPK_LRCLK,fm.fpioa.I2S0_WS)
wav_dev = I2S(I2S.DEVICE_0)

but_a_pressed = 0

def play_wav(fname):
    player = audio.Audio(path = fname)
    player.volume(20)
    wav_info = player.play_process(wav_dev)
    wav_dev.channel_config(wav_dev.CHANNEL_1,
        I2S.TRANSMITTER,resolution = I2S.RESOLUTION_16_BIT,
        align_mode = I2S.STANDARD_MODE)
    wav_dev.set_sample_rate(wav_info[1])
    while True:
        ret = player.play()
        if ret == None:
            break
        elif ret==0:
            break
    player.finish()

##audio

## LED
fm.register(board_info.BUTTON_B, fm.fpioa.GPIO2)
but_b = GPIO(GPIO.GPIO2, GPIO.IN, GPIO.PULL_UP)

fm.register(board_info.LED_W, fm.fpioa.GPIO3)
led_w = GPIO(GPIO.GPIO3, GPIO.OUT)
led_w.value(1) # LED is Active Low

fm.register(board_info.LED_R, fm.fpioa.GPIO4)
led_r = GPIO(GPIO.GPIO4, GPIO.OUT)
led_r.value(1) # LED is Active Low

fm.register(board_info.LED_G, fm.fpioa.GPIO5)
led_g = GPIO(GPIO.GPIO5, GPIO.OUT)
led_g.value(1) # LED is Active Low

fm.register(board_info.LED_B, fm.fpioa.GPIO6)
led_b = GPIO(GPIO.GPIO6, GPIO.OUT)
led_b.value(1) # LED is Active Low
##LED

##qr
clock = time.clock()
##qr

##LCD CAM
lcd.init()
lcd.rotation(2)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
#sensor.set_vflip(1) #added later
sensor.run(1)
#sensor.skip_frames(20) #added later
##LCD CAM

while True:
    clock.tick()
    img=sensor.snapshot()
    res = img.find_qrcodes()


    if len(res) > 0:
        play_wav("beep.wav")
        led_w.value(1)
        led_r.value(1)
        led_g.value(0)
        led_b.value(1)
        data = str(res[0].payload())
        token = data.split(":")
        print(token[0])
        uart_Port.write(token[0])

    else:
       # but_a_pressed = 1
        led_w.value(1)
        led_r.value(1)
        led_g.value(1)
        led_b.value(1)

    lcd.display(img)
