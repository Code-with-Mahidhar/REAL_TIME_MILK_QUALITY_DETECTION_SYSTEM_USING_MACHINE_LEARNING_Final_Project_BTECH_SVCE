import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
import serial
import time
import re
import warnings

warnings.filterwarnings("ignore")

ser = serial.Serial('COM3', baudrate=9600)  # Adjust COM port and baudrate as needed
print("Serial connection opened successfully!")

# Load data from an Excel sheet and split into features and labels
data = pd.read_excel("milk.xlsx", engine="openpyxl")

feature_1 = data['gasvalue']
feature_2 = data['ldr_value']
feature_3 = data['temperature']
feature_4 = data['ph']



label_1 = data['label_gasvalue']
label_2 = data['label_Ldrvalue']
label_3 = data['label_temp']
label_4 = data['label_ph']




# Split the data into training and testing sets for each parameter
X_train_1, X_test_1, y_train_1, y_test_1 = train_test_split(feature_1, label_1, test_size=0.2, random_state=42)
X_train_2, X_test_2, y_train_2, y_test_2 = train_test_split(feature_2, label_2, test_size=0.2, random_state=42)
X_train_3, X_test_3, y_train_3, y_test_3 = train_test_split(feature_3, label_3, test_size=0.2, random_state=42)
X_train_4, X_test_4, y_train_4, y_test_4 = train_test_split(feature_4, label_4, test_size=0.2, random_state=42)



# Build Random Forest models for each parameter
rf_model_1 = RandomForestClassifier(random_state=42)
rf_model_1.fit(X_train_1.values.reshape(-1, 1), y_train_1)

rf_model_2 = RandomForestClassifier(random_state=42)
rf_model_2.fit(X_train_2.values.reshape(-1, 1), y_train_2)

rf_model_3 = RandomForestClassifier(random_state=42)    
rf_model_3.fit(X_train_3.values.reshape(-1, 1), y_train_3)

rf_model_4 = RandomForestClassifier(random_state=42)
rf_model_4.fit(X_train_4.values.reshape(-1, 1), y_train_4)


def readData():
    time.sleep(1)
    serial_data = ser.readline().decode().strip()
    
    while not serial_data.startswith('a'):
        serial_data = ser.readline().decode().strip()

    time.sleep(1)
    print("\n----------------------------")
    print("     -= Data Received =- ")
    print("----------------------------\n")
    #time.sleep(1)
    print("Data:", serial_data, "\n")
    
    a = serial_data.find("a")
    b = serial_data.find("b")
    a = a + 1
    val_1 = float(serial_data[a:b])
    print("gasvalue   :", val_1)
    #time.sleep(1)

    b = serial_data.find("b")
    c = serial_data.find("c")
    b = b + 1
    val_2 = float(serial_data[b:c])
    print("ldrvalue    :", val_2)
    #time.sleep(1)

    c = serial_data.find("c")
    d = serial_data.find("d")
    c = c + 1
    val_3 = float(serial_data[c:d])
    print("temperature :", val_3)
    #time.sleep(1)

    d = serial_data.find("d")
    e = serial_data.find("e")
    d = d + 1
    val_4 = float(serial_data[d:e])
    print("ph   :", val_4)
    #time.sleep(1)

   
    return val_1, val_2, val_3, val_4


while True:

    serial_data = ser.readline().decode().strip()
    input_data = readData()
    if input_data is None:
        continue
    feature_1_val, feature_2_val, feature_3_val, feature_4_val= input_data
    

   # Make predictions using the trained Random Forest models
    rf_prediction_1 = rf_model_1.predict([[feature_1_val]])[0]
    rf_prediction_2 = rf_model_2.predict([[feature_2_val]])[0]
    rf_prediction_3 = rf_model_3.predict([[feature_3_val]])[0]
    rf_prediction_4 = rf_model_4.predict([[feature_4_val]])[0]
    
    
   

    print("\n----------------------------")
    print("RF-prediction")
    print("----------------------------\n")

    time.sleep(1)
    print(f'gasvalue          : {rf_prediction_1}')
    print(f'Ldrvalue      : {rf_prediction_2}')
    print(f'temperature  : {rf_prediction_3}')
    print(f'ph      : {rf_prediction_4}')

    

    print("\n----------------------------")
    values_string = f"t{rf_prediction_1}u{rf_prediction_2}v{rf_prediction_3}w{rf_prediction_4}x"
    time.sleep(1)
    print(values_string)
    time.sleep(2)
    ser.write(bytes(values_string, 'utf-8'))
    time.sleep(3)
    print("completed")
   

    time.sleep(1)
    
