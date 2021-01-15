# INDICATION-AND-UTILITY-MANAGEMENT
control system, using
ATmega16 microcontroller, which is used for monitoring and controlling
the facilities found in a student-library. The library has two doors, one of
them is for entry only and the other one is for exit only. Two infra-red
sensors are installed on the doors and connected to INT1 and INT2
respectively. This is used to count the students entering and leaving the
library.
To reduce the library run-cost (electrical power consumption), the
lightening system of the library is controlled upon the no of students found
in the library using the following category:-
 Light is completely off, if no student is inside.
 One portion of light system (Connected to B0) is ON, if students found in the library are 10
or less.
 Another portion would be turned on (Connected to B1), if students found in the library are
more than 10 and less than 20.
 A Third portion of light system (Connected to B2) is ON, if students found are more than
20.


Another power consumption reduction can be done sensing the
library temperature using an analog sensor (Connected to A3), that
senses the temperature in the range of 0o C to 50o C whose output
is linear from 0 V to 5 V.
Two different air-conditioners and
heaters are installed (Connected to
B4, B5, B6, and B7 respectively)
