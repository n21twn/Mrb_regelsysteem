# main.py
from time import time
import cv2
import numpy as np
import serial
import time
# --- Configuratie voor Hough Cirkel Transformatie ---
# Deze parameters zijn cruciaal en moet je waarschijnlijk aanpassen
# voor jouw specifieke situatie en de afstand van de bal tot de camera.

# dp: Inverse ratio van de accumulator resolutie. 1 is dezelfde resolutie, 2 is de helft.
# Meestal is 1.2 een goede start.
DP = 1.2

# minDist: Minimale afstand tussen de middelpunten van gedetecteerde cirkels.
# Als dit te klein is, krijg je meerdere cirkels voor dezelfde bal.
# Als het te groot is, mis je ballen die dicht bij elkaar zijn.
MIN_DIST = 100

# param1: Hogere drempelwaarde voor de interne Canny edge detector.
PARAM1 = 100

# param2: Drempelwaarde voor de accumulator. Hoe kleiner, hoe meer (ook valse) cirkels
# worden gedetecteerd. Dit is een belangrijke waarde om mee te experimenteren.
PARAM2 = 25 # Probeer deze waarde te verhogen of verlagen.

# minRadius / maxRadius: De minimale en maximale straal (in pixels) van de cirkel
# die je wilt detecteren. Dit helpt om andere ronde objecten uit te sluiten.
MIN_RADIUS = 10
MAX_RADIUS = 30


# --- Detectie Logica ---

def detect_ball(frame):
    """
    Analyseert een enkel frame om de pingpongbal te detecteren met HoughCirlces.

    Args:
        frame: Het videoframe (als NumPy-array) om te verwerken.

    Returns:
        Een tuple met:
        - Het frame met de visuele detectie erop getekend.
        - De centrumcoördinaten (x, y) van de gedetecteerde bal, of None.
    """
    center = None

    # 1. Converteer het frame naar grijswaarden. HoughCircles werkt op
    #    afbeeldingen met één kanaal.
    gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # 2. Maak het frame wazig (blur) om ruis te verminderen. Dit is belangrijk
    #    voor een stabiele detectie.
    blurred_frame = cv2.medianBlur(gray_frame, 5)

    # 3. Detecteer cirkels met de Hough Cirkel Transformatie.
    #    Dit is de kern van de detectie.
    circles = cv2.HoughCircles(
        blurred_frame,
        cv2.HOUGH_GRADIENT,
        dp=DP,
        minDist=MIN_DIST,
        param1=PARAM1,
        param2=PARAM2,
        minRadius=MIN_RADIUS,
        maxRadius=MAX_RADIUS
    )

    # Controleer of er cirkels zijn gevonden
    if circles is not None:
        # Converteer de coördinaten en straal naar integers
        circles = np.uint16(np.around(circles))

        # We nemen de eerste gedetecteerde cirkel.
        # In een complexere situatie zou je hier de "beste" cirkel kunnen kiezen.
        c = circles[0, 0]
        x, y, radius = c[0], c[1], c[2]
        center = (x, y)

        # Teken de omtrek van de cirkel op het originele frame
        cv2.circle(frame, (x, y), radius, (0, 255, 0), 2)
        # Teken een klein stipje in het midden van de cirkel
        cv2.circle(frame, (x, y), 5, (0, 0, 255), -1)

    return frame, center

# --- Hoofdprogramma ---

if __name__ == "__main__":
    arduino = serial.Serial('COM3', 9600, timeout=1)
    time.sleep(2)

    # Start de video capture. '0' is meestal de standaard webcam.
    camera = cv2.VideoCapture(1)
    
    if not camera.isOpened():
        print("Fout: Kon de videobron niet openen.")
        exit()

    while True:
        # Lees een nieuw frame van de camera
        (grabbed, frame) = camera.read()

        if not grabbed:
            break

        # Draai de detectiefunctie
        processed_frame, ball_center = detect_ball(frame.copy())
        
        if ball_center:
            ball_x, ball_y = ball_center
            print(f"Bal gedetecteerd op: X={ball_x}, Y-Hoogte={ball_y}")
            arduino.write(f"{ball_y}\n".encode()) 
        # Toon het verwerkte frame
        cv2.imshow("Ping-Pong Detectie (Hough)", processed_frame)

        # Sta de gebruiker toe om te stoppen door op 'q' te drukken
        key = cv2.waitKey(1) & 0xFF
        if key == ord("q"):
            break

    # Geef de camera vrij en sluit alle vensters
    camera.release()
    cv2.destroyAllWindows()
    arduino.close()
