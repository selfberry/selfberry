precision highp float; // this will make the default precision high

//we passed this in from our vert shader
varying vec2        texcoord0;

//These are variables we set in our ofApp using the ofShader API

//our texture reference
//passed in by
//shader.setUniformTexture("tex0", sourceImage.getTextureReference(), sourceImage.getTextureReference().texData.textureID);
uniform sampler2D   tex0;

//width and height that we are working with
//passed in by
//shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
uniform vec2        resolution;

//a changing value to work with
//passed in by
//shader.setUniform1f("time", ofGetElapsedTimef());
uniform float       time;

uniform vec3			iResolution;
uniform float 			iGlobalTime;
uniform vec4			iMouse;              	// mouse pixel coords. xy: current (if MLB down), zw: click
uniform vec4			iDate;
uniform int				iEffect;
uniform float 			iChromatic;
uniform float 			iShift;
uniform float			iGlitch;
uniform float			iPixelate;

//Each shader has one main() function you can use
//Below are a few implementations. Make sure you have all but one commented out

//Shaders are compiled at runtime meaning that you can just change the shader file 
//and re-run the ofApp without compiling


// just draw the texture to screen		
/*void main()
{
    gl_FragColor = texture2D(tex0, texcoord0);
}*/

// draw the texture to screen, inverted
/*void main()
{
    vec4 texColor = texture2D(tex0, texcoord0);  
    gl_FragColor = vec4(1.0-texColor.r, 1.0-texColor.g, 1.0-texColor.b, texColor.a);
}*/

void main()
{
	vec2 uv = gl_FragCoord.xy / iResolution.xy;

	if ( iEffect == 0 )
	{
		// https://www.shadertoy.com/view/ldX3Ds
		float scaleX = 60.0;
		vec2 scale = vec2(scaleX, scaleX / iResolution.x * iResolution.y);
		float width = 0.3;
		float scaleY = scaleX / iResolution.x * iResolution.y;
		vec2 pos = fract(uv * scale);
		vec2 coord = floor(uv * scale) / scale;
		float xb = dot(texture2D(tex0, vec2(coord.x, uv.y)).xyz, vec3(1.0 / 3.0));
		float yb = dot(texture2D(tex0, vec2(uv.x, coord.y)).xyz, vec3(1.0 / 3.0));
		float lit = float(abs(pos.y - width / 2.0 - (1.0 - width) * yb) < width / 2.0 || abs(pos.x - width / 2.0 - (1.0 - width) * xb) < width / 2.0);
		float b = (yb + xb) / 2.0;
		gl_FragColor = vec4(0.0, lit * b + (1.0 - lit) * b * 0.3, 0.0, 1.0);
	}
	if ( iEffect == 1 )
	{
		// let's wobble the image channels around independently, a bit Fear and Loathing in Las Vegas style
		mediump float newTime = time * 2.0;

		vec2 newTexCoord;
		newTexCoord.s = texcoord0.s + (cos(newTime + (texcoord0.s*20.0)) * 0.01);
		newTexCoord.t = texcoord0.t + (sin(newTime + (texcoord0.t*20.0)) * 0.01);

		mediump vec2 texCoordRed    = newTexCoord;
		mediump vec2 texCoordGreen  = newTexCoord;
		mediump vec2 texCoordBlue   = newTexCoord;

		texCoordRed     += vec2( cos((newTime * 2.76)), sin((newTime * 2.12)) )* 0.01;
		texCoordGreen   += vec2( cos((newTime * 2.23)), sin((newTime * 2.40)) )* 0.01;
		texCoordBlue    += vec2( cos((newTime * 2.98)), sin((newTime * 2.82)) )* 0.01;  

		mediump float colorR = texture2D( tex0, texCoordRed ).r;
		mediump float colorG = texture2D( tex0, texCoordGreen).g;
		mediump float colorB = texture2D( tex0, texCoordBlue).b;  
		mediump float colorA = texture2D( tex0, texCoordBlue).a;     
		mediump vec4 outColor = vec4( colorR, colorG, colorB, colorA);

		gl_FragColor = outColor;
	}
	if ( iEffect == 2 )
	{
		// https://www.shadertoy.com/view/llS3Rc
		vec2 p = (vec2(iResolution.x-gl_FragCoord.x, gl_FragCoord.y) / iResolution.xy);
		float speed = 2.;
		float pixelId = mod(floor(gl_FragCoord.x),2.) + 2.0 * mod(floor(gl_FragCoord.y),2.);
		float timeId = mod(floor(iDate.w*speed),4.0);
		if(pixelId != timeId) discard;
		if(timeId == 3.) {
			gl_FragColor = texture2D(tex0, p ) * 1.4;
		} else {    
			gl_FragColor = texture2D(tex0, p ) * 1.4 * vec4(timeId==0.?0.:1.,timeId==1.?0.:1.,timeId==2.?0.:1.,1.);
		}
	}

}
