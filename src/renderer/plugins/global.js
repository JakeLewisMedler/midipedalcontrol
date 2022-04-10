import Vue from "vue";

export default (ctx, inject) => {
  inject(
    "global",
    Vue.observable({
      device: null,
      debug: false,
    })
  );
};
