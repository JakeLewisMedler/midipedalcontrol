<template>
  <div class="page" v-if="port || connectionError">
    <div class="page-header" v-if="port">
      <div>
        <h1 class="page-title">Pedal Control</h1>
        <p class="page-subtitle">Configure and test your connected pedals</p>
      </div>
      <div class="header-actions">
        <button class="btn btn-danger" @click="showResetConfirm = true">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            <polyline points="1 4 1 10 7 10"/>
            <path d="M3.51 15a9 9 0 1 0 2.13-9.36L1 10"/>
          </svg>
          Factory Reset
        </button>
        <nuxt-link to="/" class="btn btn-secondary">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            <line x1="19" y1="12" x2="5" y2="12"/>
            <polyline points="12 19 5 12 12 5"/>
          </svg>
          Back
        </nuxt-link>
      </div>
    </div>

    <div class="pedals-grid" v-if="port">
      <div
        class="pedal-card"
        v-for="(pedal, index) in port.settings"
        :key="index"
      >
        <!-- Pedal Header -->
        <div class="pedal-header">
          <div class="pedal-title-row">
            <h2 class="pedal-name">Pedal {{ pedal.id }}</h2>
            <div class="status-badge" :class="{ connected: pedal.connected }">
              <div class="status-dot"></div>
              <span>{{ pedal.connected ? 'Connected' : 'Disconnected' }}</span>
            </div>
          </div>
        </div>

        <div class="pedal-body">
          <!-- Settings Section -->
          <div class="settings-section">
            <div class="section-label">
              <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                <line x1="4" y1="21" x2="4" y2="14"/>
                <line x1="4" y1="10" x2="4" y2="3"/>
                <line x1="12" y1="21" x2="12" y2="12"/>
                <line x1="12" y1="8" x2="12" y2="3"/>
                <line x1="20" y1="21" x2="20" y2="16"/>
                <line x1="20" y1="12" x2="20" y2="3"/>
                <line x1="1" y1="14" x2="7" y2="14"/>
                <line x1="9" y1="8" x2="15" y2="8"/>
                <line x1="17" y1="16" x2="23" y2="16"/>
              </svg>
              Settings
            </div>

            <!-- Input Enabled -->
            <div class="setting-row">
              <span class="setting-name">Input Enabled</span>
              <div
                class="toggle"
                :class="{ active: pedal.enabled, disabled: !pedal.connected }"
                @click="pedal.connected && toggleEnabled(pedal)"
              >
                <div class="toggle-knob"></div>
              </div>
            </div>

            <!-- Mode -->
            <div class="setting-row">
              <span class="setting-name">Mode</span>
              <div class="segment-control">
                <button
                  v-for="(mode, i) in modeOptions"
                  :key="i"
                  :class="{ active: pedal.mode === mode.value }"
                  @click="setMode(pedal, mode.value)"
                >
                  {{ mode.label }}
                </button>
              </div>
            </div>

            <!-- Type -->
            <div class="setting-row" :class="{ dimmed: pedal.mode == 1 }">
              <span class="setting-name">Type</span>
              <div class="segment-control">
                <button
                  v-for="(type, i) in typeOptions"
                  :key="i"
                  :class="{ active: pedal.type === type.value }"
                  :disabled="pedal.mode == 1"
                  @click="setType(pedal, type.value)"
                >
                  {{ type.label }}
                </button>
              </div>
            </div>

            <!-- CC Number -->
            <div class="setting-row">
              <span class="setting-name">CC Number</span>
              <div class="cc-input-wrap">
                <input
                  type="number"
                  class="cc-input"
                  min="0"
                  max="127"
                  v-model="pedal.cc"
                  @change="updatePedal(pedal, 'cc')"
                />
              </div>
            </div>

            <!-- Polarity -->
            <div class="setting-row">
              <span class="setting-name">Polarity</span>
              <div class="polarity-control">
                <button
                  :class="{ active: pedal.polarity === 1 }"
                  @click="setPolarity(pedal, 1)"
                >+</button>
                <button
                  :class="{ active: pedal.polarity === 0 }"
                  @click="setPolarity(pedal, 0)"
                >-</button>
              </div>
            </div>
          </div>

          <!-- Test Section -->
          <div class="test-section">
            <div class="section-label">
              <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                <polygon points="5 3 19 12 5 21 5 3"/>
              </svg>
              {{ pedal.connected && pedal.enabled ? 'Monitor Input' : 'Control Input' }}
            </div>

            <!-- Mode 1: Foot Switch -->
            <div v-if="pedal.mode == 1" class="test-content">
              <button
                class="foot-switch"
                :class="{ pressed: pedal.value == 127 }"
                :disabled="pedal.connected && pedal.enabled"
                @mousedown="testPedal(pedal, 127)"
                @mouseup="testPedal(pedal, 0)"
              >
                <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round">
                  <path d="M12 22c5.523 0 10-4.477 10-10S17.523 2 12 2 2 6.477 2 12s4.477 10 10 10z"/>
                  <path d="M12 18a6 6 0 1 0 0-12 6 6 0 0 0 0 12z"/>
                </svg>
                <span>Foot Switch</span>
              </button>
              <div class="value-display">
                <span class="value-label">Value</span>
                <span class="value-number">{{ pedal.value }}</span>
              </div>
            </div>

            <!-- Mode 2: Volume Pedal -->
            <div v-else-if="pedal.mode == 2" class="test-content">
              <div
                class="volume-pedal"
                :draggable="!pedal.connected || !pedal.enabled"
                @dragstart="dragstart($event, pedal)"
                @drag="drag"
              >
                <div class="volume-track">
                  <div
                    class="volume-fill"
                    :style="{ height: (pedal.value / 127) * 100 + '%' }"
                  ></div>
                </div>
                <div class="volume-value">{{ pedal.value }}</div>
                <div class="volume-label">LEVEL</div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>

    <!-- Connection Error Modal -->
    <div class="modal-overlay" v-if="connectionError">
      <div class="modal">
        <div class="modal-icon modal-icon-error">
          <svg width="28" height="28" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            <circle cx="12" cy="12" r="10"/>
            <line x1="15" y1="9" x2="9" y2="15"/>
            <line x1="9" y1="9" x2="15" y2="15"/>
          </svg>
        </div>
        <h2 class="modal-title">Connection Failed</h2>
        <p class="modal-desc">{{ connectionError }}</p>
        <div class="modal-actions">
          <button class="modal-btn modal-btn-cancel" @click="$router.push('/')">Back to Devices</button>
          <button class="modal-btn modal-btn-primary" @click="retryConnect">Retry</button>
        </div>
      </div>
    </div>

    <!-- Reset Confirmation Modal -->
    <div class="modal-overlay" v-if="showResetConfirm" @click.self="showResetConfirm = false">
      <div class="modal">
        <div class="modal-icon">
          <svg width="28" height="28" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            <path d="M10.29 3.86L1.82 18a2 2 0 0 0 1.71 3h16.94a2 2 0 0 0 1.71-3L13.71 3.86a2 2 0 0 0-3.42 0z"/>
            <line x1="12" y1="9" x2="12" y2="13"/>
            <line x1="12" y1="17" x2="12.01" y2="17"/>
          </svg>
        </div>
        <h2 class="modal-title">Factory Reset</h2>
        <p class="modal-desc">This will erase all settings on the device and restore factory defaults. This action cannot be undone.</p>
        <div class="modal-actions">
          <button class="modal-btn modal-btn-cancel" @click="showResetConfirm = false">Cancel</button>
          <button class="modal-btn modal-btn-danger" @click="performReset">Reset Device</button>
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
        { value: 1, label: "Switched" },
        { value: 2, label: "Variable" },
      ],
      typeOptions: [
        { value: 1, label: "Yamaha" },
        { value: 2, label: "Roland" },
      ],
      connectionError: null,
      showResetConfirm: false,
      dragY: 0,
      dragValue: null,
      dragPedal: null,
      dragSensitivity: 2,
    };
  },
  methods: {
    toggleEnabled(pedal) {
      pedal.enabled = !pedal.enabled;
      this.updatePedal(pedal, 'enabled');
    },
    setMode(pedal, value) {
      pedal.mode = value;
      this.updatePedal(pedal, 'mode');
    },
    setType(pedal, value) {
      pedal.type = value;
      this.updatePedal(pedal, 'type');
    },
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
      this.connectionError = null;
      this.$serial.connect();
      this.$serial.port.on("open", () => {
        this.$serial.port.getSettings();
      });
      this.$serial.port.on("error", (err) => {
        this.connectionError = "Could not connect to the device. It may be in use by another application.";
      });
      this.$serial.port.on("close", () => {
        if (!this.connectionError) {
          this.$router.push("/");
        }
      });
    },
    retryConnect() {
      this.$serial.disconnect();
      this.connect();
    },
    updatePedal(pedal, setting) {
      this.$serial.port.updatePedal(pedal, setting);
    },
    testPedal(pedal, value) {
      this.$serial.port.testPedal(pedal, value);
    },
    async performReset() {
      this.showResetConfirm = false;
      await this.$serial.resetDevice(this.device);
      this.$router.push("/");
    },
  },
};
</script>

