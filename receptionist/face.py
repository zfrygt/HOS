import numpy as np
import cv2
import time
import datetime
import getopt, sys

def usage():
	print("Usage: face.py [-w -f -u] {index_of_cam, url}")

if sys.argv[1] != '-w' and sys.argv[1] != '-f' and sys.argv[1] != '-u':
	print("select type of input: \n\t-w = Webcam\n\t-f = File\n\t-u = RTSP URL")
else:
	if sys.argv[1] == '-w':
		source = int(sys.argv[2])
	else:
		source = sys.argv[2]
		
face_cascade = cv2.CascadeClassifier('D:/External/opencv.3.0/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml')

cap = cv2.VideoCapture(source)

if cap.isOpened(): 
	rval, frame = cap.read()
else:
	print("cannot open cameara")
	rval = False
	
while rval:
	gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
	faces, neighbors, weights = face_cascade.detectMultiScale3(gray, 1.1, 5, outputRejectLevels=True)
	print(weights)
	index = 0
	for (x,y,w,h) in faces:
		if weights[index] < 2:
			continue
		roi_color = frame[y:y+h, x:x+w]
		cv2.imwrite("image_" + str(datetime.datetime.fromtimestamp(time.time()).strftime('%Y-%m-%d_%H-%M-%S')) + ".jpg", roi_color)
		cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,0),2)
	cv2.imshow('frame', frame)
	key = cv2.waitKey(1)
	if key == 27:
		break
	rval, frame = cap.read()

cv2.destroyAllWindows()