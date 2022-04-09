<template>
  <div>
    <b-col class="mt-3">
      <h1>Devices</h1>
      <b-button variant="primary" class="mb-3" @click="getDevices()"
        >Refresh</b-button
      >
      <b-row v-for="(device, index) in devices" v-bind:key="index"
        ><b-col>
          <h5>Serial Number</h5>
          <b-form-input :value="device.serialNumber" readonly></b-form-input
        ></b-col>
        <b-col>
          <h5>Actions</h5>
          <b-button @click="gotoControl(device)">Control</b-button>
        </b-col>
      </b-row></b-col
    >
  </div>
</template>

<script>
export default {
  mounted() {
    this.getDevices();
  },
  data() {
    return {
      devices: [],
    };
  },
  methods: {
    async getDevices() {
      this.devices = await this.$serial.getDevices();
      console.log(this.devices);
    },
    gotoControl(device) {
      this.$global.device = device;
      this.$router.push("/control");
    },
  },
};
</script>

<style></style>
