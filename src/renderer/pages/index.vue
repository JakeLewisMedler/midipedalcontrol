<template>
  <div>
    <b-col class="mt-3">
      <h1>Devices</h1>
      <b-button class="mb-3" @click="getDevices()">Refresh</b-button>

      <table>
        <tr>
          <th>Serial Number</th>
          <th>Actions</th>
        </tr>
        <tr v-for="(device, index) in devices" :key="index">
          <td>{{ device.serialNumber }}</td>
          <td>
            <b-button @click="gotoControl(device)">Control</b-button>
            <b-button variant="danger" @click="resetDevice(device)"
              >Reset</b-button
            >
          </td>
        </tr>
      </table>
    </b-col>
  </div>
</template>

<script>
export default {
  mounted() {
    this.$serial.disconnect();
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
    async resetDevice(device) {
      if (confirm("Really reset device?")) {
        await this.$serial.resetDevice(device);
        alert("Device Reset");
      }
    },
  },
};
</script>

<style>
table {
  font-family: Arial, Helvetica, sans-serif;
  border-collapse: collapse;
  width: 100%;
}

table td,
table th {
  border: 1px solid #ddd;
  padding: 8px;
}

table tr:nth-child(even) {
  background-color: #f2f2f2;
}

table tr:hover {
  background-color: #ddd;
}
table th {
  padding-top: 12px;
  padding-bottom: 12px;
  text-align: left;
  background-color: #04aa6d;
  color: white;
}
</style>
