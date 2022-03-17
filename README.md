# Project Friday - A Privacy Shield for Speech Assistants

Please notice, that this project ist still a prototype under development!

# What is Project Friday?

To help understand the intention of this project we suggest to watch the short commercial video we created:

Commercial video (No english version yet)\
(German Version: [Werbevideo](https://youtu.be/KQXyvrAh7YI))

The goal of Project Friday is to secure private conversations while using a speech assistant (Alexa Echo Dot). It is basically a Privacy Shield for Speech Assistants as it is probably adjustable for other products like the Google Nest as well. By now this documentation only covers the use of an EchoDot, but in the future there will also be branches for other Speech Assistants.
With this Privacy Shield the EchoDot remains usable as usual, with the difference that you can be sure that your private conversations remain really private.
Also you can use another word like "Friday" or "Jarvis" instead of "Alexa" to activate it, as if you were Iron Man.

The project was started as semester project at [UNIVERSITY OF APPLIED SCIENCES UPPER AUSTRIA, Campus Hagenberg](https://www.fh-ooe.at/campus-hagenberg/) by the students Philipp Ensinger and Frank Weber. As the prototype was pretty successful we decided to continue developing here on GitHub to improve the functionality and add new features. The Wakeword Engine is based on the git project [DIY-Alexa by Atomic14](https://github.com/atomic14/diy-alexa).

On this GitHub Page we want to provide a DIY guide for building this Privacy Shield. We want to make short videos and a lot of photos of every single step so you can easily comprehend how it is done. This comprehensive guide will be available as soon as it's not a prototype anymore and sufficiently tested, which will be at the latest in summer 2022. Please understand that we do not want to publish the (probably faulty) guide until we have made sure that everything is correct and complete.
Have fun securing your privacy and feel free to report bugs and suggestions.

# How does the Privacy Shield work? - An overview

We recommend to watch theses short videos to get an overview of how the Privacy Shield works:

Functionality overview (No english version yet)\
Explanation of basic schematics (No english version yet)

German Versions:\
[Video zur Funktionsweise](https://youtu.be/iLtr8yEG1Ug)\
[Detailvideo zur schematischen Darstellung](https://youtu.be/DTw5HjAbXnE)

---

If you watched the videos you can skip the following part, as it is just the transcription of the video.

The central unit of the Privacy Shield is an ESP32 microcontroller.
An I²S microphone is connected to this, which is used to detect the wake word. The neural network in the background is trained using the Tensorflow-Lite platform.
To prevent eavesdropping via the EchoDot, its microphones are removed. This is where the privacy shield comes into play. The connectors of the removed microphones are connected to the ESP. Thus the EchoDot is only able to hear something if the ESP passes the audio signal of the microphone to it. 
In addition, the EchoDot offers the possibility to activate it via a button without saying Alexa. The Privacy Shield takes advantage of this by connecting the button's contacts to the ESP. Likewise, the ESP passively reads the I²C communication on the bus of the EchoDot. This is the connection between the processor and the LED controller. This allows the privacy shield to determine what state the EchoDot is in.
In the normal state the EchoDot cannot hear anything and the Privacy-Shield waits for the wakeword. If this is detected, the Privacy-Shield wakes up the EchoDot via the activation button and releases the microphone. At the same time, the Privacy-Shield checks the status of the EchoDot via the mentioned I²C communication to detect when it has processed the request. After that the microphone is blocked again for the EchoDot and the Privacy-Shield waits again for the WakeWord.
If the EchoDot is compromised by an attacker who gains access to the microphones, he will still not be able to listen to any conversations, but only to the few seconds during which a command is given.
The Privacy Shield can be built into any enclosure. In the following example, an IronMan helmet is used. To make it even more visible and to provide the user with visual feedback, the eyes of the Iron Man helmet are equipped with LEDs. Since the Privacy Shield already reads the LED communication on the I2C bus of the EchoDot, this is used to reflect the status of the device on the eyes.
