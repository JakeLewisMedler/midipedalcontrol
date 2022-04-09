/**
 * By default, Nuxt.js is configured to cover most use cases.
 * This default configuration can be overwritten in this file
 * @link {https://nuxtjs.org/guide/configuration/}
 */

module.exports = {
  ssr: false,
  target: "static",
  head: {
    title: "midipedal",
    meta: [{ charset: "utf-8" }],
  },
  loading: false,
  plugins: ["~/plugins/serial.js", "~/plugins/global.js"],
  buildModules: [],
  modules: ["bootstrap-vue/nuxt"],
  components: true,
};
