<template>
  <div class="container">
    <h1>Devices</h1>
    <button @click="getDevices()">Refresh</button>

    <table>
      <tr>
        <th>Serial Number</th>
        <th>Actions</th>
      </tr>
      <tr v-for="(device, index) in devices" :key="index">
        <td>{{ device.serialNumber }}</td>
        <td>
          <button @click="gotoControl(device)">Control</button>
          <button variant="danger" @click="resetDevice(device)">Reset</button>
        </td>
      </tr>
    </table>
    <div class="card settings">
      <h3>Global Settings</h3>
      <div>
        <label>Debug Mode:</label>
        <input type="checkbox" v-model="$global.debug" />
      </div>
    </div>
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
.container {
  margin: 10px;
}
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

.card {
  border: 1px solid #ddd;
  border-radius: 5px;
  padding: 10px;
  flex-grow: 1;
}
.card.settings div {
  margin-bottom: 6px;
}
</style>
