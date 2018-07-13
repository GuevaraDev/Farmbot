from flask import *
import time
import cv2
import numpy as np
import serial
import RPi.GPIO as GPIO
from threading import Thread

wt = 0.4
photo_trigger = 17

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(photo_trigger, GPIO.IN)

try:
    arduino = serial.Serial('/dev/ttyUSB0', 9600)
    print("PORT 0")
except:
    arduino = serial.Serial('/dev/ttyUSB1', 9600)
    print("PORT 1")

app = Flask(__name__)

def axis_movment(action):
  
  if action == 1:
    print("X-")
    arduino.write('A'.encode())
  elif action == 2:
    print("X_Stop")
    arduino.write('B'.encode())
  elif action == 3:
    print("X+")
    arduino.write('C'.encode())
  elif action == 4:
    print("Y-")
    arduino.write('D'.encode())
  elif action == 5:
    print("Y_Stop")
    arduino.write('E'.encode())
  elif action == 6:
    print("Y+")
    arduino.write('F'.encode())
  elif action == 7:
    print("Z-")
    arduino.write('G'.encode())
  elif action == 8:
    print("Z_Stop")
    arduino.write('H'.encode())
  elif action == 9:
    print("Z+")
    arduino.write('I'.encode())
  else:
    return ('Error', 500)
  #action = 0
  #print("Variable action ", action)

# Index route
@app.route("/grid")
def index():
  return render_template('grid.html')

#Demo
@app.route("/plantar")
def demo():
  return render_template('plantar.html')

#Demo actions
@app.route("/demo/<int:demo_action>", methods=['POST'])
def demo_actions(demo_action):  
    if demo_action == 1:
        print("Plantar")
        arduino.write('J'.encode())
    elif demo_action == 2:
        print("Regar")
        arduino.write('K'.encode()) 
    elif demo_action == 3:
        print("Inspeccionar")
        arduino.write('L'.encode())
    return ('', 200)

#Home
@app.route("/")
def manual():
  return render_template('home.html')
'''
#Manual actions
@app.route("/manual/<int:action>", methods=['POST'])
def manual_control(action):  
  axis_movment(action)
    
  return ('', 200)
'''
# About route
@app.route("/about")
def about():
  return render_template('about.html')

@app.route("/grid/<int:status>", methods=['POST'])
def input_grid(status):  
  #print(status)
  if status == 1:
    print("A1")
    arduino.write('1'.encode())
  elif status == 2:
    print("A2")
    arduino.write('2'.encode())
  elif status == 3:
    print("A3")
    arduino.write('3'.encode())
  elif status == 4:
    print("A4")
    arduino.write('4'.encode())
  elif status == 5:
    print("A5")
    arduino.write('5'.encode())
#Grid B
  elif status == 6:
    print("B1")
    arduino.write('6'.encode())
  elif status == 7:
    print("B2")
    arduino.write('7'.encode())
  elif status == 8:
    print("B3")
    arduino.write('8'.encode())
  elif status == 9:
    print("B4")
    arduino.write('9'.encode())
  elif status == 10:
    print("B5")
    arduino.write('M'.encode())
#Grid C
  elif status == 11:
    print("C1")
    arduino.write('N'.encode())
  elif status == 12:
    print("C2")
    arduino.write('O'.encode())
  elif status == 13:
    print("C3")
    arduino.write('P'.encode())
  elif status == 14:
    print("C4")
    arduino.write('Q'.encode())
  elif status == 15:
    print("C5")
    arduino.write('R'.encode())
#Grid D
  elif status == 16:
    print("D1")
    arduino.write('S'.encode())
  elif status == 17:
    print("D2")
    arduino.write('T'.encode())
  elif status == 18:
    print("D3")
    arduino.write('U'.encode())
  elif status == 19:
    print("D4")
    arduino.write('V'.encode())
  elif status == 20:
    print("D5")
    arduino.write('W'.encode())
#Grid E
  elif status == 21:
    print("E1")
    arduino.write('X'.encode())
  elif status == 22:
    print("E2")
    arduino.write('Y'.encode())
  elif status == 23:
    print("E3")
    arduino.write('Z'.encode())
  elif status == 24:
    print("E4")
    arduino.write('-'.encode())
  elif status == 25:
    print("E5")
    arduino.write('+'.encode())
#Grid T
  elif status == 26:
    print("T1")
    arduino.write('!'.encode())
  elif status == 27:
    print("T2")
    arduino.write('#'.encode())
  elif status == 28:
    print("T3")
    arduino.write('$'.encode())
  elif status == 29:
    print("T4")
    arduino.write('%'.encode())
  elif status == 30:
    print("T5")
    arduino.write('&'.encode())
  else:
    
    arduino.write('{'.encode())
    return ('Error', 500)
  return ('', 200)

def video_capture():
    cap = cv2.VideoCapture(0)
    scaling_factor = 0.8
    time.sleep(0.5)
    
    while True:
        #frame = get_frame(cap, scaling_factor)
        ret, frame = cap.read()

        # Resize the input frame
        frame = cv2.resize(frame, None, fx=scaling_factor,
                fy=scaling_factor, interpolation=cv2.INTER_AREA)
                
        # Convert the HSV colorspace
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        s = 25
            
        # Define 'blue' range in HSV colorspace
        lower = np.array([60 - s,50,50])
        upper = np.array([60 + s,255,255])

        # Threshold the HSV image to get only blue color
        mask = cv2.inRange(hsv, lower, upper)

        # Bitwise-AND mask and original image
        res = cv2.bitwise_and(frame, frame, mask=mask)
        res = cv2.medianBlur(res, 5)

        #cv2.imshow('Original image', frame)
        cv2.imshow('Color Detector', res)

        # Check if the user pressed ESC key
        c = cv2.waitKey(5)
        if c == 27:
            break

if __name__ == "__main__":
    #Camera Feed Execution Thread
    th = Thread(target=video_capture, args=())
    th.start()
    
    app.run(host="192.168.0.105", port=5005)
  
