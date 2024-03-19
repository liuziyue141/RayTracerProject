#include "raytracer.h"
using namespace std;
struct Ray {
    vec3 P0;
    vec3 P1;
};

struct obj_norm_minT {
    object* objptr;
    vec3 normal;
    float min_t;
};
vec3 GLfloat2vec3(GLfloat* floatPtr){
    return vec3(floatPtr[0], floatPtr[1], floatPtr[2]);
}
vec4 GLfloat2vec4(GLfloat* floatPtr){
    return vec4(floatPtr[0], floatPtr[1], floatPtr[2], floatPtr[3]);
}
obj_norm_minT Intersect(Ray &ray){
    int objectId = -1;
    //vec3 P_return;
    vec3 normal = vec3(0,0,0);
    float min_t = FLT_MAX;
    for (int i = 0 ; i < numobjects ; i++) {
        object* obj = &(objects[i]);
        if(obj->type == "sphere"){
            vec3 P0 = vec3(obj->inverse_transform*vec4(ray.P0, 1.0f));
            vec3 P1 = vec3(obj->inverse_transform*vec4(ray.P1, 0.0f));
            float a = glm::dot(P1, P1);
            float b = 2.0f * glm::dot(P1, P0-(obj->sphere_loc));
            float c = glm::dot(P0 - obj->sphere_loc, P0 - obj->sphere_loc) - obj->sphere_rad*obj->sphere_rad;
            float discriminant = b*b-4*a*c;
            if(discriminant>=0){
                float root_minus = (-b-sqrt(discriminant))/(2*a);
                float root_plus = (-b+sqrt(discriminant))/(2*a);
                if(root_minus>(float)1e-5&&min_t>root_minus){
                    min_t = root_minus;
                    objectId = i;
                    normal = glm::normalize(vec3(glm::transpose(obj->inverse_transform) * vec4((min_t*P1+P0-obj->sphere_loc), 0)));
                }else if (root_plus>(float)1e-5&&min_t>root_plus){
                    min_t = root_plus;
                    objectId = i;
                    normal = glm::normalize(vec3(glm::transpose(obj->inverse_transform) * vec4((min_t*P1+P0-obj->sphere_loc), 0)));
                }
            }
     }else{
            vec3 A = obj->tri_v1;
            vec3 B = obj->tri_v2;
            vec3 C = obj->tri_v3;
            vec3 n = glm::normalize(glm::cross((C-A), (B-A)));
            //vec3 n = glm::normalize(glm::cross((B-A), (C-A)));
            float t = (glm::dot(A, n)-glm::dot(ray.P0, n))/glm::dot(ray.P1, n);
            if(t<min_t && t>(float)1e-5){
                vec3 P = ray.P0 + ray.P1*t ;
                float det = (C.x - A.x)*(B.y - A.y) - (B.x - A.x)*(C.y - A.y);
                float factor_beta = (C.x - A.x)*(P.y - A.y) - (P.x - A.x)*(C.y - A.y);
                float factor_gamma = (B.y - A.y)*(P.x - A.x) - (B.x - A.x)*(P.y - A.y);
                if(A.x == B.x && B.x == C.x){
                    det = (C.z - A.z)*(B.y - A.y) - (B.z - A.z)*(C.y - A.y);
                    factor_beta = (C.z - A.z)*(P.y - A.y) - (P.z - A.z)*(C.y - A.y);
                    factor_gamma = (B.y - A.y)*(P.z - A.z) - (B.z - A.z)*(P.y - A.y);
                }else if(A.y == B.y && B.y == C.y){
                    det = (C.x - A.x)*(B.z - A.z) - (B.x - A.x)*(C.z - A.z);
                    factor_beta = (C.x - A.x)*(P.z - A.z) - (P.x - A.x)*(C.z - A.z);
                    factor_gamma = (B.z - A.z)*(P.x - A.x) - (B.x - A.x)*(P.z - A.z);
                }
                float beta = factor_beta / det;
                float gamma = factor_gamma / det;
                float alpha = 1.0 - gamma - beta;
                if(alpha>=-1e-5 && beta>=-1e-5 && gamma >= -1e-5){
                    min_t = t;
                    objectId = i;
                    if(obj->type == "trinormal"){
                        normal = alpha * obj->tri_norm_v1 + beta * obj->tri_norm_v2 + gamma * obj->tri_norm_v3;
                    }
                    else{
                        normal = -n;
                    }
                }      
            }
        }
    }
    obj_norm_minT result;
    if(objectId>=0){
        result.objptr = &(objects[objectId]);
    }else{
        result.objptr = nullptr;
    }
    result.normal = normal;
    result.min_t = min_t;
    return result;
}

