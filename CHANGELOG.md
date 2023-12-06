# CHANGELOG

Add a brief description of your contributions to the top of this doc to let everyone know what you've been up to!

___
### 11/29 - Charlie

Improved server communication

Create graphs based on ping and IR data

Allow manual calibration on the fly
___
### 11/28 - Charlie

Modified manual control functions to be more compatible with a python server client. **We no longer require any use of PuTTY to communicate with the CyBot**.

Currently the CyBot can be driven (forward, backward, turn left, turn right) to a specified distance/degree. Much more conventient IMHO. Should be able to add scan logic to this easily. Need to determine more specific behavior we want before I can really proceed.

I built this planning to replace all of the keyboard interaction with a simple GUI. We should discuss this further when we meet next!

Python client is in `project/client/mission_control.py`. Run it with `$ python mission_control.py` ($ is a symbol representing the command prompt, don't actually type it. You will have to be in the client directory to run it). Use the python client to interact with Cybot server (`main.c`)

#### I left lots of TODOs where there are things that can be done NOW to improve this.
___