
[Source](http://www.belle-nuit.com/archives/9pin.html "Permalink to Sony 9-Pin Remote Protocol")

# Sony 9-Pin Remote Protocol

*This is not the offical Sony 9-pin protocol. It is a summary for reference puroposes only. To obtain the protocol document, contact Sony directly.*

*Updated 6/16/96. Fixed the Prog Speed formula. Added Tables. Added all group 20 descriptions, more info on status bits, and most of the group 40 commands.*

## Communication Format

The protocol is based on the EIA RS-422-A signal standard, usually at 38.4 kBit/s. The data are sent as 1 start bit + 8 data bits + 1 parity bit + 1 stop bit. Parity is odd: the bitwise sum of data bits 0 -7 and the parity bit is an odd number.

## Command Block Format

The controlling device and the controlled device communicate through the interchange of command blocks. The bytes in each command block are assigned as follows:

- **CMD-1/DATA COUNT**. **CMD-1** is the upper 4 bits, **DATA COUNT** is the lower 4.
- **CMD-2**.
- **DATA-1** up to **DATA-N**, where n is the value in data count
- **CHECKSUM**

### CMD-1
Indicates the function and direction of the command, according to:

| Value | Function
|-------|---------
| 0     | System control (Master-&gt;Slave)
| 1     | Return for 0,2, or 4 of cmd-1 (Slave-&gt;Master)
| 2     | Transport Control (Master-&gt;Slave)
| 4     | Preset/Select control (Master-&gt;Slave)
| 6     | Sense Request (Master-&gt;Slave)
| 7     | Sense Return (Slave-&gt;Master)

### DATA COUNT
Indicates the number of bytes ( max 15 ) inserted between **CMD-2** and **CHECKSUM**
### CMD-2
Designates the command. Refer to the command table for definitions. Ex. **CMD-1**=0 and **CMD-2**=0C means LOCAL DISABLE.
### DATA-1 to DATA-N
Data which correspond to those indicated by the command. Refer to the command table for data formats.
### CHECKSUM
Lower eight bits of the sum of the bytes in the command block.

## Communication Protocol

The protocol is initiated by the master. The slave should return a response within 9 msec. The response may be:

- NAK + Error Data: Undefined command or communication error
- COMMAND + Data: if Command requested data
- ACK: if Command did not request data

The master should not send another command until receiving a response from the slave device. The master must also insure that no more than 10 msec lapses between bytes in a command block. The master must immediatly stop sending data when it receives a NAK + Error Data message. If the Error Data contains "Undefined Command" the master may immediatley send another command, otherwise it must wait at least 10 msec before sending another command. When the master does not receive a response from the slave within the 10 msec timeout, it may assume that communications have ceased and take appropriate measures.

## Cabling

The pin assignments for the 9-pin cable are as follows:

| Pin | Master      | Slave
|-----|-------------|------
| 1   | Ground      | Ground
| 2   | Rcv A       | Xmit A
| 3   | Xmit B      | Rcv B
| 4   | Xmit Common | Rcv Common
| 5   | Spare       | Spare
| 6   | Rcv Common  | Xmit Common
| 7   | Rcv B       | Xmit B
| 8   | Xmit A      | Rcv A
| 9   | Ground      | Ground

*Auth - this varies alot: contact individual manufacturers for pinouts.*

## Command Table

This is the command table for the DVR-2000/2100. It is summarised here for reference purposes only. If you want to order it from Sony call the Kansas City supply center and hope you get someone who knows the part number for the recorder whose protocol you want to use.

|       | Command                                           |       | Response
|-------|---------------------------------------------------|-------|---------
| 00 0C | [Local Disable](#localDisable)                    | 10 01 | Ack
| 00 11 | [Device Type Request](#deviceTypeRequest)         | 12 11 | [Device Type](#deviceType)
| 00 1D | [Local Enable](#localEnable)                      | 10 01 | Ack
| 20 00 | [Stop](#stop)                                     | 10 01 | Ack
| 20 01 | [Play](#play)                                     | 10 01 | Ack
| 20 02 | [Record](#record)                                 | 10 01 | Ack
| 20 04 | [Standby Off](#standbyOff)                        | 10 01 | Ack
| 20 05 | [Standby On](#standbyOn)                          | 10 01 | Ack
| 20 0F | [Eject](#eject)                                   | 10 01 | Ack
| 20 10 | [Fast Fwd](#fastFwd)                              | 10 01 | Ack
| 2X 11 | [Jog Fwd](#jogFwd)                                | 10 01 | Ack
| 2X 12 | [Var Fwd](#varFwd)                                | 10 01 | Ack
| 2X 13 | [Shuttle Fwd](#shuttleFwd)                        | 10 01 | Ack
| 20 20 | [Rewind](#rewind)                                 | 10 01 | Ack
| 2X 21 | [Jog Rev](#jogRev)                                | 10 01 | Ack
| 2X 22 | [Var Rev](#varRev)                                | 10 01 | Ack
| 2X 23 | [Shuttle Rev](#shuttleRev)                        | 10 01 | Ack
| 20 30 | [Preroll](#preroll)                               | 10 01 | Ack
| 24 31 | [Cue up with Data](#cue)                          | 10 01 | Ack
| 20 34 | [Sync Play](#syncPlay)                            | 10 01 | Ack
| 21 38 | [Prog Speed Play +](#progSpeedPlay+)              | 10 01 | Ack
| 21 39 | [Prog Speed Play -](#progSpeedPlay-)              | 10 01 | Ack
| 20 40 | [Preview](#preview)                               | 10 01 | Ack
| 20 41 | [Review](#review)                                 | 10 01 | Ack
| 20 42 | [Auto Edit](#autoEdit)                            | 10 01 | Ack
| 20 43 | [Outpoint Preview](#outpointPreview)              | 10 01 | Ack
| 2X 54 | [Anti-Clog Timer Disable](#antiClogTimerDisable)  | 10 01 | Ack
| 2X 55 | [Anti-Clog Timer Enable](#antiClogTimerEnable)    | 10 01 | Ack
| 20 60 | [Full EE Off](#fullEEOff)                         | 10 01 | Ack
| 20 61 | [Full EE On](#fullEEOn)                           | 10 01 | Ack
| 20 63 | [Select EE On](#selectEEOn)                       | 10 01 | Ack
| 20 64 | [Edit Off](#editOff)                              | 10 01 | Ack
| 20 65 | [Edit On](#editOn)                                | 10 01 | Ack
| 20 6A | [Freeze Off](#freezeOff)                          | 10 01 | Ack
| 20 6B | [Freeze On](#freezeOn)                            | 10 01 | Ack
| 44 00 | [Timer-1 Preset](#timer1Preset)                   | 10 01 | Ack
| 44 04 | [Time Code Preset](#timeCodePreset)               | 10 01 | Ack
| 44 05 | [User Bit Preset](#userBitPreset)                 | 10 01 | Ack
| 40 08 | [Timer-1 Reset](#timer1Reset)                     | 10 01 | Ack
| 40 10 | [In Entry](#inEntry)                              | 10 01 | Ack
| 40 11 | [Out Entry](#outEntry)                            | 10 01 | Ack
| 40 12 | [Audio In Entry](#audioInEntry)                   | 10 01 | Ack
| 40 13 | [Audio Out Entry](#audioOutEntry)                 | 10 01 | Ack
| 44 14 | [In Data Preset](#inDataPreset)                   | 10 01 | Ack
| 44 15 | [Out Data Preset](#outDataPreset)                 | 10 01 | Ack
| 44 16 | [Audio In Data Preset](#audioInDataPreset)        | 10 01 | Ack
| 44 17 | [Audio Out Data Preset](#audioOutDataPreset)      | 10 01 | Ack
| 40 18 | [In + Shift](#inPShift)                           | 10 01 | Ack
| 40 19 | [In - Shift](#inMShift)                           | 10 01 | Ack
| 40 1A | [Out + Shift](#outPShift)                         | 10 01 | Ack
| 40 1B | [Out - Shift](#outMShift)                         | 10 01 | Ack
| 40 1C | [Audio In + Shift](#audioInPShift)                | 10 01 | Ack
| 40 1D | [Audio In - Shift](#audioInMShift)                | 10 01 | Ack
| 40 1E | [Audio Out + Shift](#audioOutPShift)              | 10 01 | Ack
| 40 1F | [Audio Out - Shift](#audioOutMShift)              | 10 01 | Ack
| 40 20 | [In Flag Reset](#inFlagReset)                     | 10 01 | Ack
| 40 21 | [Out Flag Reset](#outFlagReset)                   | 10 01 | Ack
| 40 22 | [Audio In Flag Reset](#audioInFlagReset)          | 10 01 | Ack
| 40 23 | [Audio Out Flag Reset](#audioOutFlagReset)        | 10 01 | Ack
| 40 24 | [In Recall](#inRecall)                            | 10 01 | Ack
| 40 25 | [Out Recall](#outRecall)                          | 10 01 | Ack
| 40 26 | [Audio In Recall](#audioInRecall)                 | 10 01 | Ack
| 40 27 | [Audio Out Recall](#audioOutRecall)               | 10 01 | Ack
| 40 2D | [Lost Lock Reset](#lostLockReset)                 | 10 01 | Ack
| 4X 30 | [Edit Preset](#editPreset)                        | 10 01 | Ack
| 44 31 | [Preroll time preset](#prerollTimePreset)         | 10 01 | Ack
| 41 32 | [Tape/Auto Select](#tapeAutoSelect)               | 10 01 | Ack
| 41 33 | [Servo Ref Select](#servoRefSelect)               | 10 01 | Ack
| 41 34 | [Head Select](#headSelect)                        | 10 01 | Ack
| 41 35 | [Color Frame select](#colorFrameSelect)           | 10 01 | Ack
| 41 36 | [Timer Mode Select](#timerModeSelect)             | 10 01 | Ack
| 41 37 | [Input Check](#inputCheck)                        | 10 01 | Ack
| 41 3A | [Edit Field Select](#editFieldSelect)             | 10 01 | Ack
| 41 3B | [Freeze Mode Select](#freezeModeSelect)           | 10 01 | Ack
| 4X 3E | [Record Inhibit](#recordInhibit)                  | 10 01 | Ack
| 40 40 | [Auto Mode Off](#autoModeOff)                     | 10 01 | Ack
| 40 41 | [Auto Mode On](#autoModeOn)                       | 10 01 | Ack
| 40 42 | [Spot Erase Off](#spotEraseOff)                   | 10 01 | Ack
| 40 43 | [Spot Erase On](#spotEraseOn)                     | 10 01 | Ack
| 40 44 | [Audio Split Off](#audioSplitOff)                 | 10 01 | Ack
| 40 45 | [Audio Split On](#audioSplitOn)                   | 10 01 | Ack
| 4X 98 | [Output H Phase](#outputHPhase)                   | 10 01 | Ack
| 4X 9B | [Output Video Phase](#outputVideoPhase)           | 10 01 | Ack
| 4X A0 | [Audio Input Level](#audioInputLevel)             | 10 01 | Ack
| 4X A1 | [Audio Output Level](#audioOutputLevel)           | 10 01 | Ack
| 4X A2 | [Audio Adv Level](#audioAdvLevel)                 | 10 01 | Ack
| 4X A8 | [Audio Output Phase](#audioOutpuPhase)            | 10 01 | Ack
| 4X A9 | [Audio Adv Out Phase](#audioAdvOutPhase)          | 10 01 | Ack
| 4X AA | [Cross Fade Time Preset](#crossFadeTimePreset)    | 10 01 | Ack
| 4X B8 | [Local Key Map](#localKeyMap)                     | 10 01 | Ack
| 42 F8 | [Still Off time](#stillOffTime)                   | 10 01 | Ack
| 42 FA | [Stby Off time](#stbyOffTime)                     | 10 01 | Ack
| 61 0A | [TC Gen Sense](#tcGenSense)                       | 74 08 | [Gen Time Data](#genTimeData)
|       |                                                   | 79 09 | [Gen User Bits Data](#genUserBitsData)
|       |                                                   | 74 00 | [Timer-1 Data](#timer1Data)
|       |                                                   | 74 01 | [Timer-2 Data](#timer2Data)
|       |                                                   | 74 04 | [LTC Time Data](#ltcTimeData)
|       |                                                   | 74 05 | [User Bits (LTC) Data](#userBitsLTCData)
| 61 0C | [Current Time Sense](#currentTimeSense)           | 74 06 | [VITC Time Data](#vitcTimeData)
|       |                                                   | 74 07 | [User Bits (VITC) Data](#vitcData)
|       |                                                   | 74 14 | [Corrected LTC Time Data](#correctedLTCTimeData)
|       |                                                   | 74 15 | [Hold User Bits (LTC) Data](#holdUserBitsLTCData)
|       |                                                   | 74 16 | [Hold VITC Time Data](#holdVITCTimeData)
|       |                                                   | 74 17 | [Hold User Bits (VITC) Data](#holdUserBitsVITCData)
| 60 10 | [In Data Sense](#inDataSense)                     | 74 10 | [In Data](#inData)
| 60 11 | [Out Data Sense](#outDataSense)                   | 74 11 | [Out Data](#outData)
| 60 12 | [Audio In Data Sense](#audioInDataSense)          | 74 12 | [Audio In Data](#audioInData)
| 60 13 | [Audio Out Data Sense](#audioOutDataSense)        | 74 13 | [Audio Out Data](#audioOutData)
| 61 20 | [Status Sense](#statusSense)                      | 7X 20 | [Status Data](#statusData)
| 61 21 | [Extended VTR Status](#extendedVTRStatus)         | 7X 21 | [Extended Status Data](#extendedStatusData)
| 62 23 | [Signal Control Sense](#signalControlSense)       | 7X 23 | [Signal Control Data](#signalControlData)
| 6X 28 | [Local Key Map Sense](#localKeyMapSense)          | 7X 28 | [Local Key Map](#localKeyMap)
| 61 2A | [Head Meter Sense](#headMeterSense)               | 7X 2A | [Head Meter Data](#headMeterData)
| 60 2B | [Remaining Time Sense](#remainingTimeSense)       | 76 2B | [Remaining Time](#remainingTime)
| 60 2E | [Cmd Speed Sense](#cmdSpeedSense)                 | 7X 2E | [Cmd Speed Data](#cmdSpeedData)
| 61 30 | [Edit Preset Sense](#editPresetSense)             | 7X 30 | [Edit Preset Status](#editPresetStatus)
| 60 31 | [Preroll Time Sense](#prerollTimeSense)           | 74 31 | [Preroll Time](#prerollTime)
| 60 36 | [Timer Mode Sense](#timerModeSense)               | 71 36 | [Timer Mode Status](#timerModeStatus)
| 60 3E | [Record Inhibit Sense](#recordInhibitSense)       | 72 3E | [Record Inhibit Status](#recordInhibitStatus)
| 60 52 | [DA Inp Emph Sense](#daInpEmphSense)              | 71 52 | [DA Input Emphasis Data](#daInputEmphasisData)
| 60 53 | [DA PB Emph Sense](#daPBEmphSense)                | 71 53 | [DA Playback Emphasis Data](#daPlaybackEmphasisData)
| 60 58 | [DA Samp. Freq. Sense](#daSampFreqSense)          | 71 58 | [DA Sampling Frequency Data](#daSamplingFrequencyData)
| 61 AA | [Cross Fade Time Sense](#crossFadeTimeSense)      | 7X AA | [Cross Fade Time Data](#crossFadeTimeData)

## Command Formats

### <a name="localDisable"></a> 00 0C  Local Disable
Disables operation of the slave device from its control panel.
### <a name="deviceTypeRequest"></a> 00 11  Device Type Request
Slave Responds with
### <a name="deviceType"></a> 12 11  Device Type
message, with 2 bytes of data:

| Model    | Data
|----------|------
| BVU-800  | 10 00
| BVW-10   | 2X 00
| BVW-11   | 2X 02
| BVW-15   | 2X 03
| BVW-35   | 2X 10
| BVW-40   | 2X 01
| BVW-50   | 2X 30
| BVW-60   | 2X 20
| BVW-65   | 2X 21
| BVW-95   | 2X 22
| BVW-96   | 2X 23
| BVW-70   | 2X 24
| BVW-75   | 2X 25
| BVW-D75  | 2X 46
| BVW-9000 | 2X 47
| PVW-2600 | 2X 40
| PVW-2800 | 2X 41
| BVW-35PM | 20 18
| BVW-65PM | 20 29
| BVW-95PM | 20 2A
| BVW-75PM | 20 2D
| BVW-85P  | 21 26
| BVW-70S  | 21 2C
| BVW-75S  | 21 2D
| WBR-700  | 21 2D
| DVR-2000 | 3X 10
| DVR-2100 | 3X 11

Where X=0 for NTSC/PAL-M (525) models and 1 for PAL/SECAM models.
### <a name="localEnable"></a> 00 1D  Local Enable 
Enable operation of slave device from local panel according to the local enable map set by the "4X B8" [ Local Key Map](#localKeyMap) command.
### <a name="ack"></a> 10 01  ACK
Slave Sends this when it receives a command from Master.
### <a name="nak"></a> 11 12  NAK
When a communication error is detected, the slave sends this command with a "1" in the following position indicating the appropriate error condition:

| 7        | 6             | 5             | 4            | 3 | 2              | 1 | 0
|----------|---------------|---------------|--------------|---|----------------|---|------------------
| Time Out | Framing Error | Overrun Error | Parity Error | X | Checksum Error | X | Undefined command

### <a name="stop"></a> 20 00 Stop 
Slave stops current motion.
### <a name="play"></a> 20 01 Play 
Slave Starts to play from current location. When the "Sync play" mode is selected from the System menu on the slave, "Play" has the same effect as 20.34 [ "Sync Play"](#syncPlay).
### <a name="record"></a> 20 02  Record 
Slave begins recording. Exactly what happens depends on Auto Mode, record lockout, and edit presets.
### <a name="standbyOff"></a> 20 04 Standby Off 
Turns off standby mode. For VTR, this causes the machine to unthread in stop. Affects EE/Tape selection. Available only in Stop mode.
### <a name="standbyOn"></a> 20 05 Standby On 
Turns on standby mode. For VTR, this causes the machine to stay threaded when in stop. Affects EE/Tape selection.
### <a name="eject"></a> 20 0F Eject
When this command is received, the slave will eject the tape.
### <a name="fastFwd"></a> 20.10 Fast Fwd
When this command is received, the slave device will run in fast forward mode. The speed depends on the VTR; for the DVR2000 series it is 50 x play speed.
### <a name="jogFwd"></a> 2X.11 Jog Forward

### <a name="varFwd"></a> 2X.12 Var Forward

### <a name="shuttleFwd"></a> 2X.13 Shuttle Forward
When these commands are received the slave device will move forward with the speed indicated by DATA-1 and DATA-2.

When only DATA-1 is given, the speed wil be given by

```
Tape Speed = 10^((N/32)-2) x play speed.
```

where N is the value of DATA-1. Some sample values are:

| Speed | Speed Data
|-------|-----------
| 0.1   | 32 (20H)
| 1.0   | 64 (40H)
| 2.9   | 79 (4FH)
| 48.7  | 118 (76H)

When a more precise speed value is required, then DATA-2 will be added. The speed formula for this case is

```
Tape Speed = 10^((N/32)-2) + N'/256*(10^(((N+1)/32)-2)-10^((N/32)-2))
```

where N is the value of DATA-1 and N' is the value of DATA-2.

*auth - in more standard terms, the formula says that DATA-2 is used to linealy interpolate between the value given by N and that of N+1.*

The maximum jog speed is set in the System:System menu. The maximum Var speed is 3X play speed. The maximum shuttle speed is 50X play speed.

*auth - There is considerable controversy over the minimum speed. For a speed value of 0, the above formula with only DATA-1 gives 10^-2, or .01 x play speed. The standard states that when a speed between 0 and the minumum is given, the slave moves at minimum speed. In fact, many editors and control systems intend a "Shuttle 0" command ( 21 13 00 ) to pause the device and have it stop without disengaging. Devices which fail to do so will creep about 1 frame/second in this situation.*

### <a name="rewind"></a> 20 20 Rewind
When it receives this command, the slave runs in reverse at maximum speed: on the DVR2000, this is 50xplay speed.
### <a name="jogRev"></a> 2X 21 Jog Rev

### <a name="varRev"></a> 2X Var Rev

### <a name="shuttleRev"></a> 2X 23 Shuttle Rev
When receiving one of the above commands, the slave will start running in accordance with the speed data defined by DATA-1 and DATA-2. For the maximum and minimum speed see the 2X.12 [ Shuttle Fwd](#shuttleFwd) command.
### <a name="preroll"></a> 20 30 Preroll
When this command is received the slave will search to the preroll position defined as the value obtained by subtracting the preroll time set by the 44.31 [ Preroll Time Preset](#prerollTimePreset) command from the IN POINT data stored in the IN ENTRY memory by the 40.10 [In Entry](#inEntry) command.
### <a name="cue"></a> 24 31 Cue Up With Data
Cues the slave to the indicated time. Time is formatted as follows:

| data-1 | data-2  | data-3  | data-4
|--------|---------|---------|-------
| Frame  | Seconds | minutes | hours
| 10  1  |   10 1  |   10 1  | 10 1

*auth - this is how time is represented in all commands and responses using a time code. The numbers indicate that the 10s value is stored in the high nibble and the 1s value in the low nibble. This is not to be confused with the 80-bit SMPTE timecode which is present in the analog timecode track on tape, or with the VITC timecode.*

### <a name="syncPlay"></a> 20 34 Sync Play
Prerolls the slave for the preset preroll time, then enters play mode.
### <a name="progSpeedPlay+"></a> 21 38 Prog Speed Play +

### <a name="progSpeedPlay-"></a> 21 39 Prog Speed Play -
These commands play back the slave device in steps of 0.1% within the range of +/- 25.5% of play speed. DATA-1 contains an 8-bit speed value. The deviation from nominal play speed is

```
            Deviation(%)=0.2 x speed value
```

### <a name="preview"></a> 20 40 Preview

### <a name="review"></a> 20 41 Review

### <a name="autoEdit"></a> 20 42 Auto Edit
When one of these commands is received the slave goes into the indicated mode.

*This is all the spec states here. What actually happens is determined by the edit presets, in and out point selections, and ee/tape settings. Basically, all three set the device to the preroll position, and run at play speed up to the in point. In Preview, the slave switches to EE mode at the in point, and out at the out point, simulating the edit without disturbing the recording media. In review, the slave simply continues to play at the in point and rolls to the outpoint. In Auto Edit, the channels indicated by the edit presets are put into record at the in point and the recoding proceeds to the out point.*

### <a name="outpointPreview"></a> 20 43 Outpoint Preview
Sends transport to preset out point if insert mode is preset.
### <a name="antiClogTimerDisable"></a> 2X 54 Anti-Clog Timer Disable 
Disables the anti-clog timer. This timer is responsible for unthreading the tape upon timeout to save wear on the heads. If a system disables this timer, it should take responsibility for head wear avoidance itself.
### <a name="antiClogTimerEnable"></a> 2X 55 Anti-Clog Timer Enable
Enables anti-clog timer.
### <a name="fullEEOff"></a> 20 60 Full EE Off
Clears all channels from EE mode regardless of EDIT PRESET channels assigned by the [ 41.30 EDIT PRESET](#editPreset) command. It takes the slave 5 frames to perform this operation after it receieves the command.
### <a name="fullEEOn"></a> 20 61 Full EE On
Sets all channels to EE mode regardless of EDIT PRESET channels assigned by the [ 41.30 EDIT PRESET](#editPreset) command. It takes the slave 5 frames to perform this operation after it receieves the command.
### <a name="selectEEOn"></a> 20 63 Select EE On
This command sets only the preset channels assigned by the [ 41.30 Edit Preset](#editPreset) command to EE mode. The EE mode is cleared by the [ 20.64 Edit Off](#editOff) command. It takes the slave 5 frames to perform this operation after receiving the command.
### <a name="editOff"></a> 20 64 Edit Off
This command will stop recording without affecting the state of motion of the device. Any channels in record will come out in response to this command, after 5 frames of delay. This command also clears the Manual Edit Record mode and the Select EE mode.
### <a name="editOn"></a> 20 65 Edit On
This command is used to actually initiate recording. When the device is playing, and the edit presets set by the 4X 30 [Edit Preset](#editPreset) command are in place, the preset channels will enter record a fixed delay after this command is received. The slave will enter Edit Rec mode at this point. It takes the slave 5 frames to enter Edit Rec after receiving this command.
### <a name="freezeOff"></a> 20 6A Freeze Off
This command un-freezes the output of the device.
### <a name="freezeOn"></a> 20 6A Freeze On
This command freezes the output of the device. There is usually a 2-5 frame delay associcated with the actual freeze.
### <a name="timer1Preset"></a> 44 00 Timer-1 Preset
Sets the Timer-1 value to the time code indicated by DATA-1 thru DATA-4. For the time format see the [ 24.31 Cue Up With Data](#cue) command.
### <a name="timeCodePreset"></a> 44 04 Time Code Preset
Sets the Time Code Generator value to the time code indicated by DATA-1 thru DATA-4. The data format is as per the [ 24.31 Cue Up With Data](#cue) command, with two additional bits to indicate Color Frame and Drop Frame mode as follows:

| Data-1 Bit 7 | CF  | Data-1 Bit 6 | DF
|--------------|-----|--------------|----
| 0            | Off | 0            | Off
| 1            | On  | 1            | On

### <a name="userBitPreset"></a> 44 05 User Bit Preset
This command presets the value given by DATA-1 through DATA-4 to the User Bits of the Time Code Generator

| DATA-1    |           | DATA-2    |           | DATA-3    |           | DATA-4    |
|-----------|-----------|-----------|-----------|-----------|-----------|-----------|----------
| MSD       | LSD       | MSD       | LSD       | MSD       | LSD       | MSD       | LSD
| Bin Grp 2 | Bin Grp 1 | Bin Grp 4 | Bin Grp 3 | Bin Grp 6 | Bin Grp 5 | Bin Grp 8 | Bin Grp 7

### <a name="timer1Reset"></a> 40 08 Timer-1 Reset
Clears Tape timer 1 to 0
### <a name="inEntry"></a> 40 10 In Entry
Sets the video in point to the value displayed on the slave. This is the value of the selected tape timer.
### <a name="outEntry"></a> 40 11 Out Entry
Sets the video out point to the value displayed on the slave. This is the value of the selected tape timer.
### <a name="audioInEntry"></a> 40 12 Audio In Entry
Sets the audio in point to the value displayed on the slave. This is the value of the selected tape timer.
### <a name="audioOutEntry"></a> 40 13 Audio Out Entry
Sets the audio out point to the value displayed on the slave. This is the value of the selected tape timer.
### <a name="inDataPreset"></a> 44 14 In Data Preset
Set the Video In Point to the value indicated by DATA-1 thru DATA-4. The time format is as per the [ 24.31 Cue Up With Data](#cue) command.
### <a name="outDataPreset"></a> 44 15 Out Data Preset
Set the Video Out Point to the value indicated by DATA-1 thru DATA-4. The time format is as per the [ 24.31 Cue Up With Data](#cue) command.
### <a name="audioInDataPreset"></a> 44 16 Audio In Data Preset
Set the Audio In Point to the value indicated by DATA-1 thru DATA-4. The time format is as per the [ 24.31 Cue Up With Data](#cue) command.
### <a name="audioOutDataPreset"></a> 44 17 Audio Out Data Preset
Set the Audio Out Point to the value indicated by DATA-1 thru DATA-4. The time format is as per the [ 24.31 Cue Up With Data](#cue) command.
### <a name="inPShift"></a> 40 18 In + Shift
Increments the Video in point by one frame.
### <a name="inMShift"></a> 40 19 In - Shift
Decrements the Video in point by one frame.
### <a name="outPShift"></a> 40 1A Out + Shift
Increments the Video out point by one frame.
### <a name="outMShift"></a> 40 1B Out - Shift
Decrements the Video out point by one frame.
### <a name="audioInPShift"></a> 40 1C Audio In + Shift
Increments the Audio in point by one frame.
### <a name="audioInMShift"></a> 40 1D Audio In - Shift
Decrements the Audio in point by one frame.
### <a name="audioOutPShift"></a> 40 1E Audio Out + Shift
Increments the Audio out point by one frame.
### <a name="audioOutMShift"></a> 40 1F Audio Out - Shift
Decrements the Audio out point by one frame.
### <a name="inFlagReset"></a> 40 20 In Flag Reset

### <a name="outFlagReset"></a> 40 21 Out Flag Reset

### <a name="audioInFlagReset"></a> 40 22 Audio In Flag Reset

### <a name="audioOutFlagReset"></a> 40 23 Audio Out Flag Reset
Turn off the In Entry, Out Entry Audio In, and Audio Out lamps.

*auth - do these have any effect on the status bits?*

### <a name="inRecall"></a> 40 24 In Recall

### <a name="outRecall"></a> 40 25 Out Recall

### <a name="audioInRecall"></a> 40 26 Audio In Recall

### <a name="audioOutRecall"></a> 40 27 Audio Out Recall
Turn on the In Entry, Out Entry Audio In, and Audio Out lamps.
### <a name="lostLockReset"></a> 40 2D Lost Lock Reset
Resets ( sets to 0 ) the [ Data-8/Bit-6](#lostLock) Lost Lock bit in the [ 7X.20 status data](#statusData) . The LOST LOCK staus will be set when the servo is unlocked in the Play, Rec, or Edit mode.

### <a name="editPreset"></a> 4X 30 Edit Preset
This command is used for selecting the edit mode and selection of preset audio and video channels. These values are used by the Edit On and Edit Off comands.

| Byte    | 7 | 6      | 5        | 4     | 3   | 2   | 1        | 0
|---------|---|--------|----------|-------|-----|-----|----------|---------
| DATA-1: | X | Insert | Assemble | Video | X   | TC  | A2 (Cue) | A1 (Cue)
| DATA-2: | X | X      | X        | X     | DA4 | DA3 | DA2      |DA1

when the 41.30 command is used, the audio channels are set as per the table in the Edit:Setup menu. When the 42.30 command is used and Bit1 or Bit0 of Data-1 are "1", the Cue channel is selected.

### <a name="prerollTimePreset"></a> 44 31 Preroll time preset
Presets the preroll time given by DATA-1 thru DATA-4. Only the seconds digit int DATA-2 is used, and must be in the range of 0-59. The time format is as per the [ 24.31 Cue Up With Data](#cue) command.
### <a name="tapeAutoSelect"></a> 41 32 Tape/Auto Select
the Tape/EE mode is seleted by DATA-1 as follows:

|DATA-1 | Mode
|-------|----------------
|00     | Auto ( Tape/EE )
|01     | Tape
|FF     | Follows the mode set in the STATE MAP on the SYSTEM menu in Local.

### <a name="servoRefSelect"></a> 41 33 Servo Ref Select
Selects the SERVO REFERENCE signal according to DATA-1:

| DATA-1 | State
|--------|------
| 00     | Auto
| 01     | External
| 02     | Input
| FF     | Follows the mode set in REFERENCE on the SYSTEM:SYSTEM menu in Local.

### <a name="headSelect"></a> 41 34 Head Select
*auth - this command is not implemented by the DVR2000, but many other systems use it to to select playback/record heads. On the DVR2000 the selection is automatic.*
### <a name="colorFrameSelect"></a> 41 35 Color Frame select
Sets the COLOR FRAME mode of the servo system according to DATA-1:

| DATA-1 | State
|--------|--------
| 01     | 2 Field
| 02     | 4 Field
| 03     | 8 Field
| FF     | Follows the mode set in LOCK FIELD on the SYSTEM:SYSTEM menu in Local.

### <a name="timerModeSelect"></a> 41 36 Timer Mode Select
Selects the TIMER system used in AUTO mode, and for display of the IN ENTRY, OUT ENTRY, IN PRESET, OUT PRESET, PREROLL, and CUE UP WITH DATA, etc. according to DATA-1:

| DATA-1 | Mode
|--------|----------
| 00     | TIME CODE
| 01     | TIMER-1
| 02     | TIMER-2
| FF     | Follows the mode set in F6:CRNT TM on the HOME menu in Local.

### <a name="inputCheck"></a> 41 37 Input Check
Switches the INPUT CHECK mode on/off as per DATA-1:

| DATA-1 | Mode
|--------|-----
| 00     | INPUT CHECK off
| 01     | INPUT signals sent out VIDEO and MONITOR AUDIO OUTPUT connectors.

### <a name="editFiledSelect"></a> 41 3A Edit Field Select
Assigns the field on which to start the EDIT as per DATA-1:

| DATA-1 | Mode
|--------|-----
| 00     | Field1/Field2: The edit will start on the field the command was received on
| 01     | Field 1
| 02     | Field 2
| FF     | Follows the mode set in F4:TIMING on the EDIT SETUP menu in Local.

The following commands use the field selection:

### <a name="freezeModeSelect"></a> 41 3B Freeze Mode Select
Assigns the contents of the freeze picture to be field or frame as per DATA-1:

| DATA-1 | Mode
|--------|-----
| 00     | Field-1 or Field-2 ( not fixed )
| 01     | Field 1
| 10     | Field 2
| 11     | Frame

### <a name="recordInhibit"></a> 4X 3E Record Inhibit

### <a name="autoModeOff"></a> 40 40 Auto Mode Off

### <a name="autoModeOn"></a> 40 41 Auto Mode On
These commands switch the AUTO mode off and on.
### <a name="spotEraseOff"></a> 40 42 Spot Erase Off

### <a name="spotEraseOn"></a> 40 43 Spot Erase On
These commands switch the spot erase mode off and on.
### <a name="audioSplitOff"></a> 40 44 Audio Split Off

### <a name="audioSplitOn"></a> 40 45 Audio Split On
These commands switch the audio split mode off and on.
### <a name="outputHPhase"></a> 4X 98 Output H Phase
Sets the output Horizontal phase. The 40.98 command sets the H Phase to 0, whereas the 41.98 command sets the H phase according to

```
            H Phase = DATA_1 * 148 nsec
```

where DATA_1 is interpreted as a twos complement binary number in the range of -127 to 127.

**4X 9B Output Video Phase**
    Sets the output sync phase. The 40.9B command sets the output sync phase to 0 whereas the 41.9B command sets the output sync phase according to


```
            Sync Phase = DATA_1 * 74 nsec
```

where DATA_1 is interpreted as a twos complement binary number in the range of -127 to 127.

### <a name="audioInputLevel"></a> 4X A0 Audio Input Level
Controls the audio input level. The control mode is assigned by bit 7 of DATA-1. , and the channel to be controled is assigned by bit 6 to bit 0 of DATA-1. When the 41 A0 command is received, the audio input levels of the channels assigned by DATA-1 will be set to their reference levels. When CMDLEN is not 1, the level data will be composed of two bytes per *assigned* channel in the order DA1, DA2, DA3, DA4, CUE. The DATA-1 bits look like:

| Bit   | 7    | 6 | 5 | 4   | 3   | 2   | 1   | 0
|-------|------|---|---|-----|-----|-----|-----|----
| Value | Mode | X | X | Cue | DA4 | DA3 | DA2 | DA1

For Example, when the DATA-1 is 0x11, the command looks like:

| DATA-1 | DATA-2  | DATA-3  | DATA-4  | DATA-5
|--------|---------|---------|---------|-------
| 11     | DA1 LSB | DA1 MSB | CUE LSB | CUE MSB

The level data are interpreted as per the formula:

```
    Input Level = 20 log [(Level Data)/(4000h)] (dB)
```

Which gives coverage over the range of +12dB to -infinity.

### <a name="audioOutputLevel"></a> 4X A1 Audio Output Level
Controls the audio output level. When the 41 A1 command is received, the audio output level of the channels selected in DATA-1 will be set to the reference level. Otherwise, it operates as per the [ 4X.A0 Audio input level](#audioInputLevel) command.
### <a name="audioAdvLevel"></a> 4X A2 Audio Adv Level
Controls the audio advance level. When the 41 A2 (fixed doc error here) command is received, the audio advance level of the channels selectd by DATA-1 will be set to the reference level. Otherwise, it operates as per the [ 4X.A0 Audio input level](#audioInputLevel) command.
### <a name="audioOutputPhase"></a> 4X A8 Audio Output Phase
Sets the autdio output phase. The 42.A8 command determines the audio output phase according to the 16-bit value encoded with the low byte in DATA-1 and the high byte in DATA-2. The values are interpreted as 2s complement binary, and currently have a range of +/- 80 samples. When the 40.A8 command is received, the slave will set the audio ouput phase to its nominal value.
### <a name="audioAdvOutPhase"></a> 4X A9 Audio Adv Out Phase
Sets the Advance audio output phase. The 42.A9 command determines the audio advance phase according to the 16-bit value encoded with the low byte in DATA-1 and the high byte in DATA-2. The values are interpreted as 2s complement binary, and currently have a range of +0/-50 samples. the 40.A9 will set the audio advance phase to its nominal value.
### <a name="crossFadeTimePreset"></a> 4X AA Cross Fade Time Preset

### <a name="localKeyMap"></a> 4X B8  Local Key Map
When the slave receives the 00.1D Local Enable command, the control panel may be used according to the local key map that was set by this command. When the slave receives the 00.0C Local Disable command all the keys, buttons, and adjustment controls on the control panel are disabled. The Eject button can always be used. If the slave receives the 41.B8 command, the local key map is preset by the block level in accordance with DATA-1. IF it receives the 4X.B8 command ( X &gt; 2 ) The local key map is preset by the Switch level.

Block Level switches:

| Bit 7 | Bit 6 | Bit 5 | Bit 4            | Bit 3           | Bit 2         | Bit 1         |Bit 0
|-------|-------|-------|------------------|-----------------|---------------|---------------|---------
|       |       |       | Tracking Control | Monitor Control | Audio Control | Video Control | Transport Control

- "1": This function will be enabled when in remote
- "0": This function will be disabled in remote.

When DATA-2 or more are added, control data with two bytes per each block assigned by DATA-1 are added following DATA-1.

At present the transport switches are defined as follows:

| Bit      | 7       | 6       | 5      | 4   | 3    | 2    | 1    | 0
|----------|---------|---------|--------|-----|------|------|------|--------
| 1st Byte | Execute | Preroll | Search |     | Rec  | Play | Stop | Standby
| 2nd Byte |         |         |        |     |      | Var  | Jog  | Shuttle

None of the other blocks have any switches assigned, but rather operate as follows:

Video Control: Video phase and Sync phase can be adjusted on the system menu in remote mode.

Audio Control: Audio levels and output phase can be adjusted on the Audio:DA out menu in remote mode.

Monitor Control: the wfm monitor output selection on the system:wfm monitor menu and the montior level adjustments and monitor out selection on the system:audio monitor menu can be adjusted in remote mode.

Tracking Control: Tracking adjustments in the system:tracking menu can be made in remote mode.

### <a name="stillOffTime"></a> 42 F8 Still Off time

### <a name="standbyOffTime"></a> 42 FA Standby Off time

### <a name="tcGenSense"></a> 61 0A TC Gen Sense

### <a name="inDataSense"></a> 60 10 In Data Sense

### <a name="outDataSense"></a> 60 11 Out Data Sense

### <a name="audioInDataSense"></a> 60 12 Audio In Data Sense

### <a name="audioOutDataSense"></a> 60 13 Audio Out Data Sense

### <a name="currentTimeSense"></a> 61 0C Current Time Sense
Requests the TIME DATA or USER BITS. Slave responds as per DATA-1:

| Bit 7 | Bit 6 | Bit 5   | Bit 4  | Bit 3   | Bit 2   | Bit 1     | Bit 0
|-------|-------|---------|------- |---------|---------|-----------|---------
|       |       | VITC UB | LTC UB | TIMER 2 | TIMER 1 | VITC TIME | LTC TIME

That is to say that the response follows the bit set in DATA-1 as per the following table

| Request             | Response
|---------------------|---------
| DATA-1=01 LTC TIME  | 74.04: LTC TIME DATA
|                     | 74.14: CORRECTED LTC TIME DATA
| DATA-1=02 VITC TIME | 74.06: VITC TIME DATA
|                     | 74.16: HOLD VITC TIME DATA
| DATA-1=04 TIMER-1   | 74.00: TIMER-1 DATA
| DATA-1=08 TIMER-2   | 74.01: TIMER-2 DATA
| DATA-1=10 LTC UB    | 74.05: UB (LTC) DATA
|                     | 74.15: HOLD UB (LTC) DATA
| DATA-1=20 VITC UB   | 74.07: UB (VITC) DATA
|                     | 74.17: HOLD UB (VITC) DATA

*Now, when requesting timecode, the results depend on the tape speed because at very low speeds ( less than .25 play speed ) it may not be possible to recover timecode. However, if VITC is present, that may be used instead. To automate this decision process, Sony has provided the special 61.0C.03 command which will return the best source of time code as per the table below. Note that when both LTC and VITC are not good you get back the 74.14 corrected LTC data. In this case, the time is actually the last good LTC time corrected by the tape timer.*

#### Tape speed &gt; .25 

| LTC Status | VITC Status | Return Data | Return code
|------------|-------------|-------------|------------
| OK         | OK          | LTC         | 74.04 XX XX XX XX
| NG         | OK          | VITC        | 74.06 XX XX XX XX
| OK         | NG          | LTC         | 74.04 XX XX XX XX
| NG         | NG          | LTC(&#42;)  | 74.14 XX XX XX XX

#### Tape speed &lt; .25

| LTC Status | VITC Status | Return Data | Return code
|------------|-------------|-------------|------------
| OK         | OK          | VITC        | 74.06 XX XX XX XX
| NG         | OK          | VITC        | 74.06 XX XX XX XX
| OK         | NG          | LTC         | 74.04 XX XX XX XX
| NG         | NG          | LTC(&#42;)  | 74.14 XX XX XX XX

### <a name="statusSense"></a> 61 20 Status Sense
When the slave receives a 61.20 Status Sense command, it will respond with a [74 20 Status Data](#statusData) response. The starting byte number and number of bytes requested are encoded in DATA-1, with the starting reg in the high nibble (bits 7-4) and the requested byte count in the low nibble ( bits 3-0 ).
### <a name="extendedVTRStatus"></a> 61 21 Extended VTR Status

### <a name="signalControlSense"></a> 62 23 Signal Control Sense

### <a name="localKeyMapSense"></a> 6X 28 Local Key Map Sense

### <a name="headMeterSense"></a> 61 2A Head Meter Sense

### <a name="remainingTimeSense"></a> 60 2B Remaining Time Sense

### <a name="cmdSpeedSense"></a> 60 2E Cmd Speed Sense

### <a name="editPresetSense"></a> 61 30 Edit Preset Sense

### <a name="prerollTimeSense"></a> 60 31 Preroll Time Sense

### <a name="timerModeSense"></a> 60 36 Timer Mode Sense

### <a name="recordInhibitSense"></a> 60 3E Record Inhibit Sense

### <a name="inpEmphSense"></a> 60 52 DA Inp Emph Sense

### <a name="pbEmphSense"></a> 60 53 DA PB Emph Sense

### <a name="daSampFreqSense"></a> 60 58 DA Samp. Freq. Sense

### <a name="crossFadeTimeSense"></a> 61 AA Cross Fade Time Sense

### <a name="timer1Data"></a> 74 00 Timer-1 Data
DATA-1 thru DATA-4 contain the timer-1 data formatted as per the [ 24.31 Cue up with data](#cue) command
### <a name="timer2Data"></a> 74 01 Timer-2 Data
DATA-1 thru DATA-4 contain the timer-2 data formatted as per the [ 24.31 Cue up with data](#cue) command
### <a name="ltcTimeData"></a> 74 04 LTC Time Data
DATA-1 thru DATA-4 contain the LTC data formatted as per the [ 24.31 Cue up with data](#cue) command
### <a name="userBitsLTCData"></a> 74 05 User Bits (LTC) Data
DATA-1 thru DATA-4 contain the LTC User bits data formatted at per the [ 44.05 User Bit Preset](#userBitPreset) command.
### <a name="vitcTimeData"></a> 74 06 VITC Time Data
DATA-1 thru DATA-4 contain the VITC data formatted as per the [ 24.31 Cue up with data](#cue) command
### <a name="userBitsVITCData"></a> 74 07 User Bits (VITC) Data
DATA-1 thru DATA-4 contain the VITC User bits data formatted at per the [ 44.05 User Bit Preset](#userBitPreset) command.
### <a name="genTimeData"></a> 74 08 Gen Time Data
DATA-1 thru DATA-4 contain the Time code generator data formatted as per the [ 24.31 Cue up with data](#cue) command
### <a name="genUserBitsData"></a> 79 09  Gen User Bits Data
DATA-1 thru DATA-4 contain the Time code generator User bits data formatted at per the [ 44.05 User Bit Preset](#userBitPreset) command.
### <a name="inData"></a> 74 10 In Data

### <a name="outData"></a> 74 11 Out Data

### <a name="audioInData"></a> 74 12 Audio In Data

### <a name="audioOutData"></a> 74 13 Audio Out Data

### <a name="correctedLTCTimeData"></a> 74 14 Corrected LTC Time Data

### <a name="holdUserBitsLTCData"></a> 74 15 Hold User Bits (LTC) Data

### <a name="holdVITCTimeData"></a> 74 16 Hold VITC Time Data

### <a name="holdUserBitsVITCData"></a> 74 17 Hold User Bits (VITC) Data

### <a name="statusData"></a> 74 20 Status Data
When the slave receives a 61.20 Status Sense command, the following data will be sent back in response according to the request:

| Byte No.  | 7          | 6          | 5         | 4                 | 3          | 2          | 1          | 0
|-----------|------------|------------|-----------|-------------------|------------|------------|------------|-----
| 0         | X          | X          | Tape Out  | Servo Ref Missing | X          | X          | X          |Local
| 1         | Standby    | X          | Stop      | Eject             | Rewind     | Fast Fwd   | Record     |Play
| 2         | Servo Lock | TSO Mode   | Shuttle   | Jog               | Var        | Tape Dir   | Still      |Cue Up
| 3         | Auto Mode  | Freeze On  | X         | CF Mode           | A Out      | A In       | Out        |In
| 4         | Select EE  | Full EE    | X         | Edit              | Review     | Auto Edit  | Preview    |Preroll
| 5         | X          | Insert     | Assemble  | Video             | A4         | A3         | A2         |A1
| 6         | X          | Lamp Still | Lamp Fwd  | Lamp Rev          | Srch LED 8 | Srch LED 4 | Srch LED 2 |Srch LED 1
| 7         | X          | X          | Aud Split | Sync Act          | X          | Spot Erase | X          |In/Out
| 8         | Buzzer     | Lost lock  | Near EO T | EOT               | CF Lock    | Svo Alarm  | Sys Alarm  |Rec Inhib
| 9         | Fnc Abort  | X          | X         | X                 | X          | X          | X          |X

**Description of Bits: **

Data 0:

* Bit 5: Tape Unthreaded (Cassette Out) When the tape is threaded, this is 0. When it is completely unthreaded this is 1. When threading or unthreading, who knows what it is.
* Bit 4: Servo Ref Missing When servo reference is absent this is 1.
* Bit 0: Local When remote operation is inhibited by the "remote/local" switch on the panel this is 1.

Data 1:

* Bit 7: Standby When the tape is threaded and the scanner is locked but the tape is stopped, this is 1.
* Bit 5: Stop When the machine is in full stop, this is 1. The thread state depends on the tape/ee and standby settings.
* Bit 4: Eject When the tape is ejecting this is 1.
* Bit 3: Rewind When the machine is in fast reverse this is 1.
* Bit 2: Fast Fwd When the machine is in fast forward this is 1.
* Bit 1: Record This bit goes from 0 to 1 some number of frames after the machine starts recording. For the DVR2000 we measured 5 frames. Others have varying delays on the record status.
* Bit 0: Play This bit goes from 0 to 1 some number of frames after the machine starts playing. For the DVR2000 we measured 5 frames. Others have varying delays on the play status.

Data 2:

* Bit 7: Servo Lock 1 indicates servos are locked. This is a necessary condition for an edit to occur correctly.
* Bit 6: TSO Mode Bit is 1 in tape speed override: in this mode, audio and video are still locked though speed is off play speed by +/- up to 15%.
* Bit 5: Shuttle
* Bit 4: Jog
* Bit 3: Var
* Bit 2: Tape Dir
* Bit 1: Still
* Bit 0: Cue Up

Data 3:

* Bit 7: Auto Mode
* Bit 6: Freeze On
* Bit 4: CF Mode
* Bit 3: A Out
* Bit 2: A In
* Bit 1: Out
* Bit 0: In

Data 4:

* Bit 7: Select EE
* Bit 6: Full EE
* Bit 4: Edit
* Bit 3: Review
* Bit 2: Auto Edit
* Bit 1: Preview
* Bit 0: Preroll

Data 5:

* Bit 6: Insert
* Bit 5: Assemble
* Bit 4: Video
* Bit 3: A4
* Bit 2: A3
* Bit 1: A2
* Bit 0: A1

Data 6:

* Bit 6: Lamp Still
* Bit 5: Lamp Fwd
* Bit 4: Lamp Rev
* Bit 3: Srch LED 8
* Bit 2: Srch LED 4
* Bit 1: Srch LED 2
* Bit 0: Srch LED 1

Data 7:

* Bit 5: Aud Split
* Bit 4: Sync Act
* Bit 2: Spot Erase
* Bit 0: In/Out

Data 8:

* Bit 7: Buzzer
* Bit 6: <a name="lostLock"></a>Lost lock This bit is controled by the [ Lost Lock Reset](#lostLockReset) command. It is set when the servos are unlocked in the PLAY, REC, or EDIT modes.
* Bit 5: Near EOT
* Bit 4: EOT
* Bit 3: CF Lock
* Bit 2: Svo Alarm
* Bit 1: Sys Alarm
* Bit 0: Rec Inhib

Data 9:

* * *

*auth - here's a code snippet for you c-heads.*

```
    char *StatusBitStrings[][8] =
    {
            // Data 0
            {"         ","          ","Unthread  ","Svo Ref   ",
             "          ","          ","          ","Local     "},
            // Data 1
            {"Standby   ","          ","Stop      ","Eject     ",
             "Rewind    ","Fast Fwd  ","Record    ","Play      "},
            // Data 2
            {"Servo Lock","TSO Mode  ","Shuttle   ","Jog       ",
             "Variable  ","Tape Rev  ","Still     ","Cue       "},
            // Data 3
            {"Auto Mode ","Freeze On ","          ","CF Mode   ",
             "A Out     ","A In      ","Out       ","In        "},
            // Data 4
            {"Select EE ","Full EE   ","          ","Edit      ",
             "Review    ","Auto Edit ","Preview   ","Preroll   "},
            // Data 5
            {"          ","Insert    ","Assemble  ","Video     ",
             "A4        ","A3        ","A2        ","A1        "},
            // Data 6
            {"          ","Lamp Still","Lamp Fwd  ","Lamp Rev  ",
             "Srch LED 8","Srch LED 4","Srch LED 2","Srch LED 1"},
            // Data 7
            {"          ","          ","Aud Split ","Sync Act  ",
             "          ","Spot Erase","          ","In/Out    "},
            // Data 8
            {"Buzzer    ","Lost lock ","Near EOT  ","EOT       ",
             "CF Lock   ","Svo Alarm ","Sys Alarm ","Rec Inhib "},
            // Data 9
            {"Fnc Abort ","          ","          ","          ",
             "          ","          ","          ","          "},
    };
```

* * *

The Status bits communicate much about the progress of a motion command. The precise timing of each signal varies almost from machine to machine, and many edit controlers expect certain timing behaviour of these signals. Herein lie many of the problems associated with edit controllers, edit timing, and just plain wacky transport behavior.

### <a name="extendedStatusData"></a> 7X 21 Extended Status Data

### <a name="signalControlData"></a> 7X 23 Signal Control Data

### <a name="localKeyMap"></a> 7X 28 Local Key Map

### <a name="headMeterData"></a> 7X 2A Head Meter Data

### <a name="remainingTime"></a> 76 2B Remaining Time

### <a name="cmdSpeedData"></a> 7X 2E Cmd Speed Data

### <a name="editPresetStatus"></a> 7X 30 Edit Preset Status

### <a name="prerollTime"></a> 74 31 Preroll Time

### <a name="timerModeStatus"></a> 71 36 Timer Mode Status

### <a name="recordInhibitStatus"></a> 72 3E Record Inhibit Status

### <a name="daInputEmphasisData"></a> 71 52 DA Input Emphasis Data

### <a name="daPlaybackEmphasisData"></a> 71 53 DA Playback Emphasis Data

### <a name="daSamplingFrequencyData"></a> 71 58 DA Sampling Frequency Data

### <a name="aaCrossFadeTimeData"></a> 7X AA Cross Fade Time Data

* * *

[Rick Davis ](mailto:rld@well.com)
