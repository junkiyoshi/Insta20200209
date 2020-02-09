#version 150

const int number_of_targets = 100;

uniform float time;
uniform vec2 resolution;
uniform vec2 targets[number_of_targets];
uniform sampler2DRect tex;

out vec4 outputColor;

void main() {

  vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);
  vec4 tx = texture(tex, vec2(gl_FragCoord.x, resolution.y - gl_FragCoord.y));
  vec3 color = vec3(1.0 - tx.a);
 
  for(int i = 0; i < number_of_targets; i++){

    vec2 t = vec2(targets[i].x, -targets[i].y) / min(resolution.x, resolution.y) * 2.0;
    t.xy += vec2(-resolution.x, resolution.y) / min(resolution.x, resolution.y);

	vec3 v = vec3(smoothstep(0.05, 1.0, 0.0165 / length(p - t)));
	color += ((tx.a == 0) ? -v : v);
  }

  outputColor = vec4(color, 1.0);
}