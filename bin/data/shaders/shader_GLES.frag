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

	// temp
	vec2 q = gl_FragCoord.xy / iResolution.xy;
    vec2 uv = 0.5 + (q-0.5)*(0.9 + 0.1*sin(0.2*iGlobalTime));

	vec3 oricol = texture2D(iChannel0,vec2(q.x,1.0-q.y)).xyz;
    vec3 col;

    col.r = texture2D(iChannel0,vec2(uv.x+0.003,-uv.y)).x;
    col.g = texture2D(iChannel0,vec2(uv.x+0.000,-uv.y)).y;
    col.b = texture2D(iChannel0,vec2(uv.x-0.003,-uv.y)).z;

    col = clamp(col*0.5+0.5*col*col*1.2,0.0,1.0);

    col *= 0.5 + 0.5*16.0*uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y);

    col *= vec3(0.8,1.0,0.7);

    col *= 0.9+0.1*sin(10.0*iGlobalTime+uv.y*1000.0);

    col *= 0.97+0.03*sin(110.0*iGlobalTime);

    float comp = smoothstep( 0.2, 0.7, sin(iGlobalTime) );
    col = mix( col, oricol, clamp(-2.0+2.0*q.x+3.0*comp,0.0,1.0) );

    gl_FragColor = vec4(col,1.0);
	/*
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
	} */
    
}


