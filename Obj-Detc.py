import cv2
import time
import urllib.request
import requests
import numpy as np

#thres = 0.45  threshold to detect object
url1='https://blynk.cloud/external/api/logEvent?token=brQLoKvNjS50VmpBYwBgIT6970w8QMad&code=warning&description=Person Detected'

classNames = []
classFile = "C:\Autonomous Surveillance and Response Rover\Object_Detection_Files/coco.names"
with open(classFile,"rt") as f:
    classNames = f.read().rstrip("\n").split("\n")

configPath = "C:\Autonomous Surveillance and Response Rover\Object_Detection_Files/ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt"
weightsPath = "C:\Autonomous Surveillance and Response Rover\Object_Detection_Files/frozen_inference_graph.pb"

net = cv2.dnn_DetectionModel(configPath,weightsPath)
net.setInputSize(650,650)
net.setInputScale(1.0/ 127.5)
net.setInputMean((127.5, 127.5, 127.5))
net.setInputSwapRB(True)


def getObjects(img, thres, nms, draw=True, objects=[]):
    classIds, confs, bbox = net.detect(img, confThreshold=thres, nmsThreshold=nms)
    #print(classIds,bbox)
    if len(objects) == 0: objects = classNames
    objectInfo =[]
    if len(classIds) != 0:
        for classId, confidence,box in zip(classIds.flatten(),confs.flatten(),bbox):
            className = classNames[classId - 1]
            if className in objects:
                objectInfo.append([box,className])
                if (draw):
                    cv2.rectangle(img,box,color=(0,255,0),thickness=2)
                    cv2.putText(img,classNames[classId-1].upper(),(box[0]+10,box[1]+30),
                    cv2.FONT_HERSHEY_COMPLEX,1,(0,255,0),2)
                    cv2.putText(img,str(round(confidence*100,2)),(box[0]+200,box[1]+30),
                    cv2.FONT_HERSHEY_COMPLEX,1,(0,255,0),2)
                    print(className)
                    if(className == 'person'):
                        requests.get(url1)
                        print("HUMAN")
                        time.sleep(3)

    return img,objectInfo


#
url='http://192.168.153.112/cam-hi.jpg'
#url='http://192.168.249.112/cam-hi.jpg'
cv2.namedWindow("live cam testing",cv2.WINDOW_AUTOSIZE)
cap= cv2.VideoCapture(url)


#cap= cv2.VideoCapture(0)  video from lap cam

while True:
    img_resp=urllib.request.urlopen(url)
    imgnp=np.array(bytearray(img_resp.read()),dtype=np.uint8)
    im=cv2.imdecode(imgnp,-1)
    getObjects(im,0.55,0.2, objects=['person'])
    cv2.imshow("Output",im)
    cv2.waitKey(1)
