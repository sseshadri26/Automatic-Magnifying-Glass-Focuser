import cv2
import urllib.request
import numpy as np
import paho.mqtt.client as mqtt
import time

 
def nothing(x):
    pass
 
#change the IP address below according to the
#IP shown in the Serial monitor of Arduino code
url='http://10.0.0.149/cam-hi.jpg'
 


client = mqtt.Client(client_id="ESP32-CAM")
# client.connect("localhost", 1883, 60)
client.connect("test.mosquitto.org", 1883, 60)



# cv2.namedWindow("live transmission", cv2.WINDOW_AUTOSIZE)


 
# cv2.namedWindow("Tracking")
# cv2.createTrackbar("LH", "Tracking", 0, 255, nothing)
# cv2.createTrackbar("LS", "Tracking", 0, 255, nothing)
# cv2.createTrackbar("LV", "Tracking", 0, 255, nothing)
# cv2.createTrackbar("UH", "Tracking", 255, 255, nothing)
# cv2.createTrackbar("US", "Tracking", 255, 255, nothing)
# cv2.createTrackbar("UV", "Tracking", 255, 255, nothing)
 
while True:
    
    img_resp=urllib.request.urlopen(url)
    imgnp=np.array(bytearray(img_resp.read()),dtype=np.uint8)
    frame=cv2.imdecode(imgnp,-1)
    
    # hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    
    gray = cv2.medianBlur(gray, 5)
    
    
    rows = gray.shape[0]
    circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, 1, rows / 8,
                               param1=100, param2=30,
                               minRadius=1, maxRadius=300)

    if circles is not None:
        circles = np.uint16(np.around(circles))
        for i in circles[0, :]:
            center = (i[0], i[1])
            # circle center
            cv2.circle(frame, center, 1, (0, 100, 100), 3)
            # circle outline
            radius = i[2]
            cv2.circle(frame, center, radius, (255, 0, 255), 3)
            client.publish("magDistance", str(int(i[2])))
            time.sleep(0.25)
            break
    
    
    cv2.imshow("detected circles", frame)
 
    # l_h = cv2.getTrackbarPos("LH", "Tracking")
    # l_s = cv2.getTrackbarPos("LS", "Tracking")
    # l_v = cv2.getTrackbarPos("LV", "Tracking")
 
    # u_h = cv2.getTrackbarPos("UH", "Tracking")
    # u_s = cv2.getTrackbarPos("US", "Tracking")
    # u_v = cv2.getTrackbarPos("UV", "Tracking")
 
    # l_b = np.array([l_h, l_s, l_v])
    # u_b = np.array([u_h, u_s, u_v])
    
    # mask = cv2.inRange(hsv, l_b, u_b) 
    # res = cv2.bitwise_and(frame, frame, mask=mask)
 
    # cv2.imshow("live transmission", frame)
    # cv2.imshow("mask", mask)
    # cv2.imshow("res", res)
    key=cv2.waitKey(5)
    if key==ord('q'):
        break
    
cv2.destroyAllWindows()