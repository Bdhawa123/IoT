import serial
import MySQLdb

# This code is ran every time we refresh the page.
# It reads exactly 1 line of serial data from the arduino
# and adds it to the localhost db

def insertTemp():
    device = "/dev/ttyACM0"                                                                                         # select the port that our arduino is plugged into
    arduino = serial.Serial(device, 9600)                                                                           # setup serial communication, first parameter is our device, second is the baud rate    

    data = arduino.readline()                                                                                       # read a line of data from the arduino    
    print(data)                                                                                                     # print it to the terminal so we can debug if necessary    

    dbConn = MySQLdb.connect("localhost", "iot", "iot", "tempdb") or die("Could not connect to the database")       # open database connection    

    print(dbConn)                                                                                                   # not necessary, but just prints the db connection info    

    with dbConn:                                                                                                    # only runs if we have a valid connection (i.e. "with" a dbConn)    
        cursor = dbConn.cursor()                                                                                    # this variable is bound to our dbConn and allows us to execute SQL commands on the database    
        cursor.execute("INSERT INTO tempLog (Temperature) VALUES (%s)" % (data))                                    # insert statement to add the temperature to the db    
        dbConn.commit()                                                                                             # commits the transaction to ensure it saves
        cursor.close()                                                                                              # close the connection
