# Integration notes

<p align="center">
  <img src="./images/thermal_camera_front" />
</p>
<p align="center">
<em>STM32H743VIT6-CT2022 board from the front</em>
</p>

<p align="center">
  <img src="./images/thermal_camera_back" />
</p>
<p align="center">
<em>STM32H743VIT6-CT2022 board from the back</em>
</p>
 
## Before anything

Because the stm32h743vit6-ct2022 has been tested at the end of the internship, some things still does not work as expected compared to the dev board: 
- First if you flash the board with the thermal_camera example it will work until you press the reset button. After that, the camera will miss the init part to work correctly. The dev board does not have this issue, so it's most likely a misconfiguration.
- Because the camera is not in the same orientation as the screen on the board, unlike our setup during the dev phasis, you need to rotate the image with the corresponding ft80x command.
- The USB device is not recognized by the host. During our tests, we thought it was an issue caused by VBUS not going from 0 to 5V, since this event raises an interruption telling the device there's a connection with the host. If there's one, the board starts sending data (PID,VID, device class and so on) on the D+ line. However, fixing the voltages didn't change the behavior.
- Plugging in the visible camera makes most of the tests fail...