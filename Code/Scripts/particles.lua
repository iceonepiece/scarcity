particles = {
  [0] = {
    name = "fire",
    amount = 25,
    colorBegin = { r = 204 / 255.0, g = 212 / 255.0, b = 123 / 255.0, a = 1.0 },
  	colorEnd = { r = 204 / 255.0, g = 109 / 255.0, b = 41 / 255.0, a = 1.0 },
  	sizeBegin = 0.2,
    sizeVariation = 0.15,
    sizeEnd = 0.0,
  	lifeTime = 1.0,
  	velocity = { x = 2.0, y = 0.0 },
  	velocityVariation = { x = 3.0, y = 1.0 },
  	position = { x = 0.0, y = 0.0 }
  },

  [1] = {
    name = "running",
    amount = 1,
    colorBegin = { r = 1, g = 1, b = 1, a = 1.0 },
  	colorEnd = { r = 1, g = 1, b = 1, a = 1.0 },
  	sizeBegin = 0.3,
    sizeVariation = 0.2,
    sizeEnd = 0.0,
  	lifeTime = 0.5,
  	velocity = { x = 0.0, y = 0.4 },
  	velocityVariation = { x = 0.0, y = -0.2 },
  	position = { x = 0.0, y = 0.0 }
  }
}
