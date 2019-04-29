import serial
import MySQLdb
from time import sleep
from flask import Flask, url_for, send_from_directory, render_template, request

# this is needed so we can run our "insertIntoDb" function
import insertIntoDb

app = Flask(__name__)

def getTemp():
    dbConn = MySQLdb.connect("localhost","iot","iot","tempdb") or die("Could not connect to the database")      # open a connection to the database

    with dbConn:                                                                                                # only runs if we have a valid connection (i.e. "with" a dbConn)
        cursor = dbConn.cursor()                                                                                # this variable is bound to our dbConn and allows us to execute SQL commands on the database 
        cursor.execute("SELECT Temperature FROM tempLog ORDER BY TempId DESC LIMIT 1")                          # select statement to select the latest entry to the db
        lTemp = cursor.fetchall()                                                                               # fetch all of the returned rows (should only be 1) from the above statement
        cursor.close()                                                                                          # close the connection                        
        return lTemp                                                                                            # return the temperature value


@app.route('/')                                                 # basically acts as a URL identifier. Our index page runs the following code. If we changed it to @app.route('/About') our URL would be OurWebPage.com/About
def index():
    insertIntoDb.insertTemp();                                  # run the python script to add a temperature value to the database. Happens each time the page is refreshed
    
    temp = getTemp()                                            # temporarily store the latest temperature value. Uses the above "getTemp()" method
    data = {'temp': temp}                                       # create an array of data to pass into our HTML/JS
    
    return render_template('index.html', data=data)             # this just means to display our index HTML page, and passes the array of data into it


if __name__ == "__main__":
	app.run()
