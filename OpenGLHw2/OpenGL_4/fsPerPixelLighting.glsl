#version 130

//#define NPR
//#define SILHOUETTE

in vec3 fN;
in vec3 fL;
in vec3 fV;

in vec3 fL2;
in vec3 fL3;
in vec3 fL4;

// 以下為新增的部分
uniform vec4  LightInView;     // Light's position in View Space
uniform vec4  LightInView2; 
uniform vec4  LightInView3; 
uniform vec4  LightInView4; 

uniform vec4  AmbientProduct;  // light's ambient  與 Object's ambient  與 ka 的乘積
uniform vec4  AmbientProduct2;
uniform vec4  AmbientProduct3;
uniform vec4  AmbientProduct4;

uniform vec4  DiffuseProduct;  // light's diffuse  與 Object's diffuse  與 kd 的乘積
uniform vec4  DiffuseProduct2;
uniform vec4  DiffuseProduct3;
uniform vec4  DiffuseProduct4;

uniform vec4  SpecularProduct; // light's specular 與 Object's specular 與 ks 的乘積
uniform vec4  SpecularProduct2;
uniform vec4  SpecularProduct3;
uniform vec4  SpecularProduct4;

uniform float fShininess;  //openGL4 p.32
uniform int   iLighting;
uniform vec4  vObjectColor;    // 代表物件的單一顏色

