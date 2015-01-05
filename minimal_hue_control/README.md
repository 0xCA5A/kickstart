HUE!
====
 * http://www.developers.meethue.com/documentation/core-concepts

Hue web addresses
-----------------
A hue resource web address will always start with the following.
http://\<bridge IP address\>/api

The hue username
----------------
In most of the commands (the exception is creating a username) you’ll also include a username after this:

http://\<bridge IP address\>/api/<username>

First, authorize user. 
Each new instance of an app should use a unique username which you generate using the Create New User command.

Change configuration
--------------------
http://\<bridge ip address\>/api/newdeveloper/config

First steps
-----------
 * http://www.developers.meethue.com/documentation/getting-started
 * http://\<bridge ip address\>/debug/clip.html

4 methods

    GET: this is the command to fetch all information about the addressed resource
    PUT: this is the command to modify an addressed resource
    POST: this is the command to create a new resource inside the addressed resource
    DELETE: this is the command to deleted the addressed resource

init()
------
Reset the bridge, press the button on the back of the bridge.
Per default, the bridge tries to get an address via DHCP. 
If that does not work autoip is used. 

Find the bridge and connect to the debug interface:
http://192.168.1.100/debug/clip.html

Register a new user / application:

    POST
    /api/
    {"devicetype":"power user","username":"neratecdev"}

You have to press the button on the bridge before you post your request.

Get the configuration, there you can see the whitelisted users / applications, API version etc. 

    GET 
    /api/neratecdev/config

change network configuration
    
    PUT
    /api/neratecdev/config
    {"dhcp": false, "ipaddress": "192.168.1.123", "netmask" : "255.255.255.0"}

reconnect and try of find your lights:

    GET
    /api/neratecdev/lights
 
if no lights are registered, try to find your lights

    POST
    /api/neratecdev/lights

this request should respond something like this:
"Searching for new devices"

wait for some seconds and try to list the lights again:

    GET
    /api/neratecdev/lights

set light name:

    PUT
    /api/neratecdev/lights/1
    {"name":"bbs1"}

now you should be ready to communicate with the HUE bridge using the minimal hue control!

