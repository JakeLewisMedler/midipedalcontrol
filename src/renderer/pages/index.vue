<template>
  <div class="page">
    <div class="page-header">
      <div>
        <h1 class="page-title">Devices</h1>
        <p class="page-subtitle">Discover and manage your pedal interfaces</p>
      </div>
      <button class="btn btn-secondary" @click="getDevices()">
        <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
          <polyline points="23 4 23 10 17 10"/>
          <polyline points="1 20 1 14 7 14"/>
          <path d="M3.51 9a9 9 0 0 1 14.85-3.36L23 10M1 14l4.64 4.36A9 9 0 0 0 20.49 15"/>
        </svg>
        Refresh
      </button>
    </div>

    <div class="device-list" v-if="devices.length">
      <div class="device-card" v-for="(device, index) in devices" :key="index">
        <div class="device-info">
          <div class="device-icon">
            <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round">
              <rect x="4" y="2" width="16" height="20" rx="2" ry="2"/>
              <line x1="12" y1="18" x2="12" y2="18.01"/>
              <line x1="8" y1="6" x2="16" y2="6"/>
              <line x1="8" y1="10" x2="16" y2="10"/>
              <line x1="8" y1="14" x2="16" y2="14"/>
            </svg>
          </div>
          <div>
            <div class="device-serial">{{ device.serialNumber }}</div>
          </div>
        </div>
        <div class="device-actions">
          <button class="btn btn-primary" @click="gotoControl(device)">
            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
              <circle cx="12" cy="12" r="3"/>
              <path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1-2.83 2.83l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-4 0v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83-2.83l.06-.06A1.65 1.65 0 0 0 4.68 15a1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1 0-4h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 2.83-2.83l.06.06A1.65 1.65 0 0 0 9 4.68a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 4 0v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 2.83l-.06.06A1.65 1.65 0 0 0 19.4 9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 0 4h-.09a1.65 1.65 0 0 0-1.51 1z"/>
            </svg>
            Control
          </button>
        </div>
      </div>
    </div>

    <div class="empty-state" v-else>
      <div class="empty-icon">
        <svg width="48" height="48" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1" stroke-linecap="round" stroke-linejoin="round">
          <path d="M9 18V5l12-2v13"/>
          <circle cx="6" cy="18" r="3"/>
          <circle cx="18" cy="16" r="3"/>
        </svg>
      </div>
      <p class="empty-text">No devices found</p>
      <p class="empty-hint">Connect a pedal interface and click Refresh</p>
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
  },
};
</script>

<style scoped>
.page {
  padding: 28px 32px;
  max-width: 800px;
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
}

.btn-primary {
  background: #6366f1;
  color: white;
}
.btn-primary:hover {
  background: #5558e6;
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


/* Device List */
.device-list {
  display: flex;
  flex-direction: column;
  gap: 8px;
}

.device-card {
  display: flex;
  justify-content: space-between;
  align-items: center;
  background: #161822;
  border: 1px solid rgba(255, 255, 255, 0.06);
  border-radius: 12px;
  padding: 16px 20px;
  transition: all 0.15s ease;
}
.device-card:hover {
  border-color: rgba(99, 102, 241, 0.3);
  background: #1a1c2e;
}

.device-info {
  display: flex;
  align-items: center;
  gap: 14px;
}

.device-icon {
  display: flex;
  align-items: center;
  justify-content: center;
  width: 42px;
  height: 42px;
  border-radius: 10px;
  background: rgba(99, 102, 241, 0.1);
  color: #8b8ef8;
  flex-shrink: 0;
}

.device-serial {
  font-size: 14px;
  font-weight: 500;
  color: #e1e4e8;
}

.device-actions {
  display: flex;
  gap: 8px;
}

/* Empty State */
.empty-state {
  text-align: center;
  padding: 60px 20px;
  color: #3d4158;
}

.empty-icon {
  margin-bottom: 16px;
}

.empty-text {
  font-size: 15px;
  font-weight: 500;
  color: #555a6e;
}

.empty-hint {
  font-size: 13px;
  color: #3d4158;
  margin-top: 6px;
}

</style>