<style scoped>
.page {
  padding: 28px 32px;
  max-width: 1100px;
  margin: 0 auto;
  width: 100%;
}

.page-header {
  display: flex;
  justify-content: space-between;
  align-items: flex-start;
  margin-bottom: 28px;
}

.page-title {
  font-size: 22px;
  font-weight: 600;
  color: #f0f2f5;
  letter-spacing: -0.3px;
}

.page-subtitle {
  font-size: 13px;
  color: #6b7280;
  margin-top: 4px;
}

/* Buttons */
.btn {
  display: inline-flex;
  align-items: center;
  gap: 6px;
  padding: 8px 14px;
  border-radius: 8px;
  border: none;
  font-size: 13px;
  font-weight: 500;
  font-family: inherit;
  cursor: pointer;
  transition: all 0.15s ease;
  text-decoration: none;
}

.btn-secondary {
  background: #1e2030;
  color: #c0c4cc;
  border: 1px solid rgba(255, 255, 255, 0.08);
}
.btn-secondary:hover {
  background: #262940;
  color: #e1e4e8;
}

.btn-danger {
  background: rgba(239, 68, 68, 0.1);
  color: #ef4444;
  border: 1px solid rgba(239, 68, 68, 0.15);
}
.btn-danger:hover {
  background: rgba(239, 68, 68, 0.18);
}

