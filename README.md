# Stage Camera Thermique 2022 @ Elsys Design

"Caméra thermique" internship main repository.
 
## Presentation
The goal of the "Caméra Thermique" internship is to design and implement a thermal camera with all the electronics and software that comes with.
The final product shall embed the features listed below :
- a thermal camera for IR imaging,
- a daylight camera to compare the output with the IR image,
- a touchscreen to view the camera results, change display mode and take screenshots,
- a USB port to charge and communicate with the PC,
- a battery for the autonomy,
- a UART port to exchange debug messages,
- a SD card interface to save the screenshots, 
- an external RTC to get the time more accurately than with the mcu integrated RTC,
- a SWD/JTAG port for programming/debugging purposes

Go to the [specifications](https://github.com/AlexandraHULOT/NuttX_Infrared_Camera/blob/main/documentation/Stage2022-ED_CdC_CameraThermique_V00.pdf) for more details

## What will you find in this repository ?
You'll find here all the related documents & sources for both the integration and developement parts, including the Nuttx sources.
However, from the official repo some changes have been made to meet our needs. Among them :
- The nuttx-src/apps folder contain an example named "thermal_camera". This is the `main` app of our project and it is the entrypoint for both the dev and final target  
- The Nuttx nuttx-src/nuttx/boards/arm/stm32h7 folder contain a custom folder to support our final target `stm32h743vit6-ct2022`.

## Build
### Build docker environnement - Step 1
- Modify in the .env file the uid and gid of your user on the host
- run :
```
docker-compose build
```
### Config NuttX  - Step 2
Now that you have build the docker environnement, we will create our working container:
```
docker-compose run nuttxspace
```
If everything's fine, you should be inside the nuttx-src folder. To configure Nuttx with a specific `<CONFIG>` for a specific `<BOARD>` :
```
cd nuttx
./tools/configure.sh <BOARD>\<CONFIG>
```
usage : `./nuttx/tools/configure.sh stm32h747i-disco\thermal_camera`

### Compile NuttX - Step 3

Simply :
```
make
```


## Contributions
