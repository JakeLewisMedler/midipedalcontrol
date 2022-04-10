<template>
  <div class="container" v-if="port">
    <h1 class="mb-3">Control</h1>
    <div
      class="card pedal"
      v-for="(pedal, index) in port.settings"
      :key="index"
    >
      <div class="header">
        <h2>Pedal {{ pedal.id }}</h2>
        <div :class="`status ${pedal.enabled ? 'enabled' : ''}`"></div>
      </div>

      <div class="content">
        <div class="card settings">
          <h3>Settings</h3>
          <div>
            <label>Mode:</label>
            <select v-model="pedal.mode" @change="updatePedal(pedal, 'mode')">
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
            <label>Type:</label>
            <select v-model="pedal.type" @change="updatePedal(pedal, 'type')">
              <option
                v-for="(type, index) in typeOptions"
                :key="index"
                :value="type.value"
              >
                {{ type.text }}
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
          </div>
          <div>
            <label>Polarity:</label>
            <button
              :class="`polarity__button ${pedal.polarity ? 'selected' : ''}`"
              @click="setPolarity(pedal, 1)"
            >
              +
            </button>
            <button
              :class="`polarity__button ${!pedal.polarity ? 'selected' : ''}`"
              @click="setPolarity(pedal, 0)"
            >
              -
            </button>
          </div>
        </div>
        <div class="card test">
          <h3 v-if="pedal.enabled">Monitor Input</h3>
          <h3 v-else>Control Input</h3>
          <button
            v-if="pedal.mode == 1"
            :disabled="pedal.enabled"
            :class="`switch__button ${pedal.value == 127 ? 'pressed' : ''}`"
            @mousedown="testPedal(pedal, 127)"
            @mouseup="testPedal(pedal, 0)"
          >
            Foot Switch
          </button>
          <div
            v-else-if="pedal.mode == 2"
            class="volume__pedal__container"
            :draggable="!pedal.enabled"
            @dragstart="dragstart($event, pedal)"
            @drag="drag"
          >
            <div
              :style="`height:${(pedal.value / 127) * 100}%`"
              class="volume__pedal"
            ></div>
          </div>
        </div>
      </div>
    </div>
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
      typeOptions: [
        { value: 1, text: "Type 1 - Yamaha" },
        { value: 2, text: "Type 2 - Roland" },
      ],
      dragY: 0,
      dragValue: null,
      dragPedal: null,
      dragSensitivity: 2,
    };
  },
  methods: {
    setPolarity(pedal, value) {
      pedal.polarity = value;
      this.updatePedal(pedal, "polarity");
    },
    dragstart(e, pedal) {
      var crt = e.target.cloneNode(true);
      crt.style.opacity = 0;
      document.body.appendChild(crt);
      e.dataTransfer.setDragImage(crt, 0, 0);
      this.dragY = e.clientY;
      this.dragPedal = pedal;
    },
    drag(event) {
      if (event.clientY == 0) return;
      let newValue =
        (this.dragY - event.clientY) * this.dragSensitivity +
        this.dragPedal.value;

      if (newValue > 127) newValue = 127;
      else if (newValue < 0) newValue = 0;

      if (newValue != this.dragValue) {
        this.testPedal(this.dragPedal, newValue);
      }
      this.dragValue = newValue;
    },
    connect() {
      this.$serial.connect();
      this.$serial.port.on("open", () => {
        this.$serial.port.getSettings();
      });
      this.$serial.port.on("close", () => {
        this.$router.push("/");
      });
    },
    updatePedal(pedal, setting) {
      this.$serial.port.updatePedal(pedal, setting);
    },
    testPedal(pedal, value) {
      this.$serial.port.testPedal(pedal, value);
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

.switch__button.pressed {
  background: rgb(255, 99, 99);
}
.content {
  display: flex;
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

.container {
  margin: 10px;
}

.volume__pedal__container {
  cursor: pointer;
  width: 36px;
  height: 59px;
  background: #666;
  position: relative;
  padding: 5px;
  display: flex;
  align-items: flex-end;
}
.volume__pedal {
  width: 100%;
  background: white;
}

.header {
  display: flex;
  align-items: center;
}

.status {
  margin-left: 15px;
  width: 15px;
  height: 15px;
  border-radius: 50%;
  background: red;
}

.status.enabled {
  background: green;
}

.polarity__button {
  border-radius: 5px;
  border: 1px solid #888;
  background: white;
  width: 20px;
  height: 20px;
  text-align: center;
  padding: 0;
}
.polarity__button.selected {
  background: rgb(255, 99, 99);
}
</style>
