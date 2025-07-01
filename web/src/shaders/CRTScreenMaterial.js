import * as THREE from 'three'

export function createCRTMaterial(texture) {
    return new THREE.ShaderMaterial({
        uniforms: {
            uTexture: { value: texture },
            uTime: { value: 0 }
        },
        vertexShader: `
      varying vec2 vUv;
      void main() {
        vUv = uv;
        gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
      }
    `,
        fragmentShader: `
      uniform sampler2D uTexture;
      uniform float uTime;
      varying vec2 vUv;

      void main() {
        vec2 uv = vUv;

        // barrel distortion
        uv = uv * 2.0 - 1.0;
        uv *= 1.0 + 0.2 * length(uv) * length(uv);
        uv = uv * 0.5 + 0.5;

        vec3 color = texture2D(uTexture, uv).rgb;

        // scanline
        float scanline = sin(uv.y * 400.0) * 0.1;
        color -= scanline;

        // vignette
        float vignette = smoothstep(0.4, 0.95, distance(uv, vec2(0.5)));
        color *= 1.0 - vignette * 0.4;

        // flicker
        float flicker = sin(uTime * 60.0) * 0.02;
        color += flicker;

        gl_FragColor = vec4(color, 1.0);
      }
    `
    })
}
