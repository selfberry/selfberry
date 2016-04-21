precision highp float;

uniform sampler2D		iChannel0;
uniform sampler2D		iChannel1;	
uniform vec3			iResolution;
uniform float			iGlobalTime;
uniform vec4			iMouse;              	// mouse pixel coords. xy: current (if MLB down), zw: click
uniform vec4			iDate;
uniform int				iEffect;
uniform float 			iChromatic;
uniform float 			iShift;
uniform float			iGlitch;

void main()
{
	// center vec2 uv = -1.0 + 2.0 * gl_FragCoord.xy / iResolution.xy;
	vec2 uv = gl_FragCoord.xy / iResolution.xy;

	vec4 tex0 = texture2D(iChannel0, uv);
	// chromatic aberation
	if (iChromatic > 0.0) 
	{
		vec2 offset = vec2(iChromatic/50.,.0);
		tex0.r = texture2D(iChannel0, uv+offset.xy).r;
		tex0.g = texture2D(iChannel0, uv          ).g;
		tex0.b = texture2D(iChannel0, uv+offset.yx).b;
	}

	if (iEffect == 0)
	{	
		// https://www.shadertoy.com/view/Mlj3Dw
		vec4 c = tex0;
		uv.xy+=c.bg*(iMouse.x/iResolution.x-.5);
		uv-=.5;
		float a = atan(uv.y,uv.x);
		float d = length(uv);
		a+=c.r*(iMouse.y/iResolution.y-.5)*12.0;
		uv.x = cos(a)*d;
		uv.y = sin(a)*d;
		uv+=.5;
		c = texture2D(iChannel0,uv)*2.0;
		gl_FragColor = vec4(c.rgb,1.0);
	}
	if (iEffect == 1)
	{
		// https://www.shadertoy.com/view/llS3Rc
		vec2 p = (vec2(iResolution.x-gl_FragCoord.x, gl_FragCoord.y) / iResolution.xy);
		float speed = 2.;
		float pixelId = mod(floor(gl_FragCoord.x),2.) + 2.0 * mod(floor(gl_FragCoord.y),2.);
		float timeId = mod(floor(iDate.w*speed),4.0);
		if(pixelId != timeId) discard;
		if(timeId == 3.) {
			gl_FragColor = texture2D(iChannel0, p ) * 1.4;
		} else {    
			gl_FragColor = texture2D(iChannel0, p ) * 1.4 * vec4(timeId==0.?0.:1.,timeId==1.?0.:1.,timeId==2.?0.:1.,1.);
		}
	}
	if (iEffect == 2)
	{
		gl_FragColor = vec4(texture2D(iChannel0,uv).xyz,1.0);
	}
	if (iEffect == 3)
	{
		// https://www.shadertoy.com/view/ldX3Ds
		float scaleX = 60.0;
		vec2 scale = vec2(scaleX, scaleX / iResolution.x * iResolution.y);
		float width = 0.3;
		float scaleY = scaleX / iResolution.x * iResolution.y;
		vec2 pos = fract(uv * scale);
		vec2 coord = floor(uv * scale) / scale;
		float xb = dot(texture2D(iChannel0, vec2(coord.x, uv.y)).xyz, vec3(1.0 / 3.0));
		float yb = dot(texture2D(iChannel0, vec2(uv.x, coord.y)).xyz, vec3(1.0 / 3.0));
		float lit = float(abs(pos.y - width / 2.0 - (1.0 - width) * yb) < width / 2.0 || abs(pos.x - width / 2.0 - (1.0 - width) * xb) < width / 2.0);
		float b = (yb + xb) / 2.0;
		gl_FragColor = vec4(0.0, lit * b + (1.0 - lit) * b * 0.3, 0.0, 1.0);
	} 
    
}


