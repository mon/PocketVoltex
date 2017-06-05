# Configuration

Configure your controller with the web app at:
https://mon.im/sdvx/config/

You will need Chrome 57+. You must be using SSL (https, not http).
You *might* also need to go to chrome://flags and enable
"Experimental Web Platform features". This step will not be required by
June/July, as the WebUSB feature will be shipping in Chrome 60.

If you are using b\*\*\*\*\*tools, configure as normal.
For fast configuration of the HID lighting, download and run [this exe](https://github.com/mon/PocketVoltex/raw/master/Software/HidLights.exe)
while config.exe is open and the controller is connected.

## HID Lighting fix
By default, HID lighting will crash after a few minutes of gameplay. To fix
this, simply patch libinput.dll using [this tool](https://mon.im/bemanipatcher/pocketvoltex.html).
