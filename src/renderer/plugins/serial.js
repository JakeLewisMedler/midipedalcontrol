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

  const getSettings = (port) => {
    port.send("003");
  };
  const updatePedal = (port, pedal, setting) => {
    if (setting == "cc") {
      let pedalId = pedal.id.toString().padStart(3, "0");
      let cc = pedal.cc.toString().padStart(3, "0");
      let command = `001001${pedalId}001${cc}`;
      console.log(command);
      port.send(command);
    } else if (setting == "mode") {
      let pedalId = pedal.id.toString().padStart(3, "0");
      let mode = pedal.mode.toString().padStart(3, "0");
      let command = `001001${pedalId}002${mode}`;
      console.log(command);
      port.send(command);
    }
  };

  const testPedal = (port, pedal) => {
    let pedalId = pedal.id.toString().padStart(3, "0");
    let value = pedal.value.toString().padStart(3, "0");
    let command = `002001${pedalId}${value}`;
    console.log(command);
    port.send(command);
  };

  const sendMessage = (port, message) => {
    port.write(`${message}\n`, function (err) {
      if (err) {
        return console.log("Error on write: ", err.message);
      }
    });
  };

  const parseStatus = (data) => {
    let pedalsData = data.substring(1).split("-");
    return pedalsData.map((pedal, index) => {
      let pedalData = pedal.split(",");
      let params = {};
      params.id = index + 1;
      params.mode = parseInt(pedalData[0]);
      params.cc = parseInt(pedalData[1]);
      params.value = parseInt(pedalData[2]);
      return params;
    });
  };

  const connect = () => {
    if (ctx.$global.device) {
      let port = new SerialPort({
        path: ctx.$global.device.path,
        baudRate: 115200,
      });

      port.send = (message) => sendMessage(port, message);
      port.getSettings = () => getSettings(port);
      port.updatePedal = (pedal, mode) => updatePedal(port, pedal, mode);
      port.testPedal = (pedal, value) => testPedal(port, pedal);

      port.on("close", () => {
        console.log("Port Closed");
      });
      port.receiver = port.pipe(new ReadlineParser({ delimiter: "\r\n" }));
      port.receiver.on("data", (data) => {
        console.log(data);
        if (data[0] != "-") return;
        port.settings = parseStatus(data);
      });
      ctx.$serial.port = port;
    }
  };

  const disconnect = () => {
    if (ctx.$serial.port && ctx.$serial.port.isOpen) {
      ctx.$serial.port.close();
    }
  };

  const resetDevice = async (device) => {
    let port = new SerialPort({
      path: device.path,
      baudRate: 115200,
    });

    sendMessage(port, "006");

    port.receiver = port.pipe(new ReadlineParser({ delimiter: "\r\n" }));

    await new Promise((resolve, reject) => {
      port.receiver.on("data", (data) => {
        if (data[0] != "-") return;
        resolve();
      });
    });
    port.close();
  };

  inject(
    "serial",
    Vue.observable({
      getDevices,
      connect,
      disconnect,
      resetDevice,
      port: null,
    })
  );
};
