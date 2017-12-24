#version 330 core
in vec3 vFragPosition;
in vec2 vTexCoords;
in vec3 vNormal;

out vec4 color;

#define M_PI 3.1415926535897932384626433832795
#define COLD 100

uniform sampler2D tgrass, tstone, tsand, tdirt;
uniform int mId;
uniform bool bNorm;

void main()
{
  vec3 lightDir = normalize(vec3(1.0f, 1.0f, 0.0f)); 
  vec3 norm = normalize(vNormal);
  float h = vFragPosition.y;
  float an = min(asin(max(dot(norm, vec3(0.0f, 1.0f, 0.0f)), 0.0))*2/M_PI, 1);
   vec3 col = vec3(0.3f, 0.3f, 0.3f);
   float alpha=1.0f;
   if(bNorm)
   {
		if(mId==0)
			color = vec4(vNormal,1.0f);
		else
			color= vec4(vNormal,0.5f);
		return;
   }
  if(mId==0)
  {
	 
	  if(h > COLD && an > 0.6f)
		col=vec3(0.8f,0.8f,0.8f);
	  else
	  {
		 float g=0,s=0, st=0,d=0, sn=0;
		 s = max((5-h)*0.5f, 0.0f)*an;
		 g = max((h-0)*(COLD-h)/(2*COLD),0.0f)*pow(an,5);
		 d = 1;
		 st = pow((1-an),5)+max((h-COLD+10)/20,0.0f);
		 col = (s*vec3(texture(tsand, vTexCoords))+g*vec3(texture(tgrass, vTexCoords))+d*vec3(texture(tdirt, vTexCoords))+st*vec3(texture(tstone, vTexCoords)))/(s+g+d+st);
		 
	  }
  }
  else
  {
	col=vec3(0.01f,0.2f,0.6f);
	alpha=0.5f;
  }
	
 // if(h<0)
//	col*=1.2f;
//  else if(h>100)
//    col*=0.8f;
//  else
//  {
//    col*=( 1.2f - h*0.4f/100 );
//  }
  float kd = max(dot(norm, lightDir), 0.0);

  color = vec4(kd * col, alpha);
}