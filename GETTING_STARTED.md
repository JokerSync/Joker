# Getting started

Demo files are available here: [http://www.phonations.com/tmwtga]()

It contains:

- A *DetX* file (written with [Cappella](http://www.cappella.tv))
- A video file
- A wav file (not needed)

In order to read a project with *Joker*, perform the following steps:

- Click *File > Open...* and select the *DetX* file.
- Click *File > Open a video file...* and select the video file.
- If it is asked a timecode, leave the value by default (*01:00:00:00*).
- Press the space bar to start the rythmo/video playback.

![Screenshot](data/img/screenshot.jpg)

## Midi synchronization

### System setup

In order to communicate with *MIDI*, you need to make sure port are set up and ready from the *Audio and MIDI configuration* (*Application* > *Utility*):

#### Midi device

Using a midi device like [iConnectMIDI2+](http://www.iconnectivity.com/products/midi/iConnectMIDI2plus) is the best way to use *Joker*. If the device is connected, it should show up in the available midi devices.

#### Network port

If you don't have midi device, you can emulate one with a network connection. To do so, open the *Network* icon from the available midi devices on the both computer (the one running your DAW and the one running *Joker*):

- Create a session
- Make sure *Everybody* can connect to the computer
- If both computers are connected on the same network, you should be able to connect them.

You should opt for ethernet connection because wifi is not so stable.

#### Local port

If you want to use *Joker* and your recording software on the same machine (not recommanded), you can use a local port by activating the *IAC Manager*.

### Joker setup

Go to the *Joker* preferences (menu *Joker* > *Preferences...*) in the *Synchronization* tab. Here select *Midi timecode* and *Read from existing port*, choosing the port you set up during *system setup*.

### Logic

In order to send MTC with *Logic*, go to *File* > *Project settings* > *Synchronization...* and click the *MIDI* tab. Check the *Transmit MTC* option and select the port you set up during *system setup*.
