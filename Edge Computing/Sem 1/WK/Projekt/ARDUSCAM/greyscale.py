import serial
import numpy as np
from PIL import Image

def receive_image(serial_port, file_name):
    ser = serial.Serial(serial_port, 921600, timeout=20)
    ser.write(bytes([49]))
    image_length_bytes = ser.readline()
    image_length = int(image_length_bytes)
    print("Image Length:", image_length)
    
    # Initialize a buffer to store the received data
    image_data = bytearray()
    
    # Start reading image data
    while image_length > 0:
        data = ser.read(min(1024, image_length))
        image_length -= len(data)
        print(image_length)
        image_data.extend(data)
    
    # Convert the received data to a numpy array
    image_array = np.frombuffer(image_data, dtype=np.uint8)
    
    # Reshape the array to the shape of the image
    image_array = image_array.reshape((96, 96))

    r = image_array
    b = image_array
    g = image_array
    
    # Combine channels and save as an image
    rgb_image = np.dstack((r, g, b)).astype(np.uint8)
    pil_image = Image.fromarray(rgb_image)
    pil_image.save(file_name)

    print(ser.readline())
    print(ser.readline())

    ser.close()
    print("Image received successfully.")

if __name__ == "__main__":
    serial_port = "COM4"  # Change this to your COM port
    file_name = "received_image.bmp"
    receive_image(serial_port, file_name)
