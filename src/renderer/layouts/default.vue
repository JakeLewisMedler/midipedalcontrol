<template>
  <div id="app">
    <Header />
    <nuxt />
  </div>
</template>

<script>
export default {};
</script>

<style>
@import url('https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700&display=swap');

* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body {
  margin: 0 !important;
  font-family: 'Inter', -apple-system, BlinkMacSystemFont, sans-serif;
  background: #0f1117;
  color: #e1e4e8;
  -webkit-font-smoothing: antialiased;
}

#app {
  min-height: 100vh;
  display: flex;
  flex-direction: column;
}

::-webkit-scrollbar {
  width: 6px;
}
::-webkit-scrollbar-track {
  background: #0f1117;
}
::-webkit-scrollbar-thumb {
  background: #2d3148;
  border-radius: 3px;
}
::-webkit-scrollbar-thumb:hover {
  background: #3d4168;
}
</style>
