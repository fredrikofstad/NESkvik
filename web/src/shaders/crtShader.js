import { Effect } from 'postprocessing'
import { wrapEffect } from '@react-three/postprocessing'

class CRTShaderEffect extends Effect {
    constructor() {
        super(
            'CRTShader',

            `
      void mainUv(inout vec2 uv) {
        // apply slight barrel distortion
        uv = uv * 2.0 - 1.0;
        uv *= 1.0 + 0.2 * length(uv) * length(uv);
        uv = uv * 0.5 + 0.5;
      }
      `,

            `
      uniform sampler2D tDiffuse;
      uniform float time;

      void mainImage(const in vec4 inputColor, const in vec2 uv, out vec4 outputColor) {
        vec3 col = texture2D(tDiffuse, uv).rgb;

        // stronger scanline
        float scanline = sin(uv.y * 800.0) * 0.15;
        col -= scanline;

        // RGB mask emulation
        float mask = step(0.33, fract(uv.x * 960.0));
        col.r *= mask;
        col.g *= step(0.66, fract(uv.x * 960.0));
        // B left alone

        // vignette
        float vignette = smoothstep(0.4, 0.95, distance(uv, vec2(0.5)));
        col *= 1.0 - vignette * 0.4;

        // flicker
        float flicker = sin(time * 50.0) * 0.05;
        col += flicker;

        outputColor = vec4(col, 1.0);
      }
      `,
            {
                uniforms: new Map([['time', { value: 0 }]])
            }
        )

        this.timeUniform = this.uniforms.get('time')
    }

    update(renderer, inputBuffer, deltaTime) {
        if (this.timeUniform) {
            this.timeUniform.value += deltaTime
        }
    }
}

export const CRTEffect = wrapEffect(CRTShaderEffect)
