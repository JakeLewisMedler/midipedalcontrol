import Vue from "vue";

const { SerialPort } = require("serialport");
const { ReadlineParser } = require("@serialport/parser-readline");

let vendorId = "2341";
let productId = "8036";
export default (ctx, inject) => {
  const getDevices = async () => {
    console.log("getDevices");
    try {
      let ports = await SerialPort.list();
      // let ports = [
      //   {
      //     path: "/dev/tty.URT2",
      //   },
      //   {
      //     path: "/dev/tty.URT1",
      //   },
      //   {
      //     path: "/dev/tty.BLTH",
      //   },
      //   {
      //     path: "/dev/tty.Bluetooth-Incoming-Port",
      //   },
      //   {
      //     path: "/dev/tty.usbmodem01234",
      //     manufacturer: "Arduino LLC",
      //     serialNumber: "0123",
      //     locationId: "14530000",
      //     vendorId: "2341",
      //     productId: "8036",
      //   },
      // ];
      let midiPedalDevices = ports
        .filter(
          (port) => port.vendorId == vendorId && port.productId == productId
        )
        .map((device) => {
          return { ...device, connected: false };
        });

      return midiPedalDevices;
    } catch (error) {
      console.error(error);
    }
  };

  const connect = () => {
    if (ctx.$global.device) {
      let port = new SerialPort({
        path: ctx.$global.device.path,
        baudRate: 9600,
      });
      ctx.$serial.port = port;

      port.receiver = port.pipe(new ReadlineParser({ delimiter: "\n" }));
      port.send = (message) => {
        port.write(`${message}\n`, function (err) {
          if (err) {
            return console.log("Error on write: ", err.message);
          }
        });
      };
    }
  };

  const disconnect = () => {
    if (ctx.$serial.port && ctx.$serial.port.isOpen) {
      ctx.$serial.port.close();
    }
  };

  inject(
    "serial",
    Vue.observable({
      getDevices,
      connect,
      disconnect,
      port: null,
    })
  );
};