vec3 ComputeLight (vec3 direction, vec3 lightcolor, vec3 normal, vec3 halfvec, vec3 mydiffuse,
     vec3 myspecular, float myshininess) {
        vec3 retval;

        float nDotL = dot(normal, direction)  ;         
        vec3 lambert = mydiffuse * lightcolor * max(nDotL, 0.0f) ;  

        float nDotH = dot(normal, halfvec) ; 
        vec3 phong = myspecular * lightcolor * pow (max(nDotH, 0.0f), myshininess) ;
        retval = (lambert + phong) ; 
        //cout << "retrval: " << retval.x << ", " << retval.y << ", " << retval.z << endl;
        return retval ;            
} 


vec3 findColor(Ray ray, int objectId, float min_t, vec3 normal){
    vec3 intersection = ray.P0 + ray.P1 * min_t;
    vec3 finalcolor = GLfloat2vec3(objects[objectId].ambient) + GLfloat2vec3(objects[objectId].emission);

    for(int i = 0; i<numused; i++){
            vec4 lightpos = GLfloat2vec4(lightposn+4*i);
            vec3 light_color = GLfloat2vec3(lightcolor+3*i);
            vec3 eyedirn = -normalize(ray.P1);
            vec3 diffuse = GLfloat2vec3(objects[objectId].diffuse);
            vec3 specular = GLfloat2vec3(objects[objectId].specular);
            float visibility = 1;
            if(lightpos.w==0){
                vec3 light_dirn = glm::normalize(vec3(lightpos) - intersection);
                vec3 halfvec = normalize(light_dirn + eyedirn);
                float distance = glm::length(vec3(lightpos) - intersection);
                float attentuation = 1.0f/(constant_atten+linear_atten*distance+quadratic_atten*distance*distance);
                Ray light_to_object;
                light_to_object.P0 = vec3(lightpos);
                light_to_object.P1 = -light_dirn;
                obj_norm_minT light2object = Intersect(light_to_object);
                //here I made the change
                if (light2object.min_t<distance-1e-2){
                    visibility = 0;
                }
                finalcolor += attentuation*visibility*ComputeLight(light_dirn, light_color, normal, halfvec, diffuse, specular, shininess);
            }else{
                vec3 light_dirn = glm::normalize(vec3(lightpos));
                vec3 halfvec = normalize(light_dirn + eyedirn);
                Ray minusLightRay;
                minusLightRay.P0 = intersection;
                minusLightRay.P1 = light_dirn;
                minusLightRay.P0 = intersection+(float)1e-3*minusLightRay.P1;
                obj_norm_minT object2light = Intersect(minusLightRay);
                if(object2light.min_t != FLT_MAX){
                    visibility = 0 ;
                }
                finalcolor += visibility*ComputeLight(light_dirn, light_color, normal, halfvec, diffuse, specular, shininess);
            }
        }
    return finalcolor;
    
}

