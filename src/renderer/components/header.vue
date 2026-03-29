<template>
  <header class="app-header">
    <nuxt-link to="/" class="logo-link">
      <div class="logo-icon">
        <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
          <path d="M9 18V5l12-2v13"/>
          <circle cx="6" cy="18" r="3"/>
          <circle cx="18" cy="16" r="3"/>
        </svg>
      </div>
      <span class="logo-text">MIDIPedal</span>
      <span class="logo-badge">CONTROL</span>
    </nuxt-link>
  </header>
</template>

<script>
export default {
  name: "HeaderComponent",
};
</script>

<style scoped>
.app-header {
  height: 52px;
  display: flex;
  align-items: center;
  background: #161822;
  border-bottom: 1px solid rgba(255, 255, 255, 0.06);
  padding: 0 20px;
  -webkit-app-region: drag;
  user-select: none;
}

.logo-link {
  display: flex;
  align-items: center;
  gap: 10px;
  text-decoration: none;
  -webkit-app-region: no-drag;
}

.logo-icon {
  display: flex;
  align-items: center;
  justify-content: center;
  width: 32px;
  height: 32px;
  border-radius: 8px;
  background: linear-gradient(135deg, #6366f1, #8b5cf6);
  color: white;
}

.logo-text {
  font-size: 15px;
  font-weight: 600;
  color: #e1e4e8;
  letter-spacing: -0.3px;
}

.logo-badge {
  font-size: 10px;
  font-weight: 600;
  color: #8b5cf6;
  background: rgba(139, 92, 246, 0.12);
  padding: 2px 7px;
  border-radius: 4px;
  letter-spacing: 0.5px;
}
</style>
