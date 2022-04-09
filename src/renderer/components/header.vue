<template>
  <header class="main-header">
    <nav>
      <nuxt-link to="/"> <h4>MIDIPedal CONTROL</h4> </nuxt-link>
    </nav>
  </header>
</template>

<script>
export default {
  name: "HeaderComponent",
};
</script>

<style scoped>
.main-header {
  height: 50px;
  display: flex;
  background-color: #364758;
}

nav {
  width: 100%;
  display: flex;
  justify-content: center;
  align-items: center;
  color: white;
}

nav div {
  height: 100%;
  margin: 0 20px;
  display: flex;
  align-items: center;
}

a {
  font-family: Helvetica, sans-serif;
  color: white;
  font-size: 1rem;
  font-weight: 100;
}

a:hover {
  opacity: 0.9;
}
</style>
