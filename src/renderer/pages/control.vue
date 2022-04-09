<template>
  <div>
    <b-col class="mt-3">
      <h1>Control</h1>
      <b-row><b-col v-if="device"> </b-col></b-row>
      {{ device }}</b-col
    >
  </div>
</template>

<script>
export default {
  computed: {
    device() {
      return this.$global.device;
    },
    port() {
      return this.$serial.port;
    },
  },
  beforeDestroy() {
    this.$serial.disconnect();
  },
  mounted() {
    if (!this.device) {
      this.$router.push("/");
    }
    this.connect();
  },
  data() {
    return {};
  },
  methods: {
    connect() {
      this.$serial.connect();
      this.$serial.port.receiver.on("data", (data) => {
        console.log(data);
      });
      this.$serial.port.on("open", () => {
        this.$serial.port.send("003");
      });
    },
  },
};
</script>

<style></style>