void main()
{
	// 先宣告 diffuse 與 specular
    vec4 diffuse = vec4(0.0,0.0,0.0,1.0);
	vec4 diffuse2 = vec4(0.0,0.0,0.0,1.0);
	vec4 diffuse3 = vec4(0.0,0.0,0.0,1.0);
	vec4 diffuse4 = vec4(0.0,0.0,0.0,1.0);

	vec4 specular = vec4(0.0,0.0,0.0,1.0);
	vec4 specular2 = vec4(0.0,0.0,0.0,1.0);
	vec4 specular3 = vec4(0.0,0.0,0.0,1.0);
	vec4 specular4 = vec4(0.0,0.0,0.0,1.0);
	
	if( iLighting != 1 ) {
		gl_FragColor = vObjectColor;
	}
	else {	
		// 1. 計算 Ambient color : Ia = AmbientProduct = Ka * Material.ambient * La = 
		vec4 ambient = AmbientProduct; // m_sMaterial.ka * m_sMaterial.ambient * vLightI;
		vec4 ambient2 = AmbientProduct2;
		vec4 ambient3 = AmbientProduct3;
		vec4 ambient4 = AmbientProduct4;

		// 單位化傳入的 Normal Dir
		vec3 vN = normalize(fN); 

		// 2. 單位化傳入的 Light Dir
		vec3 vL = normalize(fL); // normalize light vector
		vec3 vL2 = normalize(fL2);
		vec3 vL3 = normalize(fL3);
		vec3 vL4 = normalize(fL4);

		// 5. 計算 L dot N
		float fLdotN = vL.x*vN.x + vL.y*vN.y + vL.z*vN.z;
		float fL2dotN = vL2.x*vN.x + vL2.y*vN.y + vL2.z*vN.z;
		float fL3dotN = vL3.x*vN.x + vL3.y*vN.y + vL3.z*vN.z;
		float fL4dotN = vL4.x*vN.x + vL4.y*vN.y + vL4.z*vN.z;


		//light 1------------------------------------------------------------------------------------------------
		if( fLdotN >= 0 ) { // 該點被光源照到才需要計算
#ifndef NPR
			// Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
			diffuse = fLdotN * DiffuseProduct; 
#else
			if( fLdotN >= 0.85 ) diffuse = 1.0 * DiffuseProduct;
			else if( fLdotN >= 0.55 ) diffuse = 0.55 * DiffuseProduct;
			else diffuse = 0.35 * DiffuseProduct;
#endif

			// Specular color
			// Method 1: Phone Model
			// 計算 View Vector
			// 單位化傳入的 fV , View Direction
			vec3 vV = normalize(fV);

			//計算 Light 的 反射角 vRefL = 2 * fLdotN * vN - L
			// 同時利用 normalize 轉成單位向量
			vec3 vRefL = normalize(2.0f * (fLdotN) * vN - vL);

			//   計算  vReflectedL dot View
			float RdotV = vRefL.x*vV.x + vRefL.y*vV.y + vRefL.z*vV.z;

#ifndef NPR
			// Specular Color : Is = Ks * Material.specular * Ls * (R dot V)^Shininess;
			if( RdotV > 0 ) specular = SpecularProduct * pow(RdotV, fShininess); 
#else
			specular = vec4(0.0,0.0,0.0,1.0); 
#endif
		}


		//light 2----------------------------------------------------------

				if( fL2dotN >= 0 ) { // 該點被光源照到才需要計算
#ifndef NPR
			// Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
			diffuse2 = fL2dotN * DiffuseProduct2; 
#else
			if( fL2dotN >= 0.85 ) diffuse2 = 1.0 * DiffuseProduct2;
			else if( fL2dotN >= 0.55 ) diffuse2 = 0.55 * DiffuseProduct2;
			else diffuse2 = 0.35 * DiffuseProduct2;
#endif

			// Specular color
			// Method 1: Phone Model
			// 計算 View Vector
			// 單位化傳入的 fV , View Direction
			vec3 vV = normalize(fV);

			//計算 Light 的 反射角 vRefL = 2 * fLdotN * vN - L
			// 同時利用 normalize 轉成單位向量
			vec3 vRefL = normalize(2.0f * (fL2dotN) * vN - vL2);

			//   計算  vReflectedL dot View
			float RdotV = vRefL.x*vV.x + vRefL.y*vV.y + vRefL.z*vV.z;

#ifndef NPR
			// Specular Color : Is = Ks * Material.specular * Ls * (R dot V)^Shininess;
			if( RdotV > 0 ) specular2 = SpecularProduct2 * pow(RdotV, fShininess); 
#else
			specular2 = vec4(0.0,0.0,0.0,1.0); 
#endif
		}

				//light 3----------------------------------------------------------

				if( fL3dotN >= 0 ) { // 該點被光源照到才需要計算
#ifndef NPR
			// Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
			diffuse3 = fL3dotN * DiffuseProduct3; 
#else
			if( fL3dotN >= 0.85 ) diffuse3 = 1.0 * DiffuseProduct3;
			else if( fL3dotN >= 0.55 ) diffuse3 = 0.55 * DiffuseProduct3;
			else diffuse3 = 0.35 * DiffuseProduct3;
#endif

			// Specular color
			// Method 1: Phone Model
			// 計算 View Vector
			// 單位化傳入的 fV , View Direction
			vec3 vV = normalize(fV);

			//計算 Light 的 反射角 vRefL = 2 * fLdotN * vN - L
			// 同時利用 normalize 轉成單位向量
			vec3 vRefL = normalize(2.0f * (fL3dotN) * vN - vL3);

			//   計算  vReflectedL dot View
			float RdotV = vRefL.x*vV.x + vRefL.y*vV.y + vRefL.z*vV.z;

#ifndef NPR
			// Specular Color : Is = Ks * Material.specular * Ls * (R dot V)^Shininess;
			if( RdotV > 0 ) specular3 = SpecularProduct3 * pow(RdotV, fShininess); 
#else
			specular3 = vec4(0.0,0.0,0.0,1.0); 
#endif
		}

			//light 4----------------------------------------------------------

				if( fL4dotN >= 0 ) { // 該點被光源照到才需要計算
#ifndef NPR
			// Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
			diffuse4 = fL4dotN * DiffuseProduct4; 
#else
			if( fL4dotN >= 0.85 ) diffuse4 = 1.0 * DiffuseProduct4;
			else if( fL4dotN >= 0.55 ) diffuse4 = 0.55 * DiffuseProduct4;
			else diffuse4 = 0.35 * DiffuseProduct4;
#endif

			// Specular color
			// Method 1: Phone Model
			// 計算 View Vector
			// 單位化傳入的 fV , View Direction
			vec3 vV = normalize(fV);

			//計算 Light 的 反射角 vRefL = 2 * fLdotN * vN - L
			// 同時利用 normalize 轉成單位向量
			vec3 vRefL = normalize(2.0f * (fL4dotN) * vN - vL4);

			//   計算  vReflectedL dot View
			float RdotV = vRefL.x*vV.x + vRefL.y*vV.y + vRefL.z*vV.z;

#ifndef NPR
			// Specular Color : Is = Ks * Material.specular * Ls * (R dot V)^Shininess;
			if( RdotV > 0 ) specular4 = SpecularProduct4 * pow(RdotV, fShininess); 
#else
			specular4 = vec4(0.0,0.0,0.0,1.0); 
#endif
		}

		// 計算顏色 ambient + diffuse + specular;
		gl_FragColor = ambient + diffuse + specular + ambient2 + diffuse2 + specular2 + ambient3 + diffuse3 + specular3 + ambient4 + diffuse4 + specular4;  
		gl_FragColor.w = 1.0;	// 設定 alpha 為 1.0
		// gl_FragColor = vec4((ambient + diffuse + specular).xyz, 1.0);



#ifdef SILHOUETTE
	vec4 edgeColor = vec4(1.0, 0.0, 0.0, 1.0);
	if( abs(dot(normalize(fN), normalize(fV))) < 0.2)  gl_FragColor = edgeColor;
#endif

	}
}
