import cv2
import mediapipe as mp
import requests
import time

ESP32_IP = "http://192.168.4.1"  # Replace with your ESP32 AP IP

# MediaPipe setup
mpHands = mp.solutions.hands
hands = mpHands.Hands(max_num_hands=1, min_detection_confidence=0.7)
mpDraw = mp.solutions.drawing_utils

# Webcam
cap = cv2.VideoCapture(0)

prev_command = ""
last_sent = 0

def send_command(command):
    global prev_command, last_sent
    if command != prev_command or time.time() - last_sent > 2:
        try:
            print("Sending:", command)
            requests.get(f"{ESP32_IP}/{command}")
            prev_command = command
            last_sent = time.time()
        except:
            print("Failed to send command.")

def fingers_up(lmList):
    fingers = []
    # Thumb (x comparison because it's horizontal)
    fingers.append(lmList[4][0] > lmList[3][0])

    # Other fingers (y comparison, tip higher than pip joint)
    tips = [8, 12, 16, 20]
    for tip in tips:
        fingers.append(lmList[tip][1] < lmList[tip - 2][1])

    return fingers

while True:
    success, img = cap.read()
    img = cv2.flip(img, 1)  # Flip horizontally for mirror feel
    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    result = hands.process(imgRGB)

    command = "stop"

    if result.multi_hand_landmarks:
        for handLms in result.multi_hand_landmarks:
            lmList = []
            for id, lm in enumerate(handLms.landmark):
                h, w, c = img.shape
                cx, cy = int(lm.x * w), int(lm.y * h)
                lmList.append((cx, cy))

            if lmList:
                finger_state = fingers_up(lmList)

                if finger_state == [False, False, False, False, False]:  # Fist
                    command = "stop"
                if finger_state == [True, True, True, True, True, True]:
                    command = "stop"
                elif finger_state == [False, True, False, False, False]:  # One finger
                    command = "forward"
                elif finger_state == [False, False, True, False, False]:  # Two fingers
                    command = "backward"
                elif finger_state == [False, True, False, False, True]:  # Pinky + Index
                    command = "left"
                elif finger_state == [False, True, True, False, False]:  # Thumb up
                    command = "right"

                mpDraw.draw_landmarks(img, handLms, mpHands.HAND_CONNECTIONS)

    send_command(command)

    cv2.imshow("Gesture Control", img)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
