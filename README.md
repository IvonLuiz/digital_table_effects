# Digital table effects
Rebuilding effects from Vedo/Teyun A8 Digital Tablet using the fit TMS320C5502 eZdsp

This project was built for the class ENGG54 - INTEGRATED LABORATORY III at UFBA. This is the repository for Team 1. The steps of this project are as follows:

1. Analyze the technical specifications of the Vedo/Teyun A8 digital mixer
2. Each team will be responsible for implementing 8 effects. Analyze the provided audio files, which should be distributed as follows:
     * Team 1: En = 3n − 2
     * Team 2: En = 3n − 1
     * Team 3: En = 3n
    where n = 1, 2, 3, ... 8
3. Identify the implemented effects and their corresponding approximate parameters
4. Implement high-level solutions for conceptual validation
5. Develop a user interface using the board's resources
6. Implement the proposed DSP algorithms on the TMS320C5502 eZdsp Kit
7. Validate using the provided files, directly in CCS
8. Compare the results obtained with the kit to those produced by the mixer
9. Real-time validation using a microphone and amplified speaker
10. Documentation through a GitHub repository and written report

We will be implementing effects:
**01:** *REV-HALL1*
**04:** *REV-ROOM2*
**07:** *REV-STAGE B*
**10:** *REV-STAGE D*
**13:** *REV-STAGE F*
**16:** *REV-STAGE Gb*
**19:** *FLANGER*
**22:** *TEEMOLO*
      