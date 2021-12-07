# Messenger Application 


![N|Solid](https://www.dialotel.fr/medias/images/icon-chat.png)


A simple Qt Messenger app



## JSON Trames

- LOGIN PROCEDURE (sent from the client) :
{"action":"login","login":"monlogin","password":"monpassword"} 
    -  RESPONSE TO LOGIN PROCEDURE (sent from the server) :
            - {"action":"answerlogin","state":"ok"} (if valid )
            -  {"action":"answerlogin","state":"notok"} (if not valid )
            -  
- SEND MESSAGE  PROCEDURE (sent from the client) :
{"action":"send","to":["login1","login2"....],"content":"message"} 

- RECEIVE MESSAGE  PROCEDURE (sent from the server) :
{"action":"receive","from":"login1","content":"message","datetime":"12/11/2021 
14 :56" }

## Our client interface : 
 #IMAGES#

## Server functionment
When a message is sent ,it is stored into a csv file which is available only from the server . 
The application will save the message whoever is meant to receive the message. 

