#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect 	iChannel0;
uniform sampler2DRect 	iChannel1;
uniform vec3			iResolution;
uniform float 			iGlobalTime;
uniform vec4			iMouse;              	// mouse pixel coords. xy: current (if MLB down), zw: click
uniform vec4			iDate;
uniform int				iEffect;
uniform float 			iChromatic;
uniform float 			iShift;
uniform float			iGlitch;
uniform float			iPixelate;

float glitchHash(float x)
{
	return fract(sin(x * 11.1753) * 192652.37862);
}
float glitchNse(float x)
{
	float fl = floor(x);
	return mix(glitchHash(fl), glitchHash(fl + 1.0), smoothstep(0.0, 1.0, fract(x)));
}

void main( void )
{
	// center vec2 uv = -1.0 + 2.0 * gl_TexCoord[0].st;
	vec2 uv = gl_TexCoord[0].st * 1.0;
	vec4 tex0 = texture2DRect(iChannel0, uv);
	vec4 fragOutput;

	// glitch
	if (iGlitch > 0.0) 
	{
		// glitch the point around
		float s = iGlobalTime * iGlitch;//50.0;
		float te = iGlobalTime * 9.0 / 16.0;//0.25 + (iTempoTime + 0.25) / 2.0 * 128.0 / 60.0;
		vec2 shk = (vec2(glitchNse(s), glitchNse(s + 11.0)) * 2.0 - 1.0) * exp(-5.0 * fract(te * 4.0)) * 0.1;
		uv += shk;		
	}
	// pixelate
	if ( iPixelate < 1.0 )
	{
		vec2 divs = vec2(iResolution.x * iPixelate / iResolution.y*60.0, iPixelate*60.0);
		uv = floor(uv * divs)/ divs;
	}
	// chromatic aberation
	if (iChromatic > 0.0) 
	{
		vec2 offset = vec2(iChromatic/50.,.0);
		tex0.r = texture2DRect(iChannel0, uv+offset.xy).r;
		tex0.g = texture2DRect(iChannel0, uv          ).g;
		tex0.b = texture2DRect(iChannel0, uv+offset.yx).b;
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
		c = texture2DRect(iChannel0,uv)*1.1;
		fragOutput = vec4(c.rgb,1.0);
	}
	if (iEffect == 1)
	{
		// https://www.shadertoy.com/view/llS3Rc
		float speed = 2.;
		float pixelId = mod(floor(gl_TexCoord[0].s),2.) + 2.0 * mod(floor(gl_TexCoord[0].t),2.);
		float timeId = mod(floor(iDate.w*speed),4.0);
		if(pixelId != timeId) discard;
		if(timeId == 3.) {
			fragOutput = texture2DRect(iChannel0, uv ) * 1.4;
		} else {    
			fragOutput = texture2DRect(iChannel0, uv ) * 1.4 * vec4(timeId==0.?0.:1.,timeId==1.?0.:1.,timeId==2.?0.:1.,1.);
		}
	}
	if (iEffect == 2)
	{
		gl_FragColor = vec4(texture2DRect(iChannel1,uv).xyz,1.0);
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
		float xb = dot(texture2DRect(iChannel0, vec2(coord.x, uv.y)).xyz, vec3(1.0 / 3.0));
		float yb = dot(texture2DRect(iChannel0, vec2(uv.x, coord.y)).xyz, vec3(1.0 / 3.0));
		float lit = float(abs(pos.y - width / 2.0 - (1.0 - width) * yb) < width / 2.0 || abs(pos.x - width / 2.0 - (1.0 - width) * xb) < width / 2.0);
		float b = (yb + xb) / 2.0;
		fragOutput = vec4(0.0, lit * b + (1.0 - lit) * b * 0.3, 0.0, 1.0);
	}
	//if(iShift>-0.5) {
	//fragOutput = fragOutput + (texture2DRect(iChannel1, uv) / 2);
	//}
	
	gl_FragColor = fragOutput; 
}