from sklearn.neighbors import KNeighborsClassifier
import cv2
import pickle
import numpy as np
import os
import csv
import time
from datetime import datetime
from win32com.client import Dispatch

def speak(str1):
    speak = Dispatch(("SAPI.SpVoice"))
    speak.Speak(str1)

# Replace this with the correct ESP32 camera IP address
stream_url = "http://192.168.204.10:81/stream"  # Adjust if necessary"  # Adjust if necessary


# Open the stream with OpenCV
video = cv2.VideoCapture(stream_url)

if not video.isOpened():
    print("Error: Unable to connect to stream.")
    exit()

facedetect = cv2.CascadeClassifier('data/haarcascade_frontalface_default.xml')

with open('data/names.pkl', 'rb') as w:
    LABELS = pickle.load(w)
with open('data/faces_data.pkl', 'rb') as f:
    FACES = pickle.load(f)

print('Shape of Faces matrix --> ', FACES.shape)

knn = KNeighborsClassifier(n_neighbors=5)
knn.fit(FACES, LABELS)

COL_NAMES = ['NAME', 'TIME']

while True:
    ret, frame = video.read()

    # Check if the frame is correctly captured
    if not ret:
        print("Failed to grab frame, trying again...")
        time.sleep(0.1)  # Add a small delay before retrying
        continue

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = facedetect.detectMultiScale(gray, 1.3, 5)
    
    for (x, y, w, h) in faces:
        crop_img = frame[y:y+h, x:x+w, :]
        resized_img = cv2.resize(crop_img, (50, 50)).flatten().reshape(1, -1)
        output = knn.predict(resized_img)
        ts = time.time()
        date = datetime.fromtimestamp(ts).strftime("%d-%m-%Y")
        timestamp = datetime.fromtimestamp(ts).strftime("%H:%M:%S")
        exist = os.path.isfile("Attendance/Attendance_" + date + ".csv")
        
        # Load the attendance file if it exists to check if the person is already added
        if exist:
            with open("Attendance/Attendance_" + date + ".csv", 'r') as csvfile:
                reader = csv.reader(csvfile)
                attendance_data = list(reader)
            
            # Check if the person (identified by name) is already in the attendance file
            person_already_in_attendance = False
            for row in attendance_data:
                if row[0] == str(output[0]):  # Check if name already exists
                    person_already_in_attendance = True
                    break
        else:
            person_already_in_attendance = False
        
        # If the person is not in the attendance list, add them
        if not person_already_in_attendance:
            cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 0, 255), 1)
            cv2.rectangle(frame, (x, y), (x+w, y+h), (50, 50, 255), 2)
            cv2.rectangle(frame, (x, y-40), (x+w, y), (50, 50, 255), -1)
            cv2.putText(frame, str(output[0]), (x, y-15), cv2.FONT_HERSHEY_COMPLEX, 1, (255, 255, 255), 1)
            
            attendance = [str(output[0]), str(timestamp)]
            
            # Save the attendance in the CSV file
            if exist:
                with open("Attendance/Attendance_" + date + ".csv", "+a") as csvfile:
                    writer = csv.writer(csvfile)
                    writer.writerow(attendance)
            else:
                with open("Attendance/Attendance_" + date + ".csv", "+a") as csvfile:
                    writer = csv.writer(csvfile)
                    writer.writerow(COL_NAMES)
                    writer.writerow(attendance)

            speak(f"Attendance taken for {output[0]}")

    # Display the frame directly
    cv2.imshow("Frame", frame)
    
    k = cv2.waitKey(1)
    
    if k == ord('q'):
        break

video.release()
cv2.destroyAllWindows()
