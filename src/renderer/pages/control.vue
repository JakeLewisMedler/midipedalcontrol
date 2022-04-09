<template>
  <div>
    <b-col class="mt-3">
      <h1 class="mb-3">Control</h1>
      <b-row>
        <b-col v-if="port">
          <b-card
            v-for="(pedal, index) in port.settings"
            :key="index"
            :title="`Pedal ${pedal.id}`"
          >
            <b-row
              ><b-col>
                <b-card sub-title="Settings">
                  <div>
                    <label>Mode:</label>
                    <select
                      v-model="pedal.mode"
                      @change="updatePedal(pedal, 'mode')"
                    >
                      <option
                        v-for="(mode, index) in modeOptions"
                        :key="index"
                        :value="mode.value"
                      >
                        {{ mode.text }}
                      </option>
                    </select>
                  </div>
                  <div>
                    <label>CC:</label>
                    <input
                      type="number"
                      min="0"
                      max="127"
                      v-model="pedal.cc"
                      @change="updatePedal(pedal, 'cc')"
                    />
                  </div> </b-card></b-col
              ><b-col>
                <b-card sub-title="Test">
                  <button
                    v-if="pedal.mode == 1"
                    class="switch__button"
                    @mousedown="testPedal(pedal, 127)"
                    @mouseup="testPedal(pedal, 0)"
                  >
                    Foot Switch
                  </button>
                  <input
                    v-else-if="pedal.mode == 2"
                    type="range"
                    min="0"
                    max="127"
                    v-model="pedal.value"
                    @input="testPedal(pedal)"
                  />
                </b-card> </b-col
            ></b-row> </b-card
        ></b-col>
      </b-row>
    </b-col>
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
    this.$serial.disconnect();
    if (!this.device) {
      this.$router.push("/");
    }
    this.connect();
  },
  data() {
    return {
      modeOptions: [
        { value: 1, text: "Mode 1 - Switched" },
        { value: 2, text: "Mode 2 - Variable" },
      ],
    };
  },
  methods: {
    connect() {
      this.$serial.connect();
      this.$serial.port.on("open", () => {
        this.$serial.port.getSettings();
      });
    },
    updatePedal(pedal, setting) {
      this.$serial.port.updatePedal(pedal, setting);
    },
    testPedal(pedal, value) {
      if (value != null) pedal.value = value;
      this.$serial.port.testPedal(pedal);
    },
  },
};
</script>

<style>
.switch__button {
  border-radius: 5px;
  border: 1px solid #888;
  background: white;
  height: 50px;
}

.switch__button:active {
  background: rgb(255, 99, 99);
}
</style>
