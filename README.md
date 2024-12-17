# DIY Thermostat side project

In my first apartment when I moved to New York, my window AC unit had no thermostat. It just had a knob for temperature setting and a knob for fan mode. This annoyed me a lot over the summer when it was hot at night, and the only options were to leave it on all night (waking up freezing and having added several bucks to the electricity bill) or leave it off (waking up sweating).

So I did some research and bought
* a cheap breadboarding electronics kit off Amazon
* an ESP8266 Microcontroller
* a good metal-geared servo motor
* a USB cable to connect the ESP8266 to my Mac

All in all, the project cost me a bit less than $45 USD.

<img width="350" src="https://github.com/user-attachments/assets/ad56022b-4c41-4fa5-a97d-b463d262ce5c"/>

### Microcontroller component

I coded for my ESP8266 in C++ in the free Arduino IDE. I used some libraries to enable simple WiFi communication.

At first, I envisioned coding an iOS app with a dial in the UI to resemble a modern home thermostat that I could use to set a target temperature, and coding the microcontroller to report back the current temperature reading. I wanted the microcontroller to handle turning the AC knob on and off as needed to reach the temperature requested via my phone. However, I attempted to use the thermistor which came with my electronics kit to measure room temperature (it's a simple component that changes resistance in proportion to its temperature), but this proved very unreliable.

So rather than paying $10 to order a set of nicer thermometer components, I decided to change course and keep things reliable with a simpler microcontroller app that can just turn the AC on and off at timed intervals. It also can turn the AC on or off indefinitely, and accepts manual user input via buttons to do both of these things, or HTTP input to turn the AC on or off remotely.

I engineered a mechanism to hold the servo in place and grab my AC control knob very successfully with some stuff I had around my apartment.

https://github.com/user-attachments/assets/bed9ba1d-c7c9-4b14-9891-1e38f348003e

### Swift app component

I tried out building with SwiftUI for the first time, with 3 simple components of the app following an MVVM architecture:
* View that has a single button that reflects the current AC setting. It allows you to "Turn Air On" or "Turn Air Off"
* ViewModel that updates the button's content, saves the current state, and requests the Model layer to change the AC settings
* Model that makes HTTP network requests to my ESP8266

I also exposed the app to Siri via AppIntents and then set up 2 shortcuts on my iPhone to control the AC via Siri without ever unlocking my phone, which was surprisingly quick to implement.

The app is bare-bones simple but it worked! It was pretty fun to build and kept me at a comfortable temperature the second summer I lived in my apartment, turning on for 10 minute intervals and then off for 30 minute intervals all night. When I felt like turning it on or off without getting out of bed, it was easy to do with Siri.

<img width="1273" alt="image" src="https://github.com/user-attachments/assets/d5a1737a-0082-46ed-b694-76312c90ac59" />
