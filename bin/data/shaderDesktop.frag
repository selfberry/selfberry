#version 150
in vec2 texCoordVarying;
out vec4 fragColor;

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

void main()
{
	if ( iEffect == 0 )
	{
		fragColor = texture2D(tex0, texCoordVarying);
	}
	if ( iEffect == 2 )
	{
		vec4 texColor = texture2D(tex0, texCoordVarying);  
		fragColor = vec4(1.0-texColor.r, 1.0-texColor.g, 1.0-texColor.b, texColor.a);
	}
	if ( iEffect == 1 )
	{
		mediump float newTime = time * 2.0;

		vec2 newTexCoord;
		newTexCoord.s = texCoordVarying.s + (cos(newTime + (texCoordVarying.s*20.0)) * 0.01);
		newTexCoord.t = texCoordVarying.t + (sin(newTime + (texCoordVarying.t*20.0)) * 0.01);

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

		fragColor = outColor;
	}

}
