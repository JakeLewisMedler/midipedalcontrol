import Vue from "vue";

const { ipcRenderer } = require("electron");

export default (ctx, inject) => {
  const state = Vue.observable({
    device: null,
    debug: false,
  });

  // Listen for debug toggle from native menu
  ipcRenderer.on("set-debug", (_event, value) => {
    state.debug = value;
  });

  // Keep main process in sync when debug changes from renderer
  const unwatch = new Vue({
    data: () => ({ state }),
    watch: {
      "state.debug"(val) {
        ipcRenderer.send("debug-changed", val);
      },
    },
  });

  inject("global", state);
};