vec3 findcolorWithReflectiveEffect(Ray ray){
    int objectId = -1;
    int counter = 0;
    vec3 finalcolor = vec3(0, 0, 0);
    vec3 colorVector;
    vec3 specularMultiplier = vec3(1, 1, 1);
    do{
      obj_norm_minT intersectionInfo =  Intersect(ray);
      if(intersectionInfo.objptr!=nullptr){
        objectId = intersectionInfo.objptr -> id;
        colorVector = findColor(ray, objectId, intersectionInfo.min_t, intersectionInfo.normal);

        //cout <<  "colorVector: " << colorVector.x << ", " << colorVector.y << ", " << colorVector.z << endl;

        finalcolor += specularMultiplier * colorVector;
        specularMultiplier = specularMultiplier * GLfloat2vec3(intersectionInfo.objptr -> specular);
        float LdotN = dot(-ray.P1, intersectionInfo.normal);
        ray.P0 = ray.P0 + ray.P1 * intersectionInfo.min_t;
        ray.P1 = ray.P1 + 2.0f*LdotN*intersectionInfo.normal;
        ray.P0 = ray.P0 + (float)1e-2*ray.P1;
      }else{
        objectId = -1;
      }
      counter++;
    }while(objectId!=-1 && counter<maxdepth);
    //cout << "color: r: " << finalcolor.x << ", g: " << finalcolor.y << ", b: " << finalcolor.z << endl;

    return finalcolor;
}

//using namespace glm;

Ray RayThruPixel(int i, int j){

    float alpha = tan(fovx/2.0f)*(i-((w-1)/2.0f))/(w/2.0f);
    float beta = tan(fovy/2.0f)*(((h-1)/2.0f)-j)/(h/2.0f);
    vec3 w = glm::normalize(eye - center);
    vec3 u = glm::normalize(glm::cross(up, w));
    vec3 v = glm::cross(w, u);
    Ray ray;
    ray.P0 = eye;
    ray.P1 = glm::normalize(alpha*u+beta*v-w);
    return ray;
}
void traceRay(){
    FIBITMAP * bitmap = FreeImage_Allocate(w, h, 24);
    RGBQUAD color;
    Ray ray = RayThruPixel(0, w/2.0f);
    //if this is too slow, we change h and w order
    for(int i = 0; i<w; i++){
        for(int j = 0; j<h; j++){
            Ray ray = RayThruPixel(i, j);
            vec3 colorVector = findcolorWithReflectiveEffect(ray);
            color.rgbRed = min(colorVector.z* 255.0f, 255.0f);
            color.rgbGreen = min(colorVector.y * 255.0f, 255.0f);
            color.rgbBlue = min(colorVector.x * 255.0f, 255.0f);
            // if(color.rgbRed!=0){
            //     //cout<< "i: " << i << " j: "<< j << endl;
            //     cout << "color at pixel: width: " << i << ", height: " << j << ", colorVector: " << colorVector.x << ", " << colorVector.y << ", " << colorVector.z << endl;
            // }
            // //int objectId = Intersect(ray);
            // obj_norm_minT result = Intersect(ray);
            // // int objectId = result.objptr->id;

            // if(result.objptr!=nullptr){
            //     //object* obj = &(objects[objectId]);
            //     int objectId = result.objptr -> id;
            //     vec3 colorVector = findColor(ray, objectId, result.min_t, result.normal);
            //     if(colorVector.x == 0 && colorVector.y == 0 && colorVector.z == 0){
            //         cout << "color at pixel: width: " << i << ", height: " << j << endl;
            //     }
            //     // cout << "min t" << result.min_t << endl;
            //     // cout << "normal for pixel: x: " << result.normal.x << ", y: " << result.normal.y << ", z: " << result.normal.z << endl;
            //     // cout << "color at pixel: width: " << i << ", height: " << j << ", colorVector: " << colorVector.x << ", " << colorVector.y << ", " << colorVector.z << endl;
            //     color.rgbRed = colorVector.z* 255.0f;
            //     color.rgbGreen = colorVector.y * 255.0f;
            // //     color.rgbBlue = colorVector.x * 255.0f;
            //     // color.rgbRed = 255.0f;
            //     // color.rgbGreen = 0.0;
            //     // color.rgbBlue = 0.0f;

            // }else{
            //     color.rgbRed = 0;
            //     color.rgbGreen = 0;
            //     color.rgbBlue = 0;
            // }
            FreeImage_SetPixelColor(bitmap, i, h-j-1, &color);
        }
    }
    if (FreeImage_Save(FIF_PNG, bitmap, "test.png", 0)){
        cout << "Image successfully saved!" << endl;
    }
}