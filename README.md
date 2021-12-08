# Messenger Application ✉️ 
# (A simple Qt TCPIP Messenger application)

![N|Solid](https://www.dialotel.fr/medias/images/icon-chat.png)





# Getting Started : 
## Log in:
🖊User need to log in by filling login and password fields 
⚙️This will send a JSON weft (see : login procedure )
📌Note that logins and passwords are created upstream (=a client can not create an account ) 
- You can check existing credentials in the file : credentials.csv 
    -   Try out with  : login1 , mdp1
## Send messages:
Once logged in the user will have access to a new window(the loggin window will disapear after a sucessfull conncetion)
📌To start a conversation =  send a message the user has to click on "Add Contact"
🖊Here the user will have to enter one or severals users separed with a coma
After validation , the selected user(s) will be diplayed into the "Contacts" with a doubleclick on the contact 
⚙️This will send a JSON weft (see : get messages procedure )

Now select the wanted Contact to load the conversation 
🖊Now you can write a message to the conversation and send it by clicking on "Send" button
⚙️This will send a JSON weft (see : send messages procedure )

## JSON weft

- LOGIN PROCEDURE (sent from the client) :
{"action":"login","login":"monlogin","password":"monpassword"} 
    -  RESPONSE TO LOGIN PROCEDURE (sent from the server) :
            - {"action":"answerlogin","state":"ok"} (if valid )
            -  {"action":"answerlogin","state":"notok"} (if not valid )
            -  
- SEND MESSAGE  PROCEDURE (sent from the client) :
{"action":"send","to":["login1","login2"....],"content":"message"} 

    - RESPONSE TO MESSAGE PROCEDURE  (sent from the server) :
    {"action":"receive","from":"login1","content":"message","datetime":"12/11/2021 
    14 :56" }

- ➕ GET MESSAGES  PROCEDURE (sent from the client) :
{"action":"get_messages","contact":["login1","login2"....] }
🔴Please note that this JSON was not asked by the client , it is a feature from my self . It able the the app to store all messages even if they are meant for a not existing user . 

## Server functionment
When a message is sent ,it is stored into a csv file(messages_1.csv) which is available only from the server . 
The application will save the message whoever is meant to receive the message. 
### Example :
This means if UserA add UserB to his contacts but does not exists yet ,UserA will not be awared of it .But UserA will be able to see the conversation between UserB and him . 

#### Security : 
This is a first version of the app , security and optimisation are not handled properly , password are encrypt with SHA3-512 procedure