.header-actions {
  display: flex;
  gap: 8px;
}

/* Pedals Grid */
.pedals-grid {
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.pedal-card {
  background: #161822;
  border: 1px solid rgba(255, 255, 255, 0.06);
  border-radius: 14px;
  overflow: hidden;
}

.pedal-header {
  padding: 16px 20px;
  border-bottom: 1px solid rgba(255, 255, 255, 0.04);
}

.pedal-title-row {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.pedal-name {
  font-size: 16px;
  font-weight: 600;
  color: #e1e4e8;
}

.status-badge {
  display: flex;
  align-items: center;
  gap: 6px;
  font-size: 12px;
  font-weight: 500;
  color: #6b7280;
  background: rgba(107, 114, 128, 0.1);
  padding: 4px 10px;
  border-radius: 20px;
}

.status-dot {
  width: 7px;
  height: 7px;
  border-radius: 50%;
  background: #4b5063;
}

.status-badge.connected {
  color: #34d399;
  background: rgba(52, 211, 153, 0.1);
}

.status-badge.connected .status-dot {
  background: #34d399;
  box-shadow: 0 0 6px rgba(52, 211, 153, 0.5);
}

/* Pedal Body */
.pedal-body {
  display: flex;
  gap: 1px;
  background: rgba(255, 255, 255, 0.04);
}

.settings-section,
.test-section {
  padding: 16px 20px;
  background: #161822;
  flex: 1;
}

.test-section {
  flex: 0 0 220px;
}

.section-label {
  display: flex;
  align-items: center;
  gap: 7px;
  font-size: 11px;
  font-weight: 600;
  color: #555a6e;
  text-transform: uppercase;
  letter-spacing: 0.6px;
  margin-bottom: 16px;
}

/* Setting Rows */
.setting-row {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 8px 0;
  transition: opacity 0.2s;
}

.setting-row.dimmed {
  opacity: 0.35;
  pointer-events: none;
}

.setting-name {
  font-size: 13px;
  color: #9ca3af;
}

/* Toggle */
.toggle {
  width: 38px;
  height: 21px;
  background: #2d3148;
  border-radius: 11px;
  position: relative;
  cursor: pointer;
  transition: background 0.2s ease;
}

.toggle.active {
  background: #6366f1;
}

.toggle.disabled {
  opacity: 0.4;
  cursor: not-allowed;
}

.toggle-knob {
  position: absolute;
  top: 2.5px;
  left: 2.5px;
  width: 16px;
  height: 16px;
  background: white;
  border-radius: 50%;
  transition: transform 0.2s ease;
}

.toggle.active .toggle-knob {
  transform: translateX(17px);
}

/* Segment Control */
.segment-control {
  display: flex;
  background: #1e2030;
  border-radius: 7px;
  padding: 2px;
  gap: 2px;
}

.segment-control button {
  padding: 5px 12px;
  border: none;
  border-radius: 5px;
  background: transparent;
  color: #6b7280;
  font-size: 12px;
  font-weight: 500;
  font-family: inherit;
  cursor: pointer;
  transition: all 0.15s ease;
}

.segment-control button:hover:not(:disabled) {
  color: #9ca3af;
}

.segment-control button.active {
  background: #6366f1;
  color: white;
}

.segment-control button:disabled {
  cursor: not-allowed;
  opacity: 0.4;
}

/* CC Input */
.cc-input-wrap {
  position: relative;
}

.cc-input {
  width: 64px;
  padding: 5px 8px;
  background: #1e2030;
  border: 1px solid rgba(255, 255, 255, 0.08);
  border-radius: 6px;
  color: #e1e4e8;
  font-size: 13px;
  font-family: 'SF Mono', 'Fira Code', monospace;
  text-align: center;
  outline: none;
  transition: border-color 0.15s;
  -moz-appearance: textfield;
}

.cc-input::-webkit-inner-spin-button,
.cc-input::-webkit-outer-spin-button {
  -webkit-appearance: none;
}

.cc-input:focus {
  border-color: #6366f1;
}

/* Polarity */
.polarity-control {
  display: flex;
  background: #1e2030;
  border-radius: 7px;
  padding: 2px;
  gap: 2px;
}

.polarity-control button {
  width: 32px;
  height: 28px;
  border: none;
  border-radius: 5px;
  background: transparent;
  color: #6b7280;
  font-size: 15px;
  font-weight: 600;
  font-family: inherit;
  cursor: pointer;
  transition: all 0.15s ease;
}

.polarity-control button.active {
  background: #6366f1;
  color: white;
}

/* Test Section */
.test-content {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 14px;
}

/* Foot Switch */
.foot-switch {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 8px;
  width: 100%;
  padding: 20px 16px;
  border: 2px solid #2d3148;
  border-radius: 12px;
  background: #1a1c2e;
  color: #9ca3af;
  font-size: 12px;
  font-weight: 500;
  font-family: inherit;
  cursor: pointer;
  transition: all 0.1s ease;
  user-select: none;
}

.foot-switch:hover:not(:disabled) {
  border-color: #3d4168;
}

.foot-switch.pressed {
  background: rgba(99, 102, 241, 0.15);
  border-color: #6366f1;
  color: #a5b4fc;
}

.foot-switch:disabled {
  opacity: 0.5;
  cursor: not-allowed;
}

.value-display {
  display: flex;
  justify-content: space-between;
  width: 100%;
  padding: 8px 12px;
  background: #1a1c2e;
  border-radius: 8px;
}

.value-label {
  font-size: 12px;
  color: #555a6e;
}

.value-number {
  font-size: 13px;
  font-weight: 600;
  font-family: 'SF Mono', 'Fira Code', monospace;
  color: #e1e4e8;
}

/* Volume Pedal */
.volume-pedal {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 10px;
  cursor: ns-resize;
}

.volume-track {
  width: 48px;
  height: 120px;
  background: #1a1c2e;
  border: 1px solid rgba(255, 255, 255, 0.06);
  border-radius: 8px;
  position: relative;
  overflow: hidden;
  display: flex;
  align-items: flex-end;
}

.volume-fill {
  width: 100%;
  background: linear-gradient(to top, #6366f1, #8b5cf6);
  border-radius: 0 0 7px 7px;
  transition: height 0.05s ease-out;
  min-height: 2px;
}

.volume-value {
  font-size: 18px;
  font-weight: 700;
  font-family: 'SF Mono', 'Fira Code', monospace;
  color: #e1e4e8;
}

.volume-label {
  font-size: 10px;
  font-weight: 600;
  color: #555a6e;
  letter-spacing: 1.5px;
}

/* Modal */
.modal-overlay {
  position: fixed;
  inset: 0;
  background: rgba(0, 0, 0, 0.6);
  display: flex;
  align-items: center;
  justify-content: center;
  z-index: 1000;
}

.modal {
  background: #1a1c2e;
  border: 1px solid rgba(255, 255, 255, 0.08);
  border-radius: 16px;
  padding: 32px;
  width: 360px;
  text-align: center;
  box-shadow: 0 20px 60px rgba(0, 0, 0, 0.5);
}

.modal-icon {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  width: 52px;
  height: 52px;
  border-radius: 14px;
  background: rgba(239, 68, 68, 0.1);
  color: #ef4444;
  margin-bottom: 16px;
}

.modal-icon-error {
  background: rgba(239, 68, 68, 0.1);
  color: #ef4444;
}

.modal-btn-primary {
  background: #6366f1;
  border: 1px solid #6366f1;
  color: white;
}
.modal-btn-primary:hover {
  background: #5558e6;
  border-color: #5558e6;
}

.modal-title {
  font-size: 17px;
  font-weight: 600;
  color: #f0f2f5;
  margin: 0 0 8px;
}

.modal-desc {
  font-size: 13px;
  color: #6b7280;
  line-height: 1.5;
  margin: 0 0 24px;
}

.modal-actions {
  display: flex;
  gap: 10px;
}

.modal-btn {
  flex: 1;
  padding: 10px 0;
  border-radius: 8px;
  font-size: 13px;
  font-weight: 500;
  font-family: inherit;
  cursor: pointer;
  transition: all 0.15s ease;
}

.modal-btn-cancel {
  background: #1e2030;
  border: 1px solid rgba(255, 255, 255, 0.08);
  color: #c0c4cc;
}

.modal-btn-cancel:hover {
  background: #262940;
  color: #e1e4e8;
}

.modal-btn-danger {
  background: #ef4444;
  border: 1px solid #ef4444;
  color: white;
}

.modal-btn-danger:hover {
  background: #dc2626;
  border-color: #dc2626;
}
</style>
