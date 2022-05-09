# midipedal

#### Build your own USB MIDI pedal box

This repo contains the following:

- midipedal Control UI
- midipedal Arduino code
- midipedal Hardware

# Control UI

```bash
# install dependencies
yarn

# serve app for development
yarn dev

# build electron application for production
yarn build

```

# Arduino Code

Includes custom 'pedal' library/class.

# Hardware

The case for the midipedal is a 3D model, to be 3D printed. The example images show the model printed on a SnapMaker 2.0, with black PLA.

The repo contains a Sketchup project and exported STL model.
Prints as two separate parts:

- The box
- The sliding lid

It is designed to fit the following components (Feel free to adjust the model to fit alternative components)

- Arduino Pro Micro - https://www.amazon.co.uk/dp/B09MPKP1ST
- TRS Socket - https://www.rean-connectors.com/en/product/nys212-1
- 4 core cable - https://cpc.farnell.com/pro-elec/pelb0636/cable-7-2-4a-unshielded-black/dp/CB22626
- USB Socket - https://cpc.farnell.com/cliff-electronic-components/cp30707n/usb2-0-b-a-f-thru-adaptor-d-slim/dp/CN22727
- Short USB cable - https://cpc.farnell.com/startech/uusbhaub6in/lead-usb-a-male-micro-b-male-0/dp/CS29356
